#include "FfprobeRunner.h"
#include <QProcess>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

namespace Infrastructure
{

    FfprobeRunner::Result FfprobeRunner::extract(const QString &filePath)
    {
        Result r;

        // File size
        QFileInfo info(filePath);
        r.fileSizeBytes = info.size();

        // Run ffprobe
        QProcess proc;
        proc.start("ffprobe", {"-v", "quiet",
                               "-print_format", "json",
                               "-show_format",
                               "-show_streams",
                               filePath});
        proc.waitForFinished(30000);

        QJsonDocument doc = QJsonDocument::fromJson(proc.readAllStandardOutput());
        if (doc.isNull() || !doc.isObject())
        {
            r.resolution = "Unknown";
            r.qualityTier = "Unknown";
            r.audioLanguage = "Unknown";
            return r;
        }

        QJsonObject root = doc.object();

        // -- Streams --
        if (root.contains("streams"))
        {
            const QJsonArray streams = root["streams"].toArray();

            // Video stream
            for (const QJsonValue &sv : streams)
            {
                QJsonObject s = sv.toObject();
                if (s["codec_type"].toString() == "video")
                {
                    r.width = s["width"].toInt(0);
                    r.height = s["height"].toInt(0);
                    if (r.width > 0 && r.height > 0)
                    {
                        r.resolution = QStringLiteral("%1x%2").arg(r.width).arg(r.height);
                        r.aspectRatio = static_cast<double>(r.width) / r.height;
                    }
                    break;
                }
            }

            // Audio stream (first audio track)
            for (const QJsonValue &sv : streams)
            {
                QJsonObject s = sv.toObject();
                if (s["codec_type"].toString() == "audio")
                {
                    if (s.contains("tags"))
                    {
                        QJsonObject tags = s["tags"].toObject();
                        r.audioLanguage = tags.value("language").toString();
                    }
                    break;
                }
            }
        }

        // -- Format (duration) --
        if (root.contains("format"))
        {
            QJsonObject fmt = root["format"].toObject();
            if (fmt.contains("duration"))
            {
                bool ok;
                double secs = fmt["duration"].toString().toDouble(&ok);
                if (ok)
                    r.durationSeconds = static_cast<int>(secs);
            }
        }

        // -- Quality tier from filename --
        QString fileName = info.fileName().toLower();
        if (fileName.contains("2160p") || fileName.contains("4k"))
            r.qualityTier = "4K";
        else if (fileName.contains("1080p"))
            r.qualityTier = "1080p";
        else if (fileName.contains("720p"))
            r.qualityTier = "720p";
        else
            r.qualityTier = "Unknown";

        // Defaults
        if (r.resolution.isEmpty())
            r.resolution = "Unknown";
        if (r.audioLanguage.isEmpty())
            r.audioLanguage = "Unknown";

        return r;
    }

} // namespace Infrastructure
