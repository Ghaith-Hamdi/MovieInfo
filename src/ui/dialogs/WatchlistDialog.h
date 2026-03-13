#ifndef UI_DIALOGS_WATCHLISTDIALOG_H
#define UI_DIALOGS_WATCHLISTDIALOG_H

#include <QDialog>
#include <QList>
#include <QHash>

#include "core/VideoFile.h"
#include "core/Movie.h"

namespace Ui
{
    class WatchlistDialog;
}

namespace Infrastructure
{
    class MovieRepository;
    class OmdbApiClient;
}

namespace UI
{

    class WatchlistDialog : public QDialog
    {
        Q_OBJECT

    public:
        explicit WatchlistDialog(
            const QList<Core::VideoFile> &scannedFiles,
            const QString &apiKey,
            Infrastructure::MovieRepository *movieRepo,
            QWidget *parent = nullptr);
        ~WatchlistDialog();

    private slots:
        void onImportClicked();
        void onMovieFetched(const Core::Movie &movie);
        void onFetchFailed(const QString &title, const QString &error);

    private:
        struct WatchlistEntry
        {
            int listIndex = 0;       // 1-based position in the input file
            QString inputTitle;      // raw title from the text file
            bool haveIt = false;
            Core::VideoFile videoFile; // valid when haveIt == true
        };

        struct FetchTask
        {
            int missingTableRow = 0;
            QString inputTitle;
        };

        void loadFromFile(const QString &path);
        void populateHaveTable();
        void populateMissingTable();
        void startFetchQueue();
        void fetchNext();
        void updateMissingRow(int row, const Core::Movie &movie, bool failed = false);

        static QString normalizeTitle(const QString &title);

        Ui::WatchlistDialog *ui;

        QList<Core::VideoFile> m_scannedFiles;
        Infrastructure::OmdbApiClient *m_apiClient;
        Infrastructure::MovieRepository *m_movieRepo;

        QList<WatchlistEntry> m_haveEntries;
        QList<WatchlistEntry> m_missingEntries;
        QList<FetchTask> m_fetchQueue;
        int m_fetchedCount = 0;
        int m_totalToFetch = 0;
    };

} // namespace UI

#endif // UI_DIALOGS_WATCHLISTDIALOG_H
