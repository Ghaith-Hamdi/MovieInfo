#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QSettings>

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();

    // Getters for settings
    bool shouldShowFetchSummary() const;

private slots:
    void onSaveClicked();
    void onCancelClicked();

private:
    void setupUI();
    void loadSettings();
    void saveSettings();

    // UI Components
    QCheckBox *showFetchSummaryCheckbox;
    QPushButton *saveButton;
    QPushButton *cancelButton;
};

#endif // SETTINGSDIALOG_H
