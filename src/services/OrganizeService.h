#ifndef SERVICES_ORGANIZESERVICE_H
#define SERVICES_ORGANIZESERVICE_H

#include <QObject>
#include "core/QualityRules.h"
#include "core/VideoFile.h"
#include <QList>

namespace Services
{

    class OrganizeService : public QObject
    {
        Q_OBJECT

    public:
        explicit OrganizeService(QObject *parent = nullptr);

        // Compute which movies need to move based on quality rules
        QList<Core::MoveRequest> computeMovesToMove(
            const QList<Core::VideoFile> &files,
            const Core::QualityRule &rule);

        // Move a single folder from currentPath to targetPath
        bool moveFolder(const QString &currentPath, const QString &targetPath);

        // Organize a single movie by aspect ratio into 16-9 or UW subfolder
        bool organizeByAspectRatio(const QString &filePath, double aspectRatio);

    signals:
        void moveComplete(const QString &title, const QString &newPath);
        void moveFailed(const QString &title, const QString &error);
    };

} // namespace Services

#endif // SERVICES_ORGANIZESERVICE_H
