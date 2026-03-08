/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <ui/FilterPanel.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *toolbarLayout;
    QComboBox *driveCombo;
    QLineEdit *yearEdit;
        QPushButton *openLastBtn;
    QPushButton *loadBtn;
    QFrame *line1;
    QPushButton *toMoveBtn;
    QFrame *line2;
    QPushButton *clearBtn;
    QSpacerItem *toolbarSpacer;
    UI::FilterPanel *filterPanel;
    QTableView *tableView;
    QHBoxLayout *bottomLayout;
    QPushButton *addFolderBtn;
    QPushButton *exportBtn;
    QPushButton *organizeAllBtn;
    QPushButton *settingsBtn;
    QSpacerItem *bottomSpacer;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1400, 900);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setSpacing(4);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(8, 8, 8, 8);
        toolbarLayout = new QHBoxLayout();
        toolbarLayout->setSpacing(4);
        toolbarLayout->setObjectName("toolbarLayout");
        driveCombo = new QComboBox(centralwidget);
        driveCombo->setObjectName("driveCombo");
        driveCombo->setMinimumWidth(70);

        toolbarLayout->addWidget(driveCombo);

        yearEdit = new QLineEdit(centralwidget);
        yearEdit->setObjectName("yearEdit");
        yearEdit->setMaxLength(4);
        yearEdit->setMinimumWidth(70);
        yearEdit->setMaximumWidth(80);

        toolbarLayout->addWidget(yearEdit);

        openLastBtn = new QPushButton(centralwidget);
        openLastBtn->setObjectName("openLastBtn");
        openLastBtn->setMinimumWidth(80);

        toolbarLayout->addWidget(openLastBtn);

        loadBtn = new QPushButton(centralwidget);
        loadBtn->setObjectName("loadBtn");
        loadBtn->setMinimumWidth(70);

        toolbarLayout->addWidget(loadBtn);

        line1 = new QFrame(centralwidget);
        line1->setObjectName("line1");
        line1->setFrameShape(QFrame::VLine);
        line1->setFrameShadow(QFrame::Sunken);

        toolbarLayout->addWidget(line1);

        toMoveBtn = new QPushButton(centralwidget);
        toMoveBtn->setObjectName("toMoveBtn");
        toMoveBtn->setMinimumWidth(80);

        toolbarLayout->addWidget(toMoveBtn);

        line2 = new QFrame(centralwidget);
        line2->setObjectName("line2");
        line2->setFrameShape(QFrame::VLine);
        line2->setFrameShadow(QFrame::Sunken);

        toolbarLayout->addWidget(line2);

        clearBtn = new QPushButton(centralwidget);
        clearBtn->setObjectName("clearBtn");
        clearBtn->setMinimumWidth(60);

        toolbarLayout->addWidget(clearBtn);

        toolbarSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        toolbarLayout->addItem(toolbarSpacer);


        verticalLayout->addLayout(toolbarLayout);

        filterPanel = new UI::FilterPanel(centralwidget);
        filterPanel->setObjectName("filterPanel");

        verticalLayout->addWidget(filterPanel);

        tableView = new QTableView(centralwidget);
        tableView->setObjectName("tableView");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(tableView->sizePolicy().hasHeightForWidth());
        tableView->setSizePolicy(sizePolicy);
        tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableView->setSelectionMode(QAbstractItemView::SingleSelection);
        tableView->setAlternatingRowColors(true);
        tableView->setShowGrid(true);
        tableView->setSortingEnabled(true);
        tableView->setContextMenuPolicy(Qt::CustomContextMenu);

        verticalLayout->addWidget(tableView);

        bottomLayout = new QHBoxLayout();
        bottomLayout->setSpacing(4);
        bottomLayout->setObjectName("bottomLayout");
        addFolderBtn = new QPushButton(centralwidget);
        addFolderBtn->setObjectName("addFolderBtn");

        bottomLayout->addWidget(addFolderBtn);

        exportBtn = new QPushButton(centralwidget);
        exportBtn->setObjectName("exportBtn");

        bottomLayout->addWidget(exportBtn);

        organizeAllBtn = new QPushButton(centralwidget);
        organizeAllBtn->setObjectName("organizeAllBtn");

        bottomLayout->addWidget(organizeAllBtn);

        settingsBtn = new QPushButton(centralwidget);
        settingsBtn->setObjectName("settingsBtn");

        bottomLayout->addWidget(settingsBtn);

        bottomSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        bottomLayout->addItem(bottomSpacer);


        verticalLayout->addLayout(bottomLayout);

        MainWindow->setCentralWidget(centralwidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName("statusBar");
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MovieInfo - Video Library Manager", nullptr));
#if QT_CONFIG(tooltip)
        driveCombo->setToolTip(QCoreApplication::translate("MainWindow", "Select drive", nullptr));
#endif // QT_CONFIG(tooltip)
        yearEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "Year", nullptr));
#if QT_CONFIG(tooltip)
        yearEdit->setToolTip(QCoreApplication::translate("MainWindow", "Enter year (e.g., 2023)", nullptr));
#endif // QT_CONFIG(tooltip)
        loadBtn->setText(QCoreApplication::translate("MainWindow", "Load", nullptr));
#if QT_CONFIG(tooltip)
        loadBtn->setToolTip(QCoreApplication::translate("MainWindow", "Load movies from selected drive and year", nullptr));
#endif // QT_CONFIG(tooltip)
        toMoveBtn->setText(QCoreApplication::translate("MainWindow", "To Move", nullptr));
#if QT_CONFIG(tooltip)
        toMoveBtn->setToolTip(QCoreApplication::translate("MainWindow", "Show movies that need to be relocated", nullptr));
#endif // QT_CONFIG(tooltip)
        clearBtn->setText(QCoreApplication::translate("MainWindow", "Clear", nullptr));
#if QT_CONFIG(tooltip)
        openLastBtn->setToolTip(QCoreApplication::translate("MainWindow", "Open last used folder", nullptr));
#endif // QT_CONFIG(tooltip)
        openLastBtn->setText(QCoreApplication::translate("MainWindow", "Open Last", nullptr));
#if QT_CONFIG(tooltip)
        clearBtn->setToolTip(QCoreApplication::translate("MainWindow", "Clear all movies from table", nullptr));
#endif // QT_CONFIG(tooltip)
        addFolderBtn->setText(QCoreApplication::translate("MainWindow", "Add Folder", nullptr));
#if QT_CONFIG(tooltip)
        addFolderBtn->setToolTip(QCoreApplication::translate("MainWindow", "Select a folder containing video files", nullptr));
#endif // QT_CONFIG(tooltip)
        exportBtn->setText(QCoreApplication::translate("MainWindow", "Export", nullptr));
#if QT_CONFIG(tooltip)
        exportBtn->setToolTip(QCoreApplication::translate("MainWindow", "Export the current library to CSV or JSON", nullptr));
#endif // QT_CONFIG(tooltip)
        organizeAllBtn->setText(QCoreApplication::translate("MainWindow", "Organize All", nullptr));
#if QT_CONFIG(tooltip)
        organizeAllBtn->setToolTip(QCoreApplication::translate("MainWindow", "Organize all movies by aspect ratio into 16-9 or UW folders", nullptr));
#endif // QT_CONFIG(tooltip)
        settingsBtn->setText(QCoreApplication::translate("MainWindow", "Settings", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
