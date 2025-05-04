#ifndef MOVIEDB_H
#define MOVIEDB_H

#include <QObject>
#include <QSqlDatabase>
#include <QStringList>
#include "movie.h"

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

private:
    bool createTable();
    QSqlDatabase db;
};

#endif // MOVIEDB_H