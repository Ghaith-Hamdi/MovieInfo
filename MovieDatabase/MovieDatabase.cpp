#include "MovieDatabase.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QCoreApplication>
#include <QRegularExpression>

MovieDB::MovieDB(QObject *parent)
    : QObject(parent), cacheLoaded(false)
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
        columns << QString("%1 TEXT").arg(field);
    }

    QSqlQuery query(db);
    const bool success = query.exec(
        QString("CREATE TABLE IF NOT EXISTS movies (%1, PRIMARY KEY (title, year))").arg(columns.join(", ")));

    if (!success)
        qDebug() << "Create table error:" << query.lastError().text();

    // Also create video metadata table
    if (success)
        createVideoMetadataTable();

    return success;
}

bool MovieDB::createVideoMetadataTable()
{
    QSqlQuery query(db);
    const bool success = query.exec(
        "CREATE TABLE IF NOT EXISTS video_metadata ("
        "filepath TEXT PRIMARY KEY, "
        "resolution TEXT, "
        "aspectRatio TEXT, "
        "quality TEXT, "
        "duration TEXT, "
        "audioLanguage TEXT, "
        "fileSize TEXT, "
        "lastModified INTEGER)");

    if (!success)
        qDebug() << "Create video_metadata table error:" << query.lastError().text();

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

    bool success = query.exec();

    // Update cache if save was successful
    if (success && cacheLoaded)
    {
        QRegularExpression forbidden(R"([\\/:*?"<>|,.])");
        QString cacheKey = movie.title.toLower() + "|" + movie.year;
        movieCache.insert(cacheKey, movie);
        QString sanitized = QString(movie.title).remove(forbidden).simplified().toLower() + "|" + movie.year;
        sanitizedTitleCache.insert(sanitized, movie.title + "|" + movie.year);
    }

    return success;
}


bool MovieDB::deleteMovie(const QString &title, const QString &year)
{
    if (!db.isOpen())
    {
        qDebug() << "Database is not open";
        return false;
    }

    QSqlQuery query(db);
    query.prepare("DELETE FROM movies WHERE title = ? AND year = ?");
    query.addBindValue(title);
    query.addBindValue(year);

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

void MovieDB::preloadCache()
{
    if (cacheLoaded || !db.isOpen())
        return;

    movieCache.clear();
    sanitizedTitleCache.clear();

    QRegularExpression forbidden(R"([\\/:*?"<>|,.])");

    QSqlQuery query(db);
    if (!query.exec("SELECT * FROM movies"))
    {
        qDebug() << "Failed to preload cache:" << query.lastError().text();
        return;
    }

    while (query.next())
    {
        Movie movie;
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

        // Store in cache with lowercase key for case-insensitive lookup (title + year)
        QString cacheKey = movie.title.toLower() + "|" + movie.year;
        movieCache.insert(cacheKey, movie);

        // Also store sanitized title mapping
        QString sanitized = QString(movie.title).remove(forbidden).simplified().toLower() + "|" + movie.year;
        sanitizedTitleCache.insert(sanitized, movie.title + "|" + movie.year);
    }

    cacheLoaded = true;
    qDebug() << "Preloaded" << movieCache.size() << "movies into cache";
}

Movie MovieDB::getMovieFromCache(const QString &title, const QString &year)
{
    if (!cacheLoaded)
        preloadCache();

    QString cacheKey = title.toLower() + "|" + year;
    return movieCache.value(cacheKey, Movie());
}

Movie MovieDB::getMovieBySanitizedTitleFromCache(const QString &sanitizedTitle, const QString &year)
{
    if (!cacheLoaded)
        preloadCache();

    QRegularExpression forbidden(R"([\\/:*?\"<>|,.])");
    QString searchTitle = QString(sanitizedTitle).remove(forbidden).simplified().toLower() + "|" + year;

    QString originalTitleWithYear = sanitizedTitleCache.value(searchTitle);
    if (!originalTitleWithYear.isEmpty())
    {
        return movieCache.value(originalTitleWithYear.toLower(), Movie());
    }

    return Movie();
}

bool MovieDB::movieExistsInCache(const QString &title, const QString &year)
{
    if (!cacheLoaded)
        preloadCache();

    QString cacheKey = title.toLower() + "|" + year;
    return movieCache.contains(cacheKey);
}

// ========================================================================
// VIDEO METADATA CACHE METHODS
// ========================================================================

void MovieDB::preloadVideoMetadataCache()
{
    if (videoMetadataCacheLoaded || !db.isOpen())
        return;

    videoMetadataCache.clear();

    QSqlQuery query(db);
    if (!query.exec("SELECT * FROM video_metadata"))
    {
        qDebug() << "Failed to preload video metadata cache:" << query.lastError().text();
        return;
    }

    while (query.next())
    {
        QString filePath = query.value("filepath").toString();
        VideoMetadataCache metadata;
        metadata.resolution = query.value("resolution").toString();
        metadata.aspectRatio = query.value("aspectRatio").toString();
        metadata.quality = query.value("quality").toString();
        metadata.duration = query.value("duration").toString();
        metadata.audioLanguage = query.value("audioLanguage").toString();
        metadata.fileSize = query.value("fileSize").toString();
        metadata.lastModified = query.value("lastModified").toLongLong();

        videoMetadataCache.insert(filePath.toLower(), metadata);
    }

    videoMetadataCacheLoaded = true;
    qDebug() << "Preloaded" << videoMetadataCache.size() << "video metadata entries into cache";
}

bool MovieDB::saveVideoMetadata(const QString &filePath, const VideoMetadataCache &metadata)
{
    if (!db.isOpen())
    {
        qDebug() << "Database is not open";
        return false;
    }

    QSqlQuery query(db);
    query.prepare(
        "INSERT OR REPLACE INTO video_metadata "
        "(filepath, resolution, aspectRatio, quality, duration, audioLanguage, fileSize, lastModified) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?)");

    query.addBindValue(filePath);
    query.addBindValue(metadata.resolution);
    query.addBindValue(metadata.aspectRatio);
    query.addBindValue(metadata.quality);
    query.addBindValue(metadata.duration);
    query.addBindValue(metadata.audioLanguage);
    query.addBindValue(metadata.fileSize);
    query.addBindValue(metadata.lastModified);

    bool success = query.exec();

    // Update cache if save was successful
    if (success)
    {
        videoMetadataCache.insert(filePath.toLower(), metadata);
    }

    return success;
}

VideoMetadataCache MovieDB::getVideoMetadata(const QString &filePath)
{
    if (!videoMetadataCacheLoaded)
        preloadVideoMetadataCache();

    return videoMetadataCache.value(filePath.toLower(), VideoMetadataCache());
}

bool MovieDB::hasVideoMetadata(const QString &filePath, qint64 lastModified)
{
    if (!videoMetadataCacheLoaded)
        preloadVideoMetadataCache();

    QString key = filePath.toLower();
    if (videoMetadataCache.contains(key))
    {
        // Check if the file has been modified since cached
        return videoMetadataCache.value(key).lastModified == lastModified;
    }
    return false;
}
