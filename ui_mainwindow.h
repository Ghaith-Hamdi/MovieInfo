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
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout_3 = new QVBoxLayout(centralwidget);
        verticalLayout_3->setSpacing(20);
        verticalLayout_3->setObjectName("verticalLayout_3");
        verticalLayout_3->setContentsMargins(24, 24, 24, 24);
        searchGroupBox = new QGroupBox(centralwidget);
        searchGroupBox->setObjectName("searchGroupBox");
        searchLayout = new QVBoxLayout(searchGroupBox);
        searchLayout->setSpacing(16);
        searchLayout->setObjectName("searchLayout");
        searchLineEdit = new QLineEdit(searchGroupBox);
        searchLineEdit->setObjectName("searchLineEdit");

        searchLayout->addWidget(searchLineEdit);

        filterLayout = new QHBoxLayout();
        filterLayout->setSpacing(18);
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
        clearFiltersButton->setMinimumSize(QSize(140, 0));

        filterLayout->addWidget(clearFiltersButton);


        searchLayout->addLayout(filterLayout);


        verticalLayout_3->addWidget(searchGroupBox);

        tableGroupBox = new QGroupBox(centralwidget);
        tableGroupBox->setObjectName("tableGroupBox");
        tableLayout = new QVBoxLayout(tableGroupBox);
        tableLayout->setSpacing(14);
        tableLayout->setObjectName("tableLayout");
        tableInfoLayout = new QHBoxLayout();
        tableInfoLayout->setObjectName("tableInfoLayout");
        countLabel = new QLabel(tableGroupBox);
        countLabel->setObjectName("countLabel");
        countLabel->setStyleSheet(QString::fromUtf8("QLabel { color: #6e7681; font-size: 13px; font-style: italic; }"));

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
        actionsLayout->setSpacing(18);
        actionsLayout->setObjectName("actionsLayout");
        selectFolderButton = new QPushButton(actionsGroupBox);
        selectFolderButton->setObjectName("selectFolderButton");
        selectFolderButton->setMinimumSize(QSize(160, 46));

        actionsLayout->addWidget(selectFolderButton);

        fetchButton = new QPushButton(actionsGroupBox);
        fetchButton->setObjectName("fetchButton");
        fetchButton->setMinimumSize(QSize(180, 46));

        actionsLayout->addWidget(fetchButton);

        exportButton = new QPushButton(actionsGroupBox);
        exportButton->setObjectName("exportButton");
        exportButton->setMinimumSize(QSize(160, 46));

        actionsLayout->addWidget(exportButton);

        refreshButton = new QPushButton(actionsGroupBox);
        refreshButton->setObjectName("refreshButton");
        refreshButton->setMinimumSize(QSize(130, 46));

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
        toolBar->setIconSize(QSize(22, 22));
        MainWindow->addToolBar(Qt::ToolBarArea::TopToolBarArea, toolBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "\360\237\216\254 MovieInfo - Video Library Manager", nullptr));
        searchGroupBox->setTitle(QCoreApplication::translate("MainWindow", "\360\237\224\215  SEARCH & FILTER", nullptr));
        searchLineEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "\360\237\224\216  Search by title, genre, or actor...", nullptr));
#if QT_CONFIG(tooltip)
        searchLineEdit->setToolTip(QCoreApplication::translate("MainWindow", "Type to filter your movie collection instantly", nullptr));
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
        clearFiltersButton->setText(QCoreApplication::translate("MainWindow", "\342\234\225  Clear Filters", nullptr));
#if QT_CONFIG(tooltip)
        clearFiltersButton->setToolTip(QCoreApplication::translate("MainWindow", "Reset all filters to show entire library", nullptr));
#endif // QT_CONFIG(tooltip)
        tableGroupBox->setTitle(QCoreApplication::translate("MainWindow", "\360\237\223\232  VIDEO LIBRARY", nullptr));
        countLabel->setText(QCoreApplication::translate("MainWindow", "\360\237\223\212  0 movies", nullptr));
        actionsGroupBox->setTitle(QCoreApplication::translate("MainWindow", "\342\232\241  ACTIONS", nullptr));
        selectFolderButton->setText(QCoreApplication::translate("MainWindow", "\360\237\223\201  Select Folder", nullptr));
#if QT_CONFIG(tooltip)
        selectFolderButton->setToolTip(QCoreApplication::translate("MainWindow", "Browse and select a folder containing video files", nullptr));
#endif // QT_CONFIG(tooltip)
        fetchButton->setText(QCoreApplication::translate("MainWindow", "\360\237\216\254  Fetch Movie Data", nullptr));
#if QT_CONFIG(tooltip)
        fetchButton->setToolTip(QCoreApplication::translate("MainWindow", "Download comprehensive movie information from online databases", nullptr));
#endif // QT_CONFIG(tooltip)
        exportButton->setText(QCoreApplication::translate("MainWindow", "\360\237\223\212  Export to Excel", nullptr));
#if QT_CONFIG(tooltip)
        exportButton->setToolTip(QCoreApplication::translate("MainWindow", "Export your library data to an Excel/CSV spreadsheet", nullptr));
#endif // QT_CONFIG(tooltip)
        refreshButton->setText(QCoreApplication::translate("MainWindow", "\360\237\224\204  Refresh", nullptr));
#if QT_CONFIG(tooltip)
        refreshButton->setToolTip(QCoreApplication::translate("MainWindow", "Reload and refresh the movie library", nullptr));
#endif // QT_CONFIG(tooltip)
        toolBar->setWindowTitle(QCoreApplication::translate("MainWindow", "\360\237\233\240\357\270\217  Main Toolbar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
