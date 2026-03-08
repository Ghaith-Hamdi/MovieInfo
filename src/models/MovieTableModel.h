#ifndef MODELS_MOVIETABLEMODEL_H
#define MODELS_MOVIETABLEMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include "core/VideoFile.h"
#include "core/ColumnDef.h"

namespace Models
{

    class MovieTableModel : public QAbstractTableModel
    {
        Q_OBJECT

    public:
        enum DataRoles
        {
            RawValueRole = Qt::UserRole + 1,
            FilePathRole,
            FolderTitleRole,
            FolderYearRole,
        };

        explicit MovieTableModel(QObject *parent = nullptr);

        // QAbstractTableModel interface
        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        int columnCount(const QModelIndex &parent = QModelIndex()) const override;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        QVariant headerData(int section, Qt::Orientation orientation,
                            int role = Qt::DisplayRole) const override;

        // Bulk operations
        void setFiles(const QList<Core::VideoFile> &files);
        void addFile(const Core::VideoFile &file);
        void updateFile(int row, const Core::VideoFile &file);
        void clear();

        // Attach movie metadata to rows matching title+year
        void attachMovieMetadata(const Core::Movie &movie);

        // Access underlying data
        const Core::VideoFile &fileAt(int row) const;
        const QList<Core::VideoFile> &allFiles() const;
        int findRow(const QString &filePath) const;

    private:
        QList<Core::VideoFile> m_files;
    };

} // namespace Models

#endif // MODELS_MOVIETABLEMODEL_H
