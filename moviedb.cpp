#include "moviedb.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QCoreApplication>

MovieDB::MovieDB(QObject *parent)
    : QObject(parent)
{
    if (!QSqlDatabase::isDriverAvailable("QSQLITE"))
    {
        qDebug() << "SQLite driver is not available";
        return;
    }

    static int connectionCounter = 0;
    QString connectionName = QString("MovieDB_Connection_%1").arg(++connectionCounter);

    QString dbPath = QCoreApplication::applicationDirPath() + "/movies.db";
    qDebug() << "Database path:" << dbPath;

    db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
    db.setDatabaseName(dbPath);
}

bool MovieDB::init()
{
    if (!db.isValid() || !db.open())
    {
        qDebug() << "Failed to open database:" << db.lastError().text();
        return false;
    }

    if (!createTable())
    {
        db.close();
        return false;
    }

    return true;
}

bool MovieDB::createTable()
{
    QSqlQuery query(db);
    const bool success = query.exec(
        "CREATE TABLE IF NOT EXISTS movies ("
        "title TEXT PRIMARY KEY, "
        "rating TEXT, votes TEXT, director TEXT, year TEXT, "
        "runtime TEXT, actors TEXT, awards TEXT, language TEXT, "
        "country TEXT, boxoffice TEXT)");

    if (!success)
    {
        qDebug() << "Create table error:" << query.lastError().text();
    }

    return success;
}

bool MovieDB::saveMovie(const Movie &movie)
{
    if (!db.isOpen())
    {
        qDebug() << "Database is not open";
        return false;
    }

    QSqlQuery query(db);
    query.prepare(
        "INSERT OR REPLACE INTO movies "
        "(title, rating, votes, director, year, runtime, actors, awards, language, country, boxoffice) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");

    query.addBindValue(movie.title);
    query.addBindValue(movie.imdbRating);
    query.addBindValue(movie.imdbVotes);
    query.addBindValue(movie.director);
    query.addBindValue(movie.year);
    query.addBindValue(movie.runtime);
    query.addBindValue(movie.actors);
    query.addBindValue(movie.awards);
    query.addBindValue(movie.language);
    query.addBindValue(movie.country);
    query.addBindValue(movie.boxOffice);

    return query.exec();
}

Movie MovieDB::getMovie(const QString &title)
{
    Movie movie;
    QSqlQuery query(db);
    query.prepare("SELECT * FROM movies WHERE title = ?");
    query.addBindValue(title);

    if (query.exec() && query.next())
    {
        movie.title = query.value("title").toString();
        movie.imdbRating = query.value("rating").toString();
        movie.imdbVotes = query.value("votes").toString();
        movie.director = query.value("director").toString();
        movie.year = query.value("year").toString();
        movie.runtime = query.value("runtime").toString();
        movie.actors = query.value("actors").toString();
        movie.awards = query.value("awards").toString();
        movie.language = query.value("language").toString();
        movie.country = query.value("country").toString();
        movie.boxOffice = query.value("boxoffice").toString();
    }

    return movie;
}

bool MovieDB::movieExists(const QString &title)
{
    if (!db.isOpen())
        return false;

    QSqlQuery query(db);
    query.prepare("SELECT COUNT(*) FROM movies WHERE title = ?");
    query.addBindValue(title);

    return query.exec() && query.next() && query.value(0).toInt() > 0;
}

MovieDB::~MovieDB()
{
    const QString connectionName = db.connectionName();
    if (db.isOpen())
    {
        db.commit(); // Optional if not using transactions explicitly
        db.close();
    }
    QSqlDatabase::removeDatabase(connectionName);
}
