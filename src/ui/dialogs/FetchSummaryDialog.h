#ifndef UI_DIALOGS_FETCHSUMMARYDIALOG_H
#define UI_DIALOGS_FETCHSUMMARYDIALOG_H

#include <QDialog>
#include <QStringList>

namespace Ui
{
    class FetchSummaryDialog;
}

namespace UI
{

    class FetchSummaryDialog : public QDialog
    {
        Q_OBJECT

    public:
        explicit FetchSummaryDialog(const QStringList &fetchedFromImdb,
                                    int fromDb, int fromApi,
                                    QWidget *parent = nullptr);
        ~FetchSummaryDialog();

    private:
        Ui::FetchSummaryDialog *ui;
    };

} // namespace UI

#endif // UI_DIALOGS_FETCHSUMMARYDIALOG_H
