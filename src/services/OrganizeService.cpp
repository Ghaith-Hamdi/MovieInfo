#include "OrganizeService.h"
#include <QDir>
#include <QFileInfo>
#include <QProcess>
#include <QDateTime>

namespace Services
{

    OrganizeService::OrganizeService(QObject *parent) : QObject(parent) {}

    QList<Core::MoveRequest> OrganizeService::computeMovesToMove(
        const QList<Core::VideoFile> &files,
        const Core::QualityRule &rule)
    {
        QList<Core::MoveRequest> moves;

        for (const auto &vf : files)
        {
            if (!vf.metadata.has_value())
                continue;
            const auto &m = vf.metadata.value();

            // Determine current drive
            QString movieDrive;
            if (vf.filePath.startsWith("D:", Qt::CaseInsensitive))
                movieDrive = "D";
            else if (vf.filePath.startsWith("E:", Qt::CaseInsensitive))
                movieDrive = "E";
            else
                continue;

            bool isHighQuality = rule.matches(m.imdbRating, m.imdbVotes);

            bool needsMove = false;
            QString targetDrive;

            if (isHighQuality && movieDrive == "E")
            {
                needsMove = true;
                targetDrive = "D";
            }
            else if (!isHighQuality && movieDrive == "D")
            {
                needsMove = true;
                targetDrive = "E";
            }

            if (needsMove)
            {
                Core::MoveRequest req;
                req.title = vf.folderTitle;
                req.year = QString::number(vf.folderYear);
                req.imdbRating = m.formattedRating();
                req.imdbVotes = m.formattedVotes();

                QFileInfo fi(vf.filePath);
                req.currentPath = fi.dir().absolutePath();
                req.targetDrive = targetDrive;

                // Replace drive letter
                req.targetPath = req.currentPath;
                if (movieDrive == "D")
                    req.targetPath.replace(0, 2, "E:");
                else
                    req.targetPath.replace(0, 2, "D:");

                moves.append(req);
            }
        }

        return moves;
    }

    bool OrganizeService::moveFolder(const QString &currentPath, const QString &targetPath)
    {
        QDir srcDir(currentPath);
        QFileInfo tgtInfo(targetPath);
        QDir tgtParent = tgtInfo.dir();

        // Ensure parent exists
        if (!tgtParent.exists())
        {
            if (!tgtParent.mkpath("."))
                return false;
        }

        // Handle case-only rename on Windows: do a two-step rename via a temp name
        QString cleanSrc = QDir::cleanPath(currentPath);
        QString cleanTgt = QDir::cleanPath(targetPath);
        if (cleanSrc.compare(cleanTgt, Qt::CaseInsensitive) == 0 && cleanSrc != cleanTgt)
        {
            QString tempName = cleanTgt + ".__tmp_rename__" + QString::number(QDateTime::currentMSecsSinceEpoch());
            // Try rename -> temp, then temp -> target
            if (QDir().rename(cleanSrc, tempName))
            {
                if (QDir().rename(tempName, cleanTgt))
                    return true;
                else
                {
                    // Attempt to rollback
                    QDir().rename(tempName, cleanSrc);
                    return false;
                }
            }
            // if temp rename fails, fall through to other strategies
        }

        // Try direct rename first (works for same-drive)
        if (QDir().rename(currentPath, targetPath))
            return true;

        // Fallback: use robocopy for cross-drive move (robust on Windows)
        QProcess proc;
        QString srcNative = QDir::toNativeSeparators(currentPath);
        QString tgtNative = QDir::toNativeSeparators(targetPath);

        // robocopy <src> <dst> /E /MOVE
        proc.start("robocopy", {srcNative, tgtNative, "/E", "/MOVE"});
        if (!proc.waitForFinished(120000)) // allow up to 2 minutes
        {
            proc.kill();
            return false;
        }

        int exitCode = proc.exitCode();
        // robocopy uses exit codes where 0-7 are successful-ish; treat <=7 as success
        if (exitCode >= 0 && exitCode <= 7)
        {
            // Ensure source no longer exists
            if (!QDir(currentPath).exists())
                return true;
            // As a last resort try to remove recursively
            return !QDir(currentPath).exists() || QDir(currentPath).removeRecursively();
        }

        // Final fallback: try cmd move with quoted paths
        QString cmd = QString("move \"%1\" \"%2\"").arg(srcNative, tgtNative);
        proc.start("cmd.exe", {"/c", cmd});
        proc.waitForFinished(30000);
        return proc.exitCode() == 0;
    }

    bool OrganizeService::organizeByAspectRatio(const QString &filePath, double aspectRatio)
    {
        QFileInfo fi(filePath);
        QDir movieDir = fi.dir();
        QString movieFolderName = movieDir.dirName();
        QDir parentDir = movieDir;

        if (!parentDir.cdUp())
            return false;

        QString targetFolderName = Core::aspectRatioFolder(aspectRatio);
        QString targetBasePath = parentDir.filePath(targetFolderName);

        QDir targetBaseDir(targetBasePath);
        if (!targetBaseDir.exists() && !targetBaseDir.mkpath("."))
            return false;

        QString destination = targetBaseDir.filePath(movieFolderName);
        if (QDir(destination).exists())
            return false; // already exists

        return moveFolder(movieDir.absolutePath(), destination);
    }

} // namespace Services
