#include "MovieTableModel.h"
#include "core/QualityRules.h"
#include <QColor>
#include <QFont>

namespace Models
{

    MovieTableModel::MovieTableModel(QObject *parent)
        : QAbstractTableModel(parent)
    {
    }

    int MovieTableModel::rowCount(const QModelIndex &parent) const
    {
        if (parent.isValid())
            return 0;
        return m_files.size();
    }

    int MovieTableModel::columnCount(const QModelIndex &parent) const
    {
        if (parent.isValid())
            return 0;
        return static_cast<int>(Core::Column::COUNT);
    }

    QVariant MovieTableModel::data(const QModelIndex &index, int role) const
    {
        if (!index.isValid() || index.row() >= m_files.size())
            return {};

        const auto &vf = m_files[index.row()];
        auto col = static_cast<Core::Column>(index.column());

        if (role == FilePathRole)
            return vf.filePath;
        if (role == FolderTitleRole)
            return vf.folderTitle;
        if (role == FolderYearRole)
            return vf.folderYear;
        if (role == ImdbIdRole)
            return vf.metadata ? vf.metadata->imdbId : QString();

        if (role == Qt::DisplayRole || role == RawValueRole)
        {
            switch (col)
            {
            case Core::Column::Title:
                return vf.metadata ? vf.metadata->title : vf.folderTitle;
            case Core::Column::Year:
                return vf.metadata ? QString::number(vf.metadata->year)
                                   : (vf.folderYear > 0 ? QString::number(vf.folderYear) : "");
            case Core::Column::Rated:
                return vf.metadata ? vf.metadata->rated : "";
            case Core::Column::Genre:
                return vf.metadata ? vf.metadata->genre : "";
            case Core::Column::Director:
                return vf.metadata ? vf.metadata->director : "";
            case Core::Column::Writers:
                return vf.metadata ? vf.metadata->writer : "";
            case Core::Column::Actors:
                return vf.metadata ? vf.metadata->actors : "";
            case Core::Column::Plot:
                return vf.metadata ? vf.metadata->plot : "";
            case Core::Column::Language:
                return vf.metadata ? vf.metadata->language : "";
            case Core::Column::Country:
                return vf.metadata ? vf.metadata->country : "";
            case Core::Column::ImdbRating:
            {
                if (!vf.metadata)
                    return "";
                if (role == RawValueRole)
                    return vf.metadata->imdbRating;
                return vf.metadata->formattedRating();
            }
            case Core::Column::ImdbVotes:
            {
                if (!vf.metadata)
                    return "";
                if (role == RawValueRole)
                    return vf.metadata->imdbVotes;
                return vf.metadata->formattedVotes();
            }
            case Core::Column::Resolution:
                return vf.resolution;
            case Core::Column::Quality:
                return vf.qualityTier;
            case Core::Column::AudioLanguage:
                return vf.audioLanguage;
            case Core::Column::AspectRatio:
                return vf.formattedAspectRatio();
            case Core::Column::Duration:
                return vf.formattedDuration();
            case Core::Column::Size:
                return vf.formattedFileSize();
            case Core::Column::Actions:
                return ""; // Handled by delegate
            default:
                return {};
            }
        }

        if (role == Qt::ForegroundRole)
        {
            if (col == Core::Column::ImdbRating && vf.metadata)
            {
                double r = vf.metadata->imdbRating;
                if (r >= 7.0)
                    return QColor("#4CAF50");
                if (r >= 5.0)
                    return QColor("#FFC107");
                return QColor("#F44336");
            }
            if (col == Core::Column::AspectRatio)
            {
                if (vf.isUltraWide())
                    return QColor("#64B5F6");
            }
        }

        if (role == Qt::TextAlignmentRole)
        {
            if (col == Core::Column::ImdbRating || col == Core::Column::ImdbVotes ||
                col == Core::Column::Year || col == Core::Column::Size ||
                col == Core::Column::Duration)
            {
                return QVariant(Qt::AlignRight | Qt::AlignVCenter);
            }
        }

        return {};
    }

    QVariant MovieTableModel::headerData(int section, Qt::Orientation orientation, int role) const
    {
        if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
            return {};

        if (section >= 0 && section < static_cast<int>(Core::Column::COUNT))
            return Core::columnName(static_cast<Core::Column>(section));

        return {};
    }

    void MovieTableModel::setFiles(const QList<Core::VideoFile> &files)
    {
        beginResetModel();
        m_files = files;
        endResetModel();
    }

    void MovieTableModel::addFile(const Core::VideoFile &file)
    {
        int row = m_files.size();
        beginInsertRows(QModelIndex(), row, row);
        m_files.append(file);
        endInsertRows();
    }

    void MovieTableModel::updateFile(int row, const Core::VideoFile &file)
    {
        if (row < 0 || row >= m_files.size())
            return;
        m_files[row] = file;
        emit dataChanged(index(row, 0), index(row, columnCount() - 1));
    }

    void MovieTableModel::clear()
    {
        beginResetModel();
        m_files.clear();
        endResetModel();
    }

    void MovieTableModel::attachMovieMetadata(const Core::Movie &movie)
    {
        for (int i = 0; i < m_files.size(); ++i)
        {
            auto &vf = m_files[i];
            if (vf.folderTitle.compare(movie.title, Qt::CaseInsensitive) == 0 ||
                (vf.folderYear == movie.year && !movie.title.isEmpty()))
            {
                // More flexible: also try sanitized match
                QString sanitizedFolder = vf.folderTitle.toLower().simplified();
                QString sanitizedMovie = movie.title.toLower().simplified();
                if (sanitizedFolder == sanitizedMovie ||
                    vf.folderTitle.compare(movie.title, Qt::CaseInsensitive) == 0)
                {
                    vf.metadata = movie;
                    emit dataChanged(index(i, 0), index(i, columnCount() - 1));
                }
            }
        }
    }

    const Core::VideoFile &MovieTableModel::fileAt(int row) const
    {
        return m_files[row];
    }

    const QList<Core::VideoFile> &MovieTableModel::allFiles() const
    {
        return m_files;
    }

    int MovieTableModel::findRow(const QString &filePath) const
    {
        for (int i = 0; i < m_files.size(); ++i)
        {
            if (m_files[i].filePath == filePath)
                return i;
        }
        return -1;
    }

} // namespace Models
