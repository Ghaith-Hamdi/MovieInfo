#include "AppSettings.h"
#include "core/ColumnDef.h"

namespace Infrastructure
{

    AppSettings::AppSettings(QObject *parent)
        : QObject(parent), m_settings("MovieInfo", "MovieInfoApp")
    {
    }

    // --- API ---
    QString AppSettings::omdbApiKey() const
    {
        return m_settings.value("api/omdbKey", "").toString();
    }

    void AppSettings::setOmdbApiKey(const QString &key)
    {
        m_settings.setValue("api/omdbKey", key);
    }

    // --- Fetch settings ---
    bool AppSettings::showFetchSummary() const
    {
        return m_settings.value("fetch/showSummary", true).toBool();
    }

    void AppSettings::setShowFetchSummary(bool show)
    {
        m_settings.setValue("fetch/showSummary", show);
    }

    // --- Column visibility ---
    bool AppSettings::columnVisible(int col) const
    {
        // Use ColumnDef's visibleByDefault as the factory default
        const auto &defs = Core::columnDefinitions();
        bool def = (col >= 0 && col < defs.size()) ? defs[col].visibleByDefault : true;
        return m_settings.value(QStringLiteral("columns/visible/%1").arg(col), def).toBool();
    }

    void AppSettings::setColumnVisible(int col, bool visible)
    {
        m_settings.setValue(QStringLiteral("columns/visible/%1").arg(col), visible);
    }

    // --- Column order ---
    int AppSettings::columnVisualIndex(int logicalIndex) const
    {
        return m_settings.value(QStringLiteral("columns/order/%1").arg(logicalIndex), logicalIndex).toInt();
    }

    void AppSettings::setColumnVisualIndex(int logicalIndex, int visualIndex)
    {
        m_settings.setValue(QStringLiteral("columns/order/%1").arg(logicalIndex), visualIndex);
    }

    // --- Drives ---
    QStringList AppSettings::drives() const
    {
        return m_settings.value("drives/list", QStringList{"D:\\", "E:\\"}).toStringList();
    }

    void AppSettings::setDrives(const QStringList &drives)
    {
        m_settings.setValue("drives/list", drives);
    }

    // --- Last folder ---
    QString AppSettings::lastFolder() const
    {
        return m_settings.value("paths/lastFolder", "").toString();
    }

    void AppSettings::setLastFolder(const QString &path)
    {
        m_settings.setValue("paths/lastFolder", path);
    }

    // --- External tools ---
    QString AppSettings::browserCommand() const
    {
        // Default: use system default browser via QDesktopServices
        return m_settings.value("tools/browser", "").toString();
    }

    void AppSettings::setBrowserCommand(const QString &cmd)
    {
        m_settings.setValue("tools/browser", cmd);
    }

    // --- API aliases ---
    QString AppSettings::apiKey() const
    {
        return omdbApiKey();
    }

    void AppSettings::setApiKey(const QString &key)
    {
        setOmdbApiKey(key);
    }

    // --- Quality rules ---
    Core::QualityRule AppSettings::qualityRule() const
    {
        Core::QualityRule rule;
        rule.minRating = highQualityMinRating();
        rule.minVotes = highQualityMinVotes();
        return rule;
    }

    void AppSettings::setQualityRule(const Core::QualityRule &rule)
    {
        setHighQualityMinRating(rule.minRating);
        setHighQualityMinVotes(rule.minVotes);
    }

    double AppSettings::highQualityMinRating() const
    {
        return m_settings.value("quality/minRating", 7.0).toDouble();
    }

    void AppSettings::setHighQualityMinRating(double rating)
    {
        m_settings.setValue("quality/minRating", rating);
    }

    int AppSettings::highQualityMinVotes() const
    {
        return m_settings.value("quality/minVotes", 100000).toInt();
    }

    void AppSettings::setHighQualityMinVotes(int votes)
    {
        m_settings.setValue("quality/minVotes", votes);
    }

} // namespace Infrastructure
