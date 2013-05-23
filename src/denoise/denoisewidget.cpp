#include "denoisewidget.h"


denoiseWidget::denoiseWidget(QWidget *parent)
    : QDialog(parent)
{
    this->setWindowTitle("Doise method settings");
    layout = new QGridLayout(this);
    select=new QPushButton("Select settings",this);
    edit_ps= new QLineEdit("5",this);
    edit_sw= new QLineEdit("9",this);
    edit_pow= new QLineEdit("120",this);
    curr_ind=0;


    combo= new QComboBox(this);

    QLabel *lbl_settings= new QLabel("Denoising method settings",this);
    lbl_settings->setAlignment(Qt::AlignCenter);
    QLabel *lbl_type= new QLabel("Select denoising method",this);
    lbl_type->setAlignment(Qt::AlignCenter);
    QLabel *lbl_int_red=new QLabel("Patch size",this);
    lbl_int_red->setAlignment(Qt::AlignCenter);
    QLabel *lbl_int_green=new QLabel("Search window",this);
    lbl_int_green->setAlignment(Qt::AlignCenter);
    QLabel *lbl_int_blue=new QLabel("Denoising power",this);
    lbl_int_blue->setAlignment(Qt::AlignCenter);


    layout->addWidget(lbl_type,0,0,1,3);
    layout->addWidget(combo,1,0,1,3);
    layout->addWidget(lbl_settings,3,0,1,3);
    layout->addWidget(lbl_int_red,5,0,1,1);
    layout->addWidget(lbl_int_green,5,1,1,1);
    layout->addWidget(lbl_int_blue,5,2,1,1);
    layout->addWidget(edit_ps,6,0,1,1);
    layout->addWidget(edit_sw,6,1,1,1);
    layout->addWidget(edit_pow,6,2,1,1);
    layout->addWidget(select,11,0,1,3);

    connect(select,SIGNAL(clicked()),this,SLOT(hide()));
    connect(combo,SIGNAL(currentIndexChanged(int)),this,SLOT(on_combo_IndexChanged(int)));
}



denoiseClass* denoiseWidget::getSettings()
{
    denoiseClass* denoise= new denoiseClass(denoiseClass::method(combo->currentIndex()),edit_ps->text().toInt(),edit_sw->text().toInt(),edit_pow->text().toInt());
    return denoise;
}
void denoiseWidget::hide(){
    this->setVisible(false);
}
void denoiseWidget::on_combo_IndexChanged(int ind){
    if (ind==0){
        edit_ps->setEnabled(true);
        edit_sw->setEnabled(false);
        edit_pow->setEnabled(false);
    }
    else if ((ind==1)||(ind==2)||(ind==3)||(ind==4)){
        edit_ps->setEnabled(true);
        edit_sw->setEnabled(true);
        edit_pow->setEnabled(true);
    }
    else{
        edit_ps->setEnabled(false);
        edit_sw->setEnabled(false);
        edit_pow->setEnabled(false);
    }
    curr_ind=ind;
}
void denoiseWidget::setActiveIndex(int i)
{
    combo->setCurrentIndex(i);
    curr_ind=i;
}

void denoiseWidget::initCombo(QMenu *menu,int curr)
{
    curr_ind=curr;
    while(combo->count()>0) combo->removeItem(0);
    for (int i=0;i<menu->actions().count();i++){
        combo->addItem(menu->actions().at(i)->text());
    }
}

denoiseWidget::~denoiseWidget()
{

}
