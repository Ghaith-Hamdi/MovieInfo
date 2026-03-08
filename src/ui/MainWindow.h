#ifndef MOVIEINFO_MAINWINDOW_H
#define MOVIEINFO_MAINWINDOW_H

#include <QMainWindow>
#include <QProgressDialog>

class Ui_MainWindow;

class QProgressBar;
class QLabel;

#include <QDragEnterEvent>
#include <QDropEvent>

// Infrastructure
#include "infrastructure/db/Database.h"
#include "infrastructure/db/MovieRepository.h"
#include "infrastructure/db/VideoMetadataRepository.h"
#include "infrastructure/settings/AppSettings.h"
#include "infrastructure/api/OmdbApiClient.h"

// Services
#include "services/ScanService.h"
#include "services/OmdbService.h"
#include "services/OrganizeService.h"
#include "services/ExportService.h"

// Models
#include "models/MovieTableModel.h"
#include "models/MovieFilterProxyModel.h"

// UI
#include "ui/FilterPanel.h"
#include "ui/ActionButtonDelegate.h"

namespace UI
{

    class MainWindow : public QMainWindow
    {
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    protected:
        void dragEnterEvent(QDragEnterEvent *event) override;
        void dropEvent(QDropEvent *event) override;

    private slots:
        // Scanning
        void onSelectFolderClicked();
        void onFetchByDriveYearClicked();
        void onFileProcessed(const Core::VideoFile &file);
        void onScanComplete(int total, int cached, int fresh);
        void onScanStarted(int totalFiles);
        void onScanProgress(int current, int total, const QString &currentFile);

        // Metadata
        void onMovieFetched(const Core::Movie &movie);
        void onMovieFromCache(const Core::Movie &movie);
        void onMovieExistsInDatabase(const QString &title, const Core::Movie &existing);
        void onFetchFailed(const QString &title, const QString &error);

        // Filtering
        void onFilterChanged(const Core::FilterCriteria &criteria);

        // Actions
        void onOpenFile(const QString &filePath);
        void onImdbClicked(const QString &title, const QString &year, const QString &imdbId);
        void onPaheClicked(const QString &title, const QString &year, const QString &imdbId);

        // Context menu
        void onContextMenu(const QPoint &pos);
        void onRefreshMovieData();
        void onOpenFolder();
        void onRenameFolder();
        void onMoveToOtherDisk();
        void onOrganizeByAspectRatio();

        // Toolbar actions
        void onOrganizeAllClicked();
        void onShowMoviesToMoveClicked();
        void onClearTableClicked();
        void onOpenLastFolderClicked();
        void onExportClicked();
        void onSettingsClicked();

    private:
        void setupConnections();
        void loadStylesheet();

        void processPath(const QString &path, bool isSingleFile);
        void fetchMetadataForRow(int row);
        void updateFilterCombos();
        void applyColumnSettings();

        QString sanitizeForWindowsFolder(const QString &name);

        // Infrastructure
        Infrastructure::Database *m_db;
        Infrastructure::MovieRepository *m_movieRepo;
        Infrastructure::VideoMetadataRepository *m_videoRepo;
        Infrastructure::AppSettings *m_settings;
        Infrastructure::OmdbApiClient *m_apiClient;

        // Services
        Services::ScanService *m_scanService;
        Services::OmdbService *m_omdbService;
        Services::OrganizeService *m_organizeService;
        Services::ExportService *m_exportService;

        // Models
        Models::MovieTableModel *m_tableModel;
        Models::MovieFilterProxyModel *m_proxyModel;

        // Generated UI
        Ui_MainWindow *ui;

        // Non-UI members
        ActionButtonDelegate *m_actionDelegate;
        QProgressDialog *m_progressDialog;
        QProgressBar *m_scanProgressBar;
        QLabel *m_currentFolderLabel;

        // State
        int m_contextMenuRow = -1;
        int m_totalToFetch = 0;
        int m_fromDb = 0;
        int m_fromApi = 0;
        QStringList m_fetchedFromImdbList;
        QString m_pendingRefreshTitle;
        int m_pendingRefreshYear = 0;
    };

} // namespace UI

#endif // MOVIEINFO_MAINWINDOW_H
