QT += core gui widgets network concurrent sql

CONFIG += c++17

SOURCES += \
    OmdbClient/OmdbClient.cpp \
    main.cpp \
    MovieDatabase/MovieDatabase.cpp \
    MainWindow/MainWindow.cpp \
    TableColumnManager/TableColumnManager.cpp \
    MovieDataRefresher/MovieDataRefresher.cpp \
    MoviesToMoveWindow/MoviesToMoveWindow.cpp \
    SettingsDialog/SettingsDialog.cpp

HEADERS += \
    OmdbClient/OmdbClient.h \
    MovieDatabase/MovieDatabase.h \
    MainWindow/MainWindow.h \
    Movie/Movie.h \
    TableColumnManager/TableColumnManager.h \
    MovieDataRefresher/MovieDataRefresher.h \
    MoviesToMoveWindow/MoviesToMoveWindow.h \
    SettingsDialog/SettingsDialog.h

FORMS += \
    MainWindow/MainWindow.ui

# Deployment rules
unix:!android {
    target.path = /opt/$${TARGET}/bin
    INSTALLS += target
}
qnx {
    target.path = /tmp/$${TARGET}/bin
    INSTALLS += target
}
