#ifndef INFRASTRUCTURE_APPSETTINGS_H
#define INFRASTRUCTURE_APPSETTINGS_H

#include <QObject>
#include <QSettings>
#include <QStringList>
#include "core/QualityRules.h"

namespace Infrastructure
{

    class AppSettings : public QObject
    {
        Q_OBJECT

    public:
        explicit AppSettings(QObject *parent = nullptr);

        // API
        QString apiKey() const; // alias for omdbApiKey()
        void setApiKey(const QString &key);

        QString omdbApiKey() const;
        void setOmdbApiKey(const QString &key);

        // Fetch settings
        bool showFetchSummary() const;
        void setShowFetchSummary(bool show);

        // Column visibility
        bool columnVisible(int col) const;
        void setColumnVisible(int col, bool visible);

        // Column order
        int columnVisualIndex(int logicalIndex) const;
        void setColumnVisualIndex(int logicalIndex, int visualIndex);

        // Drives
        QStringList drives() const;
        void setDrives(const QStringList &drives);

        // Last folder
        QString lastFolder() const;
        void setLastFolder(const QString &path);

        // External tools
        QString browserCommand() const;
        void setBrowserCommand(const QString &cmd);

        // Quality rules (aggregate accessor)
        Core::QualityRule qualityRule() const;
        void setQualityRule(const Core::QualityRule &rule);

        double highQualityMinRating() const;
        void setHighQualityMinRating(double rating);
        int highQualityMinVotes() const;
        void setHighQualityMinVotes(int votes);

    private:
        QSettings m_settings;
    };

} // namespace Infrastructure

#endif // INFRASTRUCTURE_APPSETTINGS_H
