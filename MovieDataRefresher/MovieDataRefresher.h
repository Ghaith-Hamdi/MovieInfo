#ifndef MOVIEDATAREFRESHER_H
#define MOVIEDATAREFRESHER_H

#include <QObject>
#include <QWidget>
#include "Movie/Movie.h"
#include "MovieDatabase/MovieDatabase.h"
#include "OmdbClient/OmdbClient.h"

class QStatusBar;

class MovieDataRefresher : public QObject
{
    Q_OBJECT

public:
    explicit MovieDataRefresher(MovieDB *db, OmdbClient *client, QStatusBar *statusBar, QWidget *parent = nullptr);

    bool showOverwriteDialog(const QString &title, const Movie &existingMovie, QWidget *parentWidget);

signals:
    void refreshRequested(const QString &title, int year, bool forceRefresh);

private:
    MovieDB *movieDb;
    OmdbClient *omdbClient;
    QStatusBar *statusBar;
    QWidget *parentWidget;
};

#endif // MOVIEDATAREFRESHER_H
