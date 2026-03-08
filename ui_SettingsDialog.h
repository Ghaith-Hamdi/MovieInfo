/********************************************************************************
** Form generated from reading UI file 'SettingsDialog.ui'
**
** Created by: Qt User Interface Compiler version 6.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGSDIALOG_H
#define UI_SETTINGSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_SettingsDialog
{
public:
    QVBoxLayout *mainLayout;
    QGroupBox *apiGroup;
    QFormLayout *apiForm;
    QLabel *apiKeyLabel;
    QLineEdit *apiKeyEdit;
    QGroupBox *displayGroup;
    QVBoxLayout *displayLayout;
    QCheckBox *showFetchSummaryCheck;
    QGroupBox *qualityGroup;
    QFormLayout *qualityForm;
    QLabel *minVotesLabel;
    QSpinBox *minVotesSpin;
    QLabel *minRatingLabel;
    QDoubleSpinBox *minRatingSpin;
    QGroupBox *columnsGroup;
    QVBoxLayout *columnsGroupLayout;
    QScrollArea *columnsScrollArea;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *SettingsDialog)
    {
        if (SettingsDialog->objectName().isEmpty())
            SettingsDialog->setObjectName("SettingsDialog");
        SettingsDialog->resize(460, 520);
        mainLayout = new QVBoxLayout(SettingsDialog);
        mainLayout->setSpacing(12);
        mainLayout->setObjectName("mainLayout");
        apiGroup = new QGroupBox(SettingsDialog);
        apiGroup->setObjectName("apiGroup");
        apiForm = new QFormLayout(apiGroup);
        apiForm->setObjectName("apiForm");
        apiKeyLabel = new QLabel(apiGroup);
        apiKeyLabel->setObjectName("apiKeyLabel");

        apiForm->setWidget(0, QFormLayout::ItemRole::LabelRole, apiKeyLabel);

        apiKeyEdit = new QLineEdit(apiGroup);
        apiKeyEdit->setObjectName("apiKeyEdit");
        apiKeyEdit->setEchoMode(QLineEdit::Password);

        apiForm->setWidget(0, QFormLayout::ItemRole::FieldRole, apiKeyEdit);


        mainLayout->addWidget(apiGroup);

        displayGroup = new QGroupBox(SettingsDialog);
        displayGroup->setObjectName("displayGroup");
        displayLayout = new QVBoxLayout(displayGroup);
        displayLayout->setObjectName("displayLayout");
        showFetchSummaryCheck = new QCheckBox(displayGroup);
        showFetchSummaryCheck->setObjectName("showFetchSummaryCheck");

        displayLayout->addWidget(showFetchSummaryCheck);


        mainLayout->addWidget(displayGroup);

        qualityGroup = new QGroupBox(SettingsDialog);
        qualityGroup->setObjectName("qualityGroup");
        qualityForm = new QFormLayout(qualityGroup);
        qualityForm->setObjectName("qualityForm");
        minVotesLabel = new QLabel(qualityGroup);
        minVotesLabel->setObjectName("minVotesLabel");

        qualityForm->setWidget(0, QFormLayout::ItemRole::LabelRole, minVotesLabel);

        minVotesSpin = new QSpinBox(qualityGroup);
        minVotesSpin->setObjectName("minVotesSpin");
        minVotesSpin->setMinimum(0);
        minVotesSpin->setMaximum(10000000);
        minVotesSpin->setSingleStep(10000);

        qualityForm->setWidget(0, QFormLayout::ItemRole::FieldRole, minVotesSpin);

        minRatingLabel = new QLabel(qualityGroup);
        minRatingLabel->setObjectName("minRatingLabel");

        qualityForm->setWidget(1, QFormLayout::ItemRole::LabelRole, minRatingLabel);

        minRatingSpin = new QDoubleSpinBox(qualityGroup);
        minRatingSpin->setObjectName("minRatingSpin");
        minRatingSpin->setMinimum(0.000000000000000);
        minRatingSpin->setMaximum(10.000000000000000);
        minRatingSpin->setSingleStep(0.500000000000000);
        minRatingSpin->setDecimals(1);

        qualityForm->setWidget(1, QFormLayout::ItemRole::FieldRole, minRatingSpin);


        mainLayout->addWidget(qualityGroup);

        columnsGroup = new QGroupBox(SettingsDialog);
        columnsGroup->setObjectName("columnsGroup");
        columnsGroupLayout = new QVBoxLayout(columnsGroup);
        columnsGroupLayout->setObjectName("columnsGroupLayout");
        columnsGroupLayout->setContentsMargins(8, 8, 8, 8);
        columnsScrollArea = new QScrollArea(columnsGroup);
        columnsScrollArea->setObjectName("columnsScrollArea");
        columnsScrollArea->setWidgetResizable(true);
        columnsScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        columnsScrollArea->setMinimumHeight(120);
        columnsScrollArea->setMaximumHeight(180);

        columnsGroupLayout->addWidget(columnsScrollArea);


        mainLayout->addWidget(columnsGroup);

        buttonBox = new QDialogButtonBox(SettingsDialog);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Save);

        mainLayout->addWidget(buttonBox);


        retranslateUi(SettingsDialog);

        QMetaObject::connectSlotsByName(SettingsDialog);
    } // setupUi

    void retranslateUi(QDialog *SettingsDialog)
    {
        SettingsDialog->setWindowTitle(QCoreApplication::translate("SettingsDialog", "Settings", nullptr));
        apiGroup->setTitle(QCoreApplication::translate("SettingsDialog", "API Settings", nullptr));
        apiKeyLabel->setText(QCoreApplication::translate("SettingsDialog", "OMDb API Key:", nullptr));
        apiKeyEdit->setPlaceholderText(QCoreApplication::translate("SettingsDialog", "Enter OMDb API key", nullptr));
        displayGroup->setTitle(QCoreApplication::translate("SettingsDialog", "Display Settings", nullptr));
        showFetchSummaryCheck->setText(QCoreApplication::translate("SettingsDialog", "Show fetch summary after loading", nullptr));
        qualityGroup->setTitle(QCoreApplication::translate("SettingsDialog", "High-Quality Criteria", nullptr));
        minVotesLabel->setText(QCoreApplication::translate("SettingsDialog", "Min IMDb Votes:", nullptr));
        minRatingLabel->setText(QCoreApplication::translate("SettingsDialog", "Min IMDb Rating:", nullptr));
        columnsGroup->setTitle(QCoreApplication::translate("SettingsDialog", "Column Visibility", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SettingsDialog: public Ui_SettingsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGSDIALOG_H
