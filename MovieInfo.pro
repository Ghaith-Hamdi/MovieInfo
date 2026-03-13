QT += core gui widgets network concurrent sql

CONFIG += c++17

INCLUDEPATH += src

SOURCES += \
    src/main.cpp \
    src/infrastructure/db/Database.cpp \
    src/infrastructure/db/MovieRepository.cpp \
    src/infrastructure/db/VideoMetadataRepository.cpp \
    src/infrastructure/process/FfprobeRunner.cpp \
    src/infrastructure/settings/AppSettings.cpp \
    src/infrastructure/api/OmdbApiClient.cpp \
    src/services/ScanService.cpp \
    src/services/OmdbService.cpp \
    src/services/OrganizeService.cpp \
    src/services/ExportService.cpp \
    src/models/MovieTableModel.cpp \
    src/models/MovieFilterProxyModel.cpp \
    src/ui/MainWindow.cpp \
    src/ui/ActionButtonDelegate.cpp \
    src/ui/FilterPanel.cpp \
    src/ui/dialogs/SettingsDialog.cpp \
    src/ui/dialogs/MovesToMoveDialog.cpp \
    src/ui/dialogs/RenameFolderDialog.cpp \
    src/ui/dialogs/FetchSummaryDialog.cpp \
    src/ui/dialogs/WatchlistDialog.cpp

HEADERS += \
    src/core/Movie.h \
    src/core/VideoFile.h \
    src/core/FilterCriteria.h \
    src/core/QualityRules.h \
    src/core/ColumnDef.h \
    src/infrastructure/db/Database.h \
    src/infrastructure/db/MovieRepository.h \
    src/infrastructure/db/VideoMetadataRepository.h \
    src/infrastructure/process/FfprobeRunner.h \
    src/infrastructure/settings/AppSettings.h \
    src/infrastructure/api/OmdbApiClient.h \
    src/services/ScanService.h \
    src/services/OmdbService.h \
    src/services/OrganizeService.h \
    src/services/ExportService.h \
    src/models/MovieTableModel.h \
    src/models/MovieFilterProxyModel.h \
    src/ui/MainWindow.h \
    src/ui/ActionButtonDelegate.h \
    src/ui/FilterPanel.h \
    src/ui/dialogs/SettingsDialog.h \
    src/ui/dialogs/MovesToMoveDialog.h \
    src/ui/dialogs/RenameFolderDialog.h \
    src/ui/dialogs/FetchSummaryDialog.h \
    src/ui/dialogs/WatchlistDialog.h

RESOURCES += resources.qrc

FORMS += \
    src/ui/MainWindow.ui \
    src/ui/FilterPanel.ui \
    src/ui/dialogs/SettingsDialog.ui \
    src/ui/dialogs/RenameFolderDialog.ui \
    src/ui/dialogs/MovesToMoveDialog.ui \
    src/ui/dialogs/FetchSummaryDialog.ui \
    src/ui/dialogs/WatchlistDialog.ui

# Deployment rules
unix:!android {
    target.path = /opt/$${TARGET}/bin
    INSTALLS += target
}
qnx {
    target.path = /tmp/$${TARGET}/bin
    INSTALLS += target
}
