#include "settingwidget.h"

settingWidget::settingWidget(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle("Settings");
    layout = new QGridLayout(this);
    QLabel *title=new QLabel(this);
    title->setTextFormat(Qt::RichText);
    title->setText("<font size=5> Settings </font>");
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title,0,0,1,1);
    widgetCount=0;
}

void settingWidget::addWidget(QWidget* w){
    widgetCount++;
    layout->addWidget(w,widgetCount,0,1,1);
}
settingWidget::~settingWidget()
{

}
