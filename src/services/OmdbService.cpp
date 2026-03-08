#include "OmdbService.h"
#include <QDebug>

namespace Services
{

    OmdbService::OmdbService(Infrastructure::OmdbApiClient *apiClient,
                             Infrastructure::MovieRepository *movieRepo,
                             QObject *parent)
        : QObject(parent), m_apiClient(apiClient), m_movieRepo(movieRepo)
    {
        connect(m_apiClient, &Infrastructure::OmdbApiClient::movieFetched,
                this, &OmdbService::onApiFetched);
        connect(m_apiClient, &Infrastructure::OmdbApiClient::fetchFailed,
                this, &OmdbService::onApiFailed);
    }

    void OmdbService::preloadCache()
    {
        m_movieRepo->preloadCache();
    }

    void OmdbService::fetchMovie(const QString &title, int year, bool forceRefresh)
    {
        QString yearStr = (year > 0) ? QString::number(year) : "";

        // Check cache first (unless forcing)
        if (!forceRefresh && !yearStr.isEmpty())
        {
            Core::Movie movie = m_movieRepo->get(title, yearStr);
            if (movie.isValid())
            {
                emit movieFromCache(movie);
                return;
            }

            // Try sanitized match
            movie = m_movieRepo->getBySanitizedTitle(title, yearStr);
            if (movie.isValid())
            {
                emit movieFromCache(movie);
                return;
            }
        }

        // If forcing refresh and movie exists, ask for confirmation first
        if (forceRefresh && !yearStr.isEmpty() && m_movieRepo->exists(title, yearStr))
        {
            Core::Movie existing = m_movieRepo->get(title, yearStr);
            if (existing.isValid())
            {
                emit movieExistsInDatabase(title, existing);
                return;
            }
        }

        // Fetch from API
        m_apiClient->fetch(title, year);
    }

    void OmdbService::onApiFetched(const Core::Movie &movie)
    {
        // Save to database
        m_movieRepo->save(movie);
        emit movieFetched(movie);
    }

    void OmdbService::onApiFailed(const QString &title, const QString &error)
    {
        qWarning() << "OMDb fetch failed for" << title << ":" << error;
        emit fetchFailed(title, error);
    }

} // namespace Services
