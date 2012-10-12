#ifndef DENOISEWIDGET_H
#define DENOISEWIDGET_H

#include <QPushButton>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QMenu>
#include "Denoise.h"
#include <QtGui/QWidget>


class denoiseWidget : public QWidget
{
    Q_OBJECT
    QGridLayout *layout;      // layout for the centralwidget
    QLineEdit * edit_ps,*edit_sw,*edit_pow;



public:
    QPushButton *select;
    QComboBox *combo;
    denoiseWidget(QWidget *parent = 0);
    ~denoiseWidget();
    int curr_ind;

public slots:
    void hide();
    void on_combo_IndexChanged(int ind);
    denoiseClass* getSettings();
    void initCombo(QMenu *menu,int curr);
    void setActiveIndex(int i);
};


#endif // DEdenoiseIDGET_H
