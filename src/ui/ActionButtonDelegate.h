#ifndef UI_ACTIONBUTTONDELEGATE_H
#define UI_ACTIONBUTTONDELEGATE_H

#include <QStyledItemDelegate>
#include <QEvent>

namespace UI
{

    class ActionButtonDelegate : public QStyledItemDelegate
    {
        Q_OBJECT

    public:
        explicit ActionButtonDelegate(QObject *parent = nullptr);

        void paint(QPainter *painter, const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;
        QSize sizeHint(const QStyleOptionViewItem &option,
                       const QModelIndex &index) const override;
        bool editorEvent(QEvent *event, QAbstractItemModel *model,
                         const QStyleOptionViewItem &option,
                         const QModelIndex &index) override;

    signals:
        void openFileClicked(const QString &filePath) const;
        void imdbClicked(const QString &title, const QString &year, const QString &imdbId) const;
        void paheClicked(const QString &title, const QString &year) const;
    };

} // namespace UI

#endif // UI_ACTIONBUTTONDELEGATE_H
