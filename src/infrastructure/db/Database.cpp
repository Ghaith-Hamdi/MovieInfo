#include "Database.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

namespace Infrastructure
{

    static int s_connectionCounter = 0;

    Database::Database(const QString &dbPath, QObject *parent)
        : QObject(parent)
    {
        if (!QSqlDatabase::isDriverAvailable("QSQLITE"))
        {
            qWarning() << "SQLite driver is not available";
            return;
        }

        m_connectionName = QStringLiteral("MovieInfo_DB_%1").arg(++s_connectionCounter);
        m_db = QSqlDatabase::addDatabase("QSQLITE", m_connectionName);
        m_db.setDatabaseName(dbPath);
    }

    Database::~Database()
    {
        if (m_db.isOpen())
            m_db.close();
        QSqlDatabase::removeDatabase(m_connectionName);
    }

    bool Database::open()
    {
        if (!m_db.isValid())
        {
            qWarning() << "Database connection not valid";
            return false;
        }
        if (!m_db.open())
        {
            qWarning() << "Failed to open database:" << m_db.lastError().text();
            return false;
        }
        return createTables();
    }

    bool Database::isOpen() const
    {
        return m_db.isOpen();
    }

    QSqlDatabase &Database::connection()
    {
        return m_db;
    }

    bool Database::createTables()
    {
        QSqlQuery query(m_db);

        // Movies table — matches old schema for backward compatibility
        bool ok = query.exec(
            "CREATE TABLE IF NOT EXISTS movies ("
            "title TEXT, year TEXT, rated TEXT, rating TEXT, votes TEXT, "
            "runtime TEXT, director TEXT, actors TEXT, writer TEXT, awards TEXT, "
            "language TEXT, country TEXT, boxoffice TEXT, plot TEXT, genre TEXT, imdbid TEXT, "
            "fetched_at TEXT DEFAULT '', "
            "PRIMARY KEY (title, year))");
        if (!ok)
        {
            qWarning() << "Create movies table error:" << query.lastError().text();
            return false;
        }

        // Add fetched_at column if upgrading from old schema (ALTER TABLE is idempotent-safe with IF NOT EXISTS on some SQLite builds, so we try and ignore errors)
        query.exec("ALTER TABLE movies ADD COLUMN fetched_at TEXT DEFAULT ''");

        // Video metadata cache table
        ok = query.exec(
            "CREATE TABLE IF NOT EXISTS video_metadata ("
            "filepath TEXT PRIMARY KEY, "
            "resolution TEXT, aspectRatio TEXT, quality TEXT, "
            "duration TEXT, audioLanguage TEXT, fileSize TEXT, "
            "lastModified INTEGER)");
        if (!ok)
        {
            qWarning() << "Create video_metadata table error:" << query.lastError().text();
            return false;
        }

        return true;
    }

} // namespace Infrastructure
