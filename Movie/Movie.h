#ifndef MOVIE_H
#define MOVIE_H

#include <QString>
#include <QStringList>

struct Movie
{
    QString title;
    QString year;
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
    QString imdbRating;
    QString imdbVotes;
    QString imdbID;
    QString boxOffice;
    QString allRatings;
};

#endif // MOVIE_H