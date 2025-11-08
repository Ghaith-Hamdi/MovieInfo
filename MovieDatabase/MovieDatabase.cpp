#include "MovieDatabase.h"
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
    QStringList columns;
    for (const QString &field : movieFields)
    {
        QString colType = (field == "title") ? "TEXT PRIMARY KEY" : "TEXT";
        columns << QString("%1 %2").arg(field, colType);
    }

    QSqlQuery query(db);
    const bool success = query.exec(
        QString("CREATE TABLE IF NOT EXISTS movies (%1)").arg(columns.join(", ")));

    if (!success)
        qDebug() << "Create table error:" << query.lastError().text();

    return success;
}

bool MovieDB::saveMovie(const Movie &movie)
{
    if (!db.isOpen())
    {
        qDebug() << "Database is not open";
        return false;
    }

    QMap<QString, QString> movieMap = movieToMap(movie);
    QStringList fields = movieFields;
    QStringList placeholders(fields.size(), "?");

    QSqlQuery query(db);
    query.prepare(
        QString("INSERT OR REPLACE INTO movies (%1) VALUES (%2)")
            .arg(fields.join(", "), placeholders.join(", ")));

    for (const QString &field : fields)
        query.addBindValue(movieMap[field]);

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
        movie.year = query.value("year").toString();
        movie.rated = query.value("rated").toString();
        movie.imdbRating = query.value("rating").toString();
        movie.imdbVotes = query.value("votes").toString();
        movie.runtime = query.value("runtime").toString();
        movie.director = query.value("director").toString();
        movie.actors = query.value("actors").toString();
        movie.writer = query.value("writer").toString();
        movie.awards = query.value("awards").toString();
        movie.language = query.value("language").toString();
        movie.country = query.value("country").toString();
        movie.boxOffice = query.value("boxoffice").toString();
        movie.plot = query.value("plot").toString();
        movie.genre = query.value("genre").toString();
        movie.imdbID = query.value("imdbid").toString();
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

bool MovieDB::deleteMovie(const QString &title)
{
    if (!db.isOpen())
    {
        qDebug() << "Database is not open";
        return false;
    }

    QSqlQuery query(db);
    query.prepare("DELETE FROM movies WHERE title = ?");
    query.addBindValue(title);

    if (!query.exec())
    {
        qDebug() << "Delete movie error:" << query.lastError().text();
        return false;
    }

    return true;
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

QMap<QString, QString> MovieDB::movieToMap(const Movie &movie)
{
    return {
        {"title", movie.title},
        {"year", movie.year},
        {"rated", movie.rated},
        {"rating", movie.imdbRating},
        {"votes", movie.imdbVotes},
        {"runtime", movie.runtime},
        {"director", movie.director},
        {"actors", movie.actors},
        {"writer", movie.writer},
        {"awards", movie.awards},
        {"language", movie.language},
        {"country", movie.country},
        {"boxoffice", movie.boxOffice},
        {"plot", movie.plot},
        {"genre", movie.genre},
        {"imdbid", movie.imdbID}};
}
