/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
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
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
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
    QTableWidget *tableWidget;
    QVBoxLayout *verticalLayout_2;
    QLineEdit *searchLineEdit;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QComboBox *comboBoxQuality;
    QComboBox *comboBoxDecade;
    QComboBox *comboBoxAspectRatio;
    QPushButton *exportButton;
    QPushButton *fetchButton;
    QPushButton *selectFolderButton;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1496, 959);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout_3 = new QVBoxLayout(centralwidget);
        verticalLayout_3->setObjectName("verticalLayout_3");
        tableWidget = new QTableWidget(centralwidget);
        tableWidget->setObjectName("tableWidget");
        tableWidget->setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy::AdjustToContents);

        verticalLayout_3->addWidget(tableWidget);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        searchLineEdit = new QLineEdit(centralwidget);
        searchLineEdit->setObjectName("searchLineEdit");

        verticalLayout_2->addWidget(searchLineEdit);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        comboBoxQuality = new QComboBox(centralwidget);
        comboBoxQuality->setObjectName("comboBoxQuality");

        horizontalLayout->addWidget(comboBoxQuality);

        comboBoxDecade = new QComboBox(centralwidget);
        comboBoxDecade->setObjectName("comboBoxDecade");

        horizontalLayout->addWidget(comboBoxDecade);

        comboBoxAspectRatio = new QComboBox(centralwidget);
        comboBoxAspectRatio->setObjectName("comboBoxAspectRatio");

        horizontalLayout->addWidget(comboBoxAspectRatio);


        verticalLayout->addLayout(horizontalLayout);

        exportButton = new QPushButton(centralwidget);
        exportButton->setObjectName("exportButton");

        verticalLayout->addWidget(exportButton);


        verticalLayout_2->addLayout(verticalLayout);


        verticalLayout_3->addLayout(verticalLayout_2);

        fetchButton = new QPushButton(centralwidget);
        fetchButton->setObjectName("fetchButton");

        verticalLayout_3->addWidget(fetchButton);

        selectFolderButton = new QPushButton(centralwidget);
        selectFolderButton->setObjectName("selectFolderButton");

        verticalLayout_3->addWidget(selectFolderButton);

        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName("toolBar");
        MainWindow->addToolBar(Qt::ToolBarArea::TopToolBarArea, toolBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        exportButton->setText(QCoreApplication::translate("MainWindow", "Export", nullptr));
        fetchButton->setText(QCoreApplication::translate("MainWindow", "Fetch", nullptr));
        selectFolderButton->setText(QCoreApplication::translate("MainWindow", "Select Folder", nullptr));
        toolBar->setWindowTitle(QCoreApplication::translate("MainWindow", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
