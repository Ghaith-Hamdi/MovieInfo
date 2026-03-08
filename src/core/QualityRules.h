#ifndef CORE_QUALITYRULES_H
#define CORE_QUALITYRULES_H

#include <QString>
#include <QList>

namespace Core
{

    struct QualityRule
    {
        double minRating = 7.0;
        int minVotes = 100000;
        QString targetDrive = "D";
        QString description = "High quality: votes >= 100k AND rating >= 7.0";

        bool matches(double rating, int votes) const
        {
            return votes >= minVotes && rating >= minRating;
        }
    };

    struct MoveRequest
    {
        QString title;
        QString year;
        QString imdbRating;
        QString imdbVotes;
        QString currentPath;
        QString targetDrive;
        QString targetPath;
    };

    // Aspect ratio organization constants
    constexpr double kUltraWideThreshold = 2.3;
    constexpr double kUltraWideMinAR = 2.2;
    constexpr double kUltraWideMaxAR = 2.5;

    inline QString aspectRatioFolder(double ar)
    {
        return ar >= kUltraWideThreshold ? QStringLiteral("UW") : QStringLiteral("16-9");
    }

    inline bool isUltraWide(double ar)
    {
        return ar >= kUltraWideMinAR && ar <= kUltraWideMaxAR;
    }

} // namespace Core

#endif // CORE_QUALITYRULES_H
