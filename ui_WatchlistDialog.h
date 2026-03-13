/********************************************************************************
** Form generated from reading UI file 'WatchlistDialog.ui'
**
** Created by: Qt User Interface Compiler version 6.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WATCHLISTDIALOG_H
#define UI_WATCHLISTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WatchlistDialog
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *headerLayout;
    QPushButton *importBtn;
    QLabel *fileLabel;
    QSpacerItem *headerSpacer;
    QLabel *fetchStatusLabel;
    QProgressBar *progressBar;
    QTabWidget *tabWidget;
    QWidget *haveTab;
    QVBoxLayout *haveLayout;
    QTableWidget *haveTable;
    QWidget *missingTab;
    QVBoxLayout *missingLayout;
    QTableWidget *missingTable;
    QHBoxLayout *footerLayout;
    QLabel *summaryLabel;
    QSpacerItem *footerSpacer;
    QPushButton *closeBtn;

    void setupUi(QDialog *WatchlistDialog)
    {
        if (WatchlistDialog->objectName().isEmpty())
            WatchlistDialog->setObjectName("WatchlistDialog");
        WatchlistDialog->resize(1150, 720);
        verticalLayout = new QVBoxLayout(WatchlistDialog);
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(10, 10, 10, 10);
        headerLayout = new QHBoxLayout();
        headerLayout->setSpacing(8);
        headerLayout->setObjectName("headerLayout");
        importBtn = new QPushButton(WatchlistDialog);
        importBtn->setObjectName("importBtn");

        headerLayout->addWidget(importBtn);

        fileLabel = new QLabel(WatchlistDialog);
        fileLabel->setObjectName("fileLabel");

        headerLayout->addWidget(fileLabel);

        headerSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        headerLayout->addItem(headerSpacer);

        fetchStatusLabel = new QLabel(WatchlistDialog);
        fetchStatusLabel->setObjectName("fetchStatusLabel");
        fetchStatusLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

        headerLayout->addWidget(fetchStatusLabel);

        progressBar = new QProgressBar(WatchlistDialog);
        progressBar->setObjectName("progressBar");
        progressBar->setVisible(false);
        progressBar->setMinimumWidth(180);
        progressBar->setMinimum(0);
        progressBar->setMaximum(100);
        progressBar->setValue(0);
        progressBar->setTextVisible(true);

        headerLayout->addWidget(progressBar);


        verticalLayout->addLayout(headerLayout);

        tabWidget = new QTabWidget(WatchlistDialog);
        tabWidget->setObjectName("tabWidget");
        haveTab = new QWidget();
        haveTab->setObjectName("haveTab");
        haveLayout = new QVBoxLayout(haveTab);
        haveLayout->setSpacing(4);
        haveLayout->setObjectName("haveLayout");
        haveLayout->setContentsMargins(4, 4, 4, 4);
        haveTable = new QTableWidget(haveTab);
        if (haveTable->columnCount() < 11)
            haveTable->setColumnCount(11);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        haveTable->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        haveTable->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        haveTable->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        haveTable->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        haveTable->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        haveTable->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        haveTable->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        haveTable->setHorizontalHeaderItem(7, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        haveTable->setHorizontalHeaderItem(8, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        haveTable->setHorizontalHeaderItem(9, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        haveTable->setHorizontalHeaderItem(10, __qtablewidgetitem10);
        haveTable->setObjectName("haveTable");
        haveTable->setSelectionBehavior(QAbstractItemView::SelectRows);
        haveTable->setAlternatingRowColors(true);
        haveTable->setSortingEnabled(true);
        haveTable->setColumnCount(11);
        haveTable->horizontalHeader()->setStretchLastSection(true);
        haveTable->verticalHeader()->setVisible(false);

        haveLayout->addWidget(haveTable);

        tabWidget->addTab(haveTab, QString());
        missingTab = new QWidget();
        missingTab->setObjectName("missingTab");
        missingLayout = new QVBoxLayout(missingTab);
        missingLayout->setSpacing(4);
        missingLayout->setObjectName("missingLayout");
        missingLayout->setContentsMargins(4, 4, 4, 4);
        missingTable = new QTableWidget(missingTab);
        if (missingTable->columnCount() < 9)
            missingTable->setColumnCount(9);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        missingTable->setHorizontalHeaderItem(0, __qtablewidgetitem11);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        missingTable->setHorizontalHeaderItem(1, __qtablewidgetitem12);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        missingTable->setHorizontalHeaderItem(2, __qtablewidgetitem13);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        missingTable->setHorizontalHeaderItem(3, __qtablewidgetitem14);
        QTableWidgetItem *__qtablewidgetitem15 = new QTableWidgetItem();
        missingTable->setHorizontalHeaderItem(4, __qtablewidgetitem15);
        QTableWidgetItem *__qtablewidgetitem16 = new QTableWidgetItem();
        missingTable->setHorizontalHeaderItem(5, __qtablewidgetitem16);
        QTableWidgetItem *__qtablewidgetitem17 = new QTableWidgetItem();
        missingTable->setHorizontalHeaderItem(6, __qtablewidgetitem17);
        QTableWidgetItem *__qtablewidgetitem18 = new QTableWidgetItem();
        missingTable->setHorizontalHeaderItem(7, __qtablewidgetitem18);
        QTableWidgetItem *__qtablewidgetitem19 = new QTableWidgetItem();
        missingTable->setHorizontalHeaderItem(8, __qtablewidgetitem19);
        missingTable->setObjectName("missingTable");
        missingTable->setSelectionBehavior(QAbstractItemView::SelectRows);
        missingTable->setAlternatingRowColors(true);
        missingTable->setSortingEnabled(false);
        missingTable->setColumnCount(9);
        missingTable->horizontalHeader()->setStretchLastSection(true);
        missingTable->verticalHeader()->setVisible(false);

        missingLayout->addWidget(missingTable);

        tabWidget->addTab(missingTab, QString());

        verticalLayout->addWidget(tabWidget);

        footerLayout = new QHBoxLayout();
        footerLayout->setSpacing(8);
        footerLayout->setObjectName("footerLayout");
        summaryLabel = new QLabel(WatchlistDialog);
        summaryLabel->setObjectName("summaryLabel");

        footerLayout->addWidget(summaryLabel);

        footerSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        footerLayout->addItem(footerSpacer);

        closeBtn = new QPushButton(WatchlistDialog);
        closeBtn->setObjectName("closeBtn");

        footerLayout->addWidget(closeBtn);


        verticalLayout->addLayout(footerLayout);


        retranslateUi(WatchlistDialog);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(WatchlistDialog);
    } // setupUi

    void retranslateUi(QDialog *WatchlistDialog)
    {
        WatchlistDialog->setWindowTitle(QCoreApplication::translate("WatchlistDialog", "Watchlist Import", nullptr));
        importBtn->setText(QCoreApplication::translate("WatchlistDialog", "Import Text File\342\200\246", nullptr));
#if QT_CONFIG(tooltip)
        importBtn->setToolTip(QCoreApplication::translate("WatchlistDialog", "Open a plain-text file with one movie title per line", nullptr));
#endif // QT_CONFIG(tooltip)
        fileLabel->setText(QCoreApplication::translate("WatchlistDialog", "No file loaded", nullptr));
        fetchStatusLabel->setText(QString());
        QTableWidgetItem *___qtablewidgetitem = haveTable->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("WatchlistDialog", "#", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = haveTable->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("WatchlistDialog", "Title", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = haveTable->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("WatchlistDialog", "Year", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = haveTable->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("WatchlistDialog", "Rating", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = haveTable->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("WatchlistDialog", "Votes", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = haveTable->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("WatchlistDialog", "Genre", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = haveTable->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("WatchlistDialog", "Director", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = haveTable->horizontalHeaderItem(7);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("WatchlistDialog", "Aspect Ratio", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = haveTable->horizontalHeaderItem(8);
        ___qtablewidgetitem8->setText(QCoreApplication::translate("WatchlistDialog", "Quality", nullptr));
        QTableWidgetItem *___qtablewidgetitem9 = haveTable->horizontalHeaderItem(9);
        ___qtablewidgetitem9->setText(QCoreApplication::translate("WatchlistDialog", "IMDb", nullptr));
        QTableWidgetItem *___qtablewidgetitem10 = haveTable->horizontalHeaderItem(10);
        ___qtablewidgetitem10->setText(QCoreApplication::translate("WatchlistDialog", "Pahe", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(haveTab), QCoreApplication::translate("WatchlistDialog", "Have (0)", nullptr));
        QTableWidgetItem *___qtablewidgetitem11 = missingTable->horizontalHeaderItem(0);
        ___qtablewidgetitem11->setText(QCoreApplication::translate("WatchlistDialog", "#", nullptr));
        QTableWidgetItem *___qtablewidgetitem12 = missingTable->horizontalHeaderItem(1);
        ___qtablewidgetitem12->setText(QCoreApplication::translate("WatchlistDialog", "Title", nullptr));
        QTableWidgetItem *___qtablewidgetitem13 = missingTable->horizontalHeaderItem(2);
        ___qtablewidgetitem13->setText(QCoreApplication::translate("WatchlistDialog", "Year", nullptr));
        QTableWidgetItem *___qtablewidgetitem14 = missingTable->horizontalHeaderItem(3);
        ___qtablewidgetitem14->setText(QCoreApplication::translate("WatchlistDialog", "Rating", nullptr));
        QTableWidgetItem *___qtablewidgetitem15 = missingTable->horizontalHeaderItem(4);
        ___qtablewidgetitem15->setText(QCoreApplication::translate("WatchlistDialog", "Votes", nullptr));
        QTableWidgetItem *___qtablewidgetitem16 = missingTable->horizontalHeaderItem(5);
        ___qtablewidgetitem16->setText(QCoreApplication::translate("WatchlistDialog", "Genre", nullptr));
        QTableWidgetItem *___qtablewidgetitem17 = missingTable->horizontalHeaderItem(6);
        ___qtablewidgetitem17->setText(QCoreApplication::translate("WatchlistDialog", "Director", nullptr));
        QTableWidgetItem *___qtablewidgetitem18 = missingTable->horizontalHeaderItem(7);
        ___qtablewidgetitem18->setText(QCoreApplication::translate("WatchlistDialog", "IMDb", nullptr));
        QTableWidgetItem *___qtablewidgetitem19 = missingTable->horizontalHeaderItem(8);
        ___qtablewidgetitem19->setText(QCoreApplication::translate("WatchlistDialog", "Pahe", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(missingTab), QCoreApplication::translate("WatchlistDialog", "Don't Have (0)", nullptr));
        summaryLabel->setText(QString());
        closeBtn->setText(QCoreApplication::translate("WatchlistDialog", "Close", nullptr));
    } // retranslateUi

};

namespace Ui {
    class WatchlistDialog: public Ui_WatchlistDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WATCHLISTDIALOG_H
