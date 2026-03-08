#include "FetchSummaryDialog.h"
#include "ui_FetchSummaryDialog.h"
#include <QPushButton>

namespace UI
{

    FetchSummaryDialog::FetchSummaryDialog(const QStringList &fetchedFromImdb,
                                           int fromDb, int fromApi,
                                           QWidget *parent)
        : QDialog(parent), ui(new Ui::FetchSummaryDialog)
    {
        ui->setupUi(this);
        ui->titleLabel->setText(
            QString("Movies Fetched from IMDb (%1)").arg(fetchedFromImdb.size()));

        QString text;
        for (int i = 0; i < fetchedFromImdb.size(); ++i)
            text += QString("%1. %2\n").arg(i + 1).arg(fetchedFromImdb[i]);
        ui->movieListEdit->setPlainText(text);

        ui->statsLabel->setText(
            QString("Total: %1 movies | From Database: %2 | From IMDb: %3")
                .arg(fromDb + fromApi)
                .arg(fromDb)
                .arg(fromApi));

        connect(ui->closeBtn, &QPushButton::clicked, this, &QDialog::accept);
    }

    FetchSummaryDialog::~FetchSummaryDialog()
    {
        delete ui;
    }

} // namespace UI
