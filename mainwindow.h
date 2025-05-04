#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QTableWidget>
#include <QPushButton>
#include <OmdbClient.h>
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

private slots:
    void processVideos(const QString &folderPath);
    void openImdbPage(const QString &title, const QString &year);
    void openPahePage(const QString &title, const QString &year);
    void filterTable();
    void exportToExcel();
    void onFetchClicked();
    void onMovieFetched(const Movie &movie);

private:
    enum CustomRoles
    {
        FilePathRole = Qt::UserRole + 1
    };

    void addComboBoxItemIfNotExist(QComboBox *comboBox, const QString &item);
    void addComboBoxItemsSorted(QComboBox *comboBox, const QSet<QString> &items, const QString &additionalItem = "");
    QString getVideoResolution(const QString &filePath);
    QString getAspectRatio(const QString &resolution);
    QString getVideoQuality(const QString &resolution);
    QString getDecade(const QString &year);
    QString filepath;
    QPair<QString, QString> parseFolderName(const QString &folderName);

    Ui::MainWindow *ui;
    OmdbClient *omdbClient;
    MovieDB *movieDb; // Add this line
    void showContextMenu(const QPoint &pos);
    QString getVideoDuration(const QString &filePath);
    QString getAudioLanguage(const QString &filePath);
    QString getFileSize(const QString &filePath);
    void filterTableRows(const QString &text);
    QTableWidget *movieTable;
    int currentRow;
    QString sanitizeForWindowsFolder(const QString &name);
};

#endif // MAINWINDOW_H
