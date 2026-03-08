#ifndef INFRASTRUCTURE_FFPROBERUNNER_H
#define INFRASTRUCTURE_FFPROBERUNNER_H

#include <QObject>
#include "core/VideoFile.h"

namespace Infrastructure
{

    // Extracts video metadata from a single file using ffprobe.
    // All calls are synchronous — designed to be invoked from a worker thread.
    class FfprobeRunner
    {
    public:
        struct Result
        {
            QString resolution;
            int width = 0;
            int height = 0;
            double aspectRatio = 0.0;
            QString qualityTier;
            int durationSeconds = 0;
            QString audioLanguage;
            qint64 fileSizeBytes = 0;
        };

        static Result extract(const QString &filePath);
    };

} // namespace Infrastructure

#endif // INFRASTRUCTURE_FFPROBERUNNER_H
