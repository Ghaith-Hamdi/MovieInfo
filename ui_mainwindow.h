/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *searchFilterLayout;
    QLineEdit *searchLineEdit;
    QComboBox *comboBoxQuality;
    QComboBox *comboBoxDecade;
    QComboBox *comboBoxAspectRatio;
    QPushButton *clearFiltersButton;
    QSpacerItem *horizontalSpacer;
    QLabel *countLabel;
    QTableWidget *tableWidget;
    QHBoxLayout *actionsLayout;
    QPushButton *selectFolderButton;
    QPushButton *exportButton;
    QPushButton *refreshButton;
    QSpacerItem *horizontalSpacer_2;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1600, 900);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout_3 = new QVBoxLayout(centralwidget);
        verticalLayout_3->setSpacing(12);
        verticalLayout_3->setObjectName("verticalLayout_3");
        verticalLayout_3->setContentsMargins(16, 12, 16, 12);
        searchFilterLayout = new QHBoxLayout();
        searchFilterLayout->setSpacing(12);
        searchFilterLayout->setObjectName("searchFilterLayout");
        searchLineEdit = new QLineEdit(centralwidget);
        searchLineEdit->setObjectName("searchLineEdit");
        searchLineEdit->setMinimumSize(QSize(250, 36));

        searchFilterLayout->addWidget(searchLineEdit);

        comboBoxQuality = new QComboBox(centralwidget);
        comboBoxQuality->setObjectName("comboBoxQuality");
        comboBoxQuality->setMinimumSize(QSize(100, 36));

        searchFilterLayout->addWidget(comboBoxQuality);

        comboBoxDecade = new QComboBox(centralwidget);
        comboBoxDecade->setObjectName("comboBoxDecade");
        comboBoxDecade->setMinimumSize(QSize(100, 36));

        searchFilterLayout->addWidget(comboBoxDecade);

        comboBoxAspectRatio = new QComboBox(centralwidget);
        comboBoxAspectRatio->setObjectName("comboBoxAspectRatio");
        comboBoxAspectRatio->setMinimumSize(QSize(110, 36));

        searchFilterLayout->addWidget(comboBoxAspectRatio);

        clearFiltersButton = new QPushButton(centralwidget);
        clearFiltersButton->setObjectName("clearFiltersButton");
        clearFiltersButton->setMinimumSize(QSize(70, 36));

        searchFilterLayout->addWidget(clearFiltersButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        searchFilterLayout->addItem(horizontalSpacer);

        countLabel = new QLabel(centralwidget);
        countLabel->setObjectName("countLabel");

        searchFilterLayout->addWidget(countLabel);


        verticalLayout_3->addLayout(searchFilterLayout);

        tableWidget = new QTableWidget(centralwidget);
        tableWidget->setObjectName("tableWidget");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(tableWidget->sizePolicy().hasHeightForWidth());
        tableWidget->setSizePolicy(sizePolicy);
        tableWidget->setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy::AdjustToContents);
        tableWidget->setAlternatingRowColors(true);
        tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableWidget->setSortingEnabled(true);
        tableWidget->setCornerButtonEnabled(false);
        tableWidget->setShowGrid(false);

        verticalLayout_3->addWidget(tableWidget);

        actionsLayout = new QHBoxLayout();
        actionsLayout->setSpacing(10);
        actionsLayout->setObjectName("actionsLayout");
        selectFolderButton = new QPushButton(centralwidget);
        selectFolderButton->setObjectName("selectFolderButton");
        selectFolderButton->setMinimumSize(QSize(100, 38));

        actionsLayout->addWidget(selectFolderButton);

        exportButton = new QPushButton(centralwidget);
        exportButton->setObjectName("exportButton");
        exportButton->setMinimumSize(QSize(80, 38));

        actionsLayout->addWidget(exportButton);

        refreshButton = new QPushButton(centralwidget);
        refreshButton->setObjectName("refreshButton");
        refreshButton->setMinimumSize(QSize(80, 38));

        actionsLayout->addWidget(refreshButton);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        actionsLayout->addItem(horizontalSpacer_2);


        verticalLayout_3->addLayout(actionsLayout);

        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        statusbar->setSizeGripEnabled(true);
        MainWindow->setStatusBar(statusbar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName("toolBar");
        toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        toolBar->setIconSize(QSize(20, 20));
        MainWindow->addToolBar(Qt::ToolBarArea::TopToolBarArea, toolBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MovieInfo - Video Library Manager", nullptr));
        searchLineEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "Search movies...", nullptr));
#if QT_CONFIG(tooltip)
        searchLineEdit->setToolTip(QCoreApplication::translate("MainWindow", "Search by title, genre, actor, or director", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        comboBoxQuality->setToolTip(QCoreApplication::translate("MainWindow", "Filter by quality", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        comboBoxDecade->setToolTip(QCoreApplication::translate("MainWindow", "Filter by decade", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        comboBoxAspectRatio->setToolTip(QCoreApplication::translate("MainWindow", "Filter by aspect ratio", nullptr));
#endif // QT_CONFIG(tooltip)
        clearFiltersButton->setText(QCoreApplication::translate("MainWindow", "Clear", nullptr));
#if QT_CONFIG(tooltip)
        clearFiltersButton->setToolTip(QCoreApplication::translate("MainWindow", "Reset all filters", nullptr));
#endif // QT_CONFIG(tooltip)
        countLabel->setText(QCoreApplication::translate("MainWindow", "0 movies", nullptr));
        selectFolderButton->setText(QCoreApplication::translate("MainWindow", "Add Folder", nullptr));
#if QT_CONFIG(tooltip)
        selectFolderButton->setToolTip(QCoreApplication::translate("MainWindow", "Browse and select a folder containing video files", nullptr));
#endif // QT_CONFIG(tooltip)
        exportButton->setText(QCoreApplication::translate("MainWindow", "Export", nullptr));
#if QT_CONFIG(tooltip)
        exportButton->setToolTip(QCoreApplication::translate("MainWindow", "Export library to Excel", nullptr));
#endif // QT_CONFIG(tooltip)
        refreshButton->setText(QCoreApplication::translate("MainWindow", "Refresh", nullptr));
#if QT_CONFIG(tooltip)
        refreshButton->setToolTip(QCoreApplication::translate("MainWindow", "Reload the movie library", nullptr));
#endif // QT_CONFIG(tooltip)
        toolBar->setWindowTitle(QCoreApplication::translate("MainWindow", "Toolbar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
