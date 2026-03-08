#ifndef CORE_VIDEOFILE_H
#define CORE_VIDEOFILE_H

#include <QString>
#include <QDateTime>
#include <optional>
#include "Movie.h"

namespace Core
{

    struct VideoFile
    {
        QString filePath;
        QString folderTitle; // parsed from folder name
        int folderYear = 0;

        // Technical metadata (from ffprobe)
        QString resolution;
        int width = 0;
        int height = 0;
        double aspectRatio = 0.0;
        QString qualityTier; // "4K", "1080p", "720p", "SD", "Unknown"
        int durationSeconds = 0;
        QString audioLanguage;
        qint64 fileSizeBytes = 0;
        QDateTime lastModified;

        // OMDb metadata (filled after fetch)
        std::optional<Movie> metadata;

        // Computed properties
        QString decade() const
        {
            if (folderYear <= 0)
                return QStringLiteral("Unknown");
            int d = folderYear - (folderYear % 10);
            return QString::number(d) + "s";
        }

        QString formattedDuration() const
        {
            if (durationSeconds <= 0)
                return QStringLiteral("Unknown");
            int h = durationSeconds / 3600;
            int m = (durationSeconds % 3600) / 60;
            int s = durationSeconds % 60;
            return QStringLiteral("%1:%2:%3")
                .arg(h, 2, 10, QChar('0'))
                .arg(m, 2, 10, QChar('0'))
                .arg(s, 2, 10, QChar('0'));
        }

        QString formattedFileSize() const
        {
            if (fileSizeBytes <= 0)
                return QStringLiteral("0.00 GB");
            double gb = fileSizeBytes / (1024.0 * 1024.0 * 1024.0);
            return QString::number(gb, 'f', 2) + " GB";
        }

        QString formattedAspectRatio() const
        {
            if (aspectRatio <= 0.0)
                return QStringLiteral("Unknown");
            return QString::number(aspectRatio, 'f', 2);
        }

        bool isUltraWide() const
        {
            return aspectRatio >= 2.2 && aspectRatio <= 2.5;
        }

        bool isValid() const { return !filePath.isEmpty(); }
    };

} // namespace Core

#endif // CORE_VIDEOFILE_H
