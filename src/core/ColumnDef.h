#ifndef CORE_COLUMNDEF_H
#define CORE_COLUMNDEF_H

#include <QString>
#include <QVector>

namespace Core
{

    enum class Column
    {
        Title = 0,
        Year,
        Decade,
        Resolution,
        AspectRatio,
        Quality,
        Size,
        Duration,
        AudioLanguage,
        Actions,
        Rated,
        ImdbRating,
        ImdbVotes,
        Director,
        Actors,
        Writers,
        Awards,
        Language,
        Country,
        BoxOffice,
        Plot,
        Genre,
        COUNT // Always last — gives total column count
    };

    constexpr int ColumnCount = static_cast<int>(Column::COUNT);

    struct ColumnDef
    {
        Column id;
        QString name;
        int defaultWidth;
        bool visibleByDefault;
        bool sortable;
        bool isNumeric;
    };

    inline const QVector<ColumnDef> &columnDefinitions()
    {
        static const QVector<ColumnDef> defs = {
            {Column::Title, "Title", 200, true, true, false},
            {Column::Year, "Year", 60, true, true, false},
            {Column::Decade, "Decade", 70, true, true, false},
            {Column::Resolution, "Resolution", 90, true, true, false},
            {Column::AspectRatio, "Aspect Ratio", 80, true, true, true},
            {Column::Quality, "Quality", 70, true, true, false},
            {Column::Size, "Size", 70, true, true, true},
            {Column::Duration, "Duration", 70, true, true, false},
            {Column::AudioLanguage, "Language", 70, false, true, false},
            {Column::Actions, "Actions", 180, true, false, false},
            {Column::Rated, "Rated", 60, false, true, false},
            {Column::ImdbRating, "Rating", 60, true, true, true},
            {Column::ImdbVotes, "Votes", 80, true, true, true},
            {Column::Director, "Director", 120, true, true, false},
            {Column::Actors, "Actors", 150, false, true, false},
            {Column::Writers, "Writers", 120, false, true, false},
            {Column::Awards, "Awards", 120, false, true, false},
            {Column::Language, "Language", 80, false, true, false},
            {Column::Country, "Country", 80, false, true, false},
            {Column::BoxOffice, "Box Office", 80, false, true, true},
            {Column::Plot, "Plot", 200, false, true, false},
            {Column::Genre, "Genre", 120, true, true, false},
        };
        return defs;
    }

    inline QString columnName(Column col)
    {
        int idx = static_cast<int>(col);
        const auto &defs = columnDefinitions();
        if (idx >= 0 && idx < defs.size())
            return defs[idx].name;
        return QString();
    }

} // namespace Core

#endif // CORE_COLUMNDEF_H
