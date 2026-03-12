/********************************************************************************
** Form generated from reading UI file 'FilterPanel.ui'
**
** Created by: Qt User Interface Compiler version 6.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILTERPANEL_H
#define UI_FILTERPANEL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FilterPanel
{
public:
    QHBoxLayout *horizontalLayout;
    QLineEdit *searchEdit;
    QLabel *decadeLabel;
    QComboBox *decadeCombo;
    QLabel *aspectRatioLabel;
    QComboBox *aspectRatioCombo;
    QLabel *qualityLabel;
    QComboBox *qualityCombo;
    QSpacerItem *filterSpacer;

    void setupUi(QWidget *FilterPanel)
    {
        if (FilterPanel->objectName().isEmpty())
            FilterPanel->setObjectName("FilterPanel");
        FilterPanel->resize(800, 40);
        horizontalLayout = new QHBoxLayout(FilterPanel);
        horizontalLayout->setSpacing(12);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 6, 0, 6);
        searchEdit = new QLineEdit(FilterPanel);
        searchEdit->setObjectName("searchEdit");
        searchEdit->setMinimumWidth(250);
        searchEdit->setClearButtonEnabled(true);

        horizontalLayout->addWidget(searchEdit);

        decadeLabel = new QLabel(FilterPanel);
        decadeLabel->setObjectName("decadeLabel");

        horizontalLayout->addWidget(decadeLabel);

        decadeCombo = new QComboBox(FilterPanel);
        decadeCombo->addItem(QString());
        decadeCombo->setObjectName("decadeCombo");
        decadeCombo->setMinimumWidth(80);

        horizontalLayout->addWidget(decadeCombo);

        aspectRatioLabel = new QLabel(FilterPanel);
        aspectRatioLabel->setObjectName("aspectRatioLabel");

        horizontalLayout->addWidget(aspectRatioLabel);

        aspectRatioCombo = new QComboBox(FilterPanel);
        aspectRatioCombo->addItem(QString());
        aspectRatioCombo->addItem(QString());
        aspectRatioCombo->addItem(QString());
        aspectRatioCombo->setObjectName("aspectRatioCombo");
        aspectRatioCombo->setMinimumWidth(100);

        horizontalLayout->addWidget(aspectRatioCombo);

        qualityLabel = new QLabel(FilterPanel);
        qualityLabel->setObjectName("qualityLabel");

        horizontalLayout->addWidget(qualityLabel);

        qualityCombo = new QComboBox(FilterPanel);
        qualityCombo->addItem(QString());
        qualityCombo->setObjectName("qualityCombo");
        qualityCombo->setMinimumWidth(80);

        horizontalLayout->addWidget(qualityCombo);

        filterSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(filterSpacer);


        retranslateUi(FilterPanel);

        QMetaObject::connectSlotsByName(FilterPanel);
    } // setupUi

    void retranslateUi(QWidget *FilterPanel)
    {
        searchEdit->setPlaceholderText(QCoreApplication::translate("FilterPanel", "Search movies...", nullptr));
#if QT_CONFIG(tooltip)
        searchEdit->setToolTip(QCoreApplication::translate("FilterPanel", "Search through all movie titles, directors, actors, and other fields", nullptr));
#endif // QT_CONFIG(tooltip)
        decadeLabel->setText(QCoreApplication::translate("FilterPanel", "Decade:", nullptr));
        decadeCombo->setItemText(0, QCoreApplication::translate("FilterPanel", "All", nullptr));

#if QT_CONFIG(tooltip)
        decadeCombo->setToolTip(QCoreApplication::translate("FilterPanel", "Filter movies by release decade", nullptr));
#endif // QT_CONFIG(tooltip)
        aspectRatioLabel->setText(QCoreApplication::translate("FilterPanel", "Aspect Ratio:", nullptr));
        aspectRatioCombo->setItemText(0, QCoreApplication::translate("FilterPanel", "All", nullptr));
        aspectRatioCombo->setItemText(1, QCoreApplication::translate("FilterPanel", "16:9", nullptr));
        aspectRatioCombo->setItemText(2, QCoreApplication::translate("FilterPanel", "Ultra-Wide", nullptr));

#if QT_CONFIG(tooltip)
        aspectRatioCombo->setToolTip(QCoreApplication::translate("FilterPanel", "Filter movies by aspect ratio", nullptr));
#endif // QT_CONFIG(tooltip)
        qualityLabel->setText(QCoreApplication::translate("FilterPanel", "Quality:", nullptr));
        qualityCombo->setItemText(0, QCoreApplication::translate("FilterPanel", "All", nullptr));

#if QT_CONFIG(tooltip)
        qualityCombo->setToolTip(QCoreApplication::translate("FilterPanel", "Filter movies by video quality (4K, 1080p, 720p, etc.)", nullptr));
#endif // QT_CONFIG(tooltip)
        (void)FilterPanel;
    } // retranslateUi

};

namespace Ui {
    class FilterPanel: public Ui_FilterPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILTERPANEL_H
