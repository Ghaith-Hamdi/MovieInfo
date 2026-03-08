#ifndef CORE_FILTERCRITERIA_H
#define CORE_FILTERCRITERIA_H

#include <QString>
#include <QStringList>

namespace Core
{

    struct FilterCriteria
    {
        QString searchText;
        QString decade;           // empty = all
        QString qualityTier;      // empty = all
        QString aspectRatioGroup; // "all", "UltraWide", or specific ratio string
        double minRating = 0.0;
        double maxRating = 10.0;
        int minVotes = 0;
        QStringList genres; // empty = all

        bool isEmpty() const
        {
            return searchText.isEmpty() && decade.isEmpty() && qualityTier.isEmpty() && aspectRatioGroup.isEmpty() && minRating <= 0.0 && maxRating >= 10.0 && minVotes <= 0 && genres.isEmpty();
        }

        void clear()
        {
            searchText.clear();
            decade.clear();
            qualityTier.clear();
            aspectRatioGroup.clear();
            minRating = 0.0;
            maxRating = 10.0;
            minVotes = 0;
            genres.clear();
        }
    };

} // namespace Core

#endif // CORE_FILTERCRITERIA_H
