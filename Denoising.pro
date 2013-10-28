#-------------------------------------------------
#
# Project created by QtCreator 2012-03-09T13:32:55
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = ./release/Denoising
TEMPLATE = app


SOURCES += src/main.cpp\
        src/mainwindow.cpp \
    src/denoise/noisewidget.cpp \
    src/denoise/denoisewidget.cpp \
    src/droparea.cpp \
    src/blur/blurwidget.cpp \
    src/settingwidget.cpp \
    src/denoise/qdenoiser.cpp

HEADERS  += src/mainwindow.h \
    src/denoise/noisewidget.h \
    src/denoise/Noise.h \
    src/RgbMy.h \
    src/denoise/denoisewidget.h \
    src/denoise/Denoise.h \
    src/droparea.h \
    src/blur/blurwidget.h \
    src/blur/Blur.h \
    src/settingwidget.h \
    src/denoise/qdenoiser.h

FORMS    += src/mainwindow.ui
MOC_DIR += ./release/objects
OBJECTS_DIR += ./release/objects
UI_DIR +=./release/objects

#LIBS += -L"$$_PRO_FILE_PWD_/fftw3/.libs/" -lfftw3
