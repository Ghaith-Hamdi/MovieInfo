#include "OmdbClient.h"
#include <QUrl>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

OmdbClient::OmdbClient(const QString &apiKey, QObject *parent)
    : QObject(parent), apiKey(apiKey), manager(new QNetworkAccessManager(this))
{
}

OmdbClient::~OmdbClient()
{
}

void OmdbClient::fetchMovie(const QString &movieName, int year)
{
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

    emit movieFetched(movieData);
    reply->deleteLater();
}
