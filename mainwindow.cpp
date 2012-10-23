#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include <QtCore>
#include <QImageReader>
#include <QImage>
#include <QPixmap>
#include <QVector>
#include <QFuture>


//#include "matrix.h"
#include <time.h>
QImage *Orig,*Noise,*Output,*Comparison;
noiseClass* noiseSettings;
denoiseClass* denoiseSettings;

int m=0,n=0,number=0,z_ind=0;
int flag=0;
bool isPaused=false,isRendering=false;

float RandNorm( int num );


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
long getCPUnum(){
    SYSTEM_INFO sysinfo;
    GetSystemInfo( &sysinfo );
    return sysinfo.dwNumberOfProcessors;
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
long getCPUnum(){
    return sysconf( _SC_NPROCESSORS_ONLN );
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



    noiseSettings= new noiseClass();
    denoiseSettings= new denoiseClass();
    nw= new noiseWidget();
    nw->on_slideIntensity_valueChanged(30);
    nw->on_slideProbability_valueChanged(50);

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
        //delete Orig;
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
        int pro[1];
        ui->progressBar->setValue(0);
        QString settings=" "+QString::number(denoiseSettings->patch_size)+" "+QString::number(denoiseSettings->search_window)+" "+QString::number(denoiseSettings->pow)+
                " "+QString::number(0)+" "+QString::number(n)+" "+QString::number(0)+" "+QString::number(m)+" false ";
        QFuture<void> future=QtConcurrent::run(this,&MainWindow::non_local_means_method,Noise,Output,settings,pro+0);
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
    if (flag==4){
        ui->progressBar->setValue(0);
        QString settings=" "+QString::number(denoiseSettings->patch_size)+" "+QString::number(denoiseSettings->search_window)+" "+QString::number(denoiseSettings->pow)+" ";
        QtConcurrent::run(this,&MainWindow::non_local_means_method_multyThread,Noise,Output,settings);
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

void MainWindow::non_local_means_method(QImage *inim,QImage *outim,QString settings, int* progress)
{
    int size_m=settings.section(" ",1,1).toInt(), size_b=settings.section(" ",2,2).toInt(), h=settings.section(" ",3,3).toInt(),
            beginY=settings.section(" ",4,4).toInt(), endY=settings.section(" ",5,5).toInt(),beginX=settings.section(" ",6,6).toInt(),
            endX=settings.section(" ",7,7).toInt();
    bool silent=true;
    if (settings.section(" ",8,8)=="false") silent=false;
    isRendering=true;
    QElapsedTimer time,update;
    double diff=0,paused=0;
    time.start();
    if(!silent) update.start();
    if(!silent) QMetaObject::invokeMethod(this,"updateStatus", Q_ARG(QString, "Status: computing."));
    if(!silent) QMetaObject::invokeMethod(this,"iconPause");

    int m_size=(size_m-1)/2;
    int m_size_b=(size_b-1)/2;
    //int c=0;
    QColor p,q,fin,out,noize;
    rgb_my Weight[size_b][size_b],z;
    for (int y=beginY;y<endY;y++){
        if(!silent) QMetaObject::invokeMethod(this,"updateProgress", Q_ARG(int, ((y-beginY)*100)/(endY-beginY-1)));
        if (silent){
            *progress=((y-beginY)*100)/(endY-beginY-1);
        }
        for (int x=beginX;x<endX;x++){
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
                if(!silent) QMetaObject::invokeMethod(this, "finished",Q_ARG(double,diff-paused));
                //if(silent)qDebug()<<"finished"<<diff-paused;
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

                            if ((x-m_size+a>=endX)||(x-m_size+a<beginX)) x_m=x+m_size-a;
                            else x_m=x-m_size+a;
                            if ((y-m_size+b>=endY)||(y-m_size+b<beginY)) y_m=y+m_size-b;
                            else y_m=y-m_size+b;
                            p=inim->pixel(x_m,y_m);

                            if ((x-m_size_b+i>=endX)||(x-m_size_b+i<beginX)) x_f=x+m_size_b-i;
                            else x_f=x-m_size_b+i;
                            if ((y-m_size_b+j>=endY)||(y-m_size_b+j<beginY)) y_f=y+m_size_b-j;
                            else y_f=y-m_size_b+j;

                            if ((x_f-m_size+a>=endX)||(x_f-m_size+a<beginX)) x_ff=x_f+m_size-a;
                            else x_ff=x_f-m_size+a;
                            if ((y_f-m_size+b>=endY)||(y_f-m_size+b<beginY)) y_ff=y_f+m_size-b;
                            else y_ff=y_f-m_size+b;

                            q=inim->pixel(x_ff,y_ff);//*/
                            Weight[i][j].setRed(Weight[i][j].red()+pow(p.red()-q.red(),2));
                            Weight[i][j].setBlue(Weight[i][j].blue()+pow(p.blue()-q.blue(),2));
                            Weight[i][j].setGreen(Weight[i][j].green()+pow(p.green()-q.green(),2));
                        }
                    }
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
            outim->setPixel(x,y,fin.rgb());

            for (int ii=0;ii<size_b;ii++){
                for (int jj=0;jj<size_b;jj++){
                    //if ((x+ii-m_size_b<m)&&(x+ii-m_size_b>=0)&&(y+jj-m_size_b<n)&&(y+jj-m_size_b>=0)){
                        if ((x+ii-m_size_b>=endX)||(x+ii-m_size_b<beginX)) if (y+jj-m_size_b<endY) if (y+jj-m_size_b>=beginY)
                            noize=inim->pixel(x-ii+m_size_b,y+jj-m_size_b);
                        if (x+ii-m_size_b<endX) if (x+ii-m_size_b>=beginX) if ((y+jj-m_size_b>=endY)||(y+jj-m_size_b<=beginY))
                            noize=inim->pixel(x+ii-m_size_b,y-jj+m_size_b);
                        if (((x+ii-m_size_b>=endX)||(x+ii-m_size_b<beginX))&&((y+jj-m_size_b>=endY)||(y+jj-m_size_b<beginY)))
                            noize=inim->pixel(x-ii+m_size_b,y-jj+m_size_b);
                        if (x+ii-m_size_b<endX) if (x+ii-m_size_b>=beginX) if (y+jj-m_size_b<endY) if (y+jj-m_size_b>=beginY)
                            noize=inim->pixel(x+ii-m_size_b,y+jj-m_size_b);
                        Weight[ii][jj].setRed(Weight[ii][jj].red()*(noize.red())/z.red());
                        Weight[ii][jj].setBlue(Weight[ii][jj].blue()*(noize.blue())/z.blue());
                        Weight[ii][jj].setGreen(Weight[ii][jj].green()*(noize.green())/z.green());
                        fin.setRed(qRound(Weight[ii][jj].red()));
                        fin.setBlue(qRound(Weight[ii][jj].blue()));
                        fin.setGreen(qRound(Weight[ii][jj].green()));
                        out=outim->pixel(x,y);
                        out.setRed(out.red()+fin.red());
                        out.setBlue(out.blue()+fin.blue());
                        out.setGreen(out.green()+fin.green());
                        outim->setPixel(x,y,out.rgb());
                    //}

                }
            }
            if ((!silent)&&(update.elapsed()>30)){

                update.restart();
                QMetaObject::invokeMethod(this,"updatePixel");
            }

        }

    }
    diff=(double)time.elapsed()/1000;
    if(!silent) QMetaObject::invokeMethod(this,"updateStatus", Q_ARG(QString, "Status: done."));
    if(!silent) QMetaObject::invokeMethod(this, "finished",Q_ARG(double,diff-paused));
    //if(silent) isRendering=false;
    //if(silent)qDebug()<<"finished"<<diff-paused;

/*
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  */
}



void MainWindow::non_local_means_method_multyThread(QImage *inim,QImage *outim,QString settings)
{
    int CPUnum=getCPUnum()-1;
    int *progressArr=new int[CPUnum];
    // +QString::number(0)+" "+QString::number(n)+" "+QString::number(0)+" "+QString::number(m)+" ";
    QImage* in_arr=new QImage[CPUnum];
    QImage* out_arr=new QImage[CPUnum];
    QVector<QFuture<void> > results;
    for (int i=0;i<CPUnum;i++){
        in_arr[i]=  QImage(*inim);
        out_arr[i]= QImage(*outim);
        QString sets2;
        progressArr[i]=0;
        if ((i!=0)&&(i!=CPUnum-1)) sets2=QString::number(i*(n)/CPUnum-5)+" "+QString::number(((i+1)*n)/CPUnum+5)+" "+QString::number(0)+" "+QString::number(m)+" true ";
        else if (i==0) sets2=QString::number(i*(n)/CPUnum)+" "+QString::number(((i+1)*n)/CPUnum+5)+" "+QString::number(0)+" "+QString::number(m)+" true ";
        else if (i==(CPUnum-1)) sets2=QString::number(i*(n)/CPUnum-5)+" "+QString::number(((i+1)*n)/CPUnum)+" "+QString::number(0)+" "+QString::number(m)+" true ";
        QFuture<void> future =QtConcurrent::run(this,&MainWindow::non_local_means_method,in_arr+i,out_arr+i,settings+sets2,progressArr+i);
        results.append(future);
    }
    QElapsedTimer merge,common;
    QMetaObject::invokeMethod(this,"updateStatus", Q_ARG(QString, "Status: computing."));
    int finish=0;
    merge.start();
    common.start();
    double paused=0;
    while (finish!=CPUnum){
        if (isPaused){
            QElapsedTimer pauseTime;
            pauseTime.start();
            while (isPaused){
                if (!isRendering) isPaused=false;
                sleep(1);
            }
            paused+=(double)pauseTime.elapsed()/1000;
        }
        finish=0;
        for (int i=0;i<CPUnum;i++) if (results.at(i).isFinished()) finish++;
        if ((merge.elapsed()>30)&&!isPaused){
            merge.restart();
            for (int i=0;i<CPUnum;i++)
                for (int n1=(i*(n)/CPUnum);n1<((i+1)*n)/CPUnum;n1++)
                    for (int m1=0;m1<m;m1++)
                        Output->setPixel(m1,n1,out_arr[i].pixel(m1,n1));
            QMetaObject::invokeMethod(this,"updatePixel");
            int percent=0;
            for (int i=0;i<CPUnum;i++) percent+=progressArr[i];
            QMetaObject::invokeMethod(this,"updateProgress",Q_ARG(int,percent/CPUnum));
        }
        sleep(5);
    }
    double diff=(double)common.elapsed()/1000;
    QMetaObject::invokeMethod(this, "finished",Q_ARG(double,diff-paused));
    QMetaObject::invokeMethod(this,"updateStatus", Q_ARG(QString, "Status: done."));

    delete[] in_arr;

    for (int i=0;i<CPUnum;i++)
        for (int n1=(i*(n)/CPUnum);n1<((i+1)*n)/CPUnum;n1++)
            for (int m1=0;m1<m;m1++)
                Output->setPixel(m1,n1,out_arr[i].pixel(m1,n1));
    QMetaObject::invokeMethod(this,"updatePixel");
    delete[] out_arr;
    delete[] progressArr;
}

void MainWindow::non_local_means_method_fast(int size_m,int size_b,int h)
{
    isRendering=true;
    int m_size=(size_m-1)/2;
    int m_size_b=(size_b-1)/2;
    // to avoid running out of RAM, checking for available ram memory

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


    //brainblowing creating 4d dynamic array.
    // probably wrap array into something like this
    //rgb_my* Sdx=new rgb_my[size_b*size_b*m*n];
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
            //setting normalizing constant to zero
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



