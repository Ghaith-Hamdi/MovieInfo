#ifndef UI_DIALOGS_RENAMEFOLDERDIALOG_H
#define UI_DIALOGS_RENAMEFOLDERDIALOG_H

#include <QDialog>

namespace Ui
{
    class RenameFolderDialog;
}

namespace UI
{

    class RenameFolderDialog : public QDialog
    {
        Q_OBJECT

    public:
        explicit RenameFolderDialog(const QString &currentTitle, const QString &currentYear,
                                    QWidget *parent = nullptr);
        ~RenameFolderDialog();

        QString newTitle() const;
        QString newYear() const;

    private:
        Ui::RenameFolderDialog *ui;
    };

} // namespace UI

#endif // UI_DIALOGS_RENAMEFOLDERDIALOG_H
