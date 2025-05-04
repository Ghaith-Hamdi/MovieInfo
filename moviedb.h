#ifndef MOVIEDB_H
#define MOVIEDB_H

#include <QObject>
#include <QSqlDatabase>
#include <QStringList>

class MovieDB : public QObject
{
    Q_OBJECT

public:
    explicit MovieDB(QObject *parent = nullptr);
    ~MovieDB() override; // Change to override instead of virtual

    bool init();
    bool saveMovie(const QStringList &movieData);
    QStringList getMovie(const QString &title);
    bool movieExists(const QString &title);

private:
    bool createTable();
    QSqlDatabase db;
};

#endif // MOVIEDB_H