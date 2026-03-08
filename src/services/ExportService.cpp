#include "ExportService.h"
#include "models/MovieTableModel.h"
#include "models/MovieFilterProxyModel.h"
#include "core/ColumnDef.h"
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

namespace Services
{

    ExportService::ExportService(QObject *parent) : QObject(parent) {}

    bool ExportService::exportData(const Models::MovieFilterProxyModel *proxyModel,
                                   const QString &filePath, Format format)
    {
        if (!proxyModel)
        {
            emit exportFailed("No data model");
            return false;
        }

        QFile file(filePath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            emit exportFailed("Could not open file: " + filePath);
            return false;
        }

        int colCount = proxyModel->columnCount();
        int rowCount = proxyModel->rowCount();

        if (format == Format::CSV)
        {
            QTextStream out(&file);

            // Header (skip Actions column)
            QStringList headers;
            for (int c = 0; c < colCount; ++c)
            {
                if (c == static_cast<int>(Core::Column::Actions))
                    continue;
                headers << proxyModel->headerData(c, Qt::Horizontal, Qt::DisplayRole).toString();
            }
            out << headers.join(",") << "\n";

            // Rows
            for (int r = 0; r < rowCount; ++r)
            {
                QStringList cells;
                for (int c = 0; c < colCount; ++c)
                {
                    if (c == static_cast<int>(Core::Column::Actions))
                        continue;
                    QModelIndex idx = proxyModel->index(r, c);
                    QString val = proxyModel->data(idx, Qt::DisplayRole).toString();
                    val.replace("\"", "\"\"");
                    cells << ("\"" + val + "\"");
                }
                out << cells.join(",") << "\n";
            }
        }
        else if (format == Format::JSON)
        {
            QJsonArray arr;
            for (int r = 0; r < rowCount; ++r)
            {
                QJsonObject obj;
                for (int c = 0; c < colCount; ++c)
                {
                    if (c == static_cast<int>(Core::Column::Actions))
                        continue;
                    QString header = proxyModel->headerData(c, Qt::Horizontal, Qt::DisplayRole).toString();
                    QModelIndex idx = proxyModel->index(r, c);
                    obj[header] = proxyModel->data(idx, Qt::DisplayRole).toString();
                }
                arr.append(obj);
            }
            file.write(QJsonDocument(arr).toJson(QJsonDocument::Indented));
        }

        file.close();
        emit exportComplete(filePath);
        return true;
    }

} // namespace Services
