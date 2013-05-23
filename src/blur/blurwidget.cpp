#include "blurwidget.h"



blurWidget::blurWidget(QWidget *parent)
    : QWidget(parent)
{
    layout = new QGridLayout(this);
    edit_radius= new QLineEdit("0",this);
    slider= new QSlider(Qt::Horizontal,this);

    QLabel *lbl_radius= new QLabel("Blur radius value",this);
    lbl_radius->setAlignment(Qt::AlignCenter);


    QLabel *lbl_sets= new QLabel(this);
    lbl_sets->setTextFormat(Qt::RichText);
    lbl_sets->setText("<font size=4> Blur settings </font>");
    lbl_sets->setAlignment(Qt::AlignCenter);


    layout->addWidget(lbl_sets,0,0,1,1);
    layout->addWidget(lbl_radius,1,0,1,1);
    layout->addWidget(edit_radius,2,0,1,1);
    layout->addWidget(slider,3,0,1,1);

    connect(slider,SIGNAL(valueChanged(int)),this,SLOT(on_slider_valueChanged(int)));

}


void blurWidget::on_slider_valueChanged(int value)
{
    edit_radius->setText(QString::number(value));
    slider->setValue(value);
}


blurClass* blurWidget::getBlur()
{
    blurClass* blur= new blurClass(edit_radius->text().toInt());
    return blur;
}


blurWidget::~blurWidget()
{

}

