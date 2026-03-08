#ifndef MOVIEINFO_FILTERPANEL_H
#define MOVIEINFO_FILTERPANEL_H

#include <QWidget>
#include "core/FilterCriteria.h"

namespace Ui
{
    class FilterPanel;
}

namespace UI
{

    class FilterPanel : public QWidget
    {
        Q_OBJECT

    public:
        explicit FilterPanel(QWidget *parent = nullptr);
        ~FilterPanel();

        Core::FilterCriteria currentCriteria() const;
        void clearFilters();

        // Populate combo boxes from table data
        void setDecades(const QStringList &decades);
        void setQualities(const QStringList &qualities);

    signals:
        void filterChanged(const Core::FilterCriteria &criteria);

    private slots:
        void onFilterControlChanged();

    private:
        Ui::FilterPanel *ui;
    };

} // namespace UI

#endif // MOVIEINFO_FILTERPANEL_H
