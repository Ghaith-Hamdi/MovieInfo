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

    // Convert Movie to QStringList for database storage
    QStringList toStringList() const
    {
        return {title, year, rated, released, runtime, genre, director, writer,
                actors, plot, language, country, awards, poster, metascore,
                imdbRating, imdbVotes, imdbID, boxOffice, allRatings};
    }

    // Create Movie from QStringList (for database retrieval)
    static Movie fromStringList(const QStringList &list)
    {
        Movie movie;
        if (list.size() >= 20)
        {
            movie.title = list[0];
            movie.year = list[1];
            movie.rated = list[2];
            movie.released = list[3];
            movie.runtime = list[4];
            movie.genre = list[5];
            movie.director = list[6];
            movie.writer = list[7];
            movie.actors = list[8];
            movie.plot = list[9];
            movie.language = list[10];
            movie.country = list[11];
            movie.awards = list[12];
            movie.poster = list[13];
            movie.metascore = list[14];
            movie.imdbRating = list[15];
            movie.imdbVotes = list[16];
            movie.imdbID = list[17];
            movie.boxOffice = list[18];
            movie.allRatings = list[19];
        }
        return movie;
    }
};

#endif // MOVIE_H