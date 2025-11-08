#ifndef MOVIEDB_H
#define MOVIEDB_H

#include <QObject>
#include <QSqlDatabase>
#include <QStringList>
#include "Movie/Movie.h"

class MovieDB : public QObject
{
    Q_OBJECT

public:
    explicit MovieDB(QObject *parent = nullptr);
    ~MovieDB() override;

    bool init();
    bool saveMovie(const Movie &movie);
    Movie getMovie(const QString &title);
    bool movieExists(const QString &title);
    bool deleteMovie(const QString &title);

private:
    bool createTable();
    QSqlDatabase db;

    const QStringList movieFields = {
        "title", "year", "rated", "rating", "votes",
        "runtime", "director", "actors", "writer", "awards",
        "language", "country", "boxoffice", "plot", "genre", "imdbid"};
    QMap<QString, QString> movieToMap(const Movie &movie);
};

#endif // MOVIEDB_H
