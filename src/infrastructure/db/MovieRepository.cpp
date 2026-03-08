#include "MovieRepository.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QRegularExpression>
#include <QDebug>
#include <QDateTime>

namespace Infrastructure
{

    MovieRepository::MovieRepository(Database *db, QObject *parent)
        : QObject(parent), m_db(db)
    {
    }

    QString MovieRepository::cacheKey(const QString &title, const QString &year)
    {
        return title.toLower() + "|" + year;
    }

    QString MovieRepository::sanitize(const QString &text)
    {
        static const QRegularExpression forbidden(R"([\\/:*?"<>|,.])");
        return QString(text).remove(forbidden).simplified().toLower();
    }

    void MovieRepository::preloadCache()
    {
        if (m_cacheLoaded || !m_db->isOpen())
            return;

        m_cache.clear();
        m_sanitizedIndex.clear();

        QSqlQuery query(m_db->connection());
        if (!query.exec("SELECT * FROM movies"))
        {
            qWarning() << "Failed to preload movie cache:" << query.lastError().text();
            return;
        }

        while (query.next())
        {
            Core::Movie movie = rowToMovie(query);
            QString key = cacheKey(movie.title, QString::number(movie.year));
            m_cache.insert(key, movie);

            QString sanitized = sanitize(movie.title) + "|" + QString::number(movie.year);
            m_sanitizedIndex.insert(sanitized, key);
        }

        m_cacheLoaded = true;
        qDebug() << "Preloaded" << m_cache.size() << "movies into cache";
    }

    bool MovieRepository::save(const Core::Movie &movie)
    {
        if (!m_db->isOpen())
            return false;

        QSqlQuery query(m_db->connection());
        query.prepare(
            "INSERT OR REPLACE INTO movies "
            "(title, year, rated, rating, votes, runtime, director, actors, writer, "
            "awards, language, country, boxoffice, plot, genre, imdbid, fetched_at) "
            "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");

        QString yearStr = QString::number(movie.year);
        query.addBindValue(movie.title);
        query.addBindValue(yearStr);
        query.addBindValue(movie.rated);
        query.addBindValue(movie.formattedRating());
        query.addBindValue(movie.formattedVotes());
        query.addBindValue(movie.runtime);
        query.addBindValue(movie.director);
        query.addBindValue(movie.actors);
        query.addBindValue(movie.writer);
        query.addBindValue(movie.awards);
        query.addBindValue(movie.language);
        query.addBindValue(movie.country);
        query.addBindValue(movie.boxOffice);
        query.addBindValue(movie.plot);
        query.addBindValue(movie.genre);
        query.addBindValue(movie.imdbId);
        query.addBindValue(movie.fetchedAt.toString(Qt::ISODate));

        bool ok = query.exec();
        if (!ok)
        {
            qWarning() << "Save movie error:" << query.lastError().text();
            return false;
        }

        if (m_cacheLoaded)
        {
            QString key = cacheKey(movie.title, yearStr);
            m_cache.insert(key, movie);
            QString sanitized = sanitize(movie.title) + "|" + yearStr;
            m_sanitizedIndex.insert(sanitized, key);
        }

        return true;
    }

    bool MovieRepository::remove(const QString &title, const QString &year)
    {
        if (!m_db->isOpen())
            return false;

        QSqlQuery query(m_db->connection());
        query.prepare("DELETE FROM movies WHERE title = ? AND year = ?");
        query.addBindValue(title);
        query.addBindValue(year);

        if (!query.exec())
        {
            qWarning() << "Delete movie error:" << query.lastError().text();
            return false;
        }

        if (m_cacheLoaded)
        {
            QString key = cacheKey(title, year);
            m_cache.remove(key);
        }

        return true;
    }

    Core::Movie MovieRepository::get(const QString &title, const QString &year)
    {
        if (!m_cacheLoaded)
            preloadCache();

        return m_cache.value(cacheKey(title, year), Core::Movie());
    }

    Core::Movie MovieRepository::getBySanitizedTitle(const QString &sanitizedTitle, const QString &year)
    {
        if (!m_cacheLoaded)
            preloadCache();

        QString searchKey = sanitize(sanitizedTitle) + "|" + year;
        QString originalKey = m_sanitizedIndex.value(searchKey);
        if (!originalKey.isEmpty())
            return m_cache.value(originalKey, Core::Movie());

        return Core::Movie();
    }

    bool MovieRepository::exists(const QString &title, const QString &year)
    {
        if (!m_cacheLoaded)
            preloadCache();

        return m_cache.contains(cacheKey(title, year));
    }

    Core::Movie MovieRepository::rowToMovie(QSqlQuery &query)
    {
        Core::Movie movie;
        movie.title = query.value("title").toString();
        movie.year = query.value("year").toString().toInt();
        movie.rated = query.value("rated").toString();
        movie.runtime = query.value("runtime").toString();
        movie.director = query.value("director").toString();
        movie.actors = query.value("actors").toString();
        movie.writer = query.value("writer").toString();
        movie.awards = query.value("awards").toString();
        movie.language = query.value("language").toString();
        movie.country = query.value("country").toString();
        movie.boxOffice = query.value("boxoffice").toString();
        movie.plot = query.value("plot").toString();
        movie.genre = query.value("genre").toString();
        movie.imdbId = query.value("imdbid").toString();

        // Parse rating and votes from string to typed values
        QString ratingStr = query.value("rating").toString();
        movie.imdbRating = ratingStr.toDouble();

        QString votesStr = query.value("votes").toString();
        votesStr.remove(',');
        movie.imdbVotes = votesStr.toInt();

        QString fetchedAtStr = query.value("fetched_at").toString();
        if (!fetchedAtStr.isEmpty())
            movie.fetchedAt = QDateTime::fromString(fetchedAtStr, Qt::ISODate);

        return movie;
    }

} // namespace Infrastructure
