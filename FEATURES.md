# MovieInfo — Feature Overview

## Table of Contents
1. [Features Kept from Original](#features-kept-from-original)
2. [Features Dropped or Changed](#features-dropped-or-changed)
3. [Full Feature Reference — Current Implementation](#full-feature-reference--current-implementation)
4. [Suggested New Features](#suggested-new-features)

---

## Features Kept from Original

| Feature | Notes |
|---|---|
| **OMDb API integration** | Fetch title, rating, votes, director, cast, plot, genre, awards, box office, etc. via title + year |
| **Local SQLite caching** | Movies and video metadata both cached to avoid redundant API calls and ffprobe runs |
| **ffprobe metadata extraction** | Resolution, aspect ratio, duration, audio language, file size |
| **Folder-name parsing** | Regex extracts title and year from folder names (e.g. `Inception (2010)`) |
| **22-column table** | Title, Year, Decade, Resolution, Aspect Ratio, Quality, Size, Duration, Audio Language, Actions, Rated, IMDb Rating, IMDb Votes, Director, Actors, Writers, Awards, Language, Country, Box Office, Plot, Genre |
| **Action buttons per row** | Open file, open IMDb page, open Pahe search — rendered via item delegate |
| **Right-click context menu** | Refresh from IMDb, open folder, rename folder, organize by aspect ratio |
| **Folder rename** | Rename a movie's folder; updates model and re-fetches metadata |
| **Folder organize by aspect ratio** | Move movies into `16-9` or `UW` subfolders based on measured aspect ratio |
| **"Organize All" bulk organize** | Apply aspect-ratio organization to every row in one click |
| **"Movies to Move" window** | Compute which movies should move drives based on quality rules (rating + votes threshold) |
| **TeraCopy integration** | Batch-move files via TeraCopy from the Movies to Move window |
| **Drive + year toolbar** | Select a drive letter and enter a year to build and scan a standard folder path |
| **Drag & drop** | Drop a folder onto the window to scan it |
| **Add Folder button** | File dialog to pick a scan root |
| **CSV export** | Export current filtered view to CSV |
| **Settings dialog** | Configure OMDb API key, quality thresholds, drives, show/hide summary popup |
| **Fetch summary popup** | After a scan, show how many titles came from DB vs. live API |
| **Status bar feedback** | Live messages during scanning and fetching |
| **Column visibility** | Show/hide any column; persisted in QSettings |
| **Column sorting** | Click headers to sort; numeric columns sort by value, not string |
| **Dark QSS theme** | `styles.qss` applied at startup |
| **Sanitized-title fuzzy matching** | Falls back to stripped title when exact match fails in DB |
| **Force-refresh confirmation** | When a movie already exists in DB, asks before overwriting |
| **Clear table** | Reset the view without touching the database |

---

## Features Dropped or Changed

### Dropped
| Feature | Reason |
|---|---|
| **`MovieDataRefresher` dialog** | Its confirm-before-refresh UI was replaced by an inline `QMessageBox::question` inside `MainWindow::onMovieExistsInDatabase()` — simpler, same user outcome |
| **`TableColumnManager` class** | Column visibility and reordering logic was split between `AppSettings` (persistence) and `MovieFilterProxyModel::setColumnVisible()` (runtime hide/show). The dedicated manager class was removed. |
| **Column reordering via drag-drop** | The old `TableColumnManager` allowed dragging column headers to reorder them and persisting that order. The new implementation persists visibility and visual index but does not expose a reorder menu. Users can still drag headers since `setSectionsMovable` is on, but there is no dedicated UI menu for it. |
| **Export to Excel / spreadsheet** | The original had references to an Excel export path. The new implementation ships only CSV + JSON. XLSX export was not carried over. |
| **`NumericTableWidgetItem`** | The old app used a custom `QTableWidgetItem` subclass to sort numbers correctly. The new app uses `QSortFilterProxyModel` with a `RawValueRole` to sort numeric columns properly — no subclass needed. |
| **`movieExistsInDatabase` signal with empty args** | The old OmdbClient emitted a no-arg signal. The new `OmdbService` emits `movieExistsInDatabase(title, existing)` giving the window the existing record to display in the dialog. |
| **Hardcoded `cmd.exe move` fallback in OmdbClient** | Old client mixed UI concerns (cmd launch) with API logic. Separated into `OrganizeService::moveFolder()`. |

### Changed / Improved
| Feature | Old | New |
|---|---|---|
| **Architecture** | Single 2 100-line God-Object `MainWindow` | Layered: Core → Infrastructure → Services → Models → UI |
| **Movie struct** | All fields `QString`, no methods | Typed fields (`int year`, `double imdbRating`, `int imdbVotes`), helper methods `formattedRating()`, `formattedVotes()`, `isValid()` |
| **VideoFile struct** | Scattered local variables in MainWindow | Dedicated `Core::VideoFile` with helpers: `decade()`, `formattedDuration()`, `formattedFileSize()`, `isUltraWide()` |
| **Table base class** | `QTableWidget` (view and model coupled) | `QAbstractTableModel` + `QSortFilterProxyModel` (proper MVC) |
| **Filtering** | Per-column string filtering called on every keystroke, iterates all rows | `MovieFilterProxyModel::filterAcceptsRow()` with structured `FilterCriteria` (decade, quality, aspect ratio group, IMDb rating range, min votes, genre, free text) |
| **Settings** | Minimal (just API key + summary toggle) | Full `AppSettings` covering API key, drives, last folder, quality thresholds, column state, browser command |
| **OmdbClient** | Stateful, owns DB connection, saves to DB | Stateless `OmdbApiClient` (HTTP only); `OmdbService` owns caching/saving logic |
| **OMDb signal naming** | `movieFetchedFromDatabase` | `movieFromCache` |
| **JSON export** | Not present | Added alongside CSV |
| **Filter panel** | Ad-hoc widgets scattered in toolbar/layout | Dedicated `FilterPanel` widget with search box, decade/quality/aspect-ratio combos, rating range spin boxes, min-votes field, genre multi-select |

---

## Full Feature Reference — Current Implementation

### Core Domain (`src/core/`)

#### Movie (`Movie.h`)
- Typed fields: `QString` for text, `int year`, `double imdbRating`, `int imdbVotes`, `QDateTime fetchedAt`
- `isValid()` — title and year are both set
- `formattedRating()` — one decimal place string (e.g. `"7.4"`)
- `formattedVotes()` — comma-formatted integer (e.g. `"1,234,567"`)

#### VideoFile (`VideoFile.h`)
- File path, parsed folder title and year
- Technical metadata from ffprobe: resolution string, width×height ints, aspect ratio (double), quality tier string, duration seconds, audio language, file size bytes, last modified timestamp
- Optional `std::optional<Movie> metadata` — set once OMDb data arrives
- `decade()` → e.g. `"1990s"`
- `formattedDuration()` → `"HH:MM:SS"`
- `formattedFileSize()` → `"X.XX GB"`
- `formattedAspectRatio()` → rounded to 2 decimals
- `isUltraWide()` → aspect ratio between 2.2 and 2.5

#### FilterCriteria (`FilterCriteria.h`)
- Free-text search (matched against Title, Genre, Director, Actors, Plot)
- Decade filter (e.g. `"2000s"`)
- Quality tier filter (e.g. `"1080p"`)
- Aspect ratio group filter (`"16:9"` / `"Ultra-Wide"`)
- IMDb rating range: min/max double
- Minimum vote count
- Genre list (multi-select)
- `isEmpty()`, `clear()`

#### ColumnDef (`ColumnDef.h`)
- `Column` enum: 22 data columns + `COUNT` sentinel
- `ColumnDef` struct: display name, default width, default visibility, sortable flag, numeric flag
- `columnDefinitions()` static vector — single source of truth for all column metadata
- `columnName()` utility

#### QualityRules (`QualityRules.h`)
- `QualityRule`: min rating, min votes, target drive, human-readable description
- `MoveRequest`: title, year, current path, target drive, target path
- `aspectRatioFolder(double ar)` → `"UW"` or `"16-9"`
- `isUltraWide(double ar)` → bool
- Ultra-wide threshold constant: 2.3

---

### Infrastructure Layer (`src/infrastructure/`)

#### Database (`db/Database.h`)
- Wraps `QSqlDatabase` with unique per-instance connection names (thread-safe)
- `open()` creates both tables if missing
- `movies` table schema: title, year, rated, rating, votes, runtime, director, actors, writer, awards, language, country, boxOffice, plot, genre, imdbId, fetchedAt
- `video_metadata` table schema: filePath (PK), resolution, aspectRatio, quality, duration, audioLanguage, fileSize, lastModified

#### MovieRepository (`db/MovieRepository.h`)
- In-memory `QHash` cache loaded with `preloadCache()`
- `save(movie)` — INSERT OR REPLACE + update cache
- `get(title, year)` — exact match, cache-first
- `getBySanitizedTitle(title)` — strips punctuation, lowercases, fuzzy matches
- `exists(title, year)` — O(1) cache check
- `remove(title, year)` — delete from DB and cache
- `cachedCount()` — cache size

#### VideoMetadataRepository (`db/VideoMetadataRepository.h`)
- Caches ffprobe results by file path
- `hasValid(path, lastModified)` — returns true only if both path matches and file hasn't changed
- `save()` / `get()` — persist and retrieve `VideoMetadataEntry` structs
- `preloadCache()` — load all rows on startup

#### OmdbApiClient (`api/OmdbApiClient.h`)
- Stateless HTTPS client using `QNetworkAccessManager`
- `fetch(title, year)` — GET `https://www.omdbapi.com/?t=...&y=...&apikey=...`
- `setApiKey(key)` — set before first use
- Parses full JSON response: all movie fields, Ratings array (stored as semicolon-separated `"Source: Value"` pairs)
- Cleans vote strings (removes commas before `toInt`)
- Emits `movieFetched(Movie)` or `fetchFailed(title, error)`

#### AppSettings (`settings/AppSettings.h`)
- Backend: `QSettings("MovieInfo", "MovieInfoApp")`
- **API**: `omdbApiKey()` / `setOmdbApiKey()`
- **Fetch**: `showFetchSummary()` / `setShowFetchSummary()`
- **Drives**: `drives()` / `setDrives()` — default `{"D:\\", "E:\\"}`
- **Paths**: `lastFolder()` / `setLastFolder()`
- **Quality**: `highQualityMinRating()` (default 7.0), `highQualityMinVotes()` (default 100 000), setters
- **Columns**: `columnVisible(col)` / `setColumnVisible()`, `columnVisualIndex()` / `setColumnVisualIndex()`
- **Tools**: `browserCommand()` / `setBrowserCommand()`
- `qualityRule()` — builds and returns a `QualityRule` from stored settings

#### FfprobeRunner (`process/FfprobeRunner.h`)
- Synchronous static `extract(filePath)` → `Result`
- Runs `ffprobe -v quiet -print_format json -show_streams -show_format <file>` with 30-second timeout
- Parses first video stream: codec, width, height, display aspect ratio
- Parses first audio stream: language tag
- Quality tier detection from filename keywords: `4K`/`2160`, `1080`, `720`, else `SD`
- File size from `QFileInfo`
- Returns `std::nullopt` on process failure

---

### Services Layer (`src/services/`)

#### ScanService (`ScanService.h`)
Signals: `scanStarted(int)`, `fileProcessed(VideoFile)`, `progress(int,int,QString)`, `scanComplete(int,int,int)`, `error(QString)`

- `scanFolder(path, singleFile)` — entry point; runs on calling thread
- `cancel()` — sets atomic flag; checked between files
- `collectVideoFiles()` — recursive dir traversal, filters by extension (`.mp4 .mkv .avi .mov .flv .wmv`)
- `processFile(path)` — checks video metadata cache → if stale runs FfprobeRunner → saves result → populates `VideoFile`
- `parseFolderName(name)` — regex `(.+?)\s*\((\d{4})\)` extracts title and year; falls back to full folder name
- `buildFolderPath(drive, year)` — constructs standard path e.g. `D:\+2000\20s\2023`

#### OmdbService (`OmdbService.h`)
Signals: `movieFetched(Movie)`, `movieFromCache(Movie)`, `fetchFailed(title,error)`, `movieExistsInDatabase(title,Movie)`

- `preloadCache()` — loads MovieRepository into memory
- `fetchMovie(title, year, forceRefresh)` — checks cache (exact then fuzzy), decides whether to call API or emit cache hit
- On API success (`onApiFetched`): saves to repository, emits `movieFetched`
- On force-refresh when record exists: emits `movieExistsInDatabase` for user confirmation

#### OrganizeService (`OrganizeService.h`)
- `computeMovesToMove(files, rule)` — returns list of `MoveRequest`:
  - Determines current drive from file path
  - If movie meets quality rule → target drive D:
  - If movie doesn't meet quality rule → target drive E:
  - Only includes files whose current drive ≠ target drive
- `moveFolder(from, to)` — tries `QDir::rename()` first; falls back to `cmd.exe move` for cross-drive moves
- `organizeByAspectRatio(filePath, ar)` — moves movie folder into `16-9` or `UW` subfolder of its parent; creates target if needed

#### ExportService (`ExportService.h`)
Signals: `exportComplete(filepath)`, `exportFailed(error)`

- `exportData(proxyModel, format, filePath)` — reads live proxy model state (respects current filters and column order)
- **CSV**: Comma-separated with quoted fields; skips Actions column; first row is column headers
- **JSON**: Array of objects, keys are column header names; skips Actions column

---

### Models Layer (`src/models/`)

#### MovieTableModel (`MovieTableModel.h`)
Custom roles: `FilePathRole`, `FolderTitleRole`, `FolderYearRole`, `RawValueRole`

- `setFiles(list)` — batch replace with `beginResetModel`
- `addFile(vf)` — append with `beginInsertRows`
- `updateFile(row, vf)` — in-place update with `dataChanged`
- `attachMovieMetadata(movie)` — matches by title+year across all rows, sets `metadata` field
- `clear()` — reset to empty
- `fileAt(row)`, `allFiles()`, `findRow(path)` — data accessors

`data()` display logic:
- Metadata-owned columns (Title, Rated, Genre, Director, Writers, Actors, Plot, Language, Country, ImdbRating, ImdbVotes, BoxOffice, Awards): show `""` if no metadata yet
- Video-owned columns (Resolution, Quality, AudioLanguage, AspectRatio, Duration, Size): show from VideoFile
- Title falls back to `folderTitle` if no metadata
- Year falls back to `folderYear`
- `RawValueRole` returns unformatted numeric value for proper sorting

`data()` styling:
- `ForegroundRole` — rating ≥ 7.0 → `#4CAF50` green; 5–7 → `#FFC107` amber; < 5 → `#F44336` red; ultra-wide → `#64B5F6` blue
- `TextAlignmentRole` — right-align ImdbRating, ImdbVotes, Year, Size, Duration

#### MovieFilterProxyModel (`MovieFilterProxyModel.h`)
- `setFilter(criteria)` / `clearFilter()` — triggers re-evaluation of all rows
- `setColumnVisible(col, visible)` — adds/removes column from hidden set
- `filterAcceptsRow()` — evaluates all active `FilterCriteria` fields:
  - Free text → case-insensitive contains in Title, Genre, Director, Actors, Plot
  - Decade → exact match on `decade()` string
  - Quality tier → exact match
  - Aspect ratio group → `"Ultra-Wide"` or `"16:9"` check
  - Rating range → clamps to `[minRating, maxRating]`
  - Min votes → integer threshold
  - Genre list → any-of match
- `filterAcceptsColumn()` — hides columns in hidden set
- `lessThan()` — numeric columns (Year, Size, Duration, ImdbRating, ImdbVotes) sort by `RawValueRole` as double; default falls through to case-insensitive string

---

### UI Layer (`src/ui/`)

#### FilterPanel (`ui/FilterPanel.h`)
- Search box (live, connects `filterChanged` signal on every keystroke)
- Decade combo (populated dynamically after each scan)
- Quality combo (populated dynamically)
- Aspect ratio combo: All / 16:9 / Ultra-Wide
- IMDb rating range: two `QDoubleSpinBox` (0.0–10.0, step 0.1)
- Min votes `QSpinBox`
- Genre multi-select `QListWidget`
- `setDecades(list)` / `setQualities(list)` — helpers called from MainWindow after scan
- `clearFilters()` — resets all controls to defaults
- Emits `filterChanged(FilterCriteria)` whenever any control changes

#### ActionButtonDelegate (`ui/ActionButtonDelegate.h`)
- `QStyledItemDelegate` subclass for the Actions column
- `paint()` — draws three styled buttons per row: `📂 Open`, `IMDb`, `Pahe`
- `editorEvent()` — detects mouse click inside button hit rectangles, emits appropriate signal
- Signals: `openFileClicked(path)`, `imdbClicked(title, year, imdbId)`, `paheClicked(title, year)`

#### SettingsDialog (`ui/SettingsDialog.h`)
- **API Key**: `QLineEdit` (masked password mode)
- **Drives**: `QListWidget` + Add/Remove buttons; editable drive paths
- **Quality Rule**: min rating `QDoubleSpinBox`, min votes `QSpinBox`
- **Fetch Summary**: checkbox
- **Browser Command**: `QLineEdit` for custom browser path
- Reads from / writes to `AppSettings` on accept

#### MovesToMoveDialog (`ui/MovesToMoveDialog.h`)
- Shows `MoveRequest` list in a table (title, year, rating, votes, current path, target)
- Per-row `Move` action button
- `moveAllWithTeraCopy` button — writes source paths to a temp file and launches `TeraCopy.exe Move`
- Signals: `moveRequested(from, to)`, `moveAllWithTeraCopy(list)`

#### RenameFolderDialog (`ui/RenameFolderDialog.h`)
- Two `QLineEdit` inputs: new title, new year
- Pre-filled with current folder title and year
- `newTitle()`, `newYear()` accessors

#### FetchSummaryDialog (`ui/FetchSummaryDialog.h`)
- Modal popup shown after a scan completes
- Displays: total fetched, from DB cache count, from live API count
- Scrollable list of all movie titles fetched in the session

#### MainWindow (`ui/MainWindow.h`)
- Thin shell that composes all components
- Toolbar: drive combo, year line edit, Load / To Move / Clear buttons
- Central widget: `FilterPanel` (top) + `QTableView` (middle) + action bar (bottom)
- Action bar: Add Folder, Export, Organize All, Settings
- Drag & drop: `dragEnterEvent` / `dropEvent`
- `processPath(path, singleFile)` — coordinates `ScanService` + `OmdbService`
- `updateFilterCombos()` — rebuilds decade/quality lists in FilterPanel after scan
- `sanitizeForWindowsFolder()` — strips characters invalid in Windows paths
- `onFetchByDriveYearClicked()` — validates year input (1900–2100), builds path, calls `processPath`
- `onOrganizeAllClicked()` — batch aspect-ratio organize with confirm dialog
- `onExportClicked()` — file-save dialog → `ExportService::exportData()`
- Persistent editors for Action column so buttons are always rendered (not just on hover)

---

## Suggested New Features

### Quality of Life
| Feature | Description |
|---|---|
| **Undo last folder move** | Keep a move history in memory for the session; add an Undo button that reverses the last `OrganizeService::moveFolder()` call |
| **Batch metadata refresh** | Select multiple rows, right-click → "Refresh Selected from IMDb" — runs OmdbService in a worker thread with a progress bar |
| **Re-scan on startup** | Option in settings to automatically rescan the last-used folder on launch, refreshing stale video metadata |
| **Inline editable title/year** | Double-click Title or Year cell to edit; commits a folder rename and triggers a re-fetch |
| **Column profile presets** | Save/load named column visibility configurations (e.g. "Minimal", "Full", "Export") via the settings dialog |

### Search & Filter
| Feature | Description |
|---|---|
| **Saved filter presets** | Name and save the current FilterCriteria to a dropdown so favourite filter combinations can be recalled instantly |
| **Filter by director or actor** | Add dedicated combo-boxes to FilterPanel populated from the loaded dataset |
| **Missing metadata indicator** | Filter toggle to show only rows that have no OMDb metadata yet (quick way to see what failed) |
| **Duplicate detection** | Flag rows where two entries share the same IMDb ID or the same folder title and year |

### Organisation & File Management
| Feature | Description |
|---|---|
| **Folder structure templates** | Configurable path template (e.g. `{drive}\Movies\{decade}\{year}\{title} ({year})`) used by ScanService and OrganizeService |
| **Watchdog / auto-scan** | Use `QFileSystemWatcher` to monitor configured drive roots; automatically add new files as they appear |
| **Subtitles side-car check** | When scanning, flag whether an `.srt`/`.ass` file with a matching name exists next to the video |
| **Verify file integrity** | Optional background job that checks each video file can be opened by ffprobe without errors and marks corrupt entries |
| **Move history log** | Persist all folder-move operations to a `moves.log` file so they can be audited or replayed |

### UI & Display
| Feature | Description |
|---|---|
| **Poster thumbnail column** | Download and cache the OMDb poster URL; show a small thumbnail in a dedicated column or in a hover tooltip |
| **Detail side panel** | Click a row to expand a side panel showing full plot, all ratings (Rotten Tomatoes, Metacritic), awards, and poster |
| **Statistics dashboard** | Show a summary panel: total movies, total size on disk, average rating, rating distribution histogram, decade breakdown |
| **Dark / light theme toggle** | Let users switch between the existing dark QSS theme and an auto-generated light palette from `QApplication::setPalette` |
| **Keyboard navigation shortcuts** | `Ctrl+F` → focus search box; `F5` → rescan; `Delete` → clear selection; `Ctrl+E` → export |

### Data & Integration
| Feature | Description |
|---|---|
| **Letterboxd / Trakt import** | Import a watched-list CSV from Letterboxd or Trakt and cross-reference against local files to show what you own vs. what you've seen |
| **IMDb watchlist sync** | Fetch the user's IMDb watchlist (via exported CSV) and highlight matching local files in a different color |
| **Multiple API sources** | Add The Movie Database (TMDB) as a fallback when OMDb returns no result; configurable priority in settings |
| **JSON/NDJSON database export** | Export the full `movies` SQLite table as a portable JSON document for backup or use in other tools |
| **Stale cache manager** | Dialog listing all DB entries older than a configurable age (e.g. 6 months) with bulk-refresh option |

### Performance
| Feature | Description |
|---|---|
| **Threaded scanning** | Move `ScanService::scanFolder()` to a `QThread` or `QtConcurrent::run` so the UI stays responsive during large folder scans |
| **Concurrent OMDb fetches** | Use `QtConcurrent::map` with a rate-limiter to fetch multiple movies in parallel (OMDb free tier allows ~1 000 req/day) |
| **Lazy column rendering** | Only compute formatted strings for visible rows using virtual scrolling; reduces memory pressure for libraries > 1 000 titles |
