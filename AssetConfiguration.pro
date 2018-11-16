
#-------------------------------------------------
#
# Project created by QtCreator 2017-01-23T10:55:46
#
#-------------------------------------------------

QT     += core gui network

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AssetConfiguration
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    DataModel/configserializer.cpp \
    Configurations/localassetconfiguration.cpp \
    Configurations/assetconfiguration.cpp \
    Configurations/jsonconfiguration.cpp \
    MineqWidgets/assettablewidget.cpp \
    Utilities/terminalcmdexecutor.cpp \
    Utilities/usbconfigmounter.cpp \
    MineqWidgets/assetconfigprocesswidget.cpp \
    configurationmanager.cpp \
    Configurations/deviceconfiguration.cpp \
    MineqWidgets/mineqmessagewidget.cpp \
    Configurations/currentconfiguration.cpp \
    mineqtabmanager.cpp

HEADERS  += mainwindow.h \
    DataModel/configserializer.h \
    DataModel/iconfiguration.h \
    Configurations/localassetconfiguration.h \
    Configurations/assetconfiguration.h \
    Configurations/jsonconfiguration.h \
    MineqWidgets/assettablewidget.h \
    Utilities/terminalcmdexecutor.h \
    Utilities/usbconfigmounter.h \
    MineqWidgets/assetconfigprocesswidget.h \
    configurationmanager.h \
    Configurations/deviceconfiguration.h \
    MineqWidgets/mineqmessagewidget.h \
    Configurations/currentconfiguration.h \
    mineqtabmanager.h \
    qdevicewatcher.h

FORMS     += mainwindow.ui

RESOURCES += mainwindow.qrc

unix:!macx: LIBS += -L$$PWD/lib/ -lQDeviceWatcher
