#include "OmdbApiClient.h"
#include <QUrl>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
#include <QDebug>

namespace Infrastructure
{

    OmdbApiClient::OmdbApiClient(const QString &apiKey, QObject *parent)
        : QObject(parent), m_apiKey(apiKey), m_manager(new QNetworkAccessManager(this))
    {
    }

    void OmdbApiClient::fetch(const QString &title, int year)
    {
        QString encodedTitle = QUrl::toPercentEncoding(title).replace("%20", "+");
        QString urlStr = QStringLiteral("https://www.omdbapi.com/?t=%1&y=%2&apikey=%3")
                             .arg(encodedTitle)
                             .arg(year > 0 ? QString::number(year) : QString())
                             .arg(m_apiKey);

        QNetworkReply *reply = m_manager->get(QNetworkRequest(QUrl(urlStr)));
        connect(reply, &QNetworkReply::finished, this, [this, reply]()
                { onReplyFinished(reply); });
    }

    void OmdbApiClient::onReplyFinished(QNetworkReply *reply)
    {
        reply->deleteLater();

        if (reply->error() != QNetworkReply::NoError)
        {
            emit fetchFailed(QString(), reply->errorString());
            return;
        }

        const QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        const QJsonObject obj = doc.object();

        if (obj.value("Response").toString() == "False")
        {
            emit fetchFailed(obj.value("Title").toString(),
                             obj.value("Error").toString("Unknown API error"));
            return;
        }

        Core::Movie movie;
        movie.title = obj.value("Title").toString();
        movie.year = obj.value("Year").toString().toInt();
        movie.rated = obj.value("Rated").toString();
        movie.released = obj.value("Released").toString();
        movie.runtime = obj.value("Runtime").toString();
        movie.genre = obj.value("Genre").toString();
        movie.director = obj.value("Director").toString();
        movie.writer = obj.value("Writer").toString();
        movie.actors = obj.value("Actors").toString();
        movie.plot = obj.value("Plot").toString();
        movie.language = obj.value("Language").toString();
        movie.country = obj.value("Country").toString();
        movie.awards = obj.value("Awards").toString();
        movie.poster = obj.value("Poster").toString();
        movie.metascore = obj.value("Metascore").toString();
        movie.imdbId = obj.value("imdbID").toString();
        movie.boxOffice = obj.value("BoxOffice").toString();

        movie.imdbRating = obj.value("imdbRating").toString().toDouble();

        QString votesStr = obj.value("imdbVotes").toString();
        votesStr.remove(',');
        movie.imdbVotes = votesStr.toInt();

        // Parse ratings array
        const QJsonArray ratingsArray = obj.value("Ratings").toArray();
        QStringList ratingsList;
        for (const auto &val : ratingsArray)
        {
            const QJsonObject rObj = val.toObject();
            ratingsList.append(rObj.value("Source").toString() + ": " + rObj.value("Value").toString());
        }
        movie.allRatings = ratingsList.join("; ");

        movie.fetchedAt = QDateTime::currentDateTime();

        emit movieFetched(movie);
    }

} // namespace Infrastructure
