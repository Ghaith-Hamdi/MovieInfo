QT += core gui widgets network concurrent sql

CONFIG += c++17

SOURCES += \
    OmdbClient.cpp \
    main.cpp \
    moviedb.cpp \
    mainwindow.cpp

HEADERS += \
    OmdbClient.h \
    moviedb.h \
    mainwindow.h \
    movie.h

FORMS += \
    mainwindow.ui

# Deployment rules
unix:!android {
    target.path = /opt/$${TARGET}/bin
    INSTALLS += target
}
qnx {
    target.path = /tmp/$${TARGET}/bin
    INSTALLS += target
}
