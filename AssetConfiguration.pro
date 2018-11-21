
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


SOURCES += $$files(*.cpp, true)

#SOURCES += $$files(/home/ice/Qt5/5.11.2/Src/qtbase/src/*.cpp, true)

HEADERS  += $$files(*.h, true)

FORMS     += mainwindow.ui

RESOURCES += mainwindow.qrc

unix:!macx: LIBS += -L$$PWD/lib/ -lQDeviceWatcher
