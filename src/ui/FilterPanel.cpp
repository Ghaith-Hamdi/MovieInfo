#include "FilterPanel.h"
#include "ui_FilterPanel.h"

namespace UI
{

    FilterPanel::FilterPanel(QWidget *parent) : QWidget(parent), ui(new Ui::FilterPanel)
    {
        ui->setupUi(this);
        connect(ui->searchEdit, &QLineEdit::textChanged, this, &FilterPanel::onFilterControlChanged);
        connect(ui->decadeCombo, &QComboBox::currentTextChanged, this, &FilterPanel::onFilterControlChanged);
        connect(ui->aspectRatioCombo, &QComboBox::currentTextChanged, this, &FilterPanel::onFilterControlChanged);
        connect(ui->qualityCombo, &QComboBox::currentTextChanged, this, &FilterPanel::onFilterControlChanged);
    }

    FilterPanel::~FilterPanel()
    {
        delete ui;
    }

    Core::FilterCriteria FilterPanel::currentCriteria() const
    {
        Core::FilterCriteria c;
        c.searchText = ui->searchEdit->text().trimmed();

        QString decade = ui->decadeCombo->currentText();
        if (decade != "All")
            c.decade = decade;

        QString ar = ui->aspectRatioCombo->currentText();
        if (ar != "All")
            c.aspectRatioGroup = ar;

        QString quality = ui->qualityCombo->currentText();
        if (quality != "All")
            c.qualityTier = quality;

        return c;
    }

    void FilterPanel::clearFilters()
    {
        ui->searchEdit->clear();
        ui->decadeCombo->setCurrentIndex(0);
        ui->aspectRatioCombo->setCurrentIndex(0);
        ui->qualityCombo->setCurrentIndex(0);
    }

    void FilterPanel::setDecades(const QStringList &decades)
    {
        QString current = ui->decadeCombo->currentText();
        ui->decadeCombo->clear();
        ui->decadeCombo->addItem("All");
        QStringList sorted = decades;
        sorted.sort();
        ui->decadeCombo->addItems(sorted);
        int idx = ui->decadeCombo->findText(current);
        if (idx >= 0)
            ui->decadeCombo->setCurrentIndex(idx);
    }

    void FilterPanel::setQualities(const QStringList &qualities)
    {
        QString current = ui->qualityCombo->currentText();
        ui->qualityCombo->clear();
        ui->qualityCombo->addItem("All");
        ui->qualityCombo->addItems(qualities);
        int idx = ui->qualityCombo->findText(current);
        if (idx >= 0)
            ui->qualityCombo->setCurrentIndex(idx);
    }

    void FilterPanel::onFilterControlChanged()
    {
        emit filterChanged(currentCriteria());
    }

} // namespace UI
