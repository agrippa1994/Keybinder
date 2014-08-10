DEFINES += NOMINMAX
DEFINES += WIN32_LEAN_AND_MEAN

QT       += core gui widgets websockets

TARGET = Keybinder
TEMPLATE = app

SOURCES += main.cpp\
        MainWindow.cpp \
    QKeyHook.cpp \
    QLineReader.cpp

HEADERS  += MainWindow.h \
    SAMP/Addresses.hpp \
    SAMP/InjectData.hpp \
    SAMP/RemoteFunctionCaller.hpp \
    SAMP/RemoteMemory.hpp \
    SAMP/SAMP.hpp \
    windows.h \
    QKeyHook.h \
    QLineReader.h \
    StatsAPI/StatsAPI.h \
    OverlayAPI/Overlay.h

FORMS    += MainWindow.ui

win32: LIBS += -L$$PWD/ -ldx9_overlay
win32: LIBS += -L$$PWD/ -lRGN-Stats-API

