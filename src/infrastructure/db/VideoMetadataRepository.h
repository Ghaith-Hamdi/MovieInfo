#ifndef INFRASTRUCTURE_VIDEOMETADATAREPOSITORY_H
#define INFRASTRUCTURE_VIDEOMETADATAREPOSITORY_H

#include <QObject>
#include <QHash>
#include "Database.h"

namespace Infrastructure
{

    struct VideoMetadataEntry
    {
        QString resolution;
        QString aspectRatio;
        QString quality;
        QString duration;
        QString audioLanguage;
        QString fileSize;
        qint64 lastModified = 0;
    };

    class VideoMetadataRepository : public QObject
    {
        Q_OBJECT

    public:
        explicit VideoMetadataRepository(Database *db, QObject *parent = nullptr);

        void preloadCache();
        bool save(const QString &filePath, const VideoMetadataEntry &entry);
        VideoMetadataEntry get(const QString &filePath);
        bool hasValid(const QString &filePath, qint64 lastModified);

    private:
        Database *m_db;
        QHash<QString, VideoMetadataEntry> m_cache;
        bool m_cacheLoaded = false;
    };

} // namespace Infrastructure

#endif // INFRASTRUCTURE_VIDEOMETADATAREPOSITORY_H
