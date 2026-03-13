#include "WatchlistDialog.h"
#include "ui_WatchlistDialog.h"

#include "infrastructure/db/MovieRepository.h"
#include "infrastructure/api/OmdbApiClient.h"

#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QHeaderView>
#include <QRegularExpression>
#include <QTableWidgetItem>
#include <QTextStream>
#include <QDesktopServices>
#include <QUrl>
#include <QPushButton>

// ---------------------------------------------------------------------------
// NumericItem: QTableWidgetItem that sorts by a raw numeric value (Qt::UserRole)
// so that columns like Rating, Votes, Year sort correctly instead of as strings.
// ---------------------------------------------------------------------------
namespace
{
    class NumericItem : public QTableWidgetItem
    {
    public:
        NumericItem(double rawValue, const QString &display, bool rightAlign = true)
            : QTableWidgetItem(display)
        {
            setData(Qt::UserRole, rawValue);
            setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
            if (rightAlign)
                setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        }
        bool operator<(const QTableWidgetItem &other) const override
        {
            return data(Qt::UserRole).toDouble() < other.data(Qt::UserRole).toDouble();
        }
    };

    // -----------------------------------------------------------------------
    // addActionButtons — creates styled IMDb + Pahe cell-widget buttons.
    // imdbId may be empty; in that case buttons fall back to title search.
    // Existing cell-widgets at those columns are automatically replaced.
    // -----------------------------------------------------------------------
    static void addActionButtons(QTableWidget *tbl, int row,
                                 int imdbCol, int paheCol,
                                 const QString &imdbId,
                                 const QString &title,
                                 int year,
                                 QWidget *parent)
    {
        // Placeholder items let sorting compare the column
        for (int col : {imdbCol, paheCol})
        {
            auto *ph = new QTableWidgetItem();
            ph->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
            ph->setData(Qt::UserRole, imdbId.isEmpty() ? 0.0 : 1.0);
            tbl->setItem(row, col, ph);
        }

        auto *imdbBtn = new QPushButton("IMDb", parent);
        imdbBtn->setObjectName("imdbBtn");
        imdbBtn->setCursor(Qt::PointingHandCursor);
        QObject::connect(imdbBtn, &QPushButton::clicked, imdbBtn,
                         [imdbId, title, year]()
                         {
                             if (!imdbId.isEmpty())
                             {
                                 QDesktopServices::openUrl(
                                     QUrl(QString("https://www.imdb.com/title/%1/").arg(imdbId)));
                             }
                             else
                             {
                                 QString q = title +
                                             (year > 0 ? (" " + QString::number(year)) : QString());
                                 QDesktopServices::openUrl(
                                     QUrl("https://www.imdb.com/find/?q=" +
                                          QString::fromUtf8(QUrl::toPercentEncoding(q))));
                             }
                         });
        tbl->setCellWidget(row, imdbCol, imdbBtn);

        auto *paheBtn = new QPushButton("Pahe", parent);
        paheBtn->setObjectName("paheBtn");
        paheBtn->setCursor(Qt::PointingHandCursor);
        QObject::connect(paheBtn, &QPushButton::clicked, paheBtn,
                         [imdbId, title, year]()
                         {
                             if (!imdbId.isEmpty())
                             {
                                 QDesktopServices::openUrl(
                                     QUrl("https://pahe.ink/?s=" +
                                          QString::fromUtf8(QUrl::toPercentEncoding(imdbId))));
                             }
                             else
                             {
                                 QString q = title +
                                             (year > 0 ? (" " + QString::number(year)) : QString());
                                 QDesktopServices::openUrl(
                                     QUrl("https://pahe.ink/?s=" +
                                          QString::fromUtf8(QUrl::toPercentEncoding(q))));
                             }
                         });
        tbl->setCellWidget(row, paheCol, paheBtn);
    }

} // anonymous namespace

namespace UI
{

    // ========================================================================
    // Construction
    // ========================================================================

    WatchlistDialog::WatchlistDialog(
        const QList<Core::VideoFile> &scannedFiles,
        const QString &apiKey,
        Infrastructure::MovieRepository *movieRepo,
        QWidget *parent)
        : QDialog(parent), ui(new Ui::WatchlistDialog), m_scannedFiles(scannedFiles), m_movieRepo(movieRepo)
    {
        ui->setupUi(this);

        m_apiClient = new Infrastructure::OmdbApiClient(apiKey, this);
        connect(m_apiClient, &Infrastructure::OmdbApiClient::movieFetched,
                this, &WatchlistDialog::onMovieFetched);
        connect(m_apiClient, &Infrastructure::OmdbApiClient::fetchFailed,
                this, &WatchlistDialog::onFetchFailed);

        // Have table (cols 0-10 : # Title Year Rating Votes Genre Director AR Quality IMDb Pahe)
        auto *haveHdr = ui->haveTable->horizontalHeader();
        haveHdr->setSectionResizeMode(QHeaderView::ResizeToContents);
        haveHdr->setSectionResizeMode(1, QHeaderView::Stretch); // Title
        haveHdr->setSectionResizeMode(9, QHeaderView::Fixed);   // IMDb
        haveHdr->setSectionResizeMode(10, QHeaderView::Fixed);  // Pahe
        haveHdr->setStretchLastSection(false);
        haveHdr->setHighlightSections(false);
        ui->haveTable->setColumnWidth(9, 62);
        ui->haveTable->setColumnWidth(10, 62);
        ui->haveTable->verticalHeader()->setVisible(false);
        ui->haveTable->setMouseTracking(true);

        // Missing table (cols 0-8 : # Title Year Rating Votes Genre Director IMDb Pahe)
        auto *missHdr = ui->missingTable->horizontalHeader();
        missHdr->setSectionResizeMode(QHeaderView::ResizeToContents);
        missHdr->setSectionResizeMode(1, QHeaderView::Stretch); // Title
        missHdr->setSectionResizeMode(7, QHeaderView::Fixed);   // IMDb
        missHdr->setSectionResizeMode(8, QHeaderView::Fixed);   // Pahe
        missHdr->setStretchLastSection(false);
        missHdr->setHighlightSections(false);
        ui->missingTable->setColumnWidth(7, 62);
        ui->missingTable->setColumnWidth(8, 62);
        ui->missingTable->verticalHeader()->setVisible(false);
        ui->missingTable->setMouseTracking(true);

        connect(ui->importBtn, &QPushButton::clicked, this, &WatchlistDialog::onImportClicked);
        connect(ui->closeBtn, &QPushButton::clicked, this, &QDialog::close);
    }

    WatchlistDialog::~WatchlistDialog()
    {
        delete ui;
    }

    // ========================================================================
    // Import
    // ========================================================================

    void WatchlistDialog::onImportClicked()
    {
        const QString path = QFileDialog::getOpenFileName(
            this, tr("Import Watchlist"), QString(),
            tr("Text Files (*.txt);;All Files (*)"));
        if (path.isEmpty())
            return;
        loadFromFile(path);
    }

    // Lowercase, keep only alphanumeric + spaces, collapse whitespace
    QString WatchlistDialog::normalizeTitle(const QString &title)
    {
        static const QRegularExpression re(QStringLiteral("[^a-z0-9\\s]"));
        QString s = title.toLower().trimmed();
        s.remove(re);
        return s.simplified();
    }

    void WatchlistDialog::loadFromFile(const QString &path)
    {
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;

        // Reset state
        m_haveEntries.clear();
        m_missingEntries.clear();
        m_fetchQueue.clear();
        m_fetchedCount = 0;
        m_totalToFetch = 0;

        ui->haveTable->setSortingEnabled(false);
        ui->haveTable->setRowCount(0);
        ui->missingTable->setSortingEnabled(false);
        ui->missingTable->setRowCount(0);
        ui->progressBar->setVisible(false);
        ui->fetchStatusLabel->clear();

        // Build a normalised-title → scanned-file index
        QHash<QString, int> normIndex;
        for (int i = 0; i < m_scannedFiles.size(); ++i)
            normIndex[normalizeTitle(m_scannedFiles[i].folderTitle)] = i;

        QTextStream in(&file);
        int listIndex = 0;

        while (!in.atEnd())
        {
            const QString line = in.readLine().trimmed();
            if (line.isEmpty())
                continue;

            ++listIndex;
            WatchlistEntry entry;
            entry.listIndex = listIndex;
            entry.inputTitle = line;

            const auto it = normIndex.constFind(normalizeTitle(line));
            if (it != normIndex.constEnd())
            {
                entry.haveIt = true;
                entry.videoFile = m_scannedFiles[it.value()];
                m_haveEntries.append(entry);
            }
            else
            {
                m_missingEntries.append(entry);
            }
        }

        const QFileInfo fi(path);
        ui->fileLabel->setText(
            QString("%1 — %2 entries").arg(fi.fileName()).arg(listIndex));

        populateHaveTable();
        populateMissingTable();

        ui->tabWidget->setTabText(0,
                                  QString("Have (%1)").arg(m_haveEntries.size()));
        ui->tabWidget->setTabText(1,
                                  QString("Don't Have (%1)").arg(m_missingEntries.size()));

        ui->summaryLabel->setText(
            QString("Total: %1  |  Have: %2  |  Missing: %3")
                .arg(listIndex)
                .arg(m_haveEntries.size())
                .arg(m_missingEntries.size()));

        ui->haveTable->setSortingEnabled(true);

        if (!m_missingEntries.isEmpty())
            startFetchQueue();
        else
            ui->missingTable->setSortingEnabled(true);
    }

    // ========================================================================
    // Table helpers
    // ========================================================================

    static QTableWidgetItem *makeItem(const QString &text, bool rightAlign = false)
    {
        auto *item = new QTableWidgetItem(text);
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        if (rightAlign)
            item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        return item;
    }

    static QColor ratingColor(double r)
    {
        if (r <= 0.0)
            return {};
        if (r >= 9.0)
            return {255, 215, 0}; // gold
        if (r >= 8.0)
            return {0, 200, 180}; // teal
        if (r >= 7.0)
            return {80, 200, 80}; // green
        if (r >= 6.0)
            return {220, 190, 60}; // yellow
        if (r >= 4.0)
            return {220, 120, 40}; // orange
        return {220, 60, 60};      // red
    }

    void WatchlistDialog::populateHaveTable()
    {
        auto *tbl = ui->haveTable;
        tbl->setRowCount(m_haveEntries.size());

        for (int i = 0; i < m_haveEntries.size(); ++i)
        {
            const WatchlistEntry &e = m_haveEntries[i];
            const Core::VideoFile &vf = e.videoFile;

            QString yearStr = vf.folderYear > 0 ? QString::number(vf.folderYear) : QStringLiteral("—");
            QString ratingStr = QStringLiteral("—");
            QString votesStr = QStringLiteral("—");
            QString genre, director, imdbId;
            double rating = 0.0;
            int votes = 0;

            if (vf.metadata.has_value())
            {
                const Core::Movie &m = *vf.metadata;
                if (m.imdbRating > 0.0)
                {
                    ratingStr = m.formattedRating();
                    rating = m.imdbRating;
                }
                if (m.imdbVotes > 0)
                {
                    votesStr = m.formattedVotes();
                    votes = m.imdbVotes;
                }
                genre = m.genre;
                director = m.director;
                imdbId = m.imdbId;
            }

            tbl->setItem(i, 0, new NumericItem(e.listIndex, QString::number(e.listIndex)));
            tbl->setItem(i, 1, makeItem(e.inputTitle));
            tbl->setItem(i, 2, new NumericItem(vf.folderYear > 0 ? vf.folderYear : 0, yearStr));

            auto *ratingItem = new NumericItem(rating, ratingStr);
            const QColor rc = ratingColor(rating);
            if (rc.isValid())
                ratingItem->setForeground(rc);
            tbl->setItem(i, 3, ratingItem);

            tbl->setItem(i, 4, new NumericItem(votes, votesStr));
            tbl->setItem(i, 5, makeItem(genre));
            tbl->setItem(i, 6, makeItem(director));

            const QString aspectStr = vf.aspectRatio > 0.0
                                          ? QString::number(vf.aspectRatio, 'f', 2)
                                          : QStringLiteral("—");
            tbl->setItem(i, 7, makeItem(aspectStr));
            tbl->setItem(i, 8, makeItem(vf.qualityTier));

            addActionButtons(tbl, i, 9, 10, imdbId, e.inputTitle, vf.folderYear, this);
        }
    }

    void WatchlistDialog::populateMissingTable()
    {
        auto *tbl = ui->missingTable;
        tbl->setRowCount(m_missingEntries.size());

        for (int i = 0; i < m_missingEntries.size(); ++i)
        {
            const WatchlistEntry &e = m_missingEntries[i];
            tbl->setItem(i, 0, new NumericItem(e.listIndex, QString::number(e.listIndex)));
            tbl->setItem(i, 1, makeItem(e.inputTitle));
            tbl->setItem(i, 2, new NumericItem(0.0, QStringLiteral("Fetching\u2026")));
            tbl->setItem(i, 3, new NumericItem(0.0, QString()));
            tbl->setItem(i, 4, new NumericItem(0.0, QString()));
            tbl->setItem(i, 5, makeItem(QString()));
            tbl->setItem(i, 6, makeItem(QString()));
            // Buttons with empty imdbId; re-created in updateMissingRow once imdbId is known
            addActionButtons(tbl, i, 7, 8, QString(), e.inputTitle, 0, this);
        }
    }

    // ========================================================================
    // Sequential OMDB fetch for missing movies
    // ========================================================================

    void WatchlistDialog::startFetchQueue()
    {
        m_fetchQueue.clear();
        m_fetchedCount = 0;

        for (int i = 0; i < m_missingEntries.size(); ++i)
        {
            // Check DB cache first — avoids a redundant API call if this title
            // was fetched in a previous session and saved to the database.
            Core::Movie cached = m_movieRepo->findByTitle(m_missingEntries[i].inputTitle);
            if (cached.isValid())
            {
                updateMissingRow(i, cached, false);
                continue;
            }

            FetchTask task;
            task.missingTableRow = i;
            task.inputTitle = m_missingEntries[i].inputTitle;
            m_fetchQueue.append(task);
        }

        m_totalToFetch = m_fetchQueue.size();

        if (m_fetchQueue.isEmpty())
        {
            ui->fetchStatusLabel->setText(tr("All data loaded from cache"));
            ui->missingTable->setSortingEnabled(true);
            return;
        }

        ui->progressBar->setRange(0, m_totalToFetch);
        ui->progressBar->setValue(0);
        ui->progressBar->setVisible(true);

        fetchNext();
    }

    void WatchlistDialog::fetchNext()
    {
        if (m_fetchQueue.isEmpty())
        {
            ui->progressBar->setVisible(false);
            ui->fetchStatusLabel->setText(tr("Done"));
            ui->missingTable->setSortingEnabled(true);
            return;
        }

        const FetchTask &task = m_fetchQueue.constFirst();
        ui->fetchStatusLabel->setText(tr("Fetching: %1").arg(task.inputTitle));
        m_apiClient->fetch(task.inputTitle, 0);
    }

    void WatchlistDialog::onMovieFetched(const Core::Movie &movie)
    {
        if (m_fetchQueue.isEmpty())
            return;

        const FetchTask task = m_fetchQueue.takeFirst();

        // Persist to DB so future runs benefit from the cache
        m_movieRepo->save(movie);

        updateMissingRow(task.missingTableRow, movie, false);

        ++m_fetchedCount;
        ui->progressBar->setValue(m_fetchedCount);
        fetchNext();
    }

    void WatchlistDialog::onFetchFailed(const QString & /*title*/, const QString & /*error*/)
    {
        if (m_fetchQueue.isEmpty())
            return;

        const FetchTask task = m_fetchQueue.takeFirst();
        updateMissingRow(task.missingTableRow, {}, true);

        ++m_fetchedCount;
        ui->progressBar->setValue(m_fetchedCount);
        fetchNext();
    }

    void WatchlistDialog::updateMissingRow(int row, const Core::Movie &movie, bool failed)
    {
        auto *tbl = ui->missingTable;
        if (row < 0 || row >= tbl->rowCount())
            return;

        const QString &inputTitle = m_missingEntries[row].inputTitle;

        if (failed)
        {
            tbl->item(row, 2)->setText(QStringLiteral("Not Found"));
            tbl->item(row, 2)->setData(Qt::UserRole, 0.0);
            // Provide search-by-title buttons even when OMDb has no result
            addActionButtons(tbl, row, 7, 8, QString(), inputTitle, 0, this);
            return;
        }

        tbl->item(row, 2)->setText(movie.year > 0 ? QString::number(movie.year) : QStringLiteral("\u2014"));
        tbl->item(row, 2)->setData(Qt::UserRole, static_cast<double>(movie.year));

        QTableWidgetItem *ratingItem = tbl->item(row, 3);
        if (movie.imdbRating > 0.0)
        {
            ratingItem->setText(movie.formattedRating());
            ratingItem->setData(Qt::UserRole, movie.imdbRating);
            const QColor rc = ratingColor(movie.imdbRating);
            if (rc.isValid())
                ratingItem->setForeground(rc);
        }
        else
        {
            ratingItem->setText(QStringLiteral("\u2014"));
            ratingItem->setData(Qt::UserRole, 0.0);
        }

        const double votes = static_cast<double>(movie.imdbVotes);
        tbl->item(row, 4)->setText(movie.imdbVotes > 0 ? movie.formattedVotes() : QStringLiteral("\u2014"));
        tbl->item(row, 4)->setData(Qt::UserRole, votes);
        tbl->item(row, 5)->setText(movie.genre);
        tbl->item(row, 6)->setText(movie.director);

        // Re-create buttons now that the real imdbId is available
        addActionButtons(tbl, row, 7, 8, movie.imdbId, inputTitle, movie.year, this);
    }

} // namespace UI
