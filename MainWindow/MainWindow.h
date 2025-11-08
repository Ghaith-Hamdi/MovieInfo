#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QTableWidget>
#include <QPushButton>
#include <optional>
#include <QFileInfo>
#include <QProgressDialog>
#include "OmdbClient/OmdbClient.h"
#include "Movie/Movie.h"
#include "TableColumnManager/TableColumnManager.h"
#include "MovieDataRefresher/MovieDataRefresher.h"

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
    void onRefreshMovieClicked();
    void onMovieExistsInDatabase(const QString &movieName, const Movie &existingMovie);

    // ===== Table Filtering & Search =====
    void filterTable();
    void filterTableRows(const QString &text);

    // ===== Context Menu Actions =====
    void showContextMenu(const QPoint &pos);
    void onOpenFolderClicked();
    void onRenameFolderClicked();
    void onMoveFolderToArchiveClicked();

    // ===== Action Buttons =====
    void onOpenFileClicked();
    void onImdbButtonClicked();
    void onPaheButtonClicked();

    // ===== Export =====
    void exportToExcel();

    // ===== Utility Slots =====
    void cleanupProgressDialog();

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

    // ===== Video Processing Methods =====
    void processVideos(const QString &path, bool isSingleFile);
    void processVideos(const QStringList &filePaths);
    QPair<QString, QString> parseFolderName(const QString &folderName);

    // ===== Video Metadata Methods =====
    VideoMetadata getVideoMetadataBatch(const QString &filePath);
    QString runFfprobe(const QStringList &args);
    QString getVideoResolution(const QString &filePath);
    QString getAspectRatio(const QString &resolution);
    QString getVideoQuality(const QString &filePath);
    QString getVideoDuration(const QString &filePath);
    QString getAudioLanguage(const QString &filePath);
    QString getFileSize(const QString &filePath);
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
};

#endif // MAINWINDOW_H
