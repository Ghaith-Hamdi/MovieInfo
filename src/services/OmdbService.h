#ifndef SERVICES_OMDBSERVICE_H
#define SERVICES_OMDBSERVICE_H

#include <QObject>
#include <QQueue>
#include <QTimer>
#include "core/Movie.h"
#include "infrastructure/api/OmdbApiClient.h"
#include "infrastructure/db/MovieRepository.h"

namespace Services
{

    class OmdbService : public QObject
    {
        Q_OBJECT

    public:
        explicit OmdbService(Infrastructure::OmdbApiClient *apiClient,
                             Infrastructure::MovieRepository *movieRepo,
                             QObject *parent = nullptr);

        void fetchMovie(const QString &title, int year, bool forceRefresh = false);
        void preloadCache();

    signals:
        void movieFetched(const Core::Movie &movie);
        void movieFromCache(const Core::Movie &movie);
        void fetchFailed(const QString &title, const QString &error);
        void movieExistsInDatabase(const QString &title, const Core::Movie &existing);

    private slots:
        void onApiFetched(const Core::Movie &movie);
        void onApiFailed(const QString &title, const QString &error);

    private:
        Infrastructure::OmdbApiClient *m_apiClient;
        Infrastructure::MovieRepository *m_movieRepo;
    };

} // namespace Services

#endif // SERVICES_OMDBSERVICE_H
