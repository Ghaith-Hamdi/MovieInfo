#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <optional>
#include <QFileInfo>
#include <QProgressDialog>
#include "OmdbClient/OmdbClient.h"
#include "Movie/Movie.h"
#include "MovieDatabase/MovieDatabase.h"
#include "TableColumnManager/TableColumnManager.h"
#include "MovieDataRefresher/MovieDataRefresher.h"
#include "MoviesToMoveWindow/MoviesToMoveWindow.h"
#include "SettingsDialog/SettingsDialog.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    // Drag & Drop Events
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private slots:
    // ===== Video Processing =====
    void onSelectFolderClicked();

    // ===== OMDb API / Movie Data =====
    void onFetchClicked();
    void onMovieFetched(const Movie &movie);
    void onMovieFetchedFromDatabase(const Movie &movie);
    void onRefreshMovieClicked();
    void onMovieExistsInDatabase(const QString &movieName, const Movie &existingMovie);

    // ===== Table Filtering & Search =====
    void filterTable();
    void filterTableRows(const QString &text);

    // ===== Context Menu Actions =====
    void showContextMenu(const QPoint &pos);
    void onOpenFolderClicked();
    void onRenameFolderClicked();

    // ===== Action Buttons =====
    void onOpenFileClicked();
    void onImdbButtonClicked();
    void onPaheButtonClicked();
    void onOrganizeByAspectRatioClicked();
    void onOrganizeAllByAspectRatioClicked();

    // ===== Drive & Year Selection =====
    void onFetchByDriveYearClicked();
    void onShowMoviesToMoveClicked();

    // ===== Table Management =====
    void onClearTableClicked();

    // ===== Export =====
    void exportToExcel();

    // ===== Settings =====
    void onSettingsClicked();

    // ===== Utility Slots =====
    void cleanupProgressDialog();
    void showFetchSummary();

private:
    // ===== Constants & Enums =====
    enum CustomRoles
    {
        FilePathRole = Qt::UserRole + 1,
        ImdbIdRole = Qt::UserRole + 2
    };

    // ===== Data Structures =====
    struct VideoMetadata
    {
        QString resolution;
        QString aspectRatio;
        QString quality;
        QString duration;
        QString audioLanguage;
        QString fileSize;
    };

    // ===== UI Components =====
    Ui::MainWindow *ui;
    QProgressDialog *progressDialog;
    QComboBox *driveComboBox;
    QLineEdit *yearLineEdit;
    QPushButton *fetchByDriveYearButton;
    QPushButton *showMoviesToMoveButton;

    // ===== Helper Classes =====
    OmdbClient *omdbClient;
    MovieDB *movieDb;
    TableColumnManager *columnManager;
    MovieDataRefresher *dataRefresher;

    // ===== State Variables =====
    int contextMenuRow;
    QString pendingRefreshMovieName;
    int pendingRefreshMovieYear;
    int totalMoviesToFetch;
    int moviesFetched;
    int moviesFromDatabase;
    int moviesFromImdb;
    QStringList moviesFetchedFromImdbList; // Track movie titles fetched from IMDb

    // ===== Video Processing Methods =====
    void processVideos(const QString &path, bool isSingleFile);
    void processVideos(const QStringList &filePaths);
    QPair<QString, QString> parseFolderName(const QString &folderName);

    // ===== Video Metadata Methods =====
    VideoMetadata getVideoMetadataBatch(const QString &filePath);
    QString runFfprobe(const QStringList &args);
    QString getDecade(const QString &year);

    // ===== UI Widget Creation =====
    QPushButton *createActionButton(const QString &text, const QString &iconPath,
                                    const QString &styleSheet, const QString &objectName = "");
    QWidget *createActionButtonsWidget(const QString &filePath, const QString &title, const QString &year);

    // ===== ComboBox Helpers =====
    void addComboBoxItemIfNotExist(QComboBox *comboBox, const QString &item);
    void addComboBoxItemsSorted(QComboBox *comboBox, const QSet<QString> &items,
                                const QString &additionalItem = "");

    // ===== External Navigation =====
    void openImdbPage(const QString &title, const QString &year);
    void openPahePage(const QString &title, const QString &year);

    // ===== Utility Methods =====
    QString sanitizeForWindowsFolder(const QString &name);
    std::optional<QFileInfo> getFileInfoForRow(int row);
    void loadExternalStylesheet();
    QString buildFolderPath(const QString &drive, int year);
    bool meetsHighQualityCriteria(int votes, double rating);
    void updateRowFilePath(int row, const QString &newFilePath);
};

#endif // MAINWINDOW_H
