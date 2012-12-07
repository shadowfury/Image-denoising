#-------------------------------------------------
#
# Project created by QtCreator 2012-03-09T13:32:55
#
#-------------------------------------------------

QT       += core gui

TARGET = ./release/Denoising
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    noisewidget.cpp \
    denoisewidget.cpp \
    droparea.cpp \
    blurwidget.cpp \
    settingwidget.cpp

HEADERS  += mainwindow.h \
    noisewidget.h \
    Noise.h \
    RgbMy.h \
    matrix.h \
    denoisewidget.h \
    Denoise.h \
    droparea.h \
    blurwidget.h \
    Blur.h \
    settingwidget.h

FORMS    += mainwindow.ui
MOC_DIR += ./release/objects
OBJECTS_DIR += ./release/objects
