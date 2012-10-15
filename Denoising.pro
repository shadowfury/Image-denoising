#-------------------------------------------------
#
# Project created by QtCreator 2012-03-09T13:32:55
#
#-------------------------------------------------

QT       += core gui

TARGET = Denoising
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    noisewidget.cpp \
    denoisewidget.cpp

HEADERS  += mainwindow.h \
    noisewidget.h \
    Noise.h \
    RgbMy.h \
    matrix.h \
    denoisewidget.h \
    Denoise.h

FORMS    += mainwindow.ui
