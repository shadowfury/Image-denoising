#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include <QtCore>
#include <QImageReader>
#include <QImage>
#include <QPixmap>


#include "matrix.h"
#include <time.h>
QImage *Orig,*Noise,*Output,*Comparison,*Z,*X,*Y;
noiseClass* noiseSettings;
denoiseClass* denoiseSettings;

int m=0,n=0,number=0,z_ind=0;
int flag=0;
bool isPaused=false,isRendering=false;

float RandNorm( int num );
void image_to_float(QImage* im,float **mass,int color_n,int ord,int m1,int n1);

void rgbmy_to_float(rgb_my **im,float **mass,int color_n,int ord,int m1,int n1);
void pinv(rgb_my **in,rgb_my **out,int m1,int n1);

//lagrangelike

long double a_coef_upper(int k,int number,QImage* q,QImage* Y,int ind,int m1,int n1);
long double a_coef_lower(int k,int number,QImage* q,QImage* Y,int ind,int m1,int n1);


#ifdef Q_OS_WIN
#define _WIN32_WINNT  0x0501
#include <windows.h>
#include <winbase.h>

long getTotalSystemMemory()
{
    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);
    GlobalMemoryStatusEx(&status);
    return status.ullTotalPhys/(1024*1024);
}
long getAvailableSystemMemory()
{
    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);
    GlobalMemoryStatusEx(&status);
    return status.ullAvailPhys/(1024*1024);
}
long getProcMemory(){
    return 0;
}

#endif
#ifdef Q_OS_LINUX
#include <unistd.h>
long getProcMemory()
{
    QFile mem("/proc/self/status");
    mem.open(QIODevice::ReadOnly);
    mem.readLine();
    QTextStream stream(&mem);
    long ret=0;
    while(!stream.atEnd()){
        QString str=stream.readLine();
        //qDebug()<<str;
        if (str.startsWith("VmRSS:")){
            str=str.section(":",1,1);
            str.chop(3);
            str.remove(" ");
            ret+=str.toInt();
        }//*/
    }
    mem.close();
    return ret/1024;
}
long getAvailableSystemMemory()
{
    QFile mem("/proc/meminfo");
    mem.open(QIODevice::ReadOnly);
    mem.readLine();
    QTextStream stream(&mem);
    long ret=0;
    while(!stream.atEnd()){
        QString str=stream.readLine();
        //qDebug()<<str;
        if (str.startsWith("MemFree:")||str.startsWith("Cached:")){
            str=str.section(":",1,1);
            str.chop(3);
            str.remove(" ");
            ret+=str.toInt();
        }
    }
    mem.close();
    return ret/1024;
}
long getTotalSystemMemory()
{
    long pages = sysconf(_SC_PHYS_PAGES);//_SC_PHYS_PAGES);
    long page_size = sysconf(_SC_PAGE_SIZE);
    return pages * page_size/(1024*1024);
}
#endif


class Sleeper : public QThread {
public:
    static void msleep(unsigned long v) { QThread::msleep(v); }
};

void sleep(int ms) {
    Sleeper::msleep(ms);
}
void MainWindow::closeEvent(QCloseEvent* ev)
{
   nw->setVisible(false);
   dw->setVisible(false);
   QWidget::closeEvent(ev);
}



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    index=0;
    //ui->label->scroll(100,100);
    srand ( time(NULL) );
    qsrand(time(NULL));
    //ui->label_10->setAlignment(Qt::AlignCenter);
    ui->denoisedLabel->setAlignment(Qt::AlignCenter);

    ui->statusLabel->setAlignment(Qt::AlignCenter);
    ui->commandLinkButton_2->setIcon(QApplication::style()->standardIcon(QStyle::SP_CommandLink).pixmap(128, 128).transformed(QTransform().rotate(180,Qt::YAxis)));
    ui->commandLinkButton_4->setIcon(QApplication::style()->standardIcon(QStyle::SP_CommandLink).pixmap(128, 128).transformed(QTransform().rotate(180,Qt::YAxis)));
    ui->commandLinkButton_6->setIcon(QApplication::style()->standardIcon(QStyle::SP_CommandLink).pixmap(128, 128).transformed(QTransform().rotate(180,Qt::YAxis)));

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

    Z = new QImage(QSize(0,0), QImage::Format_ARGB32_Premultiplied);
    Z->fill(Qt::black);

    noiseSettings= new noiseClass();
    denoiseSettings= new denoiseClass();
    nw= new noiseWidget();
    dw= new denoiseWidget();
    dw->initCombo(ui->menuBar->actions().at(0)->menu(),flag);


    ui->tabWidget->removeTab(4);
    ui->tabWidget->removeTab(4);
    ui->tabWidget->removeTab(4);
    ui->tabWidget->removeTab(4);


    connect(ui->menuBar->actions().at(0)->menu(),SIGNAL(triggered(QAction*)),SLOT(select_m(QAction*)));
    connect(dw->select,SIGNAL(clicked()),this,SLOT(denoise_select_m()));
    connect(dw->combo,SIGNAL(currentIndexChanged(int)),this,SLOT(activeChanged(int)));


}

MainWindow::~MainWindow()
{
    //qDebug()<<"LOL destructor";
    delete Output;
    delete Orig;
    delete Noise;
    delete Comparison;
    delete Z;
    delete noiseSettings;
    nw->setVisible(false);
    dw->setVisible(false);
    dw->deleteLater();
    nw->deleteLater();
    delete ui;
    destroy(true,true);
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
        Orig->load(fileName);
        /*Noise->load(fileName);
        Output->load(fileName);
        Comparison->load(fileName);*/

        n=Orig->height();
        m=Orig->width();
        delete Noise;
        delete Output;
        delete Comparison;
        delete Orig;
        Noise = new QImage(QSize(m,n), QImage::Format_ARGB32_Premultiplied);
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
    }
}


void MainWindow::on_noiseDetailsButton_clicked()
{
    nw->move(QCursor::pos());
    if (!nw->isVisible()) nw->show();
    else nw->setVisible(false);
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
//invokes removing noise from single image
void MainWindow::on_denoiseButton_clicked(){

    if (!isRendering){
        delete denoiseSettings;
        denoiseSettings=dw->getSettings();
        flag=dw->curr_ind;
        this->setWindowTitle("Denoising with "+ui->menuBar->actions().at(0)->menu()->actions().at(flag)->text()+".");
    }
    if (isRendering){
        if (isPaused){
            isPaused=false;
            ui->denoiseButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_MediaPause));
            return;
        }
        else{
            isPaused=true;
            ui->denoiseButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_MediaPlay));
            return;
        }
    }

    if (!Output->isNull()) delete Output;
    Output = new QImage(*Noise);
    ui->denoisedLabel->setPixmap(QPixmap::fromImage(*Output));

    if (flag==0){
        ui->progressBar->setValue(0);
        int size=denoiseSettings->patch_size;
        QtConcurrent::run(this,&MainWindow::simple_squares_method,size);
    }
    if (flag==1){
        ui->progressBar->setValue(0);
        int size_m=denoiseSettings->patch_size;
        int size_b=denoiseSettings->search_window;
        int h=denoiseSettings->pow;
        QtConcurrent::run(this,&MainWindow::non_local_means_method,size_m,size_b,h);
    }
    if (flag==2){
        ui->progressBar->setValue(0);
        int size_m=denoiseSettings->patch_size;
        int size_b=denoiseSettings->search_window;
        int h=denoiseSettings->pow;
        QtConcurrent::run(this,&MainWindow::non_local_means_method_fast,size_m,size_b,h);
    }
    if (flag==3){
        ui->progressBar->setValue(0);
        int size_m=denoiseSettings->patch_size;
        int size_b=denoiseSettings->search_window;
        int h=denoiseSettings->pow;
        QtConcurrent::run(this,&MainWindow::Nlm_fast_FFT,size_m,size_b,h);
    }
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
            error[0]=pow(ori.red()-out.red(),2);
            error[1]=pow(ori.green()-out.green(),2);
            error[2]=pow(ori.blue()-out.blue(),2);
        }
    }
    error[0]=error[0]/(m*n);
    error[1]=error[1]/(m*n);
    error[2]=error[2]/(m*n);
    ui->lineEdit_20->setText(QString::number(error[0],'g',4));
    ui->lineEdit_19->setText(QString::number(error[1],'g',4));
    ui->lineEdit_18->setText(QString::number(error[2],'g',4));

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
    if ((flag!=4)&&(flag!=5)){
        dw->setActiveIndex(flag);
        setWindowTitle("Denoising with "+file+".");
    }
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
    else rnd=0;

    if ((pixVal+noiseVal>255)&&rnd) return 255;
    else if ((pixVal+noiseVal<0)&&rnd) return 0;
    else if ((pixVal+noiseVal<255)&&(pixVal+noiseVal>0)&&rnd){
            return pixVal+noiseVal*rnd;
    }
    return pixVal;
}
int MainWindow::noisePixelminus(int noiseVal,int pixVal,int prob)
{
    int rnd=rand()%100;
    if (rnd<prob) rnd=1;
    else rnd=0;

    if ((pixVal-noiseVal>255)&&rnd) return 255;
    else if ((pixVal-noiseVal<0)&&rnd) return 0;
    else if ((pixVal-noiseVal<255)&&(pixVal-noiseVal>0)&&rnd){
            return pixVal-noiseVal*rnd;
    }
    return pixVal;
}
void MainWindow::add_noise(QImage *original,QImage *noised,noiseClass* noiseSettings){
    QColor p;
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            //QColor(p)
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

float RandNorm( int num )
{
  float sum = 0.0f;
  for (int i = 0; i < num; ++i)
   sum += (float) qrand() / RAND_MAX;

  return sum / num;
}
void MainWindow::add_noise_n(QImage *original,QImage *noised,noiseClass* noiseSettings){
    QColor p;
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

void MainWindow::on_cancelButton_clicked()
{
    isRendering=false;
}

void MainWindow::updateStatus(QString status)
{
    ui->statusLabel->setText(status);
}
void MainWindow::finished(double diff)
{
    isRendering=false;
    ui->denoiseButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_MediaPlay));
    ui->denoisedLabel->setPixmap(QPixmap::fromImage(*Output));
    ui->denoisedLabel->repaint();
    ui->elapsedLabel->setText("Time elapsed:"+QString::number(diff)+" sec.");
}
void MainWindow::updateProgress(int progress)
{
    ui->progressBar->setValue(progress);
}
void MainWindow::updatePixel()
{

    if (ui->tabWidget->currentIndex()==2){

        ui->denoisedLabel->setPixmap(QPixmap::fromImage(*Output));
    }
}
void MainWindow::popMessageBox(int m1,int n1,int size_b)
{
    QMessageBox::warning(this,"Warning, can not allocate memory.","Total available memory ("+QString::number(getAvailableSystemMemory()+getProcMemory())+
                         " MB/"+QString::number(getTotalSystemMemory())+" MB) is less then reqired for denoising this image ("
                         +QString::number((sizeof(rgb_my)*m1*n1*size_b*size_b)/(1024*1024))+" MB)",QMessageBox::Ok);
}
void MainWindow::iconPause(){
    ui->denoiseButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_MediaPause));
}



void MainWindow::simple_squares_method(int size){
    isRendering=true;
    QElapsedTimer time,update;
    double diff=0,paused=0;
    time.start();
    update.start();
    QMetaObject::invokeMethod(this,"updateStatus", Q_ARG(QString, "Status: computing."));
    QMetaObject::invokeMethod(this,"iconPause");
    int m_size=(size-1)/2;
    QColor q;
    int c,z_red,z_blue,z_green;
    for (int y=0;y<n;y++){
        QMetaObject::invokeMethod(this,"updateProgress", Q_ARG(int, (y*100)/(n-1)));
        //ui->progressBar->setValue((x*100)/(m-1));
        for (int x=0;x<m;x++){
            if (isPaused){
                QElapsedTimer pauseTime;
                pauseTime.start();
                while (isPaused){
                    if (!isRendering) isPaused=false;
                    sleep(1);
                }
                paused+=(double)pauseTime.elapsed()/1000;
            }
            if (!isRendering){
                diff=(double)time.elapsed()/1000;
                QMetaObject::invokeMethod(this, "finished",Q_ARG(double,diff-paused));
                return;
            }
            z_red=0;
            z_blue=0;
            z_green=0;
            c=0;
            for (int i=0;i<size;i++){
                for (int j=0;j<size;j++){
                    if ((x-m_size+i>=0)&&(x-m_size+i<m)&&(y-m_size+j>=0)&&(y-m_size+j<n)){
                        q=Noise->pixel(x-m_size+i,y-m_size+j);
                        z_red+=q.red();
                        z_blue+=q.blue();
                        z_green+=q.green();
                        c++;
                    }
                }
            }
            q.setRed(z_red/c);
            q.setBlue(z_blue/c);
            q.setGreen(z_green/c);
            Output->setPixel(x,y,q.rgb());
            if (update.elapsed()>30){

                update.restart();
                QMetaObject::invokeMethod(this,"updatePixel");
            }
        }
    }
    diff=(double)time.elapsed()/1000;
    QMetaObject::invokeMethod(this,"updateStatus", Q_ARG(QString, "Status: done."));
    QMetaObject::invokeMethod(this, "finished",Q_ARG(double,diff-paused));

}

void MainWindow::non_local_means_method(int size_m,int size_b,int h)
{
    isRendering=true;
    QElapsedTimer time,update;
    double diff=0,paused=0;
    time.start();
    update.start();
    QMetaObject::invokeMethod(this,"updateStatus", Q_ARG(QString, "Status: computing."));
    QMetaObject::invokeMethod(this,"iconPause");

    int m_size=(size_m-1)/2;
    int m_size_b=(size_b-1)/2;
    //int c=0;
    QColor p,q,fin,out,noize;
    rgb_my Weight[size_b][size_b],z;
    for (int y=0;y<n;y++){
        QMetaObject::invokeMethod(this,"updateProgress", Q_ARG(int, (y*100)/(n-1)));        
        for (int x=0;x<m;x++){
            if (isPaused){
                QElapsedTimer pauseTime;
                pauseTime.start();
                while (isPaused){
                    if (!isRendering) isPaused=false;
                    sleep(1);
                }
                paused+=(double)pauseTime.elapsed()/1000;
            }
            if (!isRendering){
                diff=(double)time.elapsed()/1000;
                QMetaObject::invokeMethod(this, "finished",Q_ARG(double,diff-paused));
                return;
            }
            z.setBlue(0);
            z.setGreen(0);
            z.setRed(0);
            for (int i=0;i<size_b;i++){
                for(int j=0;j<size_b;j++){
                    Weight[i][j].setRed(0);
                    Weight[i][j].setBlue(0);
                    Weight[i][j].setGreen(0);
                    for(int a=0;a<size_m;a++){
                        for(int b=0;b<size_m;b++){
                            //if((x-m_size+a<m)&&(x-m_size+a>=0)&&(y-m_size+b<n)&&(y-m_size+b>-0)){
                                //if ((x-m_size_b-m_size+a+i<m)&&(x-m_size_b-m_size+a+i>=0)&&(y-m_size_b-m_size+b+j<n)&&(y-m_size_b-m_size+b+j>=0)){
                            int x_m=0,y_m=0,x_f=0,y_f=0,x_ff=0,y_ff=0;

                            if ((x-m_size+a>=m)||(x-m_size+a<0)) x_m=x+m_size-a;
                            else x_m=x-m_size+a;
                            if ((y-m_size+b>=n)||(y-m_size+b<0)) y_m=y+m_size-b;
                            else y_m=y-m_size+b;
                            p=Noise->pixel(x_m,y_m);

                            if ((x-m_size_b+i>=m)||(x-m_size_b+i<0)) x_f=x+m_size_b-i;
                            else x_f=x-m_size_b+i;
                            if ((y-m_size_b+j>=n)||(y-m_size_b+j<0)) y_f=y+m_size_b-j;
                            else y_f=y-m_size_b+j;

                            if ((x_f-m_size+a>=m)||(x_f-m_size+a<0)) x_ff=x_f+m_size-a;
                            else x_ff=x_f-m_size+a;
                            if ((y_f-m_size+b>=n)||(y_f-m_size+b<0)) y_ff=y_f+m_size-b;
                            else y_ff=y_f-m_size+b;

                            q=Noise->pixel(x_ff,y_ff);//*/
                            Weight[i][j].setRed(Weight[i][j].red()+pow(p.red()-q.red(),2));
                            Weight[i][j].setBlue(Weight[i][j].blue()+pow(p.blue()-q.blue(),2));
                            Weight[i][j].setGreen(Weight[i][j].green()+pow(p.green()-q.green(),2));
                        }
                    }
                    Weight[i][j].setRed(exp(-Weight[i][j].red()/pow(h,2)));
                    Weight[i][j].setBlue(exp(-Weight[i][j].blue()/pow(h,2)));
                    Weight[i][j].setGreen(exp(-Weight[i][j].green()/pow(h,2)));

                    z.setRed(z.red()+Weight[i][j].red());
                    //printf("iteration %d after, z.red=%f\n",c,z.red);
                    z.setBlue(z.blue()+Weight[i][j].blue());
                    z.setGreen(z.green()+Weight[i][j].green());


                }
            }            
            fin.setRed(0);
            fin.setBlue(0);
            fin.setGreen(0);
            Output->setPixel(x,y,fin.rgb());

            for (int ii=0;ii<size_b;ii++){
                for (int jj=0;jj<size_b;jj++){
                    //if ((x+ii-m_size_b<m)&&(x+ii-m_size_b>=0)&&(y+jj-m_size_b<n)&&(y+jj-m_size_b>=0)){
                        if ((x+ii-m_size_b>=m)||(x+ii-m_size_b<0)) if (y+jj-m_size_b<n) if (y+jj-m_size_b>=0)
                            noize=Noise->pixel(x-ii+m_size_b,y+jj-m_size_b);
                        if (x+ii-m_size_b<m) if (x+ii-m_size_b>=0) if ((y+jj-m_size_b>=n)||(y+jj-m_size_b<=0))
                            noize=Noise->pixel(x+ii-m_size_b,y-jj+m_size_b);
                        if (((x+ii-m_size_b>=m)||(x+ii-m_size_b<0))&&((y+jj-m_size_b>=n)||(y+jj-m_size_b<0)))
                            noize=Noise->pixel(x-ii+m_size_b,y-jj+m_size_b);
                        if (x+ii-m_size_b<m) if (x+ii-m_size_b>=0) if (y+jj-m_size_b<n) if (y+jj-m_size_b>=0)
                            noize=Noise->pixel(x+ii-m_size_b,y+jj-m_size_b);
                        Weight[ii][jj].setRed(Weight[ii][jj].red()*(noize.red())/z.red());
                        Weight[ii][jj].setBlue(Weight[ii][jj].blue()*(noize.blue())/z.blue());
                        Weight[ii][jj].setGreen(Weight[ii][jj].green()*(noize.green())/z.green());
                        fin.setRed(qRound(Weight[ii][jj].red()));
                        fin.setBlue(qRound(Weight[ii][jj].blue()));
                        fin.setGreen(qRound(Weight[ii][jj].green()));
                        out=Output->pixel(x,y);
                        out.setRed(out.red()+fin.red());
                        out.setBlue(out.blue()+fin.blue());
                        out.setGreen(out.green()+fin.green());
                        Output->setPixel(x,y,out.rgb());
                    //}

                }
            }
            if (update.elapsed()>30){

                update.restart();
                QMetaObject::invokeMethod(this,"updatePixel");
            }

        }

    }
    diff=(double)time.elapsed()/1000;
    QMetaObject::invokeMethod(this,"updateStatus", Q_ARG(QString, "Status: done."));
    QMetaObject::invokeMethod(this, "finished",Q_ARG(double,diff-paused));

/*
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  */
}
void MainWindow::non_local_means_method_fast(int size_m,int size_b,int h)
{
    isRendering=true;
    int m_size=(size_m-1)/2;
    int m_size_b=(size_b-1)/2;
    //brainblowing creating 4d dynamic array.
    //rgb_my* Sdx=new rgb_my[size_b*size_b*m*n];
    if ((getAvailableSystemMemory()+getProcMemory())<((long)sizeof(rgb_my)*m*n*size_b*size_b)/(1024*1024)){
        QMetaObject::invokeMethod(this,"popMessageBox", Q_ARG(int,m), Q_ARG(int,n), Q_ARG(int,size_b));
        return;
    }
    QMetaObject::invokeMethod(this,"iconPause");

    QElapsedTimer time,update;
    double diff=0,paused=0;
    time.start();
    update.start();
    QMetaObject::invokeMethod(this,"updateStatus", Q_ARG(QString, "Status: creating 4d-array."));
    QMetaObject::invokeMethod(this,"updateProgress", Q_ARG(int, 0));



    rgb_my**** Sdx=new rgb_my***[m];
    for (int i=0;i<m;i++){
        QMetaObject::invokeMethod(this,"updateProgress", Q_ARG(int, (i*100)/(m-1)));
        Sdx[i]=new rgb_my**[n];
        for (int j=0;j<n;j++){
            Sdx[i][j]=new rgb_my *[size_b];
            for (int k=0;k<size_b;k++){
                Sdx[i][j][k]=new rgb_my[size_b];
            }
        }
    }//*/


    QColor p,q;

    QMetaObject::invokeMethod(this,"updateStatus", Q_ARG(QString, "Status: computing 4d-array."));
    int t1,t2;
    for (int i=0;i<m;i++){
        QMetaObject::invokeMethod(this,"updateProgress", Q_ARG(int, (i*100)/(m-1)));
        for (int j=0;j<n;j++){
            for (int k=0;k<size_b;k++){
                for (int l=0;l<size_b;l++){
                    t1=i+k-m_size_b;
                    t2=j+l-m_size_b;
                    if ((t1<0)||(t1>=m)) t1=i-k+m_size_b;
                    if ((t2<0)||(t2>=n)) t2=j-l+m_size_b;
                    if ((i==0)&&(j==0)){
                            p=Noise->pixel(i,j);
                            q=Noise->pixel(t1,t2);
                            Sdx[i][j][k][l].setRed(pow(p.red()-q.red(),2));
                            Sdx[i][j][k][l].setGreen(pow(p.green()-q.green(),2));
                            Sdx[i][j][k][l].setBlue(pow(p.blue()-q.blue(),2));

                    }
                    if ((i==0)&&(j!=0)){
                            p=Noise->pixel(i,j);
                            q=Noise->pixel(t1,t2);
                            Sdx[i][j][k][l].setRed(Sdx[i][j-1][k][l].red()+pow(p.red()-q.red(),2));
                            Sdx[i][j][k][l].setGreen(Sdx[i][j-1][k][l].green()+pow(p.green()-q.green(),2));
                            Sdx[i][j][k][l].setBlue(Sdx[i][j-1][k][l].blue()+pow(p.blue()-q.blue(),2));

                    }
                    if ((j==0)&&(i!=0)){
                            p=Noise->pixel(i,j);
                            q=Noise->pixel(t1,t2);
                            Sdx[i][j][k][l].setRed(Sdx[i-1][j][k][l].red()+pow(p.red()-q.red(),2));
                            Sdx[i][j][k][l].setGreen(Sdx[i-1][j][k][l].green()+pow(p.green()-q.green(),2));
                            Sdx[i][j][k][l].setBlue(Sdx[i-1][j][k][l].blue()+pow(p.blue()-q.blue(),2));

                    }
                    if ((j>0)&&(i>0)){
                            p=Noise->pixel(i,j);
                            q=Noise->pixel(t1,t2);
                            Sdx[i][j][k][l].setRed(-Sdx[i-1][j-1][k][l].red()+Sdx[i-1][j][k][l].red()+Sdx[i][j-1][k][l].red()+pow(p.red()-q.red(),2));
                            Sdx[i][j][k][l].setGreen(-Sdx[i-1][j-1][k][l].green()+Sdx[i-1][j][k][l].green()+Sdx[i][j-1][k][l].green()+pow(p.green()-q.green(),2));
                            Sdx[i][j][k][l].setBlue(-Sdx[i-1][j-1][k][l].blue()+Sdx[i-1][j][k][l].blue()+Sdx[i][j-1][k][l].blue()+pow(p.blue()-q.blue(),2));


                    }
                }
            }
        }
    }

    QMetaObject::invokeMethod(this,"updateStatus", Q_ARG(QString, "Status: denoising image."));
    //int c=0;
    int x_p,x_m,y_p,y_m;
    QColor fin,out,noize;
    rgb_my Weight[size_b][size_b],z;

    for (int y=0;y<n;y++){
        QMetaObject::invokeMethod(this,"updateProgress", Q_ARG(int, (y*100)/(n-1)));
        for (int x=0;x<m;x++){
            if (isPaused){
                QElapsedTimer pauseTime;
                pauseTime.start();
                while (isPaused){
                    if (!isRendering) isPaused=false;
                    sleep(1);
                }
                paused+=(double)pauseTime.elapsed()/1000;
            }
            if (!isRendering){
                diff=(double)time.elapsed()/1000;
                QMetaObject::invokeMethod(this, "finished",Q_ARG(double,diff-paused));
                return;
            }
            //перед кожною ітерацією обнулення Z
            z.setBlue(0);
            z.setGreen(0);
            z.setRed(0);
            for (int i=0;i<size_b;i++){
                for(int j=0;j<size_b;j++){                    
                    /*x_p=x+m_size-m_size_b;
                    x_m=x-m_size-m_size_b;
                    y_p=y+m_size-m_size_b;
                    y_m=y-m_size-m_size_b;//*/
                    x_p=x+m_size;
                    x_m=x-m_size;
                    y_p=y+m_size;
                    y_m=y-m_size;//*/
                    if (x_p>=m) x_p=x-1;
                    if (x_m<0) x_m=x+1;
                    if (y_p>=n) y_p=y-1;
                    if (y_m<0) y_m=y+1;

                    Weight[i][j].setRed(Sdx[x_p][y_p][i][j].red()+Sdx[x_m][y_m][i][j].red()-Sdx[x_m][y_p][i][j].red()-Sdx[x_p][y_m][i][j].red());
                    Weight[i][j].setBlue(Sdx[x_p][y_p][i][j].blue()+Sdx[x_m][y_m][i][j].blue()-Sdx[x_m][y_p][i][j].blue()-Sdx[x_p][y_m][i][j].blue());
                    Weight[i][j].setGreen(Sdx[x_p][y_p][i][j].green()+Sdx[x_m][y_m][i][j].green()-Sdx[x_m][y_p][i][j].green()-Sdx[x_p][y_m][i][j].green());

                    Weight[i][j].setRed(exp(-Weight[i][j].red()/pow(h,2)));
                    Weight[i][j].setBlue(exp(-Weight[i][j].blue()/pow(h,2)));
                    Weight[i][j].setGreen(exp(-Weight[i][j].green()/pow(h,2)));

                    z.setRed(z.red()+Weight[i][j].red());
                    z.setBlue(z.blue()+Weight[i][j].blue());
                    z.setGreen(z.green()+Weight[i][j].green());
                }
            }

            fin.setRed(0);
            fin.setBlue(0);
            fin.setGreen(0);
            Output->setPixel(x,y,fin.rgb());

            for (int ii=0;ii<size_b;ii++){
                for (int jj=0;jj<size_b;jj++){
                    //if ((x+ii-m_size_b<m)&&(x+ii-m_size_b>=0)&&(y+jj-m_size_b<n)&&(y+jj-m_size_b>=0)){
                        if ((x+ii-m_size_b>=m)||(x+ii-m_size_b<0)) if (y+jj-m_size_b<n) if (y+jj-m_size_b>=0)
                            noize=Noise->pixel(x-ii+m_size_b,y+jj-m_size_b);
                        if (x+ii-m_size_b<m) if (x+ii-m_size_b>=0) if ((y+jj-m_size_b>=n)||(y+jj-m_size_b<=0))
                            noize=Noise->pixel(x+ii-m_size_b,y-jj+m_size_b);
                        if (((x+ii-m_size_b>=m)||(x+ii-m_size_b<0))&&((y+jj-m_size_b>=n)||(y+jj-m_size_b<0)))
                            noize=Noise->pixel(x-ii+m_size_b,y-jj+m_size_b);
                        if (x+ii-m_size_b<m) if (x+ii-m_size_b>=0) if (y+jj-m_size_b<n) if (y+jj-m_size_b>=0)
                            noize=Noise->pixel(x+ii-m_size_b,y+jj-m_size_b);
                        Weight[ii][jj].setRed(Weight[ii][jj].red()*(noize.red())/z.red());
                        Weight[ii][jj].setBlue(Weight[ii][jj].blue()*(noize.blue())/z.blue());
                        Weight[ii][jj].setGreen(Weight[ii][jj].green()*(noize.green())/z.green());
                        fin.setRed(qRound(Weight[ii][jj].red()));
                        fin.setBlue(qRound(Weight[ii][jj].blue()));
                        fin.setGreen(qRound(Weight[ii][jj].green()));
                        out=Output->pixel(x,y);
                        out.setRed(out.red()+fin.red());
                        out.setBlue(out.blue()+fin.blue());
                        out.setGreen(out.green()+fin.green());
                        Output->setPixel(x,y,out.rgb());
                    //}

                }
            }
            if (update.elapsed()>30){
                update.restart();
                QMetaObject::invokeMethod(this,"updatePixel");
            }
        }

    }

    QMetaObject::invokeMethod(this,"updateStatus", Q_ARG(QString, "Status: deleting 4d-array."));

    for (int i=0;i<m;i++){
        QMetaObject::invokeMethod(this,"updateProgress", Q_ARG(int, (i*100)/(m-1)));
        for (int j=0;j<n;j++){
            for (int k=0;k<size_b;k++){
                delete[] Sdx[i][j][k];
            }
            delete[] Sdx[i][j];
        }
        delete[] Sdx[i];

    }
    delete[] Sdx;//*/

    diff=(double)time.elapsed()/1000;
    QMetaObject::invokeMethod(this,"updateStatus", Q_ARG(QString, "Status: done."));
    QMetaObject::invokeMethod(this, "finished",Q_ARG(double,diff-paused));

}


void MainWindow::Nlm_fast_FFT(int size_m,int size_b,int h)
{
    isRendering=true;
    int m_size=(size_m-1)/2;
    int m_size_b=(size_b-1)/2;
    //brainblowing creating 4d dynamic array.
    //rgb_my* Sdx=new rgb_my[size_b*size_b*m*n];

    QMetaObject::invokeMethod(this,"iconPause");
    QElapsedTimer time,update;
    double diff=0,paused=0;
    time.start();
    update.start();
    QMetaObject::invokeMethod(this,"updateStatus", Q_ARG(QString, "Status: creating 4d-array."));
    QMetaObject::invokeMethod(this,"updateProgress", Q_ARG(int, 0));



    rgb_my** Sdx=new rgb_my*[m];
    for (int i=0;i<m;i++){
        QMetaObject::invokeMethod(this,"updateProgress", Q_ARG(int, (i*100)/(m-1)));
        Sdx[i]=new rgb_my[n];
    }//*/


    QColor p;

    QMetaObject::invokeMethod(this,"updateStatus", Q_ARG(QString, "Status: computing 2d-array."));
    for (int i=0;i<m;i++){
        QMetaObject::invokeMethod(this,"updateProgress", Q_ARG(int, (i*100)/(m-1)));
        for (int j=0;j<n;j++){
            p=Noise->pixel(i,j);
            if ((i==0)&&(j==0)){
                Sdx[i][j].setRed(pow(p.red(),2));
                Sdx[i][j].setGreen(pow(p.green(),2));
                Sdx[i][j].setBlue(pow(p.blue(),2));
            }
            if ((i==0)&&(j!=0)){
                    Sdx[i][j].setRed(Sdx[i][j-1].red()+pow(p.red(),2));
                    Sdx[i][j].setGreen(Sdx[i][j-1].green()+pow(p.green(),2));
                    Sdx[i][j].setBlue(Sdx[i][j-1].blue()+pow(p.blue(),2));

            }
            if ((j==0)&&(i!=0)){
                    Sdx[i][j].setRed(Sdx[i-1][j].red()+pow(p.red(),2));
                    Sdx[i][j].setGreen(Sdx[i-1][j].green()+pow(p.green(),2));
                    Sdx[i][j].setBlue(Sdx[i-1][j].blue()+pow(p.blue(),2));

            }
            if ((j>0)&&(i>0)){
                    Sdx[i][j].setRed(-Sdx[i-1][j-1].red()+Sdx[i-1][j].red()+Sdx[i][j-1].red()+pow(p.red(),2));
                    Sdx[i][j].setGreen(-Sdx[i-1][j-1].green()+Sdx[i-1][j].green()+Sdx[i][j-1].green()+pow(p.green(),2));
                    Sdx[i][j].setBlue(-Sdx[i-1][j-1].blue()+Sdx[i-1][j].blue()+Sdx[i][j-1].blue()+pow(p.blue(),2));


            }
        }
    }

    QMetaObject::invokeMethod(this,"updateStatus", Q_ARG(QString, "Status: denoising image."));
    //int c=0;
    int x_p=0,x_m=0,y_p=0,y_m=0,diff_x=0,diff_y=0;
    rgb_my square_s,square_n;
    QColor fin,out,noize;
    rgb_my Weight[size_b][size_b],z;

    for (int y=0;y<n;y++){
        QMetaObject::invokeMethod(this,"updateProgress", Q_ARG(int, (y*100)/(n-1)));
        for (int x=0;x<m;x++){
            if (isPaused){
                QElapsedTimer pauseTime;
                pauseTime.start();
                while (isPaused){
                    if (!isRendering) isPaused=false;
                    sleep(1);
                }
                paused+=(double)pauseTime.elapsed()/1000;
            }
            if (!isRendering){
                diff=(double)time.elapsed()/1000;
                QMetaObject::invokeMethod(this, "finished",Q_ARG(double,diff-paused));
                return;
            }
            //перед кожною ітерацією обнулення Z
            z.setBlue(0);
            z.setGreen(0);
            z.setRed(0);
            for (int i=0;i<size_b;i++){
                for(int j=0;j<size_b;j++){
                    x_p=x+m_size;
                    x_m=x-m_size;
                    y_p=y+m_size;
                    y_m=y-m_size;
                    if (x_p>=m) x_p=x-1;
                    if (x_m<0) x_m=x+1;
                    if (y_p>=n) y_p=y-1;
                    if (y_m<0) y_m=y+1;

                    /*x_p=x+m_size-m_size_b+i;
                    x_m=x-m_size-m_size_b+i;
                    y_p=y+m_size-m_size_b+j;
                    y_m=y-m_size-m_size_b+j;
                    if (x_p>=m) x_p=x+m_size+m_size_b-i;
                    if (x_m<0) x_m=x-m_size+m_size_b-i;
                    if (y_p>=n) y_p=y+m_size+m_size_b-j;
                    if (y_m<0) y_m=y-m_size+m_size_b-j;*/
                    //qDebug()<<x_p<<x_m<<y_p<<y_m;

                    square_s.setRed(Sdx[x_p][y_p].red()+Sdx[x_m][y_m].red()-Sdx[x_m][y_p].red()-Sdx[x_p][y_m].red());
                    square_s.setBlue(Sdx[x_p][y_p].blue()+Sdx[x_m][y_m].blue()-Sdx[x_m][y_p].blue()-Sdx[x_p][y_m].blue());
                    square_s.setGreen(Sdx[x_p][y_p].green()+Sdx[x_m][y_m].green()-Sdx[x_m][y_p].green()-Sdx[x_p][y_m].green());


                    diff_x=x-m_size_b+i;
                    if((diff_x>=m)||(diff_x<0)) diff_x=x+m_size_b-i;
                    diff_y=y-m_size_b+j;
                    if ((diff_y>=n)||(diff_y<0)) diff_y=y+m_size_b-j;


                    if ((diff_x+m_size>=m)) x_p=diff_x-1;
                    else x_p=diff_x+m_size;
                    if ((diff_x-m_size<0)) x_m=diff_x+1;
                    else x_m=diff_x-m_size;
                    if ((diff_y+m_size>=n)) y_p=diff_y-1;
                    else y_p=diff_y+m_size;
                    if ((diff_y-m_size<0)) y_m=diff_y+1;
                    else y_m=diff_y-m_size;


                    square_n.setRed(Sdx[x_p][y_p].red()+Sdx[x_m][y_m].red()-Sdx[x_m][y_p].red()-Sdx[x_p][y_m].red());
                    square_n.setBlue(Sdx[x_p][y_p].blue()+Sdx[x_m][y_m].blue()-Sdx[x_m][y_p].blue()-Sdx[x_p][y_m].blue());
                    square_n.setGreen(Sdx[x_p][y_p].green()+Sdx[x_m][y_m].green()-Sdx[x_m][y_p].green()-Sdx[x_p][y_m].green());



                    Weight[i][j].setRed(square_s.red()+square_n.red()-2*sqrt(square_s.red())*sqrt(square_n.red()));
                    Weight[i][j].setBlue(square_s.blue()+square_n.blue()-2*sqrt(square_s.blue())*sqrt(square_n.blue()));
                    Weight[i][j].setGreen(square_s.green()+square_n.green()-2*sqrt(square_s.green())*sqrt(square_n.green()));//*/


                    /*Weight[i][j].setRed(square_s.red()+square_n.red());
                    Weight[i][j].setBlue(square_s.blue()+square_n.blue());
                    Weight[i][j].setGreen(square_s.green()+square_n.green());//*/

                    Weight[i][j].setRed(exp(-Weight[i][j].red()/pow(h,2)));
                    Weight[i][j].setBlue(exp(-Weight[i][j].blue()/pow(h,2)));
                    Weight[i][j].setGreen(exp(-Weight[i][j].green()/pow(h,2)));

                    z.setRed(z.red()+Weight[i][j].red());
                    z.setBlue(z.blue()+Weight[i][j].blue());
                    z.setGreen(z.green()+Weight[i][j].green());
                }
            }

            fin.setRed(0);
            fin.setBlue(0);
            fin.setGreen(0);
            Output->setPixel(x,y,fin.rgb());

            for (int ii=0;ii<size_b;ii++){
                for (int jj=0;jj<size_b;jj++){
                    //if ((x+ii-m_size_b<m)&&(x+ii-m_size_b>=0)&&(y+jj-m_size_b<n)&&(y+jj-m_size_b>=0)){
                        if ((x+ii-m_size_b>=m)||(x+ii-m_size_b<0)) if (y+jj-m_size_b<n) if (y+jj-m_size_b>=0)
                            noize=Noise->pixel(x-ii+m_size_b,y+jj-m_size_b);
                        if (x+ii-m_size_b<m) if (x+ii-m_size_b>=0) if ((y+jj-m_size_b>=n)||(y+jj-m_size_b<=0))
                            noize=Noise->pixel(x+ii-m_size_b,y-jj+m_size_b);
                        if (((x+ii-m_size_b>=m)||(x+ii-m_size_b<0))&&((y+jj-m_size_b>=n)||(y+jj-m_size_b<0)))
                            noize=Noise->pixel(x-ii+m_size_b,y-jj+m_size_b);
                        if (x+ii-m_size_b<m) if (x+ii-m_size_b>=0) if (y+jj-m_size_b<n) if (y+jj-m_size_b>=0)
                            noize=Noise->pixel(x+ii-m_size_b,y+jj-m_size_b);
                        Weight[ii][jj].setRed(Weight[ii][jj].red()*(noize.red())/z.red());
                        Weight[ii][jj].setBlue(Weight[ii][jj].blue()*(noize.blue())/z.blue());
                        Weight[ii][jj].setGreen(Weight[ii][jj].green()*(noize.green())/z.green());
                        fin.setRed(qRound(Weight[ii][jj].red()));
                        fin.setBlue(qRound(Weight[ii][jj].blue()));
                        fin.setGreen(qRound(Weight[ii][jj].green()));
                        out=Output->pixel(x,y);
                        out.setRed(out.red()+fin.red());
                        out.setBlue(out.blue()+fin.blue());
                        out.setGreen(out.green()+fin.green());
                        Output->setPixel(x,y,out.rgb());
                    //}

                }
            }
            if (update.elapsed()>30){

                update.restart();
                QMetaObject::invokeMethod(this,"updatePixel");
            }
        }

    }

    QMetaObject::invokeMethod(this,"updateStatus", Q_ARG(QString, "Status: deleting 4d-array."));

    for (int i=0;i<m;i++){
        QMetaObject::invokeMethod(this,"updateProgress", Q_ARG(int, (i*100)/(m-1)));
        delete[] Sdx[i];

    }
    delete[] Sdx;//*/

    diff=(double)time.elapsed()/1000;
    QMetaObject::invokeMethod(this,"updateStatus", Q_ARG(QString, "Status: done."));
    QMetaObject::invokeMethod(this, "finished",Q_ARG(double,diff-paused));
}








///SERIES OF IMAGES *OPEN*
///SERIES OF IMAGES *OPEN*
///SERIES OF IMAGES *OPEN*
void MainWindow::on_pushButton_12_clicked()
{
    //sure way to shoot yourself in the foot
    delete[] X;
    delete[] Y;

    QString fileName=QFileDialog::getExistingDirectory ( this, "Select directory with image files", "", QFileDialog::ShowDirsOnly );
    if (fileName!=NULL){
        QDir directory(fileName);
        QStringList list,filters;
        QList<QByteArray> tmp=QImageReader::supportedImageFormats();
        for(int i=0;i<tmp.count();i++) filters.append("*."+tmp.at(i));
        directory.setNameFilters(filters);
        list=directory.entryList(filters);
        int num=list.count();


        number=num;
        index=num-1;
        X=new QImage[number];
        Y=new QImage[number];

        for (int i=0;i<number;i++){
            QPixmap ori(fileName+"//"+list.at(i));
            X[i]=ori.toImage();
            Y[i]=X[i];
        }

        n=Y[0].height();
        m=Y[0].width();

        ui->label_15->resize(m,n);
        ui->label_15->setAlignment(Qt::AlignCenter);
        ui->label_15->setPixmap(QPixmap::fromImage(X[index]));
        ui->lineEdit_7->setText("Viewing image "+QString::number(index+1)+" of "+QString::number(number)+".");
    }


}

//SERIES OF IMAGES *ADD NOISE*
void MainWindow::on_pushButton_7_clicked()
{
    //int red=ui->lineEdit_10->text().toInt();
    //int green=ui->lineEdit_9->text().toInt();
    //int blue=ui->lineEdit_11->text().toInt();
    //int isSelectedNormal=1,isSelectedUniform=0;

    double diff=0;
    QElapsedTimer time;
    time.start();
    //if (ui->comboBox_2->currentIndex()==isSelectedNormal) for (int i=0;i<number;i++) add_noise(&X[i],&Y[i],red,green,blue);
    //if (ui->comboBox_2->currentIndex()==isSelectedUniform) for (int i=0;i<number;i++) add_noise_n(&X[i],&Y[i],red,green,blue,30);
    diff=(double)time.elapsed()/1000;
    ui->label_16->resize(m,n);
    ui->label_16->setAlignment(Qt::AlignCenter);
    ui->label_16->setPixmap(QPixmap::fromImage(Y[index]));
    ui->lineEdit_8->setText("Viewing noised image "+QString::number(index+1)+" of "+QString::number(number)+".");
    ui->elapsedLabel->setText("Time elapsed:"+QString::number(diff)+" sec.");

}
//SERIES OF IMAGES *DENOISE*
void MainWindow::on_pushButton_14_clicked()
{
    double diff=0;
    bool ok=true;
    int index1= ui->lineEdit_17->text().toInt(&ok,10);
    if (!ok){
        index1=index+1;
        ui->lineEdit_17->setText(QString::number(index1));
    }
    z_ind= ui->lineEdit_16->text().toInt(&ok,10)-1;
    if (!ok){
        z_ind=index;
        ui->lineEdit_16->setText(QString::number(z_ind+1));
    }
    index=index1;
    if (index >=3){

    Z=Y+z_ind;
        if (flag==4){ //wiener
            QElapsedTimer time;
            time.start();
            Wiener(X,Y,Z,index1,m,n);
            diff=(double)time.elapsed()/1000;
            //int m1=m,n1=n;

        }
        if (flag==5){ //Lagrange
            QElapsedTimer time;
            time.start();
            lagrange(X,Y,Z,index1,m,n);
            diff=(double)time.elapsed()/1000;
        }


        ui->label_19->resize(m,n);
        ui->label_19->setAlignment(Qt::AlignCenter);
        ui->label_19->setPixmap(QPixmap::fromImage(*Z));
        ui->elapsedLabel->setText("Time elapsed:"+QString::number(diff)+" sec.");
        //Y[index]=Z;
        ui->lineEdit_12->setText("Denoised image "+QString::number(z_ind+1)+" of "+QString::number(number)+".");
    }
    else ui->lineEdit_12->setText("You need more then "+QString::number(z_ind+1)+" interpolation points.");

}
void MainWindow::on_commandLinkButton_2_clicked()
{
    // open -
    if (index-1>=0){
        index=index-1;
        ui->lineEdit_7->setText("Viewing image "+QString::number(index+1)+" of "+QString::number(number)+".");
        ui->label_15->resize(m,n);
        ui->label_15->setAlignment(Qt::AlignCenter);
        ui->label_15->setPixmap(QPixmap::fromImage(X[index]));
    }
}

void MainWindow::on_commandLinkButton_clicked()
{
    //open +
    if (index+1<number){
        index=index+1;
        ui->lineEdit_7->setText("Viewing image "+QString::number(index+1)+" of "+QString::number(number)+".");
        ui->label_15->resize(m,n);
        ui->label_15->setAlignment(Qt::AlignCenter);
        ui->label_15->setPixmap(QPixmap::fromImage(X[index]));
    }
}

void MainWindow::on_commandLinkButton_4_clicked()
{
    //-noise
    if (index-1>=0){
        index=index-1;
        ui->lineEdit_8->setText("Viewing noised image "+QString::number(index+1)+" of "+QString::number(number)+".");
        ui->label_16->resize(m,n);
        ui->label_16->setAlignment(Qt::AlignCenter);
        ui->label_16->setPixmap(QPixmap::fromImage(Y[index]));
    }
}

void MainWindow::on_commandLinkButton_3_clicked()
{
    //+ noise
    if (index+1<number){
        index=index+1;
        ui->lineEdit_8->setText("Viewing noised image "+QString::number(index+1)+" of "+QString::number(number)+".");
        ui->label_16->resize(m,n);
        ui->label_16->setAlignment(Qt::AlignCenter);
        ui->label_16->setPixmap(QPixmap::fromImage(Y[index]));
    }
}

void MainWindow::on_commandLinkButton_6_clicked()
{
    //- denoise
    if (index-1>=0){
        index=index-1;
        ui->label_19->resize(m,n);
        ui->label_19->setAlignment(Qt::AlignCenter);
        ui->label_19->setPixmap(QPixmap::fromImage(Y[index]));
        ui->lineEdit_12->setText("Denoising image "+QString::number(index+1)+" of "+QString::number(number)+".");
    }
}

void MainWindow::on_commandLinkButton_5_clicked()
{
    //+ denoise
    if (index+1<number){
        index=index+1;
        ui->label_19->resize(m,n);
        ui->label_19->setAlignment(Qt::AlignCenter);
        ui->label_19->setPixmap(QPixmap::fromImage(Y[index]));
        ui->lineEdit_12->setText("Denoising image "+QString::number(index+1)+" of "+QString::number(number)+".");
    }
}

void MainWindow::on_horizontalSlider_2_valueChanged()
{
    ui->lineEdit_10->setText(QString::number(ui->horizontalSlider_2->value()));
    ui->lineEdit_9->setText(QString::number(ui->horizontalSlider_2->value()));
    ui->lineEdit_11->setText(QString::number(ui->horizontalSlider_2->value()));
}
//COMPARISON Serie of images
void MainWindow::on_pushButton_15_clicked()
{
    QColor z,ori,out;
    double error[3];
    error[0]=0;
    error[1]=0;
    error[2]=0;
    Comparison=Z;
    for (int i=0;i<m;i++){
        for (int j=0;j<n;j++){
            ori=X[z_ind].pixel(i,j);
            out=Z->pixel(i,j);
            z.setRed(abs(ori.red()-out.red()));
            z.setBlue(abs(ori.blue()-out.blue()));
            z.setGreen(abs(ori.green()-out.green()));
            Comparison->setPixel(i,j,z.rgb());
            error[0]=pow(ori.red()-out.red(),2);
            error[1]=pow(ori.green()-out.green(),2);
            error[2]=pow(ori.blue()-out.blue(),2);
        }
    }
    error[0]=error[0]/(m*n);
    error[1]=error[1]/(m*n);
    error[2]=error[2]/(m*n);
    ui->lineEdit_13->setText(QString::number(error[0],'g',4));
    ui->lineEdit_14->setText(QString::number(error[1],'g',4));
    ui->lineEdit_15->setText(QString::number(error[2],'g',4));
    ui->label_20->setAlignment(Qt::AlignCenter);
    ui->label_20->setPixmap(QPixmap::fromImage(*Comparison));
}
//*SAVING RESULTS SERIES OF IMAGES*
void MainWindow::on_pushButton_16_clicked()
{
    QString fileName=QFileDialog::getExistingDirectory ( 0, "Select directory to save files", "", QFileDialog::ShowDirsOnly );
    if (fileName!=NULL)
    {
        X[z_ind].save(fileName+"//original.tiff", "",-1 );
        Y[z_ind].save(fileName+"//noise.tiff", "",-1 );
        Z->save(fileName+"//output.tiff", "",-1 );
        Comparison->save(fileName+"//compare.tiff", "",-1 );
    }
}

void rgbmy_to_float(rgb_my **im,float **mass,int color_n,int ord,int m1,int n1){
    if (ord==0){ //im to mas
        if (color_n==0){ //red
            for (int i=0;i<m1;i++){
                for (int j=0;j<n1;j++){
                    mass[i][j]=im[i][j].red();
                }
            }
        }
        if (color_n==1){//green
            for (int i=0;i<m1;i++){
                for (int j=0;j<n1;j++){
                    mass[i][j]=im[i][j].green();
                }
            }
        }
        if (color_n==2){ //blue
            for (int i=0;i<m1;i++){
                for (int j=0;j<n1;j++){
                    mass[i][j]=im[i][j].blue();
                }
            }
        }
    }
    if (ord==1){ //mas to im
        if (color_n==0){//red
            for (int i=0;i<m1;i++){
                for (int j=0;j<n1;j++){
                    im[i][j].setRed(mass[i][j]);
                }
            }
        }
        if (color_n==1){//green
            for (int i=0;i<m1;i++){
                for (int j=0;j<n1;j++){
                    im[i][j].setGreen(mass[i][j]);
                }
            }
        }
        if (color_n==2){//blue
            for (int i=0;i<m1;i++){
                for (int j=0;j<n1;j++){
                    im[i][j].setBlue(mass[i][j]);
                }
            }
        }

    }
}
void image_to_float(QImage *im,float **mass,int color_n,int ord,int m1,int n1){
    QColor q;
    if (ord==0){ //im to mas
        if (color_n==0){ //red
            for (int i=0;i<m1;i++){
                for (int j=0;j<n1;j++){
                    q=im->pixel(i,j);
                    mass[i][j]=q.red();
                }
            }
        }
        if (color_n==1){//green
            for (int i=0;i<m1;i++){
                for (int j=0;j<n1;j++){
                    q=im->pixel(i,j);
                    mass[i][j]=q.green();
                }
            }
        }
        if (color_n==2){ //blue
            for (int i=0;i<m1;i++){
                for (int j=0;j<n1;j++){
                    q=im->pixel(i,j);
                    mass[i][j]=q.blue();
                }
            }
        }
    }
    if (ord==1){ //mas to im
        if (color_n==0){//red
            for (int i=0;i<m1;i++){
                for (int j=0;j<n1;j++){
                    q=im->pixel(i,j);
                    if (mass[i][j]<0) mass[i][j]=0;
                    if (mass[i][j]>255) mass[i][j]=255;
                    q.setRed(mass[i][j]);
                    im->setPixel(i,j,q.rgb());
                }
            }
        }
        if (color_n==1){//green
            for (int i=0;i<m1;i++){
                for (int j=0;j<n1;j++){
                    q=im->pixel(i,j);
                    if (mass[i][j]<0) mass[i][j]=0;
                    if (mass[i][j]>255) mass[i][j]=255;
                    q.setGreen(mass[i][j]);
                    im->setPixel(i,j,q.rgb());
                }
            }
        }
        if (color_n==2){//blue
            for (int i=0;i<m1;i++){
                for (int j=0;j<n1;j++){
                    if (mass[i][j]<0) mass[i][j]=0;
                    if (mass[i][j]>255) mass[i][j]=255;
                    q=im->pixel(i,j);
                    q.setBlue(mass[i][j]);
                    im->setPixel(i,j,q.rgb());
                }
            }
        }

    }
}
void pinv(rgb_my **in,rgb_my **out,int m1,int n1){
/*pinv(A)=(transp(A)*A)^(-1)*transp(A)*/
    // in - m1-n1
    //out - n1-mi
    float** p_temp= new float*[m1];
    for(int i=0;i<m1;i++) p_temp[i] = new float[n1];
    float** p_temp_transp= new float*[n1];
    for(int i=0;i<n1;i++) p_temp_transp[i] = new float[m1];
    float** p_temp_mult= new float*[n1];
    for(int i=0;i<n1;i++) p_temp_mult[i] = new float[n1];
    float** p_temp_inv= new float*[n1];
    for(int i=0;i<n1;i++) p_temp_inv[i] = new float[n1];
    float** p_temp_mult_f= new float*[n1];
    for(int i=0;i<n1;i++) p_temp_mult_f[i] = new float[m1];

    for (int i=0;i<3;i++){
        rgbmy_to_float(in,p_temp,i,0,m1,n1);
        transp(p_temp,p_temp_transp,m1,n1);
        mmultiply(p_temp_transp,p_temp,p_temp_mult,n1,m1,m1,n1);
        minv(p_temp_mult,p_temp_inv,n1);
        mmultiply(p_temp_inv,p_temp_transp,p_temp_mult_f,n1,n1,n1,m1);
        rgbmy_to_float(out,p_temp_mult_f,i,1,n1,m1);
    }

    for (int i = 0; i <m1; i++) delete[] p_temp[i];
    delete[] p_temp;
    for (int i = 0; i <n1; i++) delete[] p_temp_transp[i];
    delete[] p_temp_transp;
    for (int i = 0; i <n1; i++) delete[] p_temp_mult[i];
    delete[] p_temp_mult;
    for (int i = 0; i <n1; i++) delete[] p_temp_inv[i];
    delete[] p_temp_inv;
    for (int i = 0; i <n1; i++) delete[] p_temp_mult_f[i];
    delete[] p_temp_mult_f;
}
//фільтр вінера
void MainWindow::Wiener(QImage *X,QImage *Y,QImage *Z,int number,int m1,int n1){
    rgb_my** E_x_y,**E_y_y,**W;
    float** temp_x= new float*[m1];
    for(int i=0;i<m1;i++) temp_x[i] = new float[n1];
    float** temp_y= new float*[m1];
    for(int i=0;i<m1;i++) temp_y[i] = new float[n1];
    float** temp_transp= new float*[n1];
    for(int i=0;i<n1;i++) temp_transp[i] = new float[m1];
    float** temp_mult= new float*[m1];
    for(int i=0;i<m1;i++) temp_mult[i] = new float[m1];
    float** temp_add= new float*[m1];
    for(int i=0;i<m1;i++) temp_add[i] = new float[m1];
    zero_m(temp_add,m1,m1);
    /*
E_x_y=(X_1*Y_1'+X_2*Y_2'+X_3*Y_3'+X_4*Y_4'+X_5*Y_5'+X_6*Y_6'+X_7*Y_7'+X_8*Y_8'+X_9*Y_9')/9;*/

        E_x_y= new rgb_my*[m1];
        for(int i=0;i<m1;i++) E_x_y[i] = new rgb_my[m1];
        E_y_y= new rgb_my*[m1];
        for(int i=0;i<m1;i++) E_y_y[i] = new rgb_my[m1];

        /*        if (i==0) ui->label_13->setText("Status: computing coefficients for red color");
        if (i==1) ui->label_13->setText("Status: computing coefficients for green color");
        if (i==2) ui->label_13->setText("Status: computing coefficients for blue color");
        for (int k=0;k<number1;k++){
            ui->progressBar->setValue((k*100)/(number1-1));*/

    for(int i=0;i<3;i++){
        if (i==0) ui->statusLabel->setText("Status: computing first covariance matrix for red color");
        if (i==1) ui->statusLabel->setText("Status: computing first covariance matrix for green color");
        if (i==2) ui->statusLabel->setText("Status: computing first covariance matrix for blue color");
        zero_m(temp_add,m1,m1);
        for (int j=0;j<number;j++){
            ui->progressBar->setValue(((j+1)*100)/((number-1)));
            image_to_float(&X[j],temp_x,i,0,m1,n1);
            image_to_float(&Y[j],temp_y,i,0,m1,n1);
            transp(temp_y,temp_transp,m1,n1);
            mmultiply(temp_x,temp_transp,temp_mult,m1,n1,n1,m1);
            madd(temp_mult,temp_add,temp_add,m1,m1);

        }
        mdivide(temp_add,number,m1,m1);
        rgbmy_to_float(E_x_y,temp_add,i,1,m1,n1);
    }
/*E_y_y=(Y_1*Y_1'+Y_2*Y_2'+Y_3*Y_3'+Y_4*Y_4'+Y_5*Y_5'+Y_6*Y_6'+Y_7*Y_7'+Y_8*Y_8'+Y_9*Y_9')/9;*/
    for(int i=0;i<3;i++){
        if (i==0) ui->statusLabel->setText("Status: computing second covariance matrix for red color");
        if (i==1) ui->statusLabel->setText("Status: computing second covariance matrix for green color");
        if (i==2) ui->statusLabel->setText("Status: computing second covariance matrix for blue color");
        zero_m(temp_add,m1,m1);
        for (int j=0;j<number;j++){
            ui->progressBar->setValue(((j+1)*100)/((number-1)));
            image_to_float(&Y[j],temp_y,i,0,m1,n1);
            transp(temp_y,temp_transp,m1,n1);
            mmultiply(temp_y,temp_transp,temp_mult,m1,n1,n1,m1);
            madd(temp_mult,temp_add,temp_add,m1,m1);

        }
        mdivide(temp_add,number,m1,m1);
        rgbmy_to_float(E_y_y,temp_add,i,1,m1,n1);
    }

    W= new rgb_my*[m1];
    for(int i=0;i<m1;i++) W[i] = new rgb_my[m1];

    rgb_my **p_E_y_y= new rgb_my*[m1];
    for(int i=0;i<m1;i++) p_E_y_y[i] = new rgb_my[m1];
    float **mult_x_y= new float*[m1];
    for(int i=0;i<m1;i++) mult_x_y[i] = new float[m1];
    float **mult_y_y= new float*[m1];
    for(int i=0;i<m1;i++) mult_y_y[i] = new float[m1];
    float **p_y_y= new float*[m1];
    for(int i=0;i<m1;i++) p_y_y[i] = new float[m1];
    float **t_x_y= new float*[m1];
    for(int i=0;i<m1;i++) t_x_y[i] = new float[m1];
    float **t_y_y= new float*[m1];
    for(int i=0;i<m1;i++) t_y_y[i] = new float[m1];
    float **eye= new float*[m1];
    for(int i=0;i<m1;i++) eye[i] = new float[m1];
    for(int i=0;i<m1;i++){
        for(int j=0;j<m1;j++){
            if (i!=j)eye[i][j]=0;
            else eye[i][j]=1;
        }
    }
    /*pinv(A)=(transp(A)*A)^(-1)*transp(A)*/
    pinv(E_y_y,p_E_y_y,m1,m1);
    /*
    W= E_x_y*pinv(E_y_y)+eye*(eye-E_y_y*pinv(E_y_y));
    Z_10=W*Y_10;


    */
    for (int i=0;i<3;i++){
        ui->statusLabel->setText("Status: computing Wiener's operator");
        ui->progressBar->setValue(((i+1)*100)/(3));
        rgbmy_to_float(p_E_y_y,p_y_y,i,0,m1,m1);
        rgbmy_to_float(E_x_y,t_x_y,i,0,m1,m1);
        rgbmy_to_float(E_y_y,t_y_y,i,0,m1,m1);
        mmultiply(t_x_y,p_y_y,mult_x_y,m1,m1,m1,m1);
        mmultiply(t_y_y,p_y_y,mult_y_y,m1,m1,m1,m1);
        for(int ii=0;ii<m1;ii++) for(int j=0;j<m1;j++) mult_y_y[ii][j]=eye[ii][j]-mult_y_y[ii][j];
        for(int ii=0;ii<m1;ii++) for(int j=0;j<m1;j++) mult_x_y[ii][j]=mult_x_y[ii][j]+mult_y_y[ii][j];

        rgbmy_to_float(W,mult_x_y,i,1,m1,m1);
    }

//QImage Z = rgb_my W and QImage Y_10
    float **rgb_temp= new float*[m1];
    for(int i=0;i<m1;i++) rgb_temp[i] = new float[m1];
    float **im_temp= new float*[m1];
    for(int i=0;i<m1;i++) im_temp[i] = new float[n1];
    float **res_temp= new float*[m1];
    for(int i=0;i<m1;i++) res_temp[i] = new float[n1];



    for (int i=0;i<3;i++){
        rgbmy_to_float(W,rgb_temp,i,0,m1,m1);
        image_to_float(Z,im_temp,i,0,m1,n1);
        mmultiply(rgb_temp,im_temp,res_temp,m1,m1,m1,n1);
        image_to_float(Z,res_temp,i,1,m1,n1);
    }
    ui->statusLabel->setText("Done");


    for (int i = 0; i <m1; i++) delete[] p_E_y_y[i];
    delete[] p_E_y_y;
    for(int i=0;i<m1;i++){
        delete[] rgb_temp[i];
        delete[] im_temp[i];
        delete[] res_temp[i];
        delete[] mult_x_y[i];
        delete[] mult_y_y[i];
        delete[] p_y_y[i];
        delete[] t_x_y[i];
        delete[] t_y_y[i];
        delete[] eye[i];
        delete[] W[i];
        delete[] temp_x[i];
        delete[] temp_y[i];
        delete[] temp_mult[i];
        delete[] temp_add[i];
        delete[] E_x_y[i];
        delete[] E_y_y[i];
    }
    delete[] rgb_temp;
    delete[] im_temp;
    delete[] res_temp;
    delete[] mult_x_y;
    delete[] mult_y_y;
    delete[] p_y_y;
    delete[] t_x_y;
    delete[] t_y_y;
    delete[] eye;
    delete[] W;
    delete[] temp_x;
    delete[] temp_y;
    delete[] temp_mult;
    delete[] temp_add;
    delete[] E_x_y;
    delete[] E_y_y;
    for (int i = 0; i <n1; i++) delete temp_transp[i];
    delete[] temp_transp;

}

long double a_coef_upper(int k,int number1,QImage* q,QImage* Y,int ind,int m1,int n1){
    long double product=1;
    //стандартна формула trace((Y[t]-Y[tj])*transp(Y[tk]-Y[tj]))
    float** temp_q= new float*[m1];
    for(int i=0;i<m1;i++) temp_q[i] = new float[n1];
    float** temp_y= new float*[m1];
    for(int i=0;i<m1;i++) temp_y[i] = new float[n1];
    float** temp_k= new float*[m1];
    for(int i=0;i<m1;i++) temp_k[i] = new float[n1];


    for (int i=0;i<number1;i++){
        image_to_float(&Y[i],temp_y,ind,0,m1,n1);
        image_to_float(q,temp_q,ind,0,m1,n1);
        image_to_float(&Y[k],temp_k,ind,0,m1,n1);
        if (i!=k){
            for (int c=0;c<m1;c++){
                for (int v=0;v<n1;v++){
                    temp_q[c][v]=temp_q[c][v]-temp_y[c][v]; //Y[t]-Y[tj]
                    temp_k[c][v]=temp_k[c][v]-temp_y[c][v]; //Y[tk]-Y[tj]
                }
            }
            product=product*trace_lin(temp_q,temp_k,m1,n1); //<M,M1>=trace(M*transp(M1))
        }
        //trace_lin()
    }
    for(int i=0;i<m1;i++){
        delete[] temp_q[i];
        delete[] temp_y[i];
        delete[] temp_k[i];
    }
    delete[] temp_y;
    delete[] temp_q;
    delete[] temp_k;
    return product;
}
long double a_coef_lower(int k,int number1,QImage* q,QImage* Y,int ind,int m1,int n1){
    long double product=1;
    //підраховуеться сума у квадраті усих елементів
    float** temp_q= new float*[m1];
    for(int i=0;i<m1;i++) temp_q[i] = new float[n1];
    float** temp_y= new float*[m1];
    for(int i=0;i<m1;i++) temp_y[i] = new float[n1];



    for (int i=0;i<number1;i++){
        image_to_float(&Y[i],temp_y,ind,0,m1,n1);
        image_to_float(q,temp_q,ind,0,m1,n1);
        if (i!=k){
            for (int c=0;c<m1;c++){
                for (int v=0;v<n1;v++){
                    temp_q[c][v]=temp_q[c][v]-temp_y[c][v]; //Y[t]-Y[tj]
                }
            }
            product=product*trace_sum(temp_q,m1,n1); // сума всіх елементів матриці в квадраті
        }
        //trace_sum()
    }
    for(int i=0;i<m1;i++){
        delete[] temp_q[i];
        delete[] temp_y[i];
    }
    delete[] temp_y;
    delete[] temp_q;
    return product;
}
void MainWindow::lagrange(QImage *X,QImage *Y,QImage *Z,int number1,int m1,int n1){
    /*Wiener(X,Y,&Z,number,m,n);*/
    long double coef[3][number1];
    long double z1[3];
    float** sum= new float*[m1];
    for(int i=0;i<m1;i++) sum[i] = new float[n1];
    z1[0]=0;
    z1[1]=0;
    z1[2]=0;
//шукаємо коефіцієнти та зет
    for (int i=0;i<3;i++){
        if (i==0) ui->statusLabel->setText("Status: computing coefficients for red color");
        if (i==1) ui->statusLabel->setText("Status: computing coefficients for green color");
        if (i==2) ui->statusLabel->setText("Status: computing coefficients for blue color");
        for (int k=0;k<number1;k++){
            ui->progressBar->setValue((k*100)/(number1-1));
            coef[i][k]=(a_coef_upper(k,number1,Z,Y,i,m1,n1))/(a_coef_lower(k,number1,&Y[k],Y,i,m1,n1));
            z1[i]+=coef[i][k];
        }
    }

    float** temp_x= new float*[m1];
    for(int i=0;i<m1;i++) temp_x[i] = new float[n1];

// множимо на ікс уже пронормовані коєфіцієнти
    for(int i=0;i<3;i++){
        if (i==0) ui->statusLabel->setText("Status: denoising red color of the image");
        if (i==1) ui->statusLabel->setText("Status: denoising green color of the image");
        if (i==2) ui->statusLabel->setText("Status: denoising blue color of the image");
        zero_m(sum,m1,n1);
        for (int j=0;j<number1;j++){
            ui->progressBar->setValue((j*100)/(number1-1));
            image_to_float(&X[j],temp_x,i,0,m1,n1);
            for (int w=0;w<m1;w++) for (int e=0;e<n1;e++) sum[w][e]=sum[w][e]+temp_x[w][e]*(coef[i][j]/z1[i]);

        }
        image_to_float(Z,sum,i,1,m1,n1);
    }
    //for (int i=0;i<3;i++)for (int j=0;j<number;j++) printf("coef_up %d, iter %d = %Lf \n",i,j,coef_up[i][j]);
    //for (int i=0;i<3;i++)for (int j=0;j<number;j++) printf("coef_lw %d, iter %d = %Lf \n",i,j,coef_lw[i][j]);
    //printf ("z= %Lf \n",z1);
    ui->statusLabel->setText("Done");
    for(int i=0;i<m1;i++) delete[] temp_x[i];
    for(int i=0;i<m1;i++) delete[] sum[i];
    delete[] temp_x;
    delete[] sum;

}


