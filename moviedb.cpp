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
        "country TEXT)");

    if (!success)
    {
        qDebug() << "Create table error:" << query.lastError().text();
    }
    return success;
}

bool MovieDB::saveMovie(const QStringList &movieData)
{
    if (movieData.size() < 10)
    {
        qDebug() << "Invalid movie data size";
        return false;
    }

    if (!db.isOpen())
    {
        qDebug() << "Database is not open";
        return false;
    }

    // Start transaction
    if (!db.transaction())
    {
        qDebug() << "Failed to start transaction:" << db.lastError().text();
        return false;
    }

    QSqlQuery query(db);
    query.prepare(
        "INSERT OR REPLACE INTO movies ("
        "title, rating, votes, director, year, runtime, "
        "actors, awards, language, country) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");

    for (int i = 0; i < movieData.size() && i < 10; ++i)
    {
        query.bindValue(i, movieData[i]);
    }

    bool success = query.exec();

    if (!success)
    {
        qDebug() << "Query failed:" << query.lastError().text();
        db.rollback();
        return false;
    }

    // Commit transaction
    if (!db.commit())
    {
        qDebug() << "Failed to commit transaction:" << db.lastError().text();
        db.rollback();
        return false;
    }

    qDebug() << "Movie saved successfully:" << movieData[0]; // Print title

    QSqlQuery countQuery(db);
    if (countQuery.exec("SELECT COUNT(*) FROM movies"))
    {
        countQuery.next();
        qDebug() << "Total movies in database:" << countQuery.value(0).toInt();
    }

    return true;
}

QStringList MovieDB::getMovie(const QString &title)
{
    QSqlQuery query(db); // Use the specific database connection
    query.prepare("SELECT * FROM movies WHERE title = ?");
    query.bindValue(0, title);

    if (query.exec() && query.next())
    {
        QStringList movieData;
        for (int i = 0; i < 10; ++i)
        {
            movieData << query.value(i).toString();
        }
        return movieData;
    }
    return QStringList();
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