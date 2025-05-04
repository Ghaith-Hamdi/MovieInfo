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
        QStringList movieData = movieDb->getMovie(movieName);
        if (!movieData.isEmpty())
        {
            qDebug() << "Found movie in database:" << movieName;
            emit movieFetched(movieData);
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

    QString title = jsonObject.value("Title").toString();
    QString year = jsonObject.value("Year").toString();
    QString rating = jsonObject.value("imdbRating").toString();
    QString votes = jsonObject.value("imdbVotes").toString();

    qDebug() << "Received movie:" << title << "| Year:" << year << "| Rating:" << rating << "| Votes:" << votes;

    QString runtime = jsonObject.value("Runtime").toString();
    QString director = jsonObject.value("Director").toString();
    QString actors = jsonObject.value("Actors").toString();
    QString awards = jsonObject.value("Awards").toString();
    QString language = jsonObject.value("Language").toString();
    QString country = jsonObject.value("Country").toString();

    QStringList movieData = {
        title, rating, votes, director, year, runtime,
        actors, awards, language, country};

    // Save to database
    if (movieDb && movieDb->saveMovie(movieData))
    {
        qDebug() << "Movie data saved to database";
    }
    else
    {
        qDebug() << "Failed to save movie data to database";
    }

    emit movieFetched(movieData);
    reply->deleteLater();
}
