#ifndef TABLECOLUMNMANAGER_H
#define TABLECOLUMNMANAGER_H

#include <QObject>
#include <QTableWidget>
#include <QMap>
#include <QVector>
#include <QToolBar>

class TableColumnManager : public QObject
{
    Q_OBJECT

public:
    explicit TableColumnManager(QTableWidget *table, QToolBar *toolbar, QObject *parent = nullptr);

    void setupColumnVisibilityMenu();
    void setupColumnReorderingMenu();
    void loadSettings();
    void saveSettings();

private slots:
    void onSelectAllColumnsClicked();
    void onDeselectAllColumnsClicked();
    void onColumnVisibilityToggled(bool checked);
    void onResetColumnOrderClicked();
    void onColumnMoveLeftClicked();
    void onColumnMoveRightClicked();
    void onColumnMoveToPositionClicked();
    void onHeaderSectionMoved(int logicalIndex, int oldVisualIndex, int newVisualIndex);

private:
    void updateColumnVisibility();
    void saveColumnVisibilitySettings();
    void loadColumnVisibilitySettings();
    void saveColumnOrderSettings();
    void loadColumnOrderSettings();
    void updateHeaderLabels();
    void reorderColumn(int column, int newPosition);

    QTableWidget *tableWidget;
    QToolBar *toolbar;

    QMap<int, QString> columnIndexToName;
    QMap<int, bool> columnVisibility;
};

#endif // TABLECOLUMNMANAGER_H
