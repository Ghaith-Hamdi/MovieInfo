#include "OmdbClient.h"
#include <QUrl>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include "MovieDatabase/MovieDatabase.h"

OmdbClient::OmdbClient(const QString &apiKey, MovieDB *movieDb, QObject *parent)
    : QObject(parent), apiKey(apiKey), manager(new QNetworkAccessManager(this)), movieDb(movieDb)
{
}

OmdbClient::~OmdbClient()
{
}

Movie OmdbClient::getExistingMovieData(const QString &movieName)
{
    if (movieDb && movieDb->movieExists(movieName))
    {
        return movieDb->getMovie(movieName);
    }
    return Movie(); // Return empty movie if not found
}

void OmdbClient::fetchMovie(const QString &movieName, int year, bool forceRefresh)
{
    // Check local database first (unless force refresh is requested)
    if (!forceRefresh && movieDb && movieDb->movieExists(movieName))
    {
        Movie movie = movieDb->getMovie(movieName);
        if (!movie.title.isEmpty())
        {
            qDebug() << "Found movie in database:" << movieName;
            emit movieFetched(movie);
            return;
        }
    }

    // If forceRefresh is true and movie exists, emit signal to ask for confirmation
    if (forceRefresh && movieDb && movieDb->movieExists(movieName))
    {
        Movie existingMovie = movieDb->getMovie(movieName);
        if (!existingMovie.title.isEmpty())
        {
            qDebug() << "Movie exists in database, requesting confirmation for refresh:" << movieName;
            emit movieExistsInDatabase(movieName, existingMovie);
            // The actual fetch will be triggered after user confirms
            return;
        }
    }

    // If not in DB, fetch from OMDb API
    QString encodedTitle = QUrl::toPercentEncoding(movieName).replace("%20", "+");
    QString url = QString("http://www.omdbapi.com/?t=%1&y=%2&apikey=%3")
                      .arg(encodedTitle)
                      .arg(year)
                      .arg(apiKey);

    qDebug() << "Sending request to OMDb API:" << url;

    QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(url)));

    connect(reply, &QNetworkReply::finished, this, [this, reply]()
            { onMovieFetched(reply); });
}

void OmdbClient::onMovieFetched(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError)
    {
        qDebug() << "Network error:" << reply->errorString();
        emit fetchFailed("Failed to fetch data");
        reply->deleteLater();
        return;
    }

    const QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    const QJsonObject jsonObject = doc.object();

    Movie movie;
    movie.title = jsonObject.value("Title").toString();
    movie.year = jsonObject.value("Year").toString();
    movie.rated = jsonObject.value("Rated").toString();
    movie.imdbRating = jsonObject.value("imdbRating").toString();
    movie.imdbVotes = jsonObject.value("imdbVotes").toString();
    movie.runtime = jsonObject.value("Runtime").toString();
    movie.director = jsonObject.value("Director").toString();
    movie.actors = jsonObject.value("Actors").toString();
    movie.writer = jsonObject.value("Writer").toString();
    movie.awards = jsonObject.value("Awards").toString();
    movie.language = jsonObject.value("Language").toString();
    movie.country = jsonObject.value("Country").toString();
    movie.boxOffice = jsonObject.value("BoxOffice").toString();
    movie.plot = jsonObject.value("Plot").toString();
    movie.genre = jsonObject.value("Genre").toString();

    movie.released = jsonObject.value("Released").toString();
    movie.poster = jsonObject.value("Poster").toString();
    movie.metascore = jsonObject.value("Metascore").toString();
    movie.imdbID = jsonObject.value("imdbID").toString();

    // Parse ratings
    const QJsonArray ratingsArray = jsonObject.value("Ratings").toArray();
    QStringList ratingsList;
    for (const auto &value : ratingsArray)
    {
        const QJsonObject ratingObj = value.toObject();
        ratingsList.append(ratingObj.value("Source").toString() + ": " + ratingObj.value("Value").toString());
    }
    movie.allRatings = ratingsList.join("; ");

    qDebug() << "Received movie:" << movie.title
             << "| Year:" << movie.year
             << "| IMDb Rating:" << movie.imdbRating
             << "| Votes:" << movie.imdbVotes;

    if (movieDb && movieDb->saveMovie(movie))
    {
        qDebug() << "Movie data saved to database";
    }
    else
    {
        qDebug() << "Failed to save movie data to database";
    }

    emit movieFetched(movie);
    reply->deleteLater();
}
