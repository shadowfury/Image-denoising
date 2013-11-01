#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QDebug>
#include <QtCore>
#include <QImageReader>
#include <QImage>
#include <QPixmap>
#include <QVector>
#include <QTimer>
#include <QFuture>
#include <iostream>
#include <time.h>
#include <QCloseEvent>

int flag=0;


void MainWindow::closeEvent(QCloseEvent* ev)
{
   dw->setVisible(false);
   sw->setVisible(false);
   QWidget::closeEvent(ev);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // fftwnd_plan fftw2d_create_plan(int nx, int ny, fftw_direction dir, int flags);
    isPaused=false;
    isRendering=false;
    //ui->label->scroll(100,100);
    srand ( time(NULL) );
    qsrand(time(NULL));
    //ui->label_10->setAlignment(Qt::AlignCenter);
    ui->denoisedLabel->setAlignment(Qt::AlignCenter);
    ui->originalLabel->setAcceptDrops(true);

    ui->statusLabel->setAlignment(Qt::AlignCenter);

    ui->denoiseButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_MediaPlay));
    //qDebug()<<getProcMemory();
    /*QMessageBox::warning(this,"Warning","Total available memory ("+QString::number(getAvailableSystemMemory())+
                         " MB/"+QString::number(getTotalSystemMemory())+" MB ("+QString::number(getProcMemory())+"MB used by program)) ",QMessageBox::Ok);//*/



    Orig = new QImage(QSize(0,0), QImage::Format_ARGB32_Premultiplied);
    Orig->fill(Qt::transparent);
    Noise = new QImage(QSize(0,0), QImage::Format_ARGB32_Premultiplied);
    Noise->fill(Qt::transparent);
    Output = new QImage(QSize(0,0), QImage::Format_ARGB32_Premultiplied);
    Output->fill(Qt::transparent);
    Comparison = new QImage(QSize(0,0), QImage::Format_ARGB32_Premultiplied);
    Comparison->fill(Qt::transparent);

    //ui->originalLabel->setPixmap(QPixmap::fromImage(QImage(QSize(0,0), QImage::Format_ARGB32_Premultiplied)));

    denoiser=new QDenoiser();
    noiseSettings= new noiseClass();
    denoiseSettings= new denoiseClass();
    blurSettings=new blurClass();


    sw=new settingWidget();


    bw=new blurWidget();
    bw->on_slider_valueChanged(3);
    nw= new noiseWidget();
    nw->on_slideIntensity_valueChanged(30);
    nw->on_slideProbability_valueChanged(50);

    dw= new denoiseWidget();
    dw->initCombo(ui->menuBar->actions().at(0)->menu(),flag);

    sw->addWidget(nw);
    sw->addWidget(bw);

    ui->tabWidget->removeTab(4);
    ui->tabWidget->removeTab(4);
    ui->tabWidget->removeTab(4);
    ui->tabWidget->removeTab(4);


    connect(ui->menuBar->actions().at(0)->menu(),SIGNAL(triggered(QAction*)),this,SLOT(select_m(QAction*)));
    connect(dw->select,SIGNAL(clicked()),this,SLOT(denoise_select_m()));
    connect(dw->combo,SIGNAL(currentIndexChanged(int)),this,SLOT(activeChanged(int)));

    connect(ui->originalLabel,SIGNAL(clicked()),this,SLOT(on_openButton_clicked()));
    connect(ui->originalLabel,SIGNAL(dropped()),this,SLOT(image_dropped()));

}

MainWindow::~MainWindow()
{
    delete Output;
    delete Orig;
    delete Noise;
    delete Comparison;

    delete noiseSettings;
    delete denoiseSettings;
    sw->setVisible(false);
    dw->setVisible(false);
    sw->deleteLater();
    dw->deleteLater();
    if (denoiser->isRendering()) denoiser->cancelRender();
    denoiser->deleteLater();
    delete ui;

    destroy(true,true);
}

void MainWindow::setArgs(QStringList args){
    args.removeFirst();
    m_args=args;
}

QStringList MainWindow::getArgs(){
    return m_args;
}

void MainWindow::onEventLoopStarted(){
    /* process console input here */
    ConsoleFormatter console(30,55);
    console.addCommand("-h [--help]","Display this help message.");
    console.addCommand("-i [--input]","@param path to input image");
    console.addCommand("-o [--output]","@param path to where we should save output image");
    console.addCommand("-n [--noise]","include corruption of input image with noise");
    console.addCommand("-nm [--noise-method]","@param normal/uniform, noise distribution law: normal or uniform. Uniform is default value");
    console.addCommand("-ni [--noise-intensity]","@param noise intensity: -256 to 256 (for R G B components) , 30:30:30 is default value if noise is selected");
    console.addCommand("-np [--noise-probability]","@param noise probability: 0 to 100 (for R G B components) , 50:50:50 is default value if noise is selected");
    console.addCommand("-d [--denoise]","include denoising of noisy image");
    console.addCommand("-dm [--denoise-method]","@param select denoising method, NLM_fast is default if denoise is selected (simple_squares/NLM/NLM_fast/NLM_fast_FFT/NLM_multiThread)");
    console.addCommand("-dv [--denoise-values]","@param neighbour and search window sizes and denoising power, author of original method recommends to use 7:21 as for denoising power - increase it if filter does not consume noise (30 noise is roughly equal 120 power). These are default values if denoise is selected: 7:21:120)");

    //qDebug()<<getArgs();
    ConsoleParser parser(getArgs());
    parser.parse();

    // parser test
    /*qDebug()<<"Ok "<<parser.isOk();
    qDebug()<<"Help"<<parser.isHelpSelected();
    qDebug()<<"input "<<parser.inputPath();
    qDebug()<<"output "<<parser.outputPath();
    qDebug()<<"d "<<parser.isDenoisingSelected();
    qDebug()<<"dm "<<parser.denoiseMethod();
    qDebug()<<"dv "<<parser.denoiseValues();
    qDebug()<<"n "<<parser.isNoisingSelected();
    qDebug()<<"nm "<<parser.noiseMethod();
    qDebug()<<"ni "<<parser.noiseIntensity();
    qDebug()<<"np "<<parser.noiseProbability();//*/

    if (!parser.isOk()){
        console.printCommands();
        qApp->exit(1);
        return;
    }
    if (parser.isHelpSelected()){
        console.printCommands();
        qApp->exit(0);
        return;
    }
    // open image
    Orig->load(parser.inputPath());
    int n=Orig->height();
    int m=Orig->width();
    delete Output;
    delete Orig;
    Orig = new QImage(QSize(m,n), QImage::Format_ARGB32_Premultiplied);
    Orig->load(parser.inputPath());
    Output = new QImage(QSize(m,n), QImage::Format_ARGB32_Premultiplied);
    Output->fill(Qt::black);
    // noising
    if (parser.isNoisingSelected()){
        delete Noise;
        Noise = new QImage(QSize(m,n), QImage::Format_ARGB32_Premultiplied);
        Noise->fill(Qt::black);
        delete noiseSettings;
        QStringList n_in=parser.noiseIntensity().split(":");
        QStringList n_prob=parser.noiseIntensity().split(":");

        noiseSettings= new noiseClass(0,n_in[0].toInt(),n_in[1].toInt(),n_in[2].toInt(),n_prob[0].toInt(),n_prob[1].toInt(),n_prob[2].toInt());
        if (parser.noiseMethod()=="normal"){ noiseSettings->distribution=1; noiseSettings->edit_num=15;}
        if (noiseSettings->distribution==0) add_noise(Orig,Noise,noiseSettings);
        if (noiseSettings->distribution==1) add_noise_n(Orig,Noise,noiseSettings);
        if (!parser.isDenoisingSelected()) Noise->save(parser.outputPath(),0,-1);

    }
    //denoising
    if (parser.isDenoisingSelected()){
        ui->cancelButton->setText(parser.outputPath());

        std::cout<<"Press ESC if you want to cancel denoising"<<std::endl;
        //std::cout<<"Current denoising progress (0 to 10) :";
        QStringList d_values=parser.denoiseValues().split(":");
        delete denoiseSettings;
        denoiseSettings=new denoiseClass(parser.denoiseMethod(),d_values[0].toInt(),d_values[1].toInt(),d_values[2].toInt());
        denoiser->setSettings(denoiseSettings);
        denoiser->setDenoisingMethod(parser.denoiseMethod());

        if (parser.isNoisingSelected()) denoiser->setImage(Noise);
        else denoiser->setImage(Orig);

        denoiser->startRender();
        //QtConcurrent::run(denoiser,&QDenoiser::startRender);

        QTimer *timer=new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(consoleTimeout()),Qt::UniqueConnection);
        connect(denoiser,SIGNAL(finished()),this,SLOT(consoleTimeout()),Qt::UniqueConnection);
        connect(denoiser,SIGNAL(finished()),timer,SLOT(stop()),Qt::UniqueConnection);
        connect(denoiser,SIGNAL(finished()),timer,SLOT(deleteLater()),Qt::UniqueConnection);
        connect(denoiser,SIGNAL(errorString(QString)),this,SLOT(popMessageBox(QString)),Qt::UniqueConnection);
        timer->start(50);
        timer->setSingleShot(false);//*/

    }

    //qApp->exit();

}

void MainWindow::consoleTimeout(){
    /*qDebug()<<"timeout!!! "<<denoiser->getProgress();
    static int count=0;
    QString convert=QString::number(denoiser->getProgress());
    convert.chop(1);
    if (convert.toInt()>count){
        count++;
        std::cout<<"=";
    }//*/
    if (!denoiser->isRendering()){
        if (!Output->isNull()) delete Output;
        Output = new QImage(*denoiser->getImage());
        Output->save(ui->cancelButton->text(),0,-1);
        qApp->exit();
    }
}

void MainWindow::on_openButton_clicked()
{
    QList<QByteArray> list_formats=QImageReader::supportedImageFormats();
    QString formats;
    for (int i=0;i<list_formats.count();i++) formats.append(" *."+list_formats.at(i)+" ");
    //qDebug() << formats;

    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Image"), "", "Image files ("+formats+")");

    if (fileName!=NULL){
        ui->originalLabel->setImageSelected(true);
        Orig->load(fileName);
        /*Noise->load(fileName);
        Output->load(fileName);
        Comparison->load(fileName);*/

        int n=Orig->height();
        int m=Orig->width();
        delete Noise;
        delete Output;
        delete Comparison;
        delete Orig;
        Orig = new QImage(QSize(m,n), QImage::Format_ARGB32_Premultiplied);
        Orig->load(fileName);
        Noise = new QImage(QSize(m,n), QImage::Format_ARGB32_Premultiplied);
        Noise->fill(Qt::black);
        Output = new QImage(QSize(m,n), QImage::Format_ARGB32_Premultiplied);
        Output->fill(Qt::black);
        Comparison = new QImage(QSize(m,n), QImage::Format_ARGB32_Premultiplied);
        Comparison->fill(Qt::black);
        //qDebug()<<"toimg"<<m<<"  "<<n;
        ui->originalLabel->resize(m,n);
        ui->originalLabel->setPixmap(QPixmap::fromImage(*Orig));
        ui->originalLabel->setAlignment(Qt::AlignCenter);
        ui->originalLabel->repaint();
        list_formats.clear();
    }
}

void MainWindow::image_dropped(){
    if (ui->originalLabel->isImageSelected()){
        delete Orig;
        Orig=new QImage(ui->originalLabel->pixmap()->toImage());
        /*Noise->load(fileName);
        Output->load(fileName);
        Comparison->load(fileName);*/
        int n=Orig->height();
        int m=Orig->width();
        delete Noise;
        delete Output;
        delete Comparison;
        //delete Orig;
        Noise = new QImage(QSize(m,n), QImage::Format_ARGB32_Premultiplied);
        Noise = new QImage(QSize(m,n), QImage::Format_ARGB32_Premultiplied);
        Noise->fill(Qt::black);
        Output = new QImage(QSize(m,n), QImage::Format_ARGB32_Premultiplied);
        Output->fill(Qt::black);
        Comparison = new QImage(QSize(m,n), QImage::Format_ARGB32_Premultiplied);
        Comparison->fill(Qt::black);
        //qDebug()<<"toimg"<<m<<"  "<<n;
        ui->originalLabel->resize(m,n);
        ui->originalLabel->setPixmap(QPixmap::fromImage(*Orig));
        ui->originalLabel->setAlignment(Qt::AlignCenter);
        ui->originalLabel->repaint();
    }
}

void MainWindow::on_noiseDetailsButton_clicked()
{
    sw->move(QCursor::pos());
    if (!sw->isVisible()) sw->show();
    else sw->setVisible(false);
}

void MainWindow::on_denoiseDetailsButton_clicked(){
    dw->move(QCursor::pos());
    if (!dw->isVisible()) dw->show();
    else dw->setVisible(false);
}

//adds noise to image
void MainWindow::on_noiseButton_clicked()
{
    delete noiseSettings;
    noiseSettings=nw->getNoise();
    QElapsedTimer time;
    time.start();
    if (noiseSettings->distribution==0) add_noise(Orig,Noise,noiseSettings);
    if (noiseSettings->distribution==1) add_noise_n(Orig,Noise,noiseSettings);
    //diff=GetCounter();
    //QRgb p,q;
    if (!Output->isNull()) delete Output;
    Output= new QImage(*Noise);

    ui->noisedLabel->setAlignment(Qt::AlignCenter);
    ui->noisedLabel->setPixmap(QPixmap::fromImage(*Noise));
    ui->noisedLabel->repaint();
    ui->elapsedLabel->setText("Time elapsed:"+QString::number((double)time.elapsed()/1000)+" sec.");

}


void MainWindow::on_blurButton_clicked(){

    delete blurSettings;
    blurSettings=bw->getBlur();

    QElapsedTimer time;
    time.start();

    if (!Noise->isNull()) delete Noise;


    Noise=new QImage(blurred(*Orig, QRect(0,0,Orig->width(), Orig->height()), blurSettings->radius));

    if (!Output->isNull()) delete Output;
    Output= new QImage(*Noise);


    ui->noisedLabel->setAlignment(Qt::AlignCenter);
    ui->noisedLabel->setPixmap(QPixmap::fromImage(*Noise));
    ui->noisedLabel->repaint();
    ui->elapsedLabel->setText("Time elapsed:"+QString::number((double)time.elapsed()/1000)+" sec.");

}

//invokes removing noise from single image
void MainWindow::on_denoiseButton_clicked(){
    if (!isRendering){
        delete denoiseSettings;
        denoiseSettings=dw->getSettings();
        setWindowTitle("Denoising with "+ui->menuBar->actions().at(0)->menu()->actions().at(dw->curr_ind)->text()+".");
        //delete denoiser;
        //denoiser=new QDenoiser(Noise,denoiseSettings);
        denoiser->setSettings(denoiseSettings);
        denoiser->setImage(Noise);

        denoiser->startRender();
        //QtConcurrent::run(denoiser,&QDenoiser::startRender);
        QTimer *timer=new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(timeout_slot()),Qt::UniqueConnection);
        connect(denoiser,SIGNAL(finished()),this,SLOT(timeout_slot()),Qt::UniqueConnection);
        connect(denoiser,SIGNAL(finished()),timer,SLOT(stop()),Qt::UniqueConnection);
        connect(denoiser,SIGNAL(finished()),timer,SLOT(deleteLater()),Qt::UniqueConnection);
        connect(denoiser,SIGNAL(errorString(QString)),this,SLOT(popMessageBox(QString)),Qt::UniqueConnection);
        timer->start(50);
        timer->setSingleShot(false);

        isRendering=true;//
        ui->denoiseButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_MediaPause));

    }
    else{
        if (isPaused){
            isPaused=false;
            denoiser->resumeRender();
            ui->denoiseButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_MediaPause));
            return;
        }
        else{
            isPaused=true;
            denoiser->pauseRender();
            ui->denoiseButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_MediaPlay));
            return;
        }
    }




}
// Cancel button and timeout functions

void MainWindow::on_cancelButton_clicked()
{
    isRendering=false;
    isPaused=false;
    denoiser->cancelRender();
    ui->denoiseButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_MediaPlay));
    if (!Output->isNull()) delete Output;
    Output = new QImage(*denoiser->getImage());
    ui->denoisedLabel->setPixmap(QPixmap::fromImage(*Output)); // updating denoised image tab
    on_pushButton_6_clicked(); // updating comparison tab
    ui->elapsedLabel->setText("Time elapsed: "+QString::number(denoiser->getTimeSpent(),'f',3));

}

void MainWindow::timeout_slot(){
    if (!Output->isNull()) delete Output;
    Output = new QImage(*denoiser->getImage());
    if (ui->tabWidget->currentIndex()==2) ui->denoisedLabel->setPixmap(QPixmap::fromImage(*Output)); // updating denoised image tab
    if (ui->tabWidget->currentIndex()==3) on_pushButton_6_clicked(); // updating comparison tab

    ui->elapsedLabel->setText("Time elapsed: "+QString::number(denoiser->getTimeSpent(),'f',3));
    ui->progressBar->setValue(denoiser->getProgress());
    ui->statusLabel->setText(denoiser->getStatus());

    if (!denoiser->isRendering()){
        isRendering=false;
        isPaused=false;

        ui->denoiseButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_MediaPlay));
        if (!Output->isNull()) delete Output;
        Output = new QImage(*denoiser->getImage());
        ui->denoisedLabel->setPixmap(QPixmap::fromImage(*Output));
        on_pushButton_6_clicked(); // updating comparison tab
        ui->elapsedLabel->setText("Time elapsed: "+QString::number(denoiser->getTimeSpent(),'f',3));
    }
}

void MainWindow::popMessageBox(QString line){
    QMessageBox::warning(0,line.split("|||").at(0),line.split("|||").at(1),QMessageBox::Ok);
}

//saving files to images and csv
void MainWindow::on_pushButton_5_clicked()
{
    QString fileName=QFileDialog::getExistingDirectory ( 0, "Select directory to save files", "", QFileDialog::ShowDirsOnly );
    if (fileName!=NULL){
        Orig->save(fileName+"//original.jpg", 0,-1 );
        Noise->save(fileName+"//noise.jpg", 0,-1 );
        Output->save(fileName+"//output.jpg", 0,-1 );
        Comparison->save(fileName+"//compare.jpg",0,-1 );
    }
}
//computes error (standart deviation)
void MainWindow::on_pushButton_6_clicked()
{
    int n=Orig->height();
    int m=Orig->width();
    double error[3];
    error[0]=0;
    error[1]=0;
    error[2]=0;
    QColor z,ori,out;
    for (int i=0;i<m;i++){
        for (int j=0;j<n;j++){
            ori=Orig->pixel(i,j);
            out=Output->pixel(i,j);
            z.setRed(abs(ori.red()-out.red()));
            z.setBlue(abs(ori.blue()-out.blue()));
            z.setGreen(abs(ori.green()-out.green()));
            Comparison->setPixel(i,j,z.rgb());
            error[0]=+pow(z.red(),2);
            error[1]=+pow(z.green(),2);
            error[2]=+pow(z.blue(),2);
        }
    }
    error[0]=error[0]/(m*n);
    error[1]=error[1]/(m*n);
    error[2]=error[2]/(m*n);
    //ui->lineEdit_20->setText(QString::number(error[0],'g',4));
    //ui->lineEdit_19->setText(QString::number(error[1],'g',4));
    //ui->lineEdit_18->setText(QString::number(error[2],'g',4));

    ui->label_9->setAlignment(Qt::AlignCenter);
    ui->label_9->setPixmap(QPixmap::fromImage(*Comparison));
}


void MainWindow::select_m(QAction* senderAction)
{
    QString file=senderAction->text();
    for (int i=0;i<ui->menuItem->actions().count();i++){
            if (ui->menuItem->actions().at(i)->text()==file){
                flag=i;
            }
    }
    dw->setActiveIndex(flag);
    setWindowTitle("Denoising with "+file+".");
}

void MainWindow::denoise_select_m(){
    //select_m();
    flag=dw->curr_ind;
    setWindowTitle("Denoising with "+dw->combo->currentText()+".");
}

void MainWindow::activeChanged(int comboIndex){
    flag=comboIndex;
    setWindowTitle("Denoising with "+dw->combo->currentText()+".");
}
int MainWindow::noisePixelplus(int noiseVal,int pixVal,int prob)
{
    int rnd=rand()%100;
    if (rnd<prob) rnd=1;
    else return pixVal;

    if ((pixVal+noiseVal>255)&&rnd) return 255;
    else if ((pixVal+noiseVal<0)&&rnd) return 0;
    else if ((pixVal+noiseVal<255)&&(pixVal+noiseVal>=0)&&rnd){
            return pixVal+noiseVal*rnd;
    }
    return pixVal;
}
int MainWindow::noisePixelminus(int noiseVal,int pixVal,int prob)
{
    int rnd=rand()%100;
    if (rnd<prob) rnd=1;
    else return pixVal;

    if ((pixVal-noiseVal>255)&&rnd) return 255;
    else if ((pixVal-noiseVal<0)&&rnd) return 0;
    else if ((pixVal-noiseVal<255)&&(pixVal-noiseVal>=0)&&rnd){
            return pixVal-noiseVal*rnd;
    }
    return pixVal;
}
void MainWindow::add_noise(QImage *original,QImage *noised,noiseClass* noiseSettings){
    QColor p;
    int n=original->height();
    int m=original->width();
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            p = original->pixel(i,j);

            p.setRed(noisePixelplus(noiseSettings->int_red,p.red(),noiseSettings->pro_red));
            p.setRed(noisePixelminus(noiseSettings->int_red,p.red(),noiseSettings->pro_red));

            p.setGreen(noisePixelplus(noiseSettings->int_green,p.green(),noiseSettings->pro_green));
            p.setGreen(noisePixelminus(noiseSettings->int_green,p.green(),noiseSettings->pro_green));

            p.setBlue(noisePixelplus(noiseSettings->int_blue,p.blue(),noiseSettings->pro_blue));
            p.setBlue(noisePixelminus(noiseSettings->int_blue,p.blue(),noiseSettings->pro_blue));

            noised->setPixel(i,j,p.rgb());
        }
    }
}

float MainWindow::RandNorm( int num )
{
  float sum = 0.0f;
  for (int i = 0; i < num; ++i)
   sum += (float) qrand() / RAND_MAX;

  return sum / num;
}
void MainWindow::add_noise_n(QImage *original,QImage *noised,noiseClass* noiseSettings){
    QColor p;
    int n=original->height();
    int m=original->width();
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            p = original->pixel(i,j);

            p.setRed(noisePixelplus((int)(noiseSettings->int_red*RandNorm(noiseSettings->edit_num)),p.red(),noiseSettings->pro_red));
            p.setRed(noisePixelminus((int)(noiseSettings->int_red*RandNorm(noiseSettings->edit_num)),p.red(),noiseSettings->pro_red));

            p.setGreen(noisePixelplus((int)(noiseSettings->int_green*RandNorm(noiseSettings->edit_num)),p.green(),noiseSettings->pro_green));
            p.setGreen(noisePixelminus((int)(noiseSettings->int_green*RandNorm(noiseSettings->edit_num)),p.green(),noiseSettings->pro_green));

            p.setBlue(noisePixelplus((int)(noiseSettings->int_blue*RandNorm(noiseSettings->edit_num)),p.blue(),noiseSettings->pro_blue));
            p.setBlue(noisePixelminus((int)(noiseSettings->int_blue*RandNorm(noiseSettings->edit_num)),p.blue(),noiseSettings->pro_blue));

            noised->setPixel(i,j,p.rgb());
        }
    }//*/
}




QImage MainWindow::blurred(const QImage& image, const QRect& rect, int radius, bool alphaOnly)
{
    int tab[] = { 14, 10, 8, 6, 5, 5, 4, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2 };
    int alpha = (radius < 1) ? 16 : (radius > 17) ? 1 : tab[radius-1];

    QImage result = image.convertToFormat(QImage::Format_ARGB32_Premultiplied);
    int r1 = rect.top();
    int r2 = rect.bottom();
    int c1 = rect.left();
    int c2 = rect.right();

    int bpl = result.bytesPerLine();
    int rgba[4];
    unsigned char* p;

    int i1 = 0;
    int i2 = 3;

    if (alphaOnly)
    i1 = i2 = (QSysInfo::ByteOrder == QSysInfo::BigEndian ? 0 : 3);

    for (int col = c1; col <= c2; col++) {
        p = result.scanLine(r1) + col * 4;
        for (int i = i1; i <= i2; i++)
            rgba[i] = p[i] << 4;

        p += bpl;
        for (int j = r1; j < r2; j++, p += bpl)
            for (int i = i1; i <= i2; i++)
                p[i] = (rgba[i] += ((p[i] << 4) - rgba[i]) * alpha / 16) >> 4;
    }

    for (int row = r1; row <= r2; row++) {
        p = result.scanLine(row) + c1 * 4;
        for (int i = i1; i <= i2; i++)
            rgba[i] = p[i] << 4;

        p += 4;
        for (int j = c1; j < c2; j++, p += 4)
            for (int i = i1; i <= i2; i++)
                p[i] = (rgba[i] += ((p[i] << 4) - rgba[i]) * alpha / 16) >> 4;
    }

    for (int col = c1; col <= c2; col++) {
        p = result.scanLine(r2) + col * 4;
        for (int i = i1; i <= i2; i++)
            rgba[i] = p[i] << 4;

        p -= bpl;
    for (int j = r1; j < r2; j++, p -= bpl)
        for (int i = i1; i <= i2; i++)
            p[i] = (rgba[i] += ((p[i] << 4) - rgba[i]) * alpha / 16) >> 4;
    }

    for (int row = r1; row <= r2; row++) {
        p = result.scanLine(row) + c2 * 4;
        for (int i = i1; i <= i2; i++)
            rgba[i] = p[i] << 4;

        p -= 4;
        for (int j = c1; j < c2; j++, p -= 4)
            for (int i = i1; i <= i2; i++)
                p[i] = (rgba[i] += ((p[i] << 4) - rgba[i]) * alpha / 16) >> 4;
    }

    return result;
}
