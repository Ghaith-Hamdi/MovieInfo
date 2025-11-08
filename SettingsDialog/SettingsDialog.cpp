#include "SettingsDialog.h"
#include <QLabel>

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUI();
    loadSettings();
}

SettingsDialog::~SettingsDialog()
{
}

void SettingsDialog::setupUI()
{
    setWindowTitle("Settings");
    setMinimumWidth(500);
    setMinimumHeight(300);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // Fetch Settings Group
    QGroupBox *fetchGroup = new QGroupBox("Fetch Settings");
    QVBoxLayout *fetchLayout = new QVBoxLayout(fetchGroup);
    fetchLayout->setSpacing(10);

    showFetchSummaryCheckbox = new QCheckBox("Show summary popup after fetching movie data");
    showFetchSummaryCheckbox->setToolTip("Display a popup showing which movies were fetched from IMDb");
    fetchLayout->addWidget(showFetchSummaryCheckbox);

    fetchGroup->setLayout(fetchLayout);
    mainLayout->addWidget(fetchGroup);

    // Add stretch to push buttons to bottom
    mainLayout->addStretch();

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();

    cancelButton = new QPushButton("Cancel");
    cancelButton->setMinimumWidth(100);
    cancelButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #6c757d;"
        "    color: white;"
        "    border: none;"
        "    padding: 8px 16px;"
        "    border-radius: 4px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: #5a6268;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #545b62;"
        "}");

    saveButton = new QPushButton("Save");
    saveButton->setMinimumWidth(100);
    saveButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #238636;"
        "    color: white;"
        "    border: none;"
        "    padding: 8px 16px;"
        "    border-radius: 4px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: #2ea043;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #1a7f37;"
        "}");

    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(saveButton);

    mainLayout->addLayout(buttonLayout);

    // Connect signals
    connect(saveButton, &QPushButton::clicked, this, &SettingsDialog::onSaveClicked);
    connect(cancelButton, &QPushButton::clicked, this, &SettingsDialog::onCancelClicked);

    // Apply dark theme styling to the dialog
    setStyleSheet(
        "QDialog {"
        "    background-color: #0d1117;"
        "}"
        "QGroupBox {"
        "    color: #c9d1d9;"
        "    border: 1px solid #30363d;"
        "    border-radius: 6px;"
        "    margin-top: 10px;"
        "    padding: 15px;"
        "    font-weight: bold;"
        "}"
        "QGroupBox::title {"
        "    subcontrol-origin: margin;"
        "    left: 10px;"
        "    padding: 0 5px;"
        "}"
        "QCheckBox {"
        "    color: #c9d1d9;"
        "    spacing: 8px;"
        "}"
        "QCheckBox::indicator {"
        "    width: 18px;"
        "    height: 18px;"
        "    border: 1px solid #30363d;"
        "    border-radius: 3px;"
        "    background-color: #161b22;"
        "}"
        "QCheckBox::indicator:checked {"
        "    background-color: #238636;"
        "    border-color: #238636;"
        "}"
        "QCheckBox::indicator:checked::after {"
        "    content: '✓';"
        "    color: white;"
        "}");
}

void SettingsDialog::loadSettings()
{
    QSettings settings("MovieInfo", "MovieInfoApp");
    showFetchSummaryCheckbox->setChecked(settings.value("fetch/showSummary", true).toBool());
}

void SettingsDialog::saveSettings()
{
    QSettings settings("MovieInfo", "MovieInfoApp");
    settings.setValue("fetch/showSummary", showFetchSummaryCheckbox->isChecked());
}

void SettingsDialog::onSaveClicked()
{
    saveSettings();
    accept();
}

void SettingsDialog::onCancelClicked()
{
    reject();
}

bool SettingsDialog::shouldShowFetchSummary() const
{
    QSettings settings("MovieInfo", "MovieInfoApp");
    return settings.value("fetch/showSummary", true).toBool();
}
