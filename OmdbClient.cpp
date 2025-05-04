#include "OmdbClient.h"
#include <QUrl>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include "moviedb.h"

OmdbClient::OmdbClient(const QString &apiKey, MovieDB *movieDb, QObject *parent)
    : QObject(parent), apiKey(apiKey), manager(new QNetworkAccessManager(this)), movieDb(movieDb)
{
}

OmdbClient::~OmdbClient()
{
}

void OmdbClient::fetchMovie(const QString &movieName, int year)
{
    // Check if movie exists in database first
    if (movieDb && movieDb->movieExists(movieName))
    {
        Movie movie = movieDb->getMovie(movieName);
        if (!movie.title.isEmpty())
        {
            qDebug() << "Found movie in database:" << movieName;
            emit movieFetched(movie);
            return;
        }
    }

    // If not in database, proceed with API call
    QString encodedMovieName = QUrl::toPercentEncoding(movieName);
    QString url = QString("http://www.omdbapi.com/?t=%1&apikey=%2").arg(encodedMovieName).arg(apiKey);

    if (year != 0)
    {
        url += QString("&y=%1").arg(year);
    }

    qDebug() << "Sending request to OMDb API:" << url;

    QNetworkRequest request{QUrl(url)};
    QNetworkReply *reply = manager->get(request);

    connect(reply, &QNetworkReply::finished, this, [=]()
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

    QByteArray responseData = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(responseData);
    QJsonObject jsonObject = doc.object();

    Movie movie;
    movie.title = jsonObject.value("Title").toString();
    movie.year = jsonObject.value("Year").toString();
    movie.rated = jsonObject.value("Rated").toString();
    movie.released = jsonObject.value("Released").toString();
    movie.runtime = jsonObject.value("Runtime").toString();
    movie.genre = jsonObject.value("Genre").toString();
    movie.director = jsonObject.value("Director").toString();
    movie.writer = jsonObject.value("Writer").toString();
    movie.actors = jsonObject.value("Actors").toString();
    movie.plot = jsonObject.value("Plot").toString();
    movie.language = jsonObject.value("Language").toString();
    movie.country = jsonObject.value("Country").toString();
    movie.awards = jsonObject.value("Awards").toString();
    movie.poster = jsonObject.value("Poster").toString();
    movie.metascore = jsonObject.value("Metascore").toString();
    movie.imdbRating = jsonObject.value("imdbRating").toString();
    movie.imdbVotes = jsonObject.value("imdbVotes").toString();
    movie.imdbID = jsonObject.value("imdbID").toString();
    movie.boxOffice = jsonObject.value("BoxOffice").toString();

    // Extract ratings array
    QJsonArray ratingsArray = jsonObject.value("Ratings").toArray();
    QStringList ratingsList;
    for (const QJsonValue &value : ratingsArray)
    {
        QJsonObject ratingObj = value.toObject();
        QString source = ratingObj.value("Source").toString();
        QString ratingValue = ratingObj.value("Value").toString();
        ratingsList.append(source + ": " + ratingValue);
    }
    movie.allRatings = ratingsList.join("; ");

    qDebug() << "Received movie:" << movie.title << "| Year:" << movie.year
             << "| IMDb Rating:" << movie.imdbRating << "| Votes:" << movie.imdbVotes;

    // Save to database - pass the Movie object directly
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
