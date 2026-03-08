#ifndef SERVICES_EXPORTSERVICE_H
#define SERVICES_EXPORTSERVICE_H

#include <QObject>

namespace Models
{
    class MovieTableModel;
    class MovieFilterProxyModel;
}

namespace Services
{

    class ExportService : public QObject
    {
        Q_OBJECT

    public:
        enum class Format
        {
            CSV,
            JSON
        };

        explicit ExportService(QObject *parent = nullptr);

        bool exportData(const Models::MovieFilterProxyModel *proxyModel,
                        const QString &filePath, Format format);

    signals:
        void exportComplete(const QString &filePath);
        void exportFailed(const QString &error);
    };

} // namespace Services

#endif // SERVICES_EXPORTSERVICE_H
