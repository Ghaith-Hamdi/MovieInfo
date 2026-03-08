#include "ActionButtonDelegate.h"
#include "models/MovieTableModel.h"
#include <QPainter>
#include <QMouseEvent>
#include <QPushButton>
#include <QHBoxLayout>
namespace UI
{

    ActionButtonDelegate::ActionButtonDelegate(QObject *parent)
        : QStyledItemDelegate(parent)
    {
    }

    // -----------------------------------------------------------------------
    // Layout constants
    // -----------------------------------------------------------------------
    static const int kBtnW = 52;   // button width
    static const int kBtnH = 22;   // button height
    static const int kSpacing = 4; // gap between buttons
    static const int kMarginX = 4; // left edge margin inside the cell

    static QRect buttonRect(int i, const QStyleOptionViewItem &option)
    {
        int x = option.rect.x() + kMarginX + i * (kBtnW + kSpacing);
        int y = option.rect.y() + (option.rect.height() - kBtnH) / 2;
        return QRect(x, y, kBtnW, kBtnH);
    }

    struct BtnDef
    {
        const char *label;
        QColor bg;
        QColor bgHover;
        QColor text;
    };
    static const BtnDef kBtns[3] = {
        {"Open", QColor("#1a5fb4"), QColor("#2472d4"), QColor("#ffffff")},
        {"IMDb", QColor("#c8860a"), QColor("#e8a020"), QColor("#ffffff")},
        {"Pahe", QColor("#1c7a39"), QColor("#24a34c"), QColor("#ffffff")},
    };

    // -----------------------------------------------------------------------
    // paint
    // -----------------------------------------------------------------------
    void ActionButtonDelegate::paint(QPainter *painter,
                                     const QStyleOptionViewItem &option,
                                     const QModelIndex &index) const
    {
        // Let the base class fill the cell (handles selection / hover / alt-row)
        QStyledItemDelegate::paint(painter, option, index);

        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);

        QFont f = painter->font();
        f.setPointSize(8);
        f.setBold(true);
        painter->setFont(f);

        for (int i = 0; i < 3; ++i)
        {
            QRect r = buttonRect(i, option);
            painter->setPen(Qt::NoPen);
            painter->setBrush(kBtns[i].bg);
            painter->drawRoundedRect(r, 4, 4);

            painter->setPen(kBtns[i].text);
            painter->drawText(r, Qt::AlignCenter, kBtns[i].label);
        }

        painter->restore();
    }

    // -----------------------------------------------------------------------
    // sizeHint
    // -----------------------------------------------------------------------
    QSize ActionButtonDelegate::sizeHint(const QStyleOptionViewItem &option,
                                         const QModelIndex &index) const
    {
        Q_UNUSED(option);
        Q_UNUSED(index);
        return QSize(3 * kBtnW + 2 * kSpacing + 2 * kMarginX, kBtnH + 8);
    }

    // -----------------------------------------------------------------------
    // editorEvent — detect left-click on a button
    // -----------------------------------------------------------------------
    bool ActionButtonDelegate::editorEvent(QEvent *event,
                                           QAbstractItemModel *model,
                                           const QStyleOptionViewItem &option,
                                           const QModelIndex &index)
    {
        Q_UNUSED(model);
        if (event->type() != QEvent::MouseButtonRelease)
            return false;

        auto *me = static_cast<QMouseEvent *>(event);
        if (me->button() != Qt::LeftButton)
            return false;

        QPoint pos = me->position().toPoint();
        for (int i = 0; i < 3; ++i)
        {
            if (!buttonRect(i, option).contains(pos))
                continue;

            QString filePath = index.data(Models::MovieTableModel::FilePathRole).toString();
            QString title = index.data(Models::MovieTableModel::FolderTitleRole).toString();
            int year = index.data(Models::MovieTableModel::FolderYearRole).toInt();
            QString yearStr = year > 0 ? QString::number(year) : "";
            QString imdbId = index.data(Models::MovieTableModel::ImdbIdRole).toString();

            switch (i)
            {
            case 0:
                emit openFileClicked(filePath);
                break;
            case 1:
                emit imdbClicked(title, yearStr, imdbId);
                break;
            case 2:
                emit paheClicked(title, yearStr, imdbId);
                break;
            }
            return true;
        }
        return false;
    }

} // namespace UI
