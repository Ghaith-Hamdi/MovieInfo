#ifndef UI_DIALOGS_SETTINGSDIALOG_H
#define UI_DIALOGS_SETTINGSDIALOG_H

#include <QDialog>
#include <QCheckBox>
#include <QList>
#include "infrastructure/settings/AppSettings.h"
#include "core/ColumnDef.h"

namespace Ui
{
    class SettingsDialog;
}

namespace UI
{

    class SettingsDialog : public QDialog
    {
        Q_OBJECT

    public:
        explicit SettingsDialog(Infrastructure::AppSettings *settings,
                                QWidget *parent = nullptr);
        ~SettingsDialog();

    private slots:
        void onSave();

    private:
        void buildColumnCheckboxes();
        void loadSettings();

        Ui::SettingsDialog *ui;
        Infrastructure::AppSettings *m_settings;
        QList<QCheckBox *> m_columnChecks;
    };

} // namespace UI

#endif // UI_DIALOGS_SETTINGSDIALOG_H
