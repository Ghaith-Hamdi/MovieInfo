#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QTableWidget>
#include <QPushButton>
#include <OmdbClient.h>

namespace Ui
{
    class MainWindow;
}

struct Movie
{
    QString title;
    QString year;
    QString resolution;
    QString aspectRatio;
    QString quality;
    QString fileSize;
    QString duration;
    QString audioLang;
    QString rating;
    QString votes;
};

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
    void onMovieFetched(const QList<QString> &movieData);

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
    void showContextMenu(const QPoint &pos);
    QString getVideoDuration(const QString &filePath);
    QString getAudioLanguage(const QString &filePath);
    QString getFileSize(const QString &filePath);
    void filterTableRows(const QString &text);
    OmdbClient *omdbClient;
    QTableWidget *movieTable;
    int currentRow;
};

#endif // MAINWINDOW_H
