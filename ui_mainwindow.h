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
    QPushButton *clearFiltersButton;
    QGroupBox *tableGroupBox;
    QVBoxLayout *tableLayout;
    QHBoxLayout *tableInfoLayout;
    QLabel *countLabel;
    QSpacerItem *horizontalSpacer_3;
    QTableWidget *tableWidget;
    QGroupBox *actionsGroupBox;
    QHBoxLayout *actionsLayout;
    QPushButton *selectFolderButton;
    QPushButton *fetchButton;
    QPushButton *exportButton;
    QPushButton *refreshButton;
    QSpacerItem *horizontalSpacer_2;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1600, 1000);
        MainWindow->setStyleSheet(QString::fromUtf8("QMainWindow {\n"
"    background-color: #0f0f0f;\n"
"    font-family: \"Inter\", \"Segoe UI\", \"Roboto\", sans-serif;\n"
"    font-size: 14px;\n"
"    color: #e5e5e5;\n"
"}\n"
"\n"
"/* --- Table Styling --- */\n"
"QTableWidget {\n"
"    background-color: #1a1a1a;\n"
"    border: 1px solid #2a2a2a;\n"
"    border-radius: 12px;\n"
"    gridline-color: transparent;\n"
"    selection-background-color: #4a7cf5;\n"
"    alternate-background-color: #161616;\n"
"    color: #e5e5e5;\n"
"    padding: 4px;\n"
"}\n"
"\n"
"QTableWidget::item {\n"
"    padding: 14px 12px;\n"
"    border-bottom: 1px solid #252525;\n"
"}\n"
"\n"
"QTableWidget::item:selected {\n"
"    background-color: #4a7cf5;\n"
"    color: #ffffff;\n"
"}\n"
"\n"
"QTableWidget::item:hover {\n"
"    background-color: #222222;\n"
"}\n"
"\n"
"/* --- Headers --- */\n"
"QHeaderView::section {\n"
"    background-color: #141414;\n"
"    color: #ffffff;\n"
"    padding: 14px 12px;\n"
"    font-weight: 600;\n"
"    font-size: 13px;\n"
"    border: none;\n"
"    bord"
                        "er-bottom: 2px solid #4a7cf5;\n"
"    letter-spacing: 0.3px;\n"
"}\n"
"\n"
"QHeaderView::section:hover {\n"
"    background-color: #1c1c1c;\n"
"}\n"
"\n"
"/* --- Buttons --- */\n"
"QPushButton {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"                                stop:0 #4a7cf5, stop:1 #3d6ad9);\n"
"    color: white;\n"
"    border: none;\n"
"    padding: 12px 24px;\n"
"    border-radius: 10px;\n"
"    font-weight: 600;\n"
"    font-size: 13px;\n"
"    letter-spacing: 0.3px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"                                stop:0 #5a8cff, stop:1 #4a7cf5);\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"                                stop:0 #3d6ad9, stop:1 #2d5ac9);\n"
"    padding: 13px 23px 11px 25px;\n"
"}\n"
"\n"
"QPushButton:disabled {\n"
"    background: #2a2a2a;\n"
"    color: #666666;\n"
"}\n"
"\n"
"/* --- LineEdit (Search Bar) --- */\n"
"QLin"
                        "eEdit {\n"
"    padding: 12px 16px;\n"
"    border: 2px solid #2a2a2a;\n"
"    border-radius: 10px;\n"
"    font-size: 14px;\n"
"    background-color: #1a1a1a;\n"
"    color: #e5e5e5;\n"
"    selection-background-color: #4a7cf5;\n"
"}\n"
"\n"
"QLineEdit:focus {\n"
"    border-color: #4a7cf5;\n"
"    background-color: #1f1f1f;\n"
"}\n"
"\n"
"QLineEdit::placeholder {\n"
"    color: #666666;\n"
"    font-style: normal;\n"
"}\n"
"\n"
"/* --- ComboBox --- */\n"
"QComboBox {\n"
"    padding: 10px 14px;\n"
"    border: 2px solid #2a2a2a;\n"
"    border-radius: 10px;\n"
"    background-color: #1a1a1a;\n"
"    font-size: 13px;\n"
"    color: #e5e5e5;\n"
"    min-height: 20px;\n"
"}\n"
"\n"
"QComboBox:hover {\n"
"    border-color: #3a3a3a;\n"
"    background-color: #1f1f1f;\n"
"}\n"
"\n"
"QComboBox:focus {\n"
"    border-color: #4a7cf5;\n"
"}\n"
"\n"
"QComboBox::drop-down {\n"
"    border: none;\n"
"    width: 30px;\n"
"    padding-right: 8px;\n"
"}\n"
"\n"
"QComboBox::down-arrow {\n"
"    image: none;\n"
"    border-le"
                        "ft: 5px solid transparent;\n"
"    border-right: 5px solid transparent;\n"
"    border-top: 6px solid #888888;\n"
"    margin-right: 6px;\n"
"}\n"
"\n"
"QComboBox:hover::down-arrow {\n"
"    border-top-color: #aaaaaa;\n"
"}\n"
"\n"
"QComboBox QAbstractItemView {\n"
"    background-color: #1a1a1a;\n"
"    border: 2px solid #4a7cf5;\n"
"    border-radius: 8px;\n"
"    color: #e5e5e5;\n"
"    selection-background-color: #4a7cf5;\n"
"    outline: none;\n"
"    padding: 4px;\n"
"}\n"
"\n"
"QComboBox QAbstractItemView::item {\n"
"    padding: 8px 12px;\n"
"    border-radius: 6px;\n"
"    margin: 2px;\n"
"}\n"
"\n"
"QComboBox QAbstractItemView::item:hover {\n"
"    background-color: #2a2a2a;\n"
"}\n"
"\n"
"/* --- GroupBox --- */\n"
"QGroupBox {\n"
"    border: 1px solid #2a2a2a;\n"
"    margin-top: 24px;\n"
"    padding: 20px 16px 16px 16px;\n"
"    background-color: #141414;\n"
"    border-radius: 14px;\n"
"    font-size: 14px;\n"
"    color: #e5e5e5;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: marg"
                        "in;\n"
"    left: 16px;\n"
"    top: 8px;\n"
"    padding: 0 8px;\n"
"    font-weight: 700;\n"
"    font-size: 15px;\n"
"    color: #4a7cf5;\n"
"    letter-spacing: 0.5px;\n"
"    background-color: #141414;\n"
"}\n"
"\n"
"/* --- Labels --- */\n"
"QLabel {\n"
"    color: #c0c0c0;\n"
"    font-size: 13px;\n"
"    font-weight: 500;\n"
"}\n"
"\n"
"/* --- Status Bar --- */\n"
"QStatusBar {\n"
"    background-color: #0a0a0a;\n"
"    border-top: 1px solid #2a2a2a;\n"
"    color: #888888;\n"
"    font-size: 12px;\n"
"    padding: 6px 12px;\n"
"}\n"
"\n"
"QStatusBar::item {\n"
"    border: none;\n"
"}\n"
"\n"
"/* --- Toolbar --- */\n"
"QToolBar {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"                                stop:0 #141414, stop:1 #0f0f0f);\n"
"    border-bottom: 1px solid #2a2a2a;\n"
"    spacing: 8px;\n"
"    padding: 8px 16px;\n"
"}\n"
"\n"
"QToolBar QToolButton {\n"
"    background-color: transparent;\n"
"    border: 1px solid transparent;\n"
"    border-radius: 8px;\n"
"    padding"
                        ": 8px 14px;\n"
"    margin: 2px;\n"
"    color: #e5e5e5;\n"
"    font-size: 13px;\n"
"    font-weight: 500;\n"
"}\n"
"\n"
"QToolBar QToolButton:hover {\n"
"    background-color: #4a7cf5;\n"
"    border-color: #4a7cf5;\n"
"}\n"
"\n"
"QToolBar QToolButton:pressed {\n"
"    background-color: #3d6ad9;\n"
"}\n"
"\n"
"/* --- Scrollbars --- */\n"
"QScrollBar:vertical {\n"
"    background-color: #0f0f0f;\n"
"    width: 14px;\n"
"    border-radius: 7px;\n"
"    margin: 2px;\n"
"}\n"
"\n"
"QScrollBar::handle:vertical {\n"
"    background-color: #3a3a3a;\n"
"    border-radius: 7px;\n"
"    min-height: 30px;\n"
"}\n"
"\n"
"QScrollBar::handle:vertical:hover {\n"
"    background-color: #4a4a4a;\n"
"}\n"
"\n"
"QScrollBar::handle:vertical:pressed {\n"
"    background-color: #4a7cf5;\n"
"}\n"
"\n"
"QScrollBar:horizontal {\n"
"    background-color: #0f0f0f;\n"
"    height: 14px;\n"
"    border-radius: 7px;\n"
"    margin: 2px;\n"
"}\n"
"\n"
"QScrollBar::handle:horizontal {\n"
"    background-color: #3a3a3a;\n"
"    border-radiu"
                        "s: 7px;\n"
"    min-width: 30px;\n"
"}\n"
"\n"
"QScrollBar::handle:horizontal:hover {\n"
"    background-color: #4a4a4a;\n"
"}\n"
"\n"
"QScrollBar::handle:horizontal:pressed {\n"
"    background-color: #4a7cf5;\n"
"}\n"
"\n"
"QScrollBar::add-line, QScrollBar::sub-line {\n"
"    height: 0px;\n"
"    width: 0px;\n"
"}\n"
"\n"
"/* --- Tooltips --- */\n"
"QToolTip {\n"
"    background-color: #1a1a1a;\n"
"    color: #e5e5e5;\n"
"    border: 1px solid #4a7cf5;\n"
"    border-radius: 6px;\n"
"    padding: 6px 10px;\n"
"    font-size: 12px;\n"
"}\n"
""));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout_3 = new QVBoxLayout(centralwidget);
        verticalLayout_3->setSpacing(16);
        verticalLayout_3->setObjectName("verticalLayout_3");
        verticalLayout_3->setContentsMargins(20, 20, 20, 20);
        searchGroupBox = new QGroupBox(centralwidget);
        searchGroupBox->setObjectName("searchGroupBox");
        searchLayout = new QVBoxLayout(searchGroupBox);
        searchLayout->setSpacing(14);
        searchLayout->setObjectName("searchLayout");
        searchLineEdit = new QLineEdit(searchGroupBox);
        searchLineEdit->setObjectName("searchLineEdit");

        searchLayout->addWidget(searchLineEdit);

        filterLayout = new QHBoxLayout();
        filterLayout->setSpacing(16);
        filterLayout->setObjectName("filterLayout");
        qualityLabel = new QLabel(searchGroupBox);
        qualityLabel->setObjectName("qualityLabel");
        qualityLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        qualityLabel->setMinimumSize(QSize(90, 0));

        filterLayout->addWidget(qualityLabel);

        comboBoxQuality = new QComboBox(searchGroupBox);
        comboBoxQuality->setObjectName("comboBoxQuality");
        comboBoxQuality->setMinimumSize(QSize(140, 0));

        filterLayout->addWidget(comboBoxQuality);

        decadeLabel = new QLabel(searchGroupBox);
        decadeLabel->setObjectName("decadeLabel");
        decadeLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        decadeLabel->setMinimumSize(QSize(90, 0));

        filterLayout->addWidget(decadeLabel);

        comboBoxDecade = new QComboBox(searchGroupBox);
        comboBoxDecade->setObjectName("comboBoxDecade");
        comboBoxDecade->setMinimumSize(QSize(140, 0));

        filterLayout->addWidget(comboBoxDecade);

        aspectLabel = new QLabel(searchGroupBox);
        aspectLabel->setObjectName("aspectLabel");
        aspectLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        aspectLabel->setMinimumSize(QSize(90, 0));

        filterLayout->addWidget(aspectLabel);

        comboBoxAspectRatio = new QComboBox(searchGroupBox);
        comboBoxAspectRatio->setObjectName("comboBoxAspectRatio");
        comboBoxAspectRatio->setMinimumSize(QSize(140, 0));

        filterLayout->addWidget(comboBoxAspectRatio);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        filterLayout->addItem(horizontalSpacer);

        clearFiltersButton = new QPushButton(searchGroupBox);
        clearFiltersButton->setObjectName("clearFiltersButton");
        clearFiltersButton->setMinimumSize(QSize(130, 0));

        filterLayout->addWidget(clearFiltersButton);


        searchLayout->addLayout(filterLayout);


        verticalLayout_3->addWidget(searchGroupBox);

        tableGroupBox = new QGroupBox(centralwidget);
        tableGroupBox->setObjectName("tableGroupBox");
        tableLayout = new QVBoxLayout(tableGroupBox);
        tableLayout->setSpacing(12);
        tableLayout->setObjectName("tableLayout");
        tableInfoLayout = new QHBoxLayout();
        tableInfoLayout->setObjectName("tableInfoLayout");
        countLabel = new QLabel(tableGroupBox);
        countLabel->setObjectName("countLabel");
        countLabel->setStyleSheet(QString::fromUtf8("QLabel { color: #888888; font-size: 12px; }"));

        tableInfoLayout->addWidget(countLabel);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        tableInfoLayout->addItem(horizontalSpacer_3);


        tableLayout->addLayout(tableInfoLayout);

        tableWidget = new QTableWidget(tableGroupBox);
        tableWidget->setObjectName("tableWidget");
        tableWidget->setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy::AdjustToContents);
        tableWidget->setAlternatingRowColors(true);
        tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableWidget->setSortingEnabled(true);
        tableWidget->setCornerButtonEnabled(false);
        tableWidget->setShowGrid(false);

        tableLayout->addWidget(tableWidget);


        verticalLayout_3->addWidget(tableGroupBox);

        actionsGroupBox = new QGroupBox(centralwidget);
        actionsGroupBox->setObjectName("actionsGroupBox");
        actionsLayout = new QHBoxLayout(actionsGroupBox);
        actionsLayout->setSpacing(16);
        actionsLayout->setObjectName("actionsLayout");
        selectFolderButton = new QPushButton(actionsGroupBox);
        selectFolderButton->setObjectName("selectFolderButton");
        selectFolderButton->setMinimumSize(QSize(150, 42));

        actionsLayout->addWidget(selectFolderButton);

        fetchButton = new QPushButton(actionsGroupBox);
        fetchButton->setObjectName("fetchButton");
        fetchButton->setMinimumSize(QSize(170, 42));

        actionsLayout->addWidget(fetchButton);

        exportButton = new QPushButton(actionsGroupBox);
        exportButton->setObjectName("exportButton");
        exportButton->setMinimumSize(QSize(150, 42));

        actionsLayout->addWidget(exportButton);

        refreshButton = new QPushButton(actionsGroupBox);
        refreshButton->setObjectName("refreshButton");
        refreshButton->setMinimumSize(QSize(120, 42));

        actionsLayout->addWidget(refreshButton);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        actionsLayout->addItem(horizontalSpacer_2);


        verticalLayout_3->addWidget(actionsGroupBox);

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
        searchGroupBox->setTitle(QCoreApplication::translate("MainWindow", "\360\237\224\215 Search & Filter", nullptr));
        searchLineEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "\360\237\224\216 Search by title, genre, or actor...", nullptr));
#if QT_CONFIG(tooltip)
        searchLineEdit->setToolTip(QCoreApplication::translate("MainWindow", "Type to filter your movie collection", nullptr));
#endif // QT_CONFIG(tooltip)
        qualityLabel->setText(QCoreApplication::translate("MainWindow", "Quality:", nullptr));
#if QT_CONFIG(tooltip)
        comboBoxQuality->setToolTip(QCoreApplication::translate("MainWindow", "Filter by video quality", nullptr));
#endif // QT_CONFIG(tooltip)
        decadeLabel->setText(QCoreApplication::translate("MainWindow", "Decade:", nullptr));
#if QT_CONFIG(tooltip)
        comboBoxDecade->setToolTip(QCoreApplication::translate("MainWindow", "Filter by release decade", nullptr));
#endif // QT_CONFIG(tooltip)
        aspectLabel->setText(QCoreApplication::translate("MainWindow", "Aspect Ratio:", nullptr));
#if QT_CONFIG(tooltip)
        comboBoxAspectRatio->setToolTip(QCoreApplication::translate("MainWindow", "Filter by aspect ratio", nullptr));
#endif // QT_CONFIG(tooltip)
        clearFiltersButton->setText(QCoreApplication::translate("MainWindow", "\342\234\225 Clear Filters", nullptr));
#if QT_CONFIG(tooltip)
        clearFiltersButton->setToolTip(QCoreApplication::translate("MainWindow", "Reset all filters", nullptr));
#endif // QT_CONFIG(tooltip)
        tableGroupBox->setTitle(QCoreApplication::translate("MainWindow", "\360\237\223\232 Video Library", nullptr));
        countLabel->setText(QCoreApplication::translate("MainWindow", "0 movies", nullptr));
        actionsGroupBox->setTitle(QCoreApplication::translate("MainWindow", "\342\232\241 Actions", nullptr));
        selectFolderButton->setText(QCoreApplication::translate("MainWindow", "\360\237\223\201 Select Folder", nullptr));
#if QT_CONFIG(tooltip)
        selectFolderButton->setToolTip(QCoreApplication::translate("MainWindow", "Choose a folder containing video files", nullptr));
#endif // QT_CONFIG(tooltip)
        fetchButton->setText(QCoreApplication::translate("MainWindow", "\360\237\216\254 Fetch Movie Data", nullptr));
#if QT_CONFIG(tooltip)
        fetchButton->setToolTip(QCoreApplication::translate("MainWindow", "Download movie information from online databases", nullptr));
#endif // QT_CONFIG(tooltip)
        exportButton->setText(QCoreApplication::translate("MainWindow", "\360\237\223\212 Export to Excel", nullptr));
#if QT_CONFIG(tooltip)
        exportButton->setToolTip(QCoreApplication::translate("MainWindow", "Export library data to Excel spreadsheet", nullptr));
#endif // QT_CONFIG(tooltip)
        refreshButton->setText(QCoreApplication::translate("MainWindow", "\360\237\224\204 Refresh", nullptr));
#if QT_CONFIG(tooltip)
        refreshButton->setToolTip(QCoreApplication::translate("MainWindow", "Reload the movie library", nullptr));
#endif // QT_CONFIG(tooltip)
        toolBar->setWindowTitle(QCoreApplication::translate("MainWindow", "Main Toolbar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
