#ifndef SERVICES_SCANSERVICE_H
#define SERVICES_SCANSERVICE_H

#include <QObject>
#include <QStringList>
#include <atomic>
#include "core/VideoFile.h"
#include "infrastructure/db/VideoMetadataRepository.h"

namespace Services
{

    class ScanService : public QObject
    {
        Q_OBJECT

    public:
        explicit ScanService(Infrastructure::VideoMetadataRepository *videoRepo,
                             QObject *parent = nullptr);

        // Scan a folder (or single file) for video files.
        // Emits signals as files are discovered and processed.
        void scanFolder(const QString &path, bool isSingleFile = false);
        void cancel();

        // Build a folder path from drive and year using the standard organisation scheme:
        //   drive\+2000\20s\2023  or  drive\-2000\90s\1994
        static QString buildFolderPath(const QString &drive, int year);

        // Parse a folder name like "The Matrix (1999)" into {title, year}
        static QPair<QString, int> parseFolderName(const QString &folderName);

    signals:
        void scanStarted(int totalFiles);
        void fileProcessed(const Core::VideoFile &file);
        void progress(int current, int total, const QString &currentFile);
        void scanComplete(int total, int cached, int fresh);
        void error(const QString &message);

    private:
        QStringList collectVideoFiles(const QString &path, bool isSingleFile);
        Core::VideoFile processFile(const QString &filePath);

        Infrastructure::VideoMetadataRepository *m_videoRepo;
        std::atomic<bool> m_cancelled{false};

        static const QStringList s_videoExtensions;
    };

} // namespace Services

#endif // SERVICES_SCANSERVICE_H
