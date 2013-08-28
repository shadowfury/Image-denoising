#ifndef SETTINGWIDGET_H
#define SETTINGWIDGET_H

#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>

class settingWidget : public QDialog
{
    Q_OBJECT
    QGridLayout *layout;
    int widgetCount;
public:
    settingWidget(QWidget *parent = 0);
    ~settingWidget();
    
signals:
    
public slots:
    void addWidget(QWidget* w);
};

#endif // SETTINGWIDGET_H
