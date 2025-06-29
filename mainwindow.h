#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QTableWidget>
#include <QPushButton>
#include "OmdbClient.h"
#include "movie.h"

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
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private slots:
    // Core functionality
    void onFetchClicked();
    void onMovieFetched(const Movie &movie);

    // Filtering / Search / Export
    void filterTable();
    void filterTableRows(const QString &text);
    void exportToExcel();

    // External navigation
    void openImdbPage(const QString &title, const QString &year);
    void openPahePage(const QString &title, const QString &year);

    // UI interaction
    void showContextMenu(const QPoint &pos);

private:
    // UI and app state
    Ui::MainWindow *ui;
    QTableWidget *movieTable;
    int currentRow = 0;

    // Data
    OmdbClient *omdbClient;
    MovieDB *movieDb;

    // Constants
    enum CustomRoles
    {
        FilePathRole = Qt::UserRole + 1
    };

    // Helpers: ComboBoxes
    void addComboBoxItemIfNotExist(QComboBox *comboBox, const QString &item);
    void addComboBoxItemsSorted(QComboBox *comboBox, const QSet<QString> &items, const QString &additionalItem = "");

    // Helpers: Metadata
    QString getVideoResolution(const QString &filePath);
    QString getAspectRatio(const QString &resolution);
    QString getVideoQuality(const QString &filePath);
    QString getDecade(const QString &year);
    QString getFileSize(const QString &filePath);
    QString getVideoDuration(const QString &filePath);
    QString getAudioLanguage(const QString &filePath);
    QPair<QString, QString> parseFolderName(const QString &folderName);
    QString runFfprobe(const QStringList &args);

    // Helpers: Utility
    QString sanitizeForWindowsFolder(const QString &name);
    QString filepath;

    void setupColumnVisibilityMenu();
    void saveColumnVisibilitySettings();
    void loadColumnVisibilitySettings();
    void updateColumnVisibility();
    QMap<int, QString> columnIndexToName;
    QMap<int, bool> columnVisibility;
    QAction *columnVisibilityAction;

    void setupColumnReorderingMenu();
    void saveColumnOrderSettings();
    void loadColumnOrderSettings();
    void reorderColumn(int column, int newPosition);
    QMap<int, int> columnOriginalToCurrentMap; // Maps original column index to current position
    QMap<int, int> columnCurrentToOriginalMap; // Maps current position to original column index
    void updateHeaderLabels();
    void onSelectFolderClicked();
    void processVideos(const QString &path, bool isSingleFile);
    void processVideos(const QStringList &filePaths);
    void loadExternalStylesheet();
};

#endif // MAINWINDOW_H
