/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
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
#include <QtWidgets/QGroupBox>
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
    QGroupBox *tableGroupBox;
    QVBoxLayout *tableLayout;
    QTableWidget *tableWidget;
    QGroupBox *searchGroupBox;
    QVBoxLayout *searchLayout;
    QLineEdit *searchLineEdit;
    QHBoxLayout *filterLayout;
    QLabel *qualityLabel;
    QComboBox *comboBoxQuality;
    QLabel *decadeLabel;
    QComboBox *comboBoxDecade;
    QLabel *aspectLabel;
    QComboBox *comboBoxAspectRatio;
    QSpacerItem *horizontalSpacer;
    QGroupBox *actionsGroupBox;
    QHBoxLayout *actionsLayout;
    QPushButton *selectFolderButton;
    QPushButton *fetchButton;
    QPushButton *exportButton;
    QSpacerItem *horizontalSpacer_2;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1600, 1000);
        MainWindow->setStyleSheet(QString::fromUtf8("QMainWindow {\n"
"    background-color: #1e1e1e;\n"
"}\n"
"\n"
"QTableWidget {\n"
"    background-color: #2d2d2d;\n"
"    border: 1px solid #404040;\n"
"    border-radius: 8px;\n"
"    gridline-color: #404040;\n"
"    selection-background-color: #3c5a96;\n"
"    alternate-background-color: #252525;\n"
"    color: #e0e0e0;\n"
"}\n"
"\n"
"QTableWidget::item {\n"
"    padding: 8px;\n"
"    border-bottom: 1px solid #404040;\n"
"    color: #e0e0e0;\n"
"}\n"
"\n"
"QTableWidget::item:selected {\n"
"    background-color: #3c5a96;\n"
"    color: #ffffff;\n"
"}\n"
"\n"
"QHeaderView::section {\n"
"    background-color: #1a1a1a;\n"
"    color: #ffffff;\n"
"    padding: 12px 8px;\n"
"    border: none;\n"
"    font-weight: bold;\n"
"    font-size: 12px;\n"
"    border-bottom: 1px solid #404040;\n"
"}\n"
"\n"
"QHeaderView::section:hover {\n"
"    background-color: #2a2a2a;\n"
"}\n"
"\n"
"QPushButton {\n"
"    background-color: #3c5a96;\n"
"    color: white;\n"
"    border: none;\n"
"    padding: 10px 20px;\n"
"    border-rad"
                        "ius: 6px;\n"
"    font-weight: bold;\n"
"    font-size: 12px;\n"
"    min-height: 20px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #4a6ba8;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #2d4a7a;\n"
"}\n"
"\n"
"QPushButton:disabled {\n"
"    background-color: #404040;\n"
"    color: #808080;\n"
"}\n"
"\n"
"QLineEdit {\n"
"    padding: 10px;\n"
"    border: 2px solid #404040;\n"
"    border-radius: 6px;\n"
"    font-size: 14px;\n"
"    background-color: #2d2d2d;\n"
"    color: #e0e0e0;\n"
"}\n"
"\n"
"QLineEdit:focus {\n"
"    border-color: #3c5a96;\n"
"    outline: none;\n"
"}\n"
"\n"
"QLineEdit::placeholder {\n"
"    color: #808080;\n"
"}\n"
"\n"
"QComboBox {\n"
"    padding: 8px 12px;\n"
"    border: 2px solid #404040;\n"
"    border-radius: 6px;\n"
"    background-color: #2d2d2d;\n"
"    font-size: 12px;\n"
"    min-height: 20px;\n"
"    color: #e0e0e0;\n"
"}\n"
"\n"
"QComboBox:focus {\n"
"    border-color: #3c5a96;\n"
"}\n"
"\n"
"QComboBox::drop-down {\n"
"    border: none;"
                        "\n"
"    width: 20px;\n"
"}\n"
"\n"
"QComboBox::down-arrow {\n"
"    image: none;\n"
"    border-left: 5px solid transparent;\n"
"    border-right: 5px solid transparent;\n"
"    border-top: 5px solid #808080;\n"
"    margin-right: 5px;\n"
"}\n"
"\n"
"QComboBox QAbstractItemView {\n"
"    background-color: #2d2d2d;\n"
"    border: 2px solid #3c5a96;\n"
"    border-radius: 6px;\n"
"    color: #e0e0e0;\n"
"    selection-background-color: #3c5a96;\n"
"    selection-color: #ffffff;\n"
"}\n"
"\n"
"QGroupBox {\n"
"    font-weight: bold;\n"
"    border: 2px solid #404040;\n"
"    border-radius: 8px;\n"
"    margin-top: 10px;\n"
"    padding-top: 10px;\n"
"    background-color: #2d2d2d;\n"
"    color: #e0e0e0;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    left: 10px;\n"
"    padding: 0 8px 0 8px;\n"
"    color: #3c5a96;\n"
"}\n"
"\n"
"QLabel {\n"
"    color: #e0e0e0;\n"
"}\n"
"\n"
"QStatusBar {\n"
"    background-color: #1a1a1a;\n"
"    border-top: 1px solid #404040;\n"
"    color: #e0e0"
                        "e0;\n"
"}\n"
"\n"
"QToolBar {\n"
"    background-color: #1a1a1a;\n"
"    border-bottom: 1px solid #404040;\n"
"    spacing: 5px;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"QToolBar QToolButton {\n"
"    background-color: transparent;\n"
"    border: 1px solid transparent;\n"
"    border-radius: 4px;\n"
"    padding: 6px;\n"
"    margin: 2px;\n"
"    color: #e0e0e0;\n"
"}\n"
"\n"
"QToolBar QToolButton:hover {\n"
"    background-color: #3c5a96;\n"
"    border-color: #3c5a96;\n"
"}\n"
"\n"
"QScrollBar:vertical {\n"
"    background-color: #1a1a1a;\n"
"    width: 12px;\n"
"    border-radius: 6px;\n"
"}\n"
"\n"
"QScrollBar::handle:vertical {\n"
"    background-color: #404040;\n"
"    border-radius: 6px;\n"
"    min-height: 20px;\n"
"}\n"
"\n"
"QScrollBar::handle:vertical:hover {\n"
"    background-color: #505050;\n"
"}\n"
"\n"
"QScrollBar:horizontal {\n"
"    background-color: #1a1a1a;\n"
"    height: 12px;\n"
"    border-radius: 6px;\n"
"}\n"
"\n"
"QScrollBar::handle:horizontal {\n"
"    background-color: #404040;\n"
"  "
                        "  border-radius: 6px;\n"
"    min-width: 20px;\n"
"}\n"
"\n"
"QScrollBar::handle:horizontal:hover {\n"
"    background-color: #505050;\n"
"}"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout_3 = new QVBoxLayout(centralwidget);
        verticalLayout_3->setSpacing(10);
        verticalLayout_3->setObjectName("verticalLayout_3");
        verticalLayout_3->setContentsMargins(15, 15, 15, 15);
        tableGroupBox = new QGroupBox(centralwidget);
        tableGroupBox->setObjectName("tableGroupBox");
        tableLayout = new QVBoxLayout(tableGroupBox);
        tableLayout->setObjectName("tableLayout");
        tableWidget = new QTableWidget(tableGroupBox);
        tableWidget->setObjectName("tableWidget");
        tableWidget->setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy::AdjustToContents);
        tableWidget->setAlternatingRowColors(true);
        tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

        tableLayout->addWidget(tableWidget);


        verticalLayout_3->addWidget(tableGroupBox);

        searchGroupBox = new QGroupBox(centralwidget);
        searchGroupBox->setObjectName("searchGroupBox");
        searchLayout = new QVBoxLayout(searchGroupBox);
        searchLayout->setSpacing(10);
        searchLayout->setObjectName("searchLayout");
        searchLineEdit = new QLineEdit(searchGroupBox);
        searchLineEdit->setObjectName("searchLineEdit");

        searchLayout->addWidget(searchLineEdit);

        filterLayout = new QHBoxLayout();
        filterLayout->setSpacing(10);
        filterLayout->setObjectName("filterLayout");
        qualityLabel = new QLabel(searchGroupBox);
        qualityLabel->setObjectName("qualityLabel");
        qualityLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        filterLayout->addWidget(qualityLabel);

        comboBoxQuality = new QComboBox(searchGroupBox);
        comboBoxQuality->setObjectName("comboBoxQuality");
        comboBoxQuality->setMinimumSize(QSize(120, 0));

        filterLayout->addWidget(comboBoxQuality);

        decadeLabel = new QLabel(searchGroupBox);
        decadeLabel->setObjectName("decadeLabel");
        decadeLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        filterLayout->addWidget(decadeLabel);

        comboBoxDecade = new QComboBox(searchGroupBox);
        comboBoxDecade->setObjectName("comboBoxDecade");
        comboBoxDecade->setMinimumSize(QSize(120, 0));

        filterLayout->addWidget(comboBoxDecade);

        aspectLabel = new QLabel(searchGroupBox);
        aspectLabel->setObjectName("aspectLabel");
        aspectLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        filterLayout->addWidget(aspectLabel);

        comboBoxAspectRatio = new QComboBox(searchGroupBox);
        comboBoxAspectRatio->setObjectName("comboBoxAspectRatio");
        comboBoxAspectRatio->setMinimumSize(QSize(120, 0));

        filterLayout->addWidget(comboBoxAspectRatio);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        filterLayout->addItem(horizontalSpacer);


        searchLayout->addLayout(filterLayout);


        verticalLayout_3->addWidget(searchGroupBox);

        actionsGroupBox = new QGroupBox(centralwidget);
        actionsGroupBox->setObjectName("actionsGroupBox");
        actionsLayout = new QHBoxLayout(actionsGroupBox);
        actionsLayout->setSpacing(15);
        actionsLayout->setObjectName("actionsLayout");
        selectFolderButton = new QPushButton(actionsGroupBox);
        selectFolderButton->setObjectName("selectFolderButton");
        selectFolderButton->setMinimumSize(QSize(140, 0));

        actionsLayout->addWidget(selectFolderButton);

        fetchButton = new QPushButton(actionsGroupBox);
        fetchButton->setObjectName("fetchButton");
        fetchButton->setMinimumSize(QSize(160, 0));

        actionsLayout->addWidget(fetchButton);

        exportButton = new QPushButton(actionsGroupBox);
        exportButton->setObjectName("exportButton");
        exportButton->setMinimumSize(QSize(140, 0));

        actionsLayout->addWidget(exportButton);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        actionsLayout->addItem(horizontalSpacer_2);


        verticalLayout_3->addWidget(actionsGroupBox);

        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName("toolBar");
        toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        MainWindow->addToolBar(Qt::ToolBarArea::TopToolBarArea, toolBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MovieInfo - Video Library Manager", nullptr));
        tableGroupBox->setTitle(QCoreApplication::translate("MainWindow", "Video Library", nullptr));
        searchGroupBox->setTitle(QCoreApplication::translate("MainWindow", "Search & Filter", nullptr));
        searchLineEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "Search movies by title...", nullptr));
        qualityLabel->setText(QCoreApplication::translate("MainWindow", "Quality:", nullptr));
        decadeLabel->setText(QCoreApplication::translate("MainWindow", "Decade:", nullptr));
        aspectLabel->setText(QCoreApplication::translate("MainWindow", "Aspect Ratio:", nullptr));
        actionsGroupBox->setTitle(QCoreApplication::translate("MainWindow", "Actions", nullptr));
        selectFolderButton->setText(QCoreApplication::translate("MainWindow", "\360\237\223\201 Select Folder", nullptr));
        fetchButton->setText(QCoreApplication::translate("MainWindow", "\360\237\224\215 Fetch Movie Data", nullptr));
        exportButton->setText(QCoreApplication::translate("MainWindow", "\360\237\223\212 Export to Excel", nullptr));
        toolBar->setWindowTitle(QCoreApplication::translate("MainWindow", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
