#include "moviedb.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QCoreApplication>

MovieDB::MovieDB(QObject *parent)
    : QObject(parent)
{
    // Check if QSQLITE driver is available first
    if (!QSqlDatabase::isDriverAvailable("QSQLITE"))
    {
        qDebug() << "SQLite driver is not available";
        return;
    }

    // Create database in the application's directory
    QString appDir = QCoreApplication::applicationDirPath();
    QString dbPath = appDir + "/movies.db";
    qDebug() << "Database path:" << dbPath;

    // Create a unique connection name
    static int connectionCounter = 0;
    QString connectionName = QString("MovieDB_Connection_%1").arg(++connectionCounter);

    // Remove any existing connection with the same name
    if (QSqlDatabase::contains(connectionName))
    {
        QSqlDatabase::removeDatabase(connectionName);
    }

    // Create new database connection
    db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
    db.setDatabaseName(dbPath);
}

bool MovieDB::init()
{
    if (!db.isValid())
    {
        qDebug() << "Database is not valid";
        return false;
    }

    if (!db.open())
    {
        qDebug() << "Failed to open database:" << db.lastError().text();
        return false;
    }

    if (!createTable())
    {
        qDebug() << "Failed to create table:" << db.lastError().text();
        db.close();
        return false;
    }

    return true;
}

bool MovieDB::createTable()
{
    QSqlQuery query(db); // Use the specific database connection
    bool success = query.exec(
        "CREATE TABLE IF NOT EXISTS movies ("
        "title TEXT PRIMARY KEY, "
        "rating TEXT, "
        "votes TEXT, "
        "director TEXT, "
        "year TEXT, "
        "runtime TEXT, "
        "actors TEXT, "
        "awards TEXT, "
        "language TEXT, "
        "country TEXT, "
        "boxoffice TEXT)");

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
    query.prepare("INSERT OR REPLACE INTO movies (title, rating, votes, director, year, runtime, "
                  "actors, awards, language, country, boxoffice) "
                  "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");

    query.bindValue(0, movie.title);
    query.bindValue(1, movie.imdbRating);
    query.bindValue(2, movie.imdbVotes);
    query.bindValue(3, movie.director);
    query.bindValue(4, movie.year);
    query.bindValue(5, movie.runtime);
    query.bindValue(6, movie.actors);
    query.bindValue(7, movie.awards);
    query.bindValue(8, movie.language);
    query.bindValue(9, movie.country);
    query.bindValue(10, movie.boxOffice);

    return query.exec();
}

Movie MovieDB::getMovie(const QString &title)
{
    Movie movie;
    QSqlQuery query(db);
    query.prepare("SELECT * FROM movies WHERE title = ?");
    query.bindValue(0, title);

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
    {
        return false;
    }

    QSqlQuery query(db);
    query.prepare("SELECT COUNT(*) FROM movies WHERE title = ?");
    query.bindValue(0, title);

    if (query.exec() && query.next())
    {
        return query.value(0).toInt() > 0;
    }
    return false;
}

MovieDB::~MovieDB()
{
    QString connectionName = db.connectionName();
    if (db.isOpen())
    {
        db.commit(); // Commit any pending transactions
        db.close();
    }
    QSqlDatabase::removeDatabase(connectionName);
}