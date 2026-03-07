#ifndef MOVIEDB_H
#define MOVIEDB_H

#include <QObject>
#include <QSqlDatabase>
#include <QStringList>
#include <QHash>
#include "Movie/Movie.h"

// Video metadata structure for caching
struct VideoMetadataCache
{
    QString resolution;
    QString aspectRatio;
    QString quality;
    QString duration;
    QString audioLanguage;
    QString fileSize;
    qint64 lastModified; // File modification time for cache invalidation
};

class MovieDB : public QObject
{
    Q_OBJECT

public:
    explicit MovieDB(QObject *parent = nullptr);
    ~MovieDB() override;

    bool init();
    bool saveMovie(const Movie &movie);
    Movie getMovie(const QString &title, const QString &year);
    bool deleteMovie(const QString &title, const QString &year);

    // Cache methods for faster loading
    void preloadCache();
    Movie getMovieFromCache(const QString &title, const QString &year);
    Movie getMovieBySanitizedTitleFromCache(const QString &sanitizedTitle, const QString &year);
    bool movieExistsInCache(const QString &title, const QString &year);

    // Video metadata cache methods
    bool saveVideoMetadata(const QString &filePath, const VideoMetadataCache &metadata);
    VideoMetadataCache getVideoMetadata(const QString &filePath);
    bool hasVideoMetadata(const QString &filePath, qint64 lastModified);
    void preloadVideoMetadataCache();

private:
    bool createTable();
    bool createVideoMetadataTable();
    QSqlDatabase db;

    // Cache for faster lookups
    QHash<QString, Movie> movieCache;
    QHash<QString, QString> sanitizedTitleCache; // sanitized -> original title
    bool cacheLoaded = false;

    // Video metadata cache
    QHash<QString, VideoMetadataCache> videoMetadataCache;
    bool videoMetadataCacheLoaded = false;

    const QStringList movieFields = {
        "title", "year", "rated", "rating", "votes",
        "runtime", "director", "actors", "writer", "awards",
        "language", "country", "boxoffice", "plot", "genre", "imdbid"};
    QMap<QString, QString> movieToMap(const Movie &movie);
};

#endif // MOVIEDB_H
