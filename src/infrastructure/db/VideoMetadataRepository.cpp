#include "VideoMetadataRepository.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

namespace Infrastructure
{

    VideoMetadataRepository::VideoMetadataRepository(Database *db, QObject *parent)
        : QObject(parent), m_db(db)
    {
    }

    void VideoMetadataRepository::preloadCache()
    {
        if (m_cacheLoaded || !m_db->isOpen())
            return;

        m_cache.clear();

        QSqlQuery query(m_db->connection());
        if (!query.exec("SELECT * FROM video_metadata"))
        {
            qWarning() << "Failed to preload video metadata cache:" << query.lastError().text();
            return;
        }

        while (query.next())
        {
            VideoMetadataEntry entry;
            entry.resolution = query.value("resolution").toString();
            entry.aspectRatio = query.value("aspectRatio").toString();
            entry.quality = query.value("quality").toString();
            entry.duration = query.value("duration").toString();
            entry.audioLanguage = query.value("audioLanguage").toString();
            entry.fileSize = query.value("fileSize").toString();
            entry.lastModified = query.value("lastModified").toLongLong();

            QString filePath = query.value("filepath").toString();
            m_cache.insert(filePath.toLower(), entry);
        }

        m_cacheLoaded = true;
        qDebug() << "Preloaded" << m_cache.size() << "video metadata entries";
    }

    bool VideoMetadataRepository::save(const QString &filePath, const VideoMetadataEntry &entry)
    {
        if (!m_db->isOpen())
            return false;

        QSqlQuery query(m_db->connection());
        query.prepare(
            "INSERT OR REPLACE INTO video_metadata "
            "(filepath, resolution, aspectRatio, quality, duration, audioLanguage, fileSize, lastModified) "
            "VALUES (?, ?, ?, ?, ?, ?, ?, ?)");

        query.addBindValue(filePath);
        query.addBindValue(entry.resolution);
        query.addBindValue(entry.aspectRatio);
        query.addBindValue(entry.quality);
        query.addBindValue(entry.duration);
        query.addBindValue(entry.audioLanguage);
        query.addBindValue(entry.fileSize);
        query.addBindValue(entry.lastModified);

        bool ok = query.exec();
        if (ok)
            m_cache.insert(filePath.toLower(), entry);

        return ok;
    }

    VideoMetadataEntry VideoMetadataRepository::get(const QString &filePath)
    {
        if (!m_cacheLoaded)
            preloadCache();

        return m_cache.value(filePath.toLower(), VideoMetadataEntry());
    }

    bool VideoMetadataRepository::hasValid(const QString &filePath, qint64 lastModified)
    {
        if (!m_cacheLoaded)
            preloadCache();

        QString key = filePath.toLower();
        if (!m_cache.contains(key))
            return false;

        return m_cache.value(key).lastModified == lastModified;
    }

} // namespace Infrastructure
