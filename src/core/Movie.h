#ifndef CORE_MOVIE_H
#define CORE_MOVIE_H

#include <QString>
#include <QDateTime>

namespace Core
{

    struct Movie
    {
        QString imdbId;
        QString title;
        int year = 0;
        QString rated;
        QString released;
        QString runtime;
        QString genre;
        QString director;
        QString writer;
        QString actors;
        QString plot;
        QString language;
        QString country;
        QString awards;
        QString poster;
        QString metascore;
        double imdbRating = 0.0;
        int imdbVotes = 0;
        QString boxOffice;
        QString allRatings;
        QDateTime fetchedAt;

        bool isValid() const { return !title.isEmpty() && year > 0; }

        // Helper to format votes with commas for display
        QString formattedVotes() const
        {
            if (imdbVotes <= 0)
                return QString();
            return QLocale(QLocale::English).toString(imdbVotes);
        }

        // Helper to format rating as string
        QString formattedRating() const
        {
            if (imdbRating <= 0.0)
                return QString();
            return QString::number(imdbRating, 'f', 1);
        }
    };

} // namespace Core

#endif // CORE_MOVIE_H
