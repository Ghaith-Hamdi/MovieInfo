#include "MovieFilterProxyModel.h"
#include "MovieTableModel.h"
#include "core/QualityRules.h"
#include <QRegularExpression>

namespace Models
{

    MovieFilterProxyModel::MovieFilterProxyModel(QObject *parent)
        : QSortFilterProxyModel(parent)
    {
        setDynamicSortFilter(true);
        setSortCaseSensitivity(Qt::CaseInsensitive);
    }

    void MovieFilterProxyModel::setFilter(const Core::FilterCriteria &criteria)
    {
        m_criteria = criteria;
        beginFilterChange();
        endFilterChange();
    }

    const Core::FilterCriteria &MovieFilterProxyModel::filter() const
    {
        return m_criteria;
    }

    void MovieFilterProxyModel::clearFilter()
    {
        m_criteria.clear();
        beginFilterChange();
        endFilterChange();
    }

    void MovieFilterProxyModel::setColumnVisible(Core::Column col, bool visible)
    {
        int idx = static_cast<int>(col);
        if (visible)
            m_hiddenColumns.remove(idx);
        else
            m_hiddenColumns.insert(idx);
    }

    bool MovieFilterProxyModel::isColumnVisible(Core::Column col) const
    {
        return !m_hiddenColumns.contains(static_cast<int>(col));
    }

    bool MovieFilterProxyModel::filterAcceptsRow(int sourceRow,
                                                 const QModelIndex &sourceParent) const
    {
        if (m_criteria.isEmpty())
            return true;

        auto *model = sourceModel();
        if (!model)
            return true;

        // Text search — match across title, genre, director, actors, plot, folder
        if (!m_criteria.searchText.isEmpty())
        {
            bool found = false;
            QList<Core::Column> searchCols = {
                Core::Column::Title, Core::Column::Genre, Core::Column::Director,
                Core::Column::Actors, Core::Column::Plot};
            for (auto col : searchCols)
            {
                QModelIndex idx = model->index(sourceRow, static_cast<int>(col), sourceParent);
                if (idx.data().toString().contains(m_criteria.searchText, Qt::CaseInsensitive))
                {
                    found = true;
                    break;
                }
            }
            if (!found)
                return false;
        }

        // Decade filter
        if (!m_criteria.decade.isEmpty())
        {
            QModelIndex idx = model->index(sourceRow, static_cast<int>(Core::Column::Year), sourceParent);
            int year = idx.data().toString().toInt();
            int decade = (year / 10) * 10;
            if (m_criteria.decade != QString::number(decade) + "s")
                return false;
        }

        // Quality tier filter
        if (!m_criteria.qualityTier.isEmpty())
        {
            QModelIndex idx = model->index(sourceRow, static_cast<int>(Core::Column::Quality), sourceParent);
            if (idx.data().toString() != m_criteria.qualityTier)
                return false;
        }

        // Aspect ratio group filter
        if (!m_criteria.aspectRatioGroup.isEmpty())
        {
            QModelIndex idx = model->index(sourceRow, static_cast<int>(Core::Column::AspectRatio), sourceParent);
            double ar = idx.data(MovieTableModel::RawValueRole).toDouble();
            if (m_criteria.aspectRatioGroup == "Ultra-Wide")
            {
                if (!Core::isUltraWide(ar))
                    return false;
            }
            else if (m_criteria.aspectRatioGroup == "16:9")
            {
                if (Core::isUltraWide(ar))
                    return false;
            }
        }

        // Rating range
        if (m_criteria.minRating > 0.0 || m_criteria.maxRating < 10.0)
        {
            QModelIndex idx = model->index(sourceRow, static_cast<int>(Core::Column::ImdbRating), sourceParent);
            double rating = idx.data(MovieTableModel::RawValueRole).toDouble();
            if (rating > 0.0)
            { // only filter if we have a rating
                if (rating < m_criteria.minRating || rating > m_criteria.maxRating)
                    return false;
            }
        }

        // Votes range
        if (m_criteria.minVotes > 0)
        {
            QModelIndex idx = model->index(sourceRow, static_cast<int>(Core::Column::ImdbVotes), sourceParent);
            int votes = idx.data(MovieTableModel::RawValueRole).toInt();
            if (votes > 0 && votes < m_criteria.minVotes)
                return false;
        }

        // Genre filter
        if (!m_criteria.genres.isEmpty())
        {
            QModelIndex idx = model->index(sourceRow, static_cast<int>(Core::Column::Genre), sourceParent);
            QString genre = idx.data().toString();
            bool matchAny = false;
            for (const auto &g : m_criteria.genres)
            {
                if (genre.contains(g, Qt::CaseInsensitive))
                {
                    matchAny = true;
                    break;
                }
            }
            if (!matchAny)
                return false;
        }

        return true;
    }

    bool MovieFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
    {
        auto col = static_cast<Core::Column>(left.column());

        // Numeric columns: compare by raw value
        if (col == Core::Column::ImdbRating || col == Core::Column::ImdbVotes ||
            col == Core::Column::Year)
        {
            QVariant lVal = left.data(MovieTableModel::RawValueRole);
            QVariant rVal = right.data(MovieTableModel::RawValueRole);
            return lVal.toDouble() < rVal.toDouble();
        }

        // File size: compare by bytes
        if (col == Core::Column::Size)
        {
            QVariant lVal = left.data(MovieTableModel::RawValueRole);
            QVariant rVal = right.data(MovieTableModel::RawValueRole);
            return lVal.toLongLong() < rVal.toLongLong();
        }

        // Default: case-insensitive string comparison
        return QSortFilterProxyModel::lessThan(left, right);
    }

} // namespace Models
