#ifndef INFRASTRUCTURE_OMDBAPICLIENT_H
#define INFRASTRUCTURE_OMDBAPICLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include "core/Movie.h"

namespace Infrastructure
{

    class OmdbApiClient : public QObject
    {
        Q_OBJECT

    public:
        explicit OmdbApiClient(const QString &apiKey, QObject *parent = nullptr);

        void fetch(const QString &title, int year);
        void setApiKey(const QString &apiKey) { m_apiKey = apiKey; }

    signals:
        void movieFetched(const Core::Movie &movie);
        void fetchFailed(const QString &title, const QString &error);

    private:
        void onReplyFinished(class QNetworkReply *reply);

        QString m_apiKey;
        QNetworkAccessManager *m_manager;
    };

} // namespace Infrastructure

#endif // INFRASTRUCTURE_OMDBAPICLIENT_H
