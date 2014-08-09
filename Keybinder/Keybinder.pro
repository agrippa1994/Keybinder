QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

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
    QLineReader.h

FORMS    += MainWindow.ui