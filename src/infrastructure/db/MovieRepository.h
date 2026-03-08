#ifndef INFRASTRUCTURE_MOVIEREPOSITORY_H
#define INFRASTRUCTURE_MOVIEREPOSITORY_H

#include <QObject>
#include <QHash>
#include "Database.h"
#include "core/Movie.h"

namespace Infrastructure
{

    class MovieRepository : public QObject
    {
        Q_OBJECT

    public:
        explicit MovieRepository(Database *db, QObject *parent = nullptr);

        void preloadCache();
        bool save(const Core::Movie &movie);
        bool remove(const QString &title, const QString &year);
        Core::Movie get(const QString &title, const QString &year);
        Core::Movie getBySanitizedTitle(const QString &sanitizedTitle, const QString &year);
        bool exists(const QString &title, const QString &year);

        int cachedCount() const { return m_cache.size(); }

    private:
        Core::Movie rowToMovie(class QSqlQuery &query);
        static QString cacheKey(const QString &title, const QString &year);
        static QString sanitize(const QString &text);

        Database *m_db;
        QHash<QString, Core::Movie> m_cache;
        QHash<QString, QString> m_sanitizedIndex; // sanitized key -> original cache key
        bool m_cacheLoaded = false;
    };

} // namespace Infrastructure

#endif // INFRASTRUCTURE_MOVIEREPOSITORY_H
