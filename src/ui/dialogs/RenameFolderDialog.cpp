#include "RenameFolderDialog.h"
#include "ui_RenameFolderDialog.h"
#include <QIntValidator>

namespace UI
{

    RenameFolderDialog::RenameFolderDialog(const QString &currentTitle,
                                           const QString &currentYear,
                                           QWidget *parent)
        : QDialog(parent), ui(new Ui::RenameFolderDialog)
    {
        ui->setupUi(this);
        ui->titleEdit->setText(currentTitle);
        ui->yearEdit->setText(currentYear);
        ui->yearEdit->setValidator(new QIntValidator(1800, 3000, ui->yearEdit));
        connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
        connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    }

    RenameFolderDialog::~RenameFolderDialog()
    {
        delete ui;
    }

    QString RenameFolderDialog::newTitle() const
    {
        return ui->titleEdit->text().trimmed();
    }

    QString RenameFolderDialog::newYear() const
    {
        return ui->yearEdit->text().trimmed();
    }

} // namespace UI
