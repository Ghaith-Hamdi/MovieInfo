/********************************************************************************
** Form generated from reading UI file 'FetchSummaryDialog.ui'
**
** Created by: Qt User Interface Compiler version 6.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FETCHSUMMARYDIALOG_H
#define UI_FETCHSUMMARYDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_FetchSummaryDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *titleLabel;
    QTextEdit *movieListEdit;
    QLabel *statsLabel;
    QHBoxLayout *btnLayout;
    QSpacerItem *btnSpacer;
    QPushButton *closeBtn;

    void setupUi(QDialog *FetchSummaryDialog)
    {
        if (FetchSummaryDialog->objectName().isEmpty())
            FetchSummaryDialog->setObjectName("FetchSummaryDialog");
        FetchSummaryDialog->resize(620, 420);
        verticalLayout = new QVBoxLayout(FetchSummaryDialog);
        verticalLayout->setSpacing(15);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(20, 20, 20, 20);
        titleLabel = new QLabel(FetchSummaryDialog);
        titleLabel->setObjectName("titleLabel");

        verticalLayout->addWidget(titleLabel);

        movieListEdit = new QTextEdit(FetchSummaryDialog);
        movieListEdit->setObjectName("movieListEdit");
        movieListEdit->setReadOnly(true);

        verticalLayout->addWidget(movieListEdit);

        statsLabel = new QLabel(FetchSummaryDialog);
        statsLabel->setObjectName("statsLabel");

        verticalLayout->addWidget(statsLabel);

        btnLayout = new QHBoxLayout();
        btnLayout->setObjectName("btnLayout");
        btnSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        btnLayout->addItem(btnSpacer);

        closeBtn = new QPushButton(FetchSummaryDialog);
        closeBtn->setObjectName("closeBtn");
        closeBtn->setMinimumWidth(100);

        btnLayout->addWidget(closeBtn);


        verticalLayout->addLayout(btnLayout);


        retranslateUi(FetchSummaryDialog);

        QMetaObject::connectSlotsByName(FetchSummaryDialog);
    } // setupUi

    void retranslateUi(QDialog *FetchSummaryDialog)
    {
        FetchSummaryDialog->setWindowTitle(QCoreApplication::translate("FetchSummaryDialog", "Fetch Summary", nullptr));
        titleLabel->setText(QCoreApplication::translate("FetchSummaryDialog", "Movies Fetched from IMDb", nullptr));
        statsLabel->setText(QCoreApplication::translate("FetchSummaryDialog", "Total: 0 | From Database: 0 | From IMDb: 0", nullptr));
        closeBtn->setText(QCoreApplication::translate("FetchSummaryDialog", "Close", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FetchSummaryDialog: public Ui_FetchSummaryDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FETCHSUMMARYDIALOG_H
