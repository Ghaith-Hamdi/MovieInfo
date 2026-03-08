#ifndef MODELS_MOVIEFILTERPROXYMODEL_H
#define MODELS_MOVIEFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include "core/FilterCriteria.h"
#include "core/ColumnDef.h"

namespace Models
{

    class MovieFilterProxyModel : public QSortFilterProxyModel
    {
        Q_OBJECT

    public:
        explicit MovieFilterProxyModel(QObject *parent = nullptr);

        void setFilter(const Core::FilterCriteria &criteria);
        const Core::FilterCriteria &filter() const;
        void clearFilter();

        // Column visibility
        void setColumnVisible(Core::Column col, bool visible);
        bool isColumnVisible(Core::Column col) const;

    protected:
        bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
        bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;

    private:
        Core::FilterCriteria m_criteria;
        QSet<int> m_hiddenColumns;
    };

} // namespace Models

#endif // MODELS_MOVIEFILTERPROXYMODEL_H
