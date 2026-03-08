#include "ScanService.h"
#include "infrastructure/process/FfprobeRunner.h"
#include <QDirIterator>
#include <QFileInfo>
#include <QRegularExpression>

namespace Services
{

    const QStringList ScanService::s_videoExtensions = {
        ".mp4", ".mkv", ".avi", ".mov", ".flv", ".wmv"};

    ScanService::ScanService(Infrastructure::VideoMetadataRepository *videoRepo, QObject *parent)
        : QObject(parent), m_videoRepo(videoRepo)
    {
    }

    void ScanService::cancel()
    {
        m_cancelled.store(true);
    }

    QString ScanService::buildFolderPath(const QString &drive, int year)
    {
        QString path = drive;

        if (year >= 2000)
        {
            path += "+2000\\";
            int decade = (year / 10) % 10;
            path += QString::number(decade) + "0s\\";
        }
        else
        {
            path += "-2000\\";
            int decade = (year / 10) % 10;
            path += QString::number(decade) + "0s\\";
        }

        path += QString::number(year);
        return path;
    }

    QPair<QString, int> ScanService::parseFolderName(const QString &folderName)
    {
        static const QRegularExpression re("(.+?)\\s*\\((\\d{4})\\)");
        QRegularExpressionMatch match = re.match(folderName);
        if (match.hasMatch())
            return {match.captured(1).trimmed(), match.captured(2).toInt()};
        return {folderName, 0};
    }

    QStringList ScanService::collectVideoFiles(const QString &path, bool isSingleFile)
    {
        QStringList files;

        if (isSingleFile)
        {
            QFileInfo fi(path);
            if (s_videoExtensions.contains("." + fi.suffix().toLower()))
                files.append(path);
        }
        else
        {
            QDirIterator it(path, QDirIterator::Subdirectories);
            while (it.hasNext())
            {
                QString fp = it.next();
                QFileInfo fi(fp);
                if (fi.isFile() && s_videoExtensions.contains("." + fi.suffix().toLower()))
                    files.append(fp);
            }
        }

        return files;
    }

    Core::VideoFile ScanService::processFile(const QString &filePath)
    {
        Core::VideoFile vf;
        vf.filePath = filePath;

        QFileInfo fi(filePath);
        vf.lastModified = fi.lastModified();
        vf.fileSizeBytes = fi.size();

        // Parse folder name
        QString folderName = fi.dir().dirName();
        auto [title, year] = parseFolderName(folderName);
        vf.folderTitle = title;
        vf.folderYear = year;

        // Check video metadata cache
        qint64 mtime = fi.lastModified().toSecsSinceEpoch();

        if (m_videoRepo->hasValid(filePath, mtime))
        {
            // Cache hit
            auto cached = m_videoRepo->get(filePath);
            vf.resolution = cached.resolution;
            vf.qualityTier = cached.quality;
            vf.audioLanguage = cached.audioLanguage;

            // Parse cached aspect ratio
            vf.aspectRatio = cached.aspectRatio.toDouble();

            // Parse cached duration
            QStringList parts = cached.duration.split(':');
            if (parts.size() == 3)
                vf.durationSeconds = parts[0].toInt() * 3600 + parts[1].toInt() * 60 + parts[2].toInt();

            // Parse cached resolution
            QStringList resParts = cached.resolution.split('x');
            if (resParts.size() == 2)
            {
                vf.width = resParts[0].toInt();
                vf.height = resParts[1].toInt();
            }
        }
        else
        {
            // Extract with ffprobe
            auto result = Infrastructure::FfprobeRunner::extract(filePath);
            vf.resolution = result.resolution;
            vf.width = result.width;
            vf.height = result.height;
            vf.aspectRatio = result.aspectRatio;
            vf.qualityTier = result.qualityTier;
            vf.durationSeconds = result.durationSeconds;
            vf.audioLanguage = result.audioLanguage;
            vf.fileSizeBytes = result.fileSizeBytes;

            // Save to cache
            Infrastructure::VideoMetadataEntry entry;
            entry.resolution = vf.resolution;
            entry.aspectRatio = vf.formattedAspectRatio();
            entry.quality = vf.qualityTier;
            entry.duration = vf.formattedDuration();
            entry.audioLanguage = vf.audioLanguage;
            entry.fileSize = vf.formattedFileSize();
            entry.lastModified = mtime;
            m_videoRepo->save(filePath, entry);
        }

        return vf;
    }

    void ScanService::scanFolder(const QString &path, bool isSingleFile)
    {
        m_cancelled.store(false);
        m_videoRepo->preloadCache();

        QStringList files = collectVideoFiles(path, isSingleFile);
        int total = files.size();

        if (total == 0)
        {
            emit error("No video files found in: " + path);
            return;
        }

        emit scanStarted(total);

        int cached = 0, fresh = 0;

        for (int i = 0; i < total; ++i)
        {
            if (m_cancelled.load())
                break;

            const QString &fp = files[i];
            QFileInfo fi(fp);
            qint64 mtime = fi.lastModified().toSecsSinceEpoch();

            bool wasCached = m_videoRepo->hasValid(fp, mtime);

            Core::VideoFile vf = processFile(fp);
            emit fileProcessed(vf);

            if (wasCached)
                ++cached;
            else
                ++fresh;

            emit progress(i + 1, total, vf.folderTitle);
        }

        emit scanComplete(total, cached, fresh);
    }

} // namespace Services
