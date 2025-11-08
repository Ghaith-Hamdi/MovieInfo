#include "TableColumnManager.h"
#include <QPushButton>
#include <QMenu>
#include <QAction>
#include <QSettings>
#include <QHeaderView>
#include <algorithm>

TableColumnManager::TableColumnManager(QTableWidget *table, QToolBar *toolbar, QObject *parent)
    : QObject(parent), tableWidget(table), toolbar(toolbar)
{
    // Set up column name mapping
    columnIndexToName = {
        {0, "Title"}, {1, "Year"}, {2, "Decade"}, {3, "Resolution"}, {4, "Aspect Ratio"}, {5, "Quality"}, {6, "Size"}, {7, "Duration"}, {8, "Language"}, {9, "Actions"}, {10, "Rated"}, {11, "Rating"}, {12, "Votes"}, {13, "Director"}, {14, "Actors"}, {15, "Writers"}, {16, "Awards"}, {17, "Language"}, {18, "Country"}, {19, "Box Office"}, {20, "Plot"}, {21, "Genre"}};

    // Initialize visibility for all columns
    for (int i = 0; i < tableWidget->columnCount(); i++)
    {
        columnVisibility[i] = true;
    }
}

void TableColumnManager::setupColumnVisibilityMenu()
{
    QPushButton *columnVisibilityButton = new QPushButton("Columns");
    toolbar->addWidget(columnVisibilityButton);

    QMenu *columnMenu = new QMenu();
    columnVisibilityButton->setMenu(columnMenu);

    // Add "Select All" option
    QAction *selectAllAction = columnMenu->addAction("Select All");
    connect(selectAllAction, &QAction::triggered, this, &TableColumnManager::onSelectAllColumnsClicked);

    // Add "Deselect All" option
    QAction *deselectAllAction = columnMenu->addAction("Deselect All");
    connect(deselectAllAction, &QAction::triggered, this, &TableColumnManager::onDeselectAllColumnsClicked);

    columnMenu->addSeparator();

    // Add checkboxes for each column
    for (int i = 0; i < tableWidget->columnCount(); i++)
    {
        QString columnName = columnIndexToName.value(i, tableWidget->horizontalHeaderItem(i)->text());
        QAction *action = columnMenu->addAction(columnName);
        action->setCheckable(true);
        action->setChecked(columnVisibility.value(i, true));
        action->setData(i);

        connect(action, &QAction::toggled, this, &TableColumnManager::onColumnVisibilityToggled);
    }
}

void TableColumnManager::setupColumnReorderingMenu()
{
    QPushButton *reorderButton = new QPushButton("Reorder");
    toolbar->addWidget(reorderButton);

    QMenu *reorderMenu = new QMenu();
    reorderButton->setMenu(reorderMenu);

    // Add "Reset Order" option
    QAction *resetOrderAction = reorderMenu->addAction("Reset to Default Order");
    connect(resetOrderAction, &QAction::triggered, this, &TableColumnManager::onResetColumnOrderClicked);

    reorderMenu->addSeparator();

    // Add move options for each column
    for (int i = 0; i < tableWidget->columnCount(); i++)
    {
        QString columnName = columnIndexToName.value(i, tableWidget->horizontalHeaderItem(i)->text());
        QMenu *columnMenu = reorderMenu->addMenu(columnName);

        // Move Left action
        QAction *moveLeftAction = columnMenu->addAction("Move Left");
        moveLeftAction->setData(i);
        connect(moveLeftAction, &QAction::triggered, this, &TableColumnManager::onColumnMoveLeftClicked);

        // Move Right action
        QAction *moveRightAction = columnMenu->addAction("Move Right");
        moveRightAction->setData(i);
        connect(moveRightAction, &QAction::triggered, this, &TableColumnManager::onColumnMoveRightClicked);

        // Move to Position submenu
        QMenu *moveToMenu = columnMenu->addMenu("Move to Position");
        for (int pos = 0; pos < tableWidget->columnCount(); pos++)
        {
            QAction *moveToAction = moveToMenu->addAction(QString::number(pos + 1));
            QVariantList data;
            data << i << pos;
            moveToAction->setData(data);
            connect(moveToAction, &QAction::triggered, this, &TableColumnManager::onColumnMoveToPositionClicked);
        }
    }

    // Make headers reorderable by drag and drop
    tableWidget->horizontalHeader()->setSectionsMovable(true);
    connect(tableWidget->horizontalHeader(), &QHeaderView::sectionMoved,
            this, &TableColumnManager::onHeaderSectionMoved);
}

void TableColumnManager::loadSettings()
{
    loadColumnVisibilitySettings();
    loadColumnOrderSettings();
}

void TableColumnManager::saveSettings()
{
    saveColumnVisibilitySettings();
    saveColumnOrderSettings();
}

void TableColumnManager::onSelectAllColumnsClicked()
{
    for (int i = 0; i < tableWidget->columnCount(); i++)
    {
        columnVisibility[i] = true;
    }
    updateColumnVisibility();
    saveColumnVisibilitySettings();
}

void TableColumnManager::onDeselectAllColumnsClicked()
{
    // Always keep Title column visible
    for (int i = 0; i < tableWidget->columnCount(); i++)
    {
        columnVisibility[i] = (i == 0);
    }
    updateColumnVisibility();
    saveColumnVisibilitySettings();
}

void TableColumnManager::onColumnVisibilityToggled(bool checked)
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (!action)
        return;

    int columnIndex = action->data().toInt();
    columnVisibility[columnIndex] = checked;
    updateColumnVisibility();
    saveColumnVisibilitySettings();
}

void TableColumnManager::onResetColumnOrderClicked()
{
    // Reset column order to default
    for (int i = 0; i < tableWidget->columnCount(); i++)
    {
        tableWidget->horizontalHeader()->moveSection(
            tableWidget->horizontalHeader()->visualIndex(i), i);
    }

    saveColumnOrderSettings();
    updateHeaderLabels();
}

void TableColumnManager::onColumnMoveLeftClicked()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (!action)
        return;

    int columnIndex = action->data().toInt();
    int visualIndex = tableWidget->horizontalHeader()->visualIndex(columnIndex);
    if (visualIndex > 0)
    {
        reorderColumn(columnIndex, visualIndex - 1);
        saveColumnOrderSettings();
    }
}

void TableColumnManager::onColumnMoveRightClicked()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (!action)
        return;

    int columnIndex = action->data().toInt();
    int visualIndex = tableWidget->horizontalHeader()->visualIndex(columnIndex);
    if (visualIndex < tableWidget->columnCount() - 1)
    {
        reorderColumn(columnIndex, visualIndex + 1);
        saveColumnOrderSettings();
    }
}

void TableColumnManager::onColumnMoveToPositionClicked()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (!action)
        return;

    QVariantList data = action->data().toList();
    if (data.size() != 2)
        return;

    int columnIndex = data[0].toInt();
    int position = data[1].toInt();
    reorderColumn(columnIndex, position);
    saveColumnOrderSettings();
}

void TableColumnManager::onHeaderSectionMoved(int logicalIndex, int oldVisualIndex, int newVisualIndex)
{
    Q_UNUSED(logicalIndex);
    Q_UNUSED(oldVisualIndex);
    Q_UNUSED(newVisualIndex);
    saveColumnOrderSettings();
}

void TableColumnManager::updateColumnVisibility()
{
    // Ensure Title column is always visible
    if (!columnVisibility.value(0, true))
    {
        columnVisibility[0] = true;
    }

    // Update column visibility
    for (int i = 0; i < tableWidget->columnCount(); i++)
    {
        bool isVisible = columnVisibility.value(i, true);
        tableWidget->setColumnHidden(i, !isVisible);
    }

    tableWidget->resizeColumnsToContents();
}

void TableColumnManager::saveColumnVisibilitySettings()
{
    QSettings settings("YourCompany", "VideoBrowserApp");
    settings.beginGroup("ColumnVisibility");
    settings.remove("");

    for (auto it = columnVisibility.constBegin(); it != columnVisibility.constEnd(); ++it)
    {
        settings.setValue(QString::number(it.key()), it.value());
    }

    settings.endGroup();
}

void TableColumnManager::loadColumnVisibilitySettings()
{
    QSettings settings("YourCompany", "VideoBrowserApp");
    settings.beginGroup("ColumnVisibility");

    QStringList keys = settings.childKeys();
    if (keys.isEmpty())
    {
        for (int i = 0; i < tableWidget->columnCount(); i++)
        {
            columnVisibility[i] = true;
        }
    }
    else
    {
        for (int i = 0; i < tableWidget->columnCount(); i++)
        {
            bool isVisible = settings.value(QString::number(i), true).toBool();
            columnVisibility[i] = isVisible;
        }
    }

    settings.endGroup();
    updateColumnVisibility();
}

void TableColumnManager::saveColumnOrderSettings()
{
    QSettings settings("YourCompany", "VideoBrowserApp");
    settings.beginGroup("ColumnOrder");
    settings.remove("");

    for (int i = 0; i < tableWidget->columnCount(); i++)
    {
        int visualIndex = tableWidget->horizontalHeader()->visualIndex(i);
        settings.setValue(QString::number(i), visualIndex);
    }

    settings.endGroup();
}

void TableColumnManager::loadColumnOrderSettings()
{
    QSettings settings("YourCompany", "VideoBrowserApp");
    settings.beginGroup("ColumnOrder");

    QStringList keys = settings.childKeys();

    if (!keys.isEmpty())
    {
        QMap<int, int> loadedOrder;
        for (int i = 0; i < tableWidget->columnCount(); i++)
        {
            int visualIndex = settings.value(QString::number(i), i).toInt();
            loadedOrder[i] = visualIndex;
        }

        // Sort columns by their visual index
        QList<int> sortedColumns;
        for (int i = 0; i < tableWidget->columnCount(); i++)
        {
            sortedColumns.append(i);
        }

        std::sort(sortedColumns.begin(), sortedColumns.end(), [&loadedOrder](int a, int b)
                  { return loadedOrder[a] < loadedOrder[b]; });

        // Apply the order
        for (int i = 0; i < sortedColumns.size(); i++)
        {
            int logicalIndex = sortedColumns[i];
            int currentVisualIndex = tableWidget->horizontalHeader()->visualIndex(logicalIndex);
            tableWidget->horizontalHeader()->moveSection(currentVisualIndex, i);
        }
    }

    settings.endGroup();
}

void TableColumnManager::updateHeaderLabels()
{
    QStringList headerLabels = {
        "Title", "Year", "Decade", "Resolution", "Aspect Ratio", "Quality", "Size", "Duration", "Language",
        "Actions", "Rated", "Rating", "Votes", "Director", "Actors", "Writers", "Awards", "Language",
        "Country", "Box Office", "Plot", "Genre"};

    tableWidget->setHorizontalHeaderLabels(headerLabels);
}

void TableColumnManager::reorderColumn(int column, int newPosition)
{
    int currentVisualIndex = tableWidget->horizontalHeader()->visualIndex(column);
    tableWidget->horizontalHeader()->moveSection(currentVisualIndex, newPosition);
}
