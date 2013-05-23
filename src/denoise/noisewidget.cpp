#include "noisewidget.h"



noiseWidget::noiseWidget(QWidget *parent)
    : QWidget(parent)
{
    this->setWindowTitle("Noise settings");
    layout = new QGridLayout(this);
    edit_int_red= new QLineEdit("0",this);
    edit_int_green= new QLineEdit("0",this);
    edit_int_blue= new QLineEdit("0",this);
    edit_pro_red= new QLineEdit("0",this);
    edit_pro_green= new QLineEdit("0",this);
    edit_pro_blue= new QLineEdit("0",this);
    edit_num= new QLineEdit("15",this);
    edit_num->setEnabled(false);


    slideIntensity= new QSlider(Qt::Horizontal,this);
    slideIntensity->setMaximum(255);
    slideIntensity->setMinimum(-255);
    slideProbability= new QSlider(Qt::Horizontal,this);
    slideProbability->setMaximum(100);
    slideProbability->setMinimum(0);
    combo= new QComboBox(this);
    combo->addItem("Uniform distribution");
    combo->addItem("Normal distribution");

    QLabel *lbl_intensity= new QLabel("Noise corruption intensity",this);
    lbl_intensity->setAlignment(Qt::AlignCenter);
    QLabel *lbl_probability= new QLabel("Probability of pixel corruption with noise",this);
    lbl_probability->setAlignment(Qt::AlignCenter);
    QLabel *lbl_type= new QLabel("Noise distribution law",this);
    lbl_type->setAlignment(Qt::AlignCenter);
    QLabel *lbl_int_red=new QLabel("red",this);
    lbl_int_red->setAlignment(Qt::AlignCenter);
    QLabel *lbl_int_green=new QLabel("green",this);
    lbl_int_green->setAlignment(Qt::AlignCenter);
    QLabel *lbl_int_blue=new QLabel("blue",this);
    lbl_int_blue->setAlignment(Qt::AlignCenter);
    QLabel *lbl_pro_red=new QLabel("red",this);
    lbl_pro_red->setAlignment(Qt::AlignCenter);
    QLabel *lbl_pro_green=new QLabel("green",this);
    lbl_pro_green->setAlignment(Qt::AlignCenter);
    QLabel *lbl_pro_blue=new QLabel("blue",this);
    lbl_pro_blue->setAlignment(Qt::AlignCenter);

    QLabel *lbl_sets= new QLabel(this);
    lbl_sets->setTextFormat(Qt::RichText);
    lbl_sets->setText("<font size=4> Noise settings </font>");
    lbl_sets->setAlignment(Qt::AlignCenter);

    layout->addWidget(lbl_sets,0,0,1,3);
    layout->addWidget(lbl_type,1,0,1,3);
    layout->addWidget(combo,2,0,1,3);
    layout->addWidget(edit_num,3,0,1,3);
    layout->addWidget(lbl_intensity,4,0,1,3);
    layout->addWidget(slideIntensity,5,0,1,3);
    layout->addWidget(lbl_int_red,6,0,1,1);
    layout->addWidget(lbl_int_green,6,1,1,1);
    layout->addWidget(lbl_int_blue,6,2,1,1);
    layout->addWidget(edit_int_red,7,0,1,1);
    layout->addWidget(edit_int_green,7,1,1,1);
    layout->addWidget(edit_int_blue,7,2,1,1);
    layout->addWidget(lbl_probability,8,0,1,3);
    layout->addWidget(slideProbability,9,0,1,3);
    layout->addWidget(lbl_pro_red,10,0,1,1);
    layout->addWidget(lbl_pro_green,10,1,1,1);
    layout->addWidget(lbl_pro_blue,10,2,1,1);
    layout->addWidget(edit_pro_red,11,0,1,1);
    layout->addWidget(edit_pro_green,11,1,1,1);
    layout->addWidget(edit_pro_blue,11,2,1,1);

    connect(slideIntensity,SIGNAL(valueChanged(int)),this,SLOT(on_slideIntensity_valueChanged(int)));
    connect(slideProbability,SIGNAL(valueChanged(int)),this,SLOT(on_slideProbability_valueChanged(int)));

    connect(combo,SIGNAL(currentIndexChanged(int)),this,SLOT(on_combo_IndexChanged(int)));
}

void noiseWidget::on_slideIntensity_valueChanged(int value)
{
    edit_int_red->setText(QString::number(value));
    edit_int_green->setText(QString::number(value));
    edit_int_blue->setText(QString::number(value));
    slideIntensity->setValue(value);
}
void noiseWidget::on_slideProbability_valueChanged(int value)
{
    edit_pro_red->setText(QString::number(value));
    edit_pro_green->setText(QString::number(value));
    edit_pro_blue->setText(QString::number(value));
    slideProbability->setValue(value);
}


noiseClass* noiseWidget::getNoise()
{
    noiseClass* noise= new noiseClass(combo->currentIndex(),edit_int_red->text().toInt(),edit_int_green->text().toInt(),edit_int_blue->text().toInt(),
                            edit_pro_red->text().toInt(),edit_pro_green->text().toInt(),edit_pro_blue->text().toInt(),edit_num->text().toInt());
    return noise;
}
void noiseWidget::hide(){
    this->setVisible(false);
}
void noiseWidget::on_combo_IndexChanged(int ind){
    if (ind==1) edit_num->setEnabled(true);
    else edit_num->setEnabled(false);
}

noiseWidget::~noiseWidget()
{
    
}
