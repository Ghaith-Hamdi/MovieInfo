/********************************************************************************
** Form generated from reading UI file 'MovesToMoveDialog.ui'
**
** Created by: Qt User Interface Compiler version 6.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MOVESTOMOVEDIALOG_H
#define UI_MOVESTOMOVEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_MovesToMoveDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *countLabel;
    QTableWidget *moviesTable;
    QHBoxLayout *buttonLayout;
    QSpacerItem *buttonSpacer;
    QPushButton *moveAllBtn;
    QPushButton *closeBtn;

    void setupUi(QDialog *MovesToMoveDialog)
    {
        if (MovesToMoveDialog->objectName().isEmpty())
            MovesToMoveDialog->setObjectName("MovesToMoveDialog");
        MovesToMoveDialog->resize(960, 520);
        verticalLayout = new QVBoxLayout(MovesToMoveDialog);
        verticalLayout->setObjectName("verticalLayout");
        countLabel = new QLabel(MovesToMoveDialog);
        countLabel->setObjectName("countLabel");

        verticalLayout->addWidget(countLabel);

        moviesTable = new QTableWidget(MovesToMoveDialog);
        if (moviesTable->columnCount() < 7)
            moviesTable->setColumnCount(7);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        moviesTable->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        moviesTable->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        moviesTable->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        moviesTable->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        moviesTable->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        moviesTable->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        moviesTable->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        moviesTable->setObjectName("moviesTable");
        moviesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
        moviesTable->setAlternatingRowColors(true);
        moviesTable->setColumnCount(7);
        moviesTable->horizontalHeader()->setStretchLastSection(true);
        moviesTable->verticalHeader()->setVisible(false);

        verticalLayout->addWidget(moviesTable);

        buttonLayout = new QHBoxLayout();
        buttonLayout->setObjectName("buttonLayout");
        buttonSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        buttonLayout->addItem(buttonSpacer);

        moveAllBtn = new QPushButton(MovesToMoveDialog);
        moveAllBtn->setObjectName("moveAllBtn");

        buttonLayout->addWidget(moveAllBtn);

        closeBtn = new QPushButton(MovesToMoveDialog);
        closeBtn->setObjectName("closeBtn");

        buttonLayout->addWidget(closeBtn);


        verticalLayout->addLayout(buttonLayout);


        retranslateUi(MovesToMoveDialog);

        QMetaObject::connectSlotsByName(MovesToMoveDialog);
    } // setupUi

    void retranslateUi(QDialog *MovesToMoveDialog)
    {
        MovesToMoveDialog->setWindowTitle(QCoreApplication::translate("MovesToMoveDialog", "Movies to Move", nullptr));
        countLabel->setText(QCoreApplication::translate("MovesToMoveDialog", "0 movies", nullptr));
        QTableWidgetItem *___qtablewidgetitem = moviesTable->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MovesToMoveDialog", "Title", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = moviesTable->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MovesToMoveDialog", "Year", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = moviesTable->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MovesToMoveDialog", "Rating", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = moviesTable->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MovesToMoveDialog", "Votes", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = moviesTable->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MovesToMoveDialog", "Current Path", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = moviesTable->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("MovesToMoveDialog", "Target", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = moviesTable->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("MovesToMoveDialog", "Action", nullptr));
        moveAllBtn->setText(QCoreApplication::translate("MovesToMoveDialog", "Move All (TeraCopy)", nullptr));
#if QT_CONFIG(tooltip)
        moveAllBtn->setToolTip(QCoreApplication::translate("MovesToMoveDialog", "Use TeraCopy to move all movies", nullptr));
#endif // QT_CONFIG(tooltip)
        closeBtn->setText(QCoreApplication::translate("MovesToMoveDialog", "Close", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MovesToMoveDialog: public Ui_MovesToMoveDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MOVESTOMOVEDIALOG_H
