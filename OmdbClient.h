#ifndef OMDBCLIENT_H
#define OMDBCLIENT_H

#include <QObject>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTableWidget>
#include <QThreadPool>
#include <QtConcurrent>
#include <moviedb.h>

class OmdbClient : public QObject
{
    Q_OBJECT

public:
    explicit OmdbClient(const QString &apiKey, MovieDB *movieDb, QObject *parent = nullptr);
    ~OmdbClient();

    void fetchMovie(const QString &movieName, int year = 0);

signals:
    void movieFetched(const QList<QString> &movieData);
    void fetchFailed(const QString &error);

private slots:
    void onMovieFetched(QNetworkReply *reply);

private:
    QString apiKey;
    QNetworkAccessManager *manager;
    MovieDB *movieDb;
};

#endif // OMDBCLIENT_H
