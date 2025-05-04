QT += core gui widgets network concurrent

CONFIG += c++17

SOURCES += \
    OmdbClient.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    OmdbClient.h \
    mainwindow.h

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
