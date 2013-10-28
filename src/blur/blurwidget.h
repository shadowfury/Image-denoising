#ifndef BLURWIDGET_H
#define BLURWIDGET_H
#include <QLineEdit>
#include <QHBoxLayout>
#include <QSlider>
#include <QLabel>
#include <QDialog>
#include <QComboBox>
#include "Blur.h"
#include <QWidget>


class blurWidget : public QWidget
{
    Q_OBJECT
    QGridLayout *layout;      // layout for the centralwidget
    QLineEdit *edit_radius;
    QSlider *slider;


public:
    blurWidget(QWidget *parent = 0);
    ~blurWidget();

public slots:
    blurClass* getBlur();
    void on_slider_valueChanged(int value);
};

#endif // BLURWIDGET_H
