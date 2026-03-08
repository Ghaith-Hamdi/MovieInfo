#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"
#include <QGridLayout>

namespace UI
{

    SettingsDialog::SettingsDialog(Infrastructure::AppSettings *settings, QWidget *parent)
        : QDialog(parent), ui(new Ui::SettingsDialog), m_settings(settings)
    {
        ui->setupUi(this);
        buildColumnCheckboxes();
        loadSettings();
        connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &SettingsDialog::onSave);
        connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    }

    SettingsDialog::~SettingsDialog()
    {
        delete ui;
    }

    void SettingsDialog::buildColumnCheckboxes()
    {
        QWidget *scrollWidget = new QWidget(ui->columnsScrollArea);
        QGridLayout *grid = new QGridLayout(scrollWidget);
        grid->setSpacing(6);
        grid->setContentsMargins(6, 6, 6, 6);

        const auto &defs = Core::columnDefinitions();
        int gridRow = 0, gridCol = 0;
        for (const auto &def : defs)
        {
            if (def.id == Core::Column::Actions)
                continue;

            int colIdx = static_cast<int>(def.id);
            auto *check = new QCheckBox(def.name, scrollWidget);
            check->setChecked(m_settings->columnVisible(colIdx));
            check->setProperty("colIndex", colIdx);

            if (def.id == Core::Column::Title)
            {
                check->setEnabled(false);
                check->setChecked(true);
            }

            m_columnChecks.append(check);
            grid->addWidget(check, gridRow, gridCol);
            gridCol++;
            if (gridCol == 3)
            {
                gridCol = 0;
                gridRow++;
            }
        }

        ui->columnsScrollArea->setWidget(scrollWidget);
    }

    void SettingsDialog::loadSettings()
    {
        ui->apiKeyEdit->setText(m_settings->apiKey());
        ui->showFetchSummaryCheck->setChecked(m_settings->showFetchSummary());

        Core::QualityRule rule = m_settings->qualityRule();
        ui->minVotesSpin->setValue(rule.minVotes);
        ui->minRatingSpin->setValue(rule.minRating);
    }

    void SettingsDialog::onSave()
    {
        m_settings->setApiKey(ui->apiKeyEdit->text().trimmed());
        m_settings->setShowFetchSummary(ui->showFetchSummaryCheck->isChecked());

        Core::QualityRule rule;
        rule.minVotes = ui->minVotesSpin->value();
        rule.minRating = ui->minRatingSpin->value();
        m_settings->setQualityRule(rule);

        for (auto *check : m_columnChecks)
        {
            int col = check->property("colIndex").toInt();
            m_settings->setColumnVisible(col, check->isChecked());
        }

        accept();
    }

} // namespace UI
