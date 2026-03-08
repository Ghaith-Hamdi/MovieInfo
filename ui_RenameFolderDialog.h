/********************************************************************************
** Form generated from reading UI file 'RenameFolderDialog.ui'
**
** Created by: Qt User Interface Compiler version 6.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RENAMEFOLDERDIALOG_H
#define UI_RENAMEFOLDERDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_RenameFolderDialog
{
public:
    QFormLayout *formLayout;
    QLabel *titleLabel;
    QLineEdit *titleEdit;
    QLabel *yearLabel;
    QLineEdit *yearEdit;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *RenameFolderDialog)
    {
        if (RenameFolderDialog->objectName().isEmpty())
            RenameFolderDialog->setObjectName("RenameFolderDialog");
        RenameFolderDialog->resize(520, 150);
        formLayout = new QFormLayout(RenameFolderDialog);
        formLayout->setObjectName("formLayout");
        titleLabel = new QLabel(RenameFolderDialog);
        titleLabel->setObjectName("titleLabel");

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, titleLabel);

        titleEdit = new QLineEdit(RenameFolderDialog);
        titleEdit->setObjectName("titleEdit");

        formLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, titleEdit);

        yearLabel = new QLabel(RenameFolderDialog);
        yearLabel->setObjectName("yearLabel");

        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, yearLabel);

        yearEdit = new QLineEdit(RenameFolderDialog);
        yearEdit->setObjectName("yearEdit");
        yearEdit->setMaxLength(4);

        formLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, yearEdit);

        buttonBox = new QDialogButtonBox(RenameFolderDialog);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        formLayout->setWidget(2, QFormLayout::ItemRole::SpanningRole, buttonBox);


        retranslateUi(RenameFolderDialog);

        QMetaObject::connectSlotsByName(RenameFolderDialog);
    } // setupUi

    void retranslateUi(QDialog *RenameFolderDialog)
    {
        RenameFolderDialog->setWindowTitle(QCoreApplication::translate("RenameFolderDialog", "Rename Folder", nullptr));
        titleLabel->setText(QCoreApplication::translate("RenameFolderDialog", "Movie name:", nullptr));
        yearLabel->setText(QCoreApplication::translate("RenameFolderDialog", "Year:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RenameFolderDialog: public Ui_RenameFolderDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RENAMEFOLDERDIALOG_H
