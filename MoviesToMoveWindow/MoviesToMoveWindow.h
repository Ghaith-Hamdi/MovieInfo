#ifndef MOVIESTOMOVEWINDOW_H
#define MOVIESTOMOVEWINDOW_H

#include <QDialog>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QString>
#include <QList>

struct MovieToMove
{
    QString title;
    QString year;
    QString imdbRating;
    QString imdbVotes;
    QString currentPath;
    QString targetDrive;
    QString targetPath;
};

class MoviesToMoveWindow : public QDialog
{
    Q_OBJECT

public:
    explicit MoviesToMoveWindow(QWidget *parent = nullptr);
    ~MoviesToMoveWindow();

    void setMoviesToMove(const QList<MovieToMove> &movies);

private:
    void setupUI();
    void populateTable();
    void addMoveButtonToRow(int row, const QString &moviePath, const QString &targetPath);

    QTableWidget *tableWidget;
    QLabel *countLabel;
    QPushButton *closeButton;
    QList<MovieToMove> moviesToMove;

private slots:
    void onMoveButtonClicked();
};

#endif // MOVIESTOMOVEWINDOW_H
