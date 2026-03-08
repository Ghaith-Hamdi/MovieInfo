#ifndef UI_DIALOGS_MOVESTOMOVEDIALOG_H
#define UI_DIALOGS_MOVESTOMOVEDIALOG_H

#include <QDialog>
#include "core/QualityRules.h"

namespace Ui
{
    class MovesToMoveDialog;
}

namespace UI
{

    class MovesToMoveDialog : public QDialog
    {
        Q_OBJECT

    public:
        explicit MovesToMoveDialog(QWidget *parent = nullptr);
        ~MovesToMoveDialog();

        void setMovies(const QList<Core::MoveRequest> &movies);

    signals:
        void moveRequested(const QString &currentPath, const QString &targetPath);
        void moveAllWithTeraCopy(const QList<Core::MoveRequest> &movies);

    private slots:
        void onMoveClicked();
        void onMoveAllClicked();

    private:
        void populateTable();

        Ui::MovesToMoveDialog *ui;
        QList<Core::MoveRequest> m_movies;
    };

} // namespace UI

#endif // UI_DIALOGS_MOVESTOMOVEDIALOG_H
