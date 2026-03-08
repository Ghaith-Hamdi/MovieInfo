#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "ui/dialogs/SettingsDialog.h"
#include "ui/dialogs/MovesToMoveDialog.h"
#include "ui/dialogs/RenameFolderDialog.h"
#include "ui/dialogs/FetchSummaryDialog.h"

#include <QApplication>
#include <QDragEnterEvent>
#include <QDesktopServices>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QHeaderView>
#include <QMenu>
#include <QMessageBox>
#include <QMimeData>
#include <QProcess>
#include <QRegularExpression>
#include <QTextStream>
#include <QTimer>
#include <QUrl>
#include <QTableView>
#include <QStatusBar>
#include <QProgressBar>
#include <QLabel>
#include <QFuture>
#include <QFutureWatcher>
#include <QtConcurrent/QtConcurrent>
namespace UI
{

    // ========================================================================
    // CONSTRUCTOR / DESTRUCTOR
    // ========================================================================

    MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui_MainWindow), m_progressDialog(nullptr)
    {
        ui->setupUi(this);

        // Infrastructure
        m_settings = new Infrastructure::AppSettings(this);
        m_db = new Infrastructure::Database(
            QCoreApplication::applicationDirPath() + "/movies.db", this);
        m_movieRepo = new Infrastructure::MovieRepository(m_db, this);
        m_videoRepo = new Infrastructure::VideoMetadataRepository(m_db, this);
        m_apiClient = new Infrastructure::OmdbApiClient(m_settings->apiKey(), this);

        if (!m_db->open())
        {
            QMessageBox::warning(this, "Database Error", "Failed to initialize movie database");
        }

        // Services
        m_scanService = new Services::ScanService(m_videoRepo, this);
        m_omdbService = new Services::OmdbService(m_apiClient, m_movieRepo, this);
        m_organizeService = new Services::OrganizeService(this);
        m_exportService = new Services::ExportService(this);

        // Models
        m_tableModel = new Models::MovieTableModel(this);
        m_proxyModel = new Models::MovieFilterProxyModel(this);
        m_proxyModel->setSourceModel(m_tableModel);

        // Table setup (runtime, cannot go in .ui)
        ui->tableView->setModel(m_proxyModel);
        QHeaderView *header = ui->tableView->horizontalHeader();
        header->setSectionResizeMode(QHeaderView::ResizeToContents);
        header->setStretchLastSection(true);
        header->setHighlightSections(false);
        header->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui->tableView->verticalHeader()->setVisible(false);
        ui->tableView->setMouseTracking(true);
        ui->tableView->viewport()->setMouseTracking(true);

        m_actionDelegate = new ActionButtonDelegate(this);
        ui->tableView->setItemDelegateForColumn(
            static_cast<int>(Core::Column::Actions), m_actionDelegate);

        // Populate drive combo
        ui->driveCombo->addItems(m_settings->drives());

        // Status bar widgets: scan progress and current folder label
        m_scanProgressBar = new QProgressBar(this);
        m_scanProgressBar->setVisible(false);
        m_scanProgressBar->setMinimumWidth(200);
        m_scanProgressBar->setTextVisible(true);

        m_currentFolderLabel = new QLabel(this);
        m_currentFolderLabel->setMinimumWidth(200);
        m_currentFolderLabel->setMaximumWidth(500);
        m_currentFolderLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

        statusBar()->addPermanentWidget(m_scanProgressBar);
        statusBar()->addPermanentWidget(m_currentFolderLabel);

        setupConnections();
        loadStylesheet();

        setWindowTitle("MovieInfo - Video Library Manager");
        setAcceptDrops(true);

        applyColumnSettings();

        statusBar()->showMessage("Ready - drag files or use Add Folder to start");
    }

    MainWindow::~MainWindow()
    {
        delete ui;
    }

    // ========================================================================
    // UI SETUP
    // ========================================================================

    void MainWindow::setupConnections()
    {
        // Filter
        connect(ui->filterPanel, &FilterPanel::filterChanged,
                this, &MainWindow::onFilterChanged);

        // Scan service
        connect(m_scanService, &Services::ScanService::fileProcessed,
                this, &MainWindow::onFileProcessed);
        connect(m_scanService, &Services::ScanService::scanStarted,
                this, &MainWindow::onScanStarted);
        connect(m_scanService, &Services::ScanService::progress,
                this, &MainWindow::onScanProgress);
        connect(m_scanService, &Services::ScanService::scanComplete,
                this, &MainWindow::onScanComplete);

        // OMDb service
        connect(m_omdbService, &Services::OmdbService::movieFetched,
                this, &MainWindow::onMovieFetched);
        connect(m_omdbService, &Services::OmdbService::movieFromCache,
                this, &MainWindow::onMovieFromCache);
        connect(m_omdbService, &Services::OmdbService::movieExistsInDatabase,
                this, &MainWindow::onMovieExistsInDatabase);
        connect(m_omdbService, &Services::OmdbService::fetchFailed,
                this, &MainWindow::onFetchFailed);

        // Action delegate
        connect(m_actionDelegate, &ActionButtonDelegate::openFileClicked,
                this, &MainWindow::onOpenFile);
        connect(m_actionDelegate, &ActionButtonDelegate::imdbClicked,
                this, &MainWindow::onImdbClicked);
        connect(m_actionDelegate, &ActionButtonDelegate::paheClicked,
                this, &MainWindow::onPaheClicked);

        // Table context menu
        connect(ui->tableView, &QTableView::customContextMenuRequested,
                this, &MainWindow::onContextMenu);

        // Toolbar row
        connect(ui->loadBtn, &QPushButton::clicked, this, &MainWindow::onFetchByDriveYearClicked);
        connect(ui->openLastBtn, &QPushButton::clicked, this, &MainWindow::onOpenLastFolderClicked);
        connect(ui->toMoveBtn, &QPushButton::clicked, this, &MainWindow::onShowMoviesToMoveClicked);
        connect(ui->clearBtn, &QPushButton::clicked, this, &MainWindow::onClearTableClicked);

        // Bottom bar
        connect(ui->addFolderBtn, &QPushButton::clicked, this, &MainWindow::onSelectFolderClicked);
        connect(ui->exportBtn, &QPushButton::clicked, this, &MainWindow::onExportClicked);
        connect(ui->organizeAllBtn, &QPushButton::clicked, this, &MainWindow::onOrganizeAllClicked);
        connect(ui->settingsBtn, &QPushButton::clicked, this, &MainWindow::onSettingsClicked);
    }

    void MainWindow::loadStylesheet()
    {
        QFile styleFile("styles.qss");
        if (styleFile.open(QFile::ReadOnly | QFile::Text))
        {
            setStyleSheet(QTextStream(&styleFile).readAll());
        }
    }

    void MainWindow::applyColumnSettings()
    {
        const auto &defs = Core::columnDefinitions();
        for (const auto &def : defs)
        {
            if (def.id == Core::Column::Actions)
                continue;
            int col = static_cast<int>(def.id);
            ui->tableView->setColumnHidden(col, !m_settings->columnVisible(col));
        }
    }

    // ========================================================================
    // SCANNING
    // ========================================================================

    void MainWindow::onSelectFolderClicked()
    {
        QString lastFolder = m_settings->lastFolder();
        QString path = QFileDialog::getExistingDirectory(this, "Select Video Folder", lastFolder);
        if (path.isEmpty())
            return;

        statusBar()->showMessage("Processing videos from: " + path);
        processPath(path, false);
    }

    void MainWindow::processPath(const QString &path, bool isSingleFile)
    {
        m_omdbService->preloadCache();

        // Reset counters
        m_totalToFetch = 0;
        m_fromDb = 0;
        m_fromApi = 0;
        m_fetchedFromImdbList.clear();

        m_settings->setLastFolder(path);
        m_currentFolderLabel->setText(path);
        m_scanService->scanFolder(path, isSingleFile);
    }

    void MainWindow::onFileProcessed(const Core::VideoFile &file)
    {
        m_tableModel->addFile(file);

        // Fetch metadata
        m_totalToFetch++;
        m_omdbService->fetchMovie(file.folderTitle, file.folderYear);

        statusBar()->showMessage(
            QString("Processing: %1").arg(file.folderTitle));
    }

    void MainWindow::onScanStarted(int totalFiles)
    {
        m_scanProgressBar->setRange(0, totalFiles);
        m_scanProgressBar->setValue(0);
        m_scanProgressBar->setVisible(true);
    }

    void MainWindow::onScanProgress(int current, int total, const QString &currentFile)
    {
        Q_UNUSED(total);
        m_scanProgressBar->setValue(current);
        m_currentFolderLabel->setText(currentFile);
    }

    void MainWindow::onScanComplete(int total, int cached, int fresh)
    {
        updateFilterCombos();
        ui->tableView->resizeColumnsToContents();

        statusBar()->showMessage(
            QString("Processed %1 video files (%2 cached, %3 fresh)")
                .arg(total)
                .arg(cached)
                .arg(fresh));

        // Show fetch summary if enabled
        if (m_settings->showFetchSummary() && !m_fetchedFromImdbList.isEmpty())
        {
            QTimer::singleShot(500, this, [this]()
                               {
            FetchSummaryDialog dlg(m_fetchedFromImdbList, m_fromDb, m_fromApi, this);
            dlg.exec(); });
        }

        m_scanProgressBar->setVisible(false);
        m_currentFolderLabel->setText(m_settings->lastFolder());
    }

    // ========================================================================
    // METADATA
    // ========================================================================

    void MainWindow::onMovieFetched(const Core::Movie &movie)
    {
        m_fromApi++;
        m_fetchedFromImdbList.append(movie.title);
        m_tableModel->attachMovieMetadata(movie);

        statusBar()->showMessage(
            QString("Fetched %1 of %2 | DB: %3 | API: %4")
                .arg(m_fromDb + m_fromApi)
                .arg(m_totalToFetch)
                .arg(m_fromDb)
                .arg(m_fromApi));
    }

    void MainWindow::onMovieFromCache(const Core::Movie &movie)
    {
        m_fromDb++;
        m_tableModel->attachMovieMetadata(movie);

        statusBar()->showMessage(
            QString("Fetched %1 of %2 | DB: %3 | API: %4")
                .arg(m_fromDb + m_fromApi)
                .arg(m_totalToFetch)
                .arg(m_fromDb)
                .arg(m_fromApi));
    }

    void MainWindow::onMovieExistsInDatabase(const QString &title, const Core::Movie &existing)
    {
        QMessageBox::StandardButton reply = QMessageBox::question(
            this, "Movie Exists",
            QString("'%1' already exists in database.\n\nRating: %2\nVotes: %3\n\nRefresh from IMDb?")
                .arg(title, existing.formattedRating(), existing.formattedVotes()),
            QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

        if (reply == QMessageBox::Yes)
        {
            m_apiClient->fetch(m_pendingRefreshTitle, m_pendingRefreshYear);
        }
    }

    void MainWindow::onFetchFailed(const QString &title, const QString &error)
    {
        Q_UNUSED(error);
        statusBar()->showMessage(QString("Failed to fetch: %1").arg(title), 3000);
    }

    // ========================================================================
    // FILTERING
    // ========================================================================

    void MainWindow::onFilterChanged(const Core::FilterCriteria &criteria)
    {
        m_proxyModel->setFilter(criteria);
    }

    void MainWindow::updateFilterCombos()
    {
        QSet<QString> decades, qualities;
        const auto &files = m_tableModel->allFiles();
        for (const auto &vf : files)
        {
            if (vf.folderYear > 0)
            {
                int d = (vf.folderYear / 10) * 10;
                decades.insert(QString::number(d) + "s");
            }
            if (!vf.qualityTier.isEmpty() && vf.qualityTier != "Unknown")
                qualities.insert(vf.qualityTier);
        }
        ui->filterPanel->setDecades(decades.values());
        ui->filterPanel->setQualities(qualities.values());
    }

    // ========================================================================
    // ACTION BUTTONS
    // ========================================================================

    void MainWindow::onOpenFile(const QString &filePath)
    {
        QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
    }

    void MainWindow::onImdbClicked(const QString &title, const QString &year,
                                   const QString &imdbId)
    {
        if (!imdbId.isEmpty())
        {
            QString url = QString("https://www.imdb.com/title/%1/").arg(imdbId);
            QDesktopServices::openUrl(QUrl(url));
        }
        else
        {
            QString query = title + " " + year;
            QString url = "https://www.imdb.com/find/?q=" + QUrl::toPercentEncoding(query);
            QDesktopServices::openUrl(QUrl(url));
        }
    }

    void MainWindow::onPaheClicked(const QString &title, const QString &year, const QString &imdbId)
    {
        QString url;
        if (!imdbId.isEmpty())
        {
            url = "https://pahe.ink/?s=" + QUrl::toPercentEncoding(imdbId);
        }
        else
        {
            QString query = title + " " + year;
            url = "https://pahe.ink/?s=" + QUrl::toPercentEncoding(query);
        }
        QDesktopServices::openUrl(QUrl(url));
    }

    // ========================================================================
    // CONTEXT MENU
    // ========================================================================

    void MainWindow::onContextMenu(const QPoint &pos)
    {
        QModelIndex proxyIdx = ui->tableView->indexAt(pos);
        if (!proxyIdx.isValid())
            return;

        m_contextMenuRow = m_proxyModel->mapToSource(proxyIdx).row();

        QMenu menu(this);
        menu.addAction("Refresh Movie Data from IMDb", this, &MainWindow::onRefreshMovieData);
        menu.addSeparator();
        menu.addAction("Open Containing Folder", this, &MainWindow::onOpenFolder);
        menu.addAction("Move To Other Disk", this, &MainWindow::onMoveToOtherDisk);
        menu.addAction("Rename Folder", this, &MainWindow::onRenameFolder);
        menu.addAction("Organize by Aspect Ratio", this, &MainWindow::onOrganizeByAspectRatio);

        menu.exec(ui->tableView->viewport()->mapToGlobal(pos));
    }

    void MainWindow::onRefreshMovieData()
    {
        if (m_contextMenuRow < 0)
            return;

        const auto &vf = m_tableModel->fileAt(m_contextMenuRow);
        m_pendingRefreshTitle = vf.folderTitle;
        m_pendingRefreshYear = vf.folderYear;

        m_omdbService->fetchMovie(vf.folderTitle, vf.folderYear, true);
    }

    void MainWindow::onOpenFolder()
    {
        if (m_contextMenuRow < 0)
            return;

        const auto &vf = m_tableModel->fileAt(m_contextMenuRow);
        QFileInfo fi(vf.filePath);
        QDir dir = fi.dir();

        // If the directory exists, open it
        if (dir.exists())
        {
            QDesktopServices::openUrl(QUrl::fromLocalFile(dir.absolutePath()));
            return;
        }

        // Fallback: try to locate a folder with the same movie title under the parent directory
        QDir parent = dir;
        if (parent.cdUp())
        {
            QString searchName = vf.folderTitle;
            if (vf.folderYear > 0)
                searchName += " (" + QString::number(vf.folderYear) + ")";

            auto entries = parent.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
            for (const QString &entry : entries)
            {
                if (entry.compare(searchName, Qt::CaseInsensitive) == 0 ||
                    entry.contains(vf.folderTitle, Qt::CaseInsensitive))
                {
                    QDesktopServices::openUrl(QUrl::fromLocalFile(parent.filePath(entry)));
                    return;
                }
            }
        }

        QMessageBox::warning(this, "Error", "Containing folder not found.");
    }

    void MainWindow::onRenameFolder()
    {
        if (m_contextMenuRow < 0)
            return;

        const auto &vf = m_tableModel->fileAt(m_contextMenuRow);
        QFileInfo fi(vf.filePath);
        QDir movieDir = fi.dir();

        QString title = vf.folderTitle;
        QString year = vf.folderYear > 0 ? QString::number(vf.folderYear) : "";

        RenameFolderDialog dlg(title, year, this);
        if (dlg.exec() != QDialog::Accepted)
            return;

        QString newTitle = dlg.newTitle();
        QString newYear = dlg.newYear();
        if (newTitle.isEmpty())
        {
            QMessageBox::warning(this, "Error", "Movie name cannot be empty.");
            return;
        }

        QString newName = newTitle;
        if (!newYear.isEmpty())
            newName += " (" + newYear + ")";
        QString originalEntered = newName;
        newName = sanitizeForWindowsFolder(newName).replace('\n', ' ').trimmed();
        if (newName != originalEntered)
        {
            QMessageBox::StandardButton reply = QMessageBox::question(
                this, "Sanitized Folder Name",
                QString("The name you entered contains characters that are not allowed on Windows and will be changed to:\n\n%1\n\nProceed?").arg(newName),
                QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
            if (reply != QMessageBox::Yes)
                return;
        }
        if (newName.isEmpty())
        {
            QMessageBox::warning(this, "Error", "Invalid folder name after sanitization.");
            return;
        }

        QDir parentDir = movieDir;
        if (!parentDir.cdUp())
        {
            QMessageBox::warning(this, "Error", "Unable to determine parent directory for rename.");
            return;
        }

        QString newPath = QDir::cleanPath(parentDir.absoluteFilePath(newName));
        QString currentPath = movieDir.absolutePath();

        // Safety: prevent moving into the same folder or into a subdirectory of itself
        QString cleanCurrent = QDir::cleanPath(currentPath);
        QString cleanNew = QDir::cleanPath(newPath);
        if (cleanNew == cleanCurrent)
        {
            QMessageBox::warning(this, "Error", "Target folder is identical to current folder. Rename aborted.");
            return;
        }
        if (cleanNew.startsWith(cleanCurrent + QDir::separator()))
        {
            QMessageBox::warning(this, "Error", "Target folder would be created inside the source folder. Rename aborted.");
            return;
        }

        // Allow case-only renames
        bool isCaseOnly = QDir::cleanPath(currentPath).toLower() == QDir::cleanPath(newPath).toLower();
        if (QDir(newPath).exists() && !isCaseOnly)
        {
            QMessageBox::warning(this, "Error", "Target folder already exists.");
            return;
        }

        const QString src = QDir::toNativeSeparators(currentPath);
        const QString dst = QDir::toNativeSeparators(newPath);
        const int row = m_contextMenuRow;
        runFolderMoveAsync(src, dst, "Renaming folder...", [this, row, vf, fi, newTitle, newYear, dst](bool ok)
                           {
            if (ok)
            {
                Core::VideoFile updated = vf;
                updated.filePath = dst + QDir::separator() + fi.fileName();
                updated.folderTitle = newTitle;
                updated.folderYear = newYear.toInt();
                m_tableModel->updateFile(row, updated);
                m_omdbService->fetchMovie(newTitle, newYear.toInt());
                m_settings->setLastFolder(dst);
                QMessageBox::information(this, "Success", "Folder renamed successfully.");
            }
            else
            {
                QMessageBox::warning(this, "Error", "Failed to rename folder. Check permissions.");
            } });
    }

    void MainWindow::onMoveToOtherDisk()
    {
        if (m_contextMenuRow < 0)
            return;

        const auto &vf = m_tableModel->fileAt(m_contextMenuRow);
        QFileInfo fi(vf.filePath);
        QDir movieDir = fi.dir();

        QString abs = QDir::cleanPath(movieDir.absolutePath());
        QString native = QDir::fromNativeSeparators(abs); // use '/' separators
        if (native.size() < 2)
        {
            QMessageBox::warning(this, "Error", "Invalid movie folder path.");
            return;
        }

        QString drive = native.mid(0, 2); // e.g. "E:"
        QString otherDrive;
        if (drive.compare("D:", Qt::CaseInsensitive) == 0)
            otherDrive = "E:";
        else if (drive.compare("E:", Qt::CaseInsensitive) == 0)
            otherDrive = "D:";
        else
        {
            QMessageBox::warning(this, "Unsupported Drive", "Move to other disk only supports D: <-> E: drives.");
            return;
        }

        // Split path into parts after drive
        QString rest = native.mid(2);
        if (rest.startsWith('/'))
            rest = rest.mid(1);
        QStringList parts = rest.split('/', Qt::SkipEmptyParts);
        if (parts.isEmpty())
        {
            QMessageBox::warning(this, "Error", "Cannot determine folder hierarchy.");
            return;
        }

        QString movieFolderName = parts.takeLast(); // last segment is the movie folder
        // parts now contains the parent path segments

        QString chosenTargetBase;
        // Try same depth down to shallowest parent
        for (int k = 0; k <= parts.size(); ++k)
        {
            int endIndex = parts.size() - k; // number of segments to keep
            QStringList candidateParts = parts.mid(0, endIndex);
            QString candidatePath;
            if (candidateParts.isEmpty())
            {
                // otherDrive is like "E:", append separator -> "E:\"
                candidatePath = otherDrive + QDir::separator();
            }
            else
            {
                // Build path like "E:\parent\sub"
                QString joined = candidateParts.join(QDir::separator());
                candidatePath = otherDrive + QDir::separator() + joined;
            }

            QDir candDir(candidatePath);
            if (candDir.exists())
            {
                chosenTargetBase = candDir.absolutePath();
                break;
            }
        }

        if (chosenTargetBase.isEmpty())
        {
            QMessageBox::warning(this, "No Target", "No suitable parent folder found on other drive.");
            return;
        }

        QString finalTarget = QDir::cleanPath(chosenTargetBase + QDir::separator() + movieFolderName);

        // Confirm with the user before moving
        QMessageBox::StandardButton confirm = QMessageBox::question(
            this,
            "Confirm Move",
            QString("Move folder:\n%1\n\nTo:\n%2\n\nProceed?").arg(movieDir.absolutePath(), finalTarget),
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::No);
        if (confirm != QMessageBox::Yes)
            return;

        const QString src = QDir::toNativeSeparators(movieDir.absolutePath());
        const QString dst = QDir::toNativeSeparators(finalTarget);
        const int row = m_contextMenuRow;
        runFolderMoveAsync(src, dst, "Moving folder to other disk...", [this, row, vf, fi, dst](bool ok)
                           {
            if (!ok)
            {
                QMessageBox::warning(this, "Error", "Failed to move folder. Check permissions and available space.");
                return;
            }
            Core::VideoFile updated = vf;
            updated.filePath = dst + QDir::separator() + fi.fileName();
            m_tableModel->updateFile(row, updated);
            m_settings->setLastFolder(dst);
            statusBar()->showMessage("Moved folder to other disk", 4000); });
    }

    void MainWindow::onOrganizeByAspectRatio()
    {
        if (m_contextMenuRow < 0)
            return;

        const auto &vf = m_tableModel->fileAt(m_contextMenuRow);
        if (vf.aspectRatio <= 0)
        {
            QMessageBox::warning(this, "Error", "Invalid aspect ratio.");
            return;
        }

        QString target = Core::aspectRatioFolder(vf.aspectRatio);
        QMessageBox::StandardButton reply = QMessageBox::question(
            this, "Confirm",
            QString("Move to '%1' folder?\n\n%2").arg(target, vf.filePath),
            QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

        if (reply != QMessageBox::Yes)
            return;

        if (m_organizeService->organizeByAspectRatio(vf.filePath, vf.aspectRatio))
        {
            // Update file path in model
            QFileInfo fi(vf.filePath);
            QDir movieDir = fi.dir();
            QDir parentDir = movieDir;
            parentDir.cdUp();
            QString newPath = parentDir.filePath(target) + QDir::separator() + movieDir.dirName() + QDir::separator() + fi.fileName();

            Core::VideoFile updated = vf;
            updated.filePath = newPath;
            m_tableModel->updateFile(m_contextMenuRow, updated);

            statusBar()->showMessage(QString("Moved to %1 folder").arg(target), 3000);
        }
        else
        {
            QMessageBox::warning(this, "Error", "Failed to move folder.");
        }
    }

    // ========================================================================
    // TOOLBAR ACTIONS
    // ========================================================================

    void MainWindow::onFetchByDriveYearClicked()
    {
        QString drive = ui->driveCombo->currentText();
        QString yearText = ui->yearEdit->text().trimmed();

        if (yearText.isEmpty())
        {
            QMessageBox::warning(this, "Input Required", "Please enter a year (e.g., 2023, 1994)");
            return;
        }

        bool ok;
        int year = yearText.toInt(&ok);
        if (!ok || year < 1900 || year > 2100)
        {
            QMessageBox::warning(this, "Invalid Year", "Please enter a valid year between 1900 and 2100");
            return;
        }

        QString folderPath = Services::ScanService::buildFolderPath(drive, year);
        if (!QDir(folderPath).exists())
        {
            QMessageBox::warning(this, "Folder Not Found",
                                 QString("Folder does not exist:\n%1").arg(folderPath));
            return;
        }

        statusBar()->showMessage(QString("Scanning %1...").arg(folderPath));
        processPath(folderPath, false);
    }

    void MainWindow::onShowMoviesToMoveClicked()
    {
        Core::QualityRule rule = m_settings->qualityRule();
        QList<Core::MoveRequest> moves =
            m_organizeService->computeMovesToMove(m_tableModel->allFiles(), rule);

        MovesToMoveDialog *dlg = new MovesToMoveDialog(this);
        dlg->setAttribute(Qt::WA_DeleteOnClose);
        dlg->setMovies(moves);

        connect(dlg, &MovesToMoveDialog::moveRequested,
                this, [this](const QString &from, const QString &to)
                {
                    const QString dst = QDir::toNativeSeparators(to);
                    runFolderMoveAsync(
                        QDir::toNativeSeparators(from), dst, "Moving folder...",
                        [this, dst](bool ok) {
                            if (ok)
                            {
                                m_settings->setLastFolder(dst);
                                statusBar()->showMessage("Move successful", 3000);
                            }
                            else
                            {
                                QMessageBox::warning(this, "Error", "Failed to move folder.");
                            }
                        }); });

        connect(dlg, &MovesToMoveDialog::moveAllWithTeraCopy,
                this, [this](const QList<Core::MoveRequest> &movies)
                {
                // Write TeraCopy file list
                QString listPath = QDir::temp().filePath("movieinfo_move.txt");
                QFile file(listPath);
                if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                    QTextStream out(&file);
                    for (const auto &m : movies)
                        out << QDir::toNativeSeparators(m.currentPath) << "\n";
                    file.close();

                    // Launch TeraCopy
                    QString target = movies.first().targetPath;
                    QDir targetDir(target);
                    targetDir.cdUp();
                    QProcess::startDetached("TeraCopy.exe",
                        {"Move", listPath, QDir::toNativeSeparators(targetDir.absolutePath())});
                } });

        dlg->show();
        statusBar()->showMessage(
            QString("Found %1 movie(s) that need to be moved").arg(moves.size()), 5000);
    }

    void MainWindow::onClearTableClicked()
    {
        QMessageBox::StandardButton reply = QMessageBox::question(
            this, "Clear Table",
            "Clear all movies from table?\n\nThis does not delete any files or database entries.",
            QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

        if (reply == QMessageBox::Yes)
        {
            m_tableModel->clear();
            ui->filterPanel->clearFilters();
            statusBar()->showMessage("Table cleared", 3000);
        }
    }

    void MainWindow::onOrganizeAllClicked()
    {
        int total = m_tableModel->rowCount();
        if (total == 0)
        {
            QMessageBox::information(this, "No Movies", "No movies to organize.");
            return;
        }

        QMessageBox::StandardButton reply = QMessageBox::question(
            this, "Confirm Organize All",
            QString("Organize all %1 movies by aspect ratio?\n\nThis cannot be undone!").arg(total),
            QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

        if (reply != QMessageBox::Yes)
            return;

        int moved = 0, skipped = 0;
        const auto &files = m_tableModel->allFiles();

        for (int i = 0; i < files.size(); ++i)
        {
            const auto &vf = files[i];
            if (vf.aspectRatio <= 0)
            {
                skipped++;
                continue;
            }

            if (m_organizeService->organizeByAspectRatio(vf.filePath, vf.aspectRatio))
                moved++;
            else
                skipped++; // already in place or failed
        }

        QMessageBox::information(this, "Complete",
                                 QString("Moved: %1\nSkipped: %2").arg(moved).arg(skipped));
        statusBar()->showMessage(
            QString("Organized %1 movies, skipped %2").arg(moved).arg(skipped), 5000);
    }

    void MainWindow::onExportClicked()
    {
        QString filePath = QFileDialog::getSaveFileName(
            this, "Export", "", "CSV Files (*.csv);;JSON Files (*.json)");
        if (filePath.isEmpty())
            return;

        Services::ExportService::Format format = filePath.endsWith(".json")
                                                     ? Services::ExportService::Format::JSON
                                                     : Services::ExportService::Format::CSV;

        if (m_exportService->exportData(m_proxyModel, filePath, format))
            QMessageBox::information(this, "Export", "Export completed successfully.");
        else
            QMessageBox::warning(this, "Export Error", "Failed to export data.");
    }

    void MainWindow::onSettingsClicked()
    {
        SettingsDialog dlg(m_settings, this);
        if (dlg.exec() == QDialog::Accepted)
        {
            m_apiClient->setApiKey(m_settings->apiKey());
            applyColumnSettings();
        }
    }

    void MainWindow::runFolderMoveAsync(const QString &src, const QString &dst,
                                        const QString &progressMsg,
                                        std::function<void(bool)> onDone)
    {
        auto *progress = new QProgressDialog(progressMsg, QString(), 0, 0, this);
        progress->setWindowModality(Qt::WindowModal);
        progress->setCancelButton(nullptr);
        progress->setMinimumDuration(0);
        progress->show();

        auto *watcher = new QFutureWatcher<bool>(this);
        connect(watcher, &QFutureWatcher<bool>::finished, this,
                [watcher, progress, onDone = std::move(onDone)]() mutable
                {
                    progress->deleteLater();
                    const bool ok = watcher->result();
                    watcher->deleteLater();
                    onDone(ok);
                });
        watcher->setFuture(QtConcurrent::run([this, src, dst]
                                             { return m_organizeService->moveFolder(src, dst); }));
    }

    void MainWindow::onOpenLastFolderClicked()
    {
        QString lastFolder = m_settings->lastFolder();
        if (lastFolder.isEmpty())
        {
            QMessageBox::information(this, "No Last Folder", "No last folder recorded.");
            return;
        }

        QDir d(lastFolder);
        if (!d.exists())
        {
            QMessageBox::warning(this, "Folder Not Found", "The last folder does not exist: " + lastFolder);
            return;
        }

        // Load the last folder in-app
        statusBar()->showMessage("Loading folder: " + lastFolder);
        processPath(lastFolder, false);
    }

    // ========================================================================
    // DRAG & DROP
    // ========================================================================

    void MainWindow::dragEnterEvent(QDragEnterEvent *event)
    {
        if (event->mimeData()->hasUrls())
        {
            event->acceptProposedAction();
            statusBar()->showMessage("Drop files or folders to add them to the library");
        }
    }

    void MainWindow::dropEvent(QDropEvent *event)
    {
        QList<QUrl> urls = event->mimeData()->urls();
        if (urls.isEmpty())
            return;

        static const QStringList videoExts = {".mp4", ".mkv", ".avi", ".mov", ".flv", ".wmv"};

        int files = 0, folders = 0;
        for (const QUrl &url : urls)
        {
            QString path = url.toLocalFile();
            QFileInfo fi(path);
            if (fi.isFile() && videoExts.contains("." + fi.suffix().toLower()))
            {
                processPath(path, true);
                files++;
            }
            else if (fi.isDir())
            {
                processPath(path, false);
                folders++;
            }
        }

        if (files > 0 || folders > 0)
            statusBar()->showMessage(
                QString("Added %1 files and %2 folders").arg(files).arg(folders), 3000);
    }

    // ========================================================================
    // UTILITY
    // ========================================================================

    QString MainWindow::sanitizeForWindowsFolder(const QString &name)
    {
        static const QRegularExpression forbidden(R"([\\/:*?"<>|,.])");
        return QString(name).remove(forbidden);
    }

} // namespace UI
