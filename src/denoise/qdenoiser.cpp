#include "qdenoiser.h"

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

QDenoiser::QDenoiser()
{
    computingTime=new QTimer(this);
    connect(computingTime,SIGNAL(timeout()),this,SLOT(updateTimer()),Qt::QueuedConnection);
    computingTime->setSingleShot(false);
    computingTime->setInterval(25);
    connect(this,SIGNAL(stoptimer()),computingTime,SLOT(stop()),Qt::QueuedConnection);
    connect(this,SIGNAL(starttimer()),computingTime,SLOT(start()),Qt::QueuedConnection);
    setTimeSpent(0.0);
    input = new QImage(QSize(0,0), QImage::Format_ARGB32_Premultiplied);
    input->fill(Qt::transparent);
    output = new QImage(QSize(0,0), QImage::Format_ARGB32_Premultiplied);
    output->fill(Qt::transparent);
    setSettings(new denoiseClass());
    setRendering(false);
    setPaused(false);
    setProgress(0);
    setStatus("");
    setDenoisingMethod(denoiseClass::NLM_fast_FFT);
}

QDenoiser::QDenoiser(QImage* in,denoiseClass* settings){
    computingTime=new QTimer(this);
    connect(computingTime,SIGNAL(timeout()),this,SLOT(updateTimer()),Qt::QueuedConnection);
    computingTime->setSingleShot(false);
    computingTime->setInterval(25);
    connect(this,SIGNAL(stoptimer()),computingTime,SLOT(stop()),Qt::QueuedConnection);
    connect(this,SIGNAL(starttimer()),computingTime,SLOT(start()),Qt::QueuedConnection);
    setTimeSpent(0.0);
    input = new QImage(QSize(0,0), QImage::Format_ARGB32_Premultiplied);
    input->fill(Qt::transparent);
    output = new QImage(QSize(0,0), QImage::Format_ARGB32_Premultiplied);
    output->fill(Qt::transparent);
    setImage(in);
    setSettings(settings);
    setRendering(false);
    setPaused(false);
    setProgress(0);
    setStatus(0);
    setDenoisingMethod(settings->denoisingMethod);
}
QDenoiser::~QDenoiser(){
    delete input;
    delete output;
    delete computingTime;
    delete sets;
}

void QDenoiser::setSettings(denoiseClass* settings){
    QMutexLocker lock(&mutex);
    sets=settings;
}
void QDenoiser::setDenoisingMethod(denoiseClass::method val){
    QMutexLocker lock(&mutex);
    denoisingMethod=val;
}
void QDenoiser::setRendering(bool state){
    QMutexLocker lock(&mutex);
    rendering=state;
}

void QDenoiser::setPaused(bool state){
    QMutexLocker lock(&mutex);
    paused=state;
}
void QDenoiser::setProgress(int prog){
    QMutexLocker lock(&mutex);
    progress=prog;
}
void QDenoiser::setStatus(QString status){
    QMutexLocker lock(&mutex);
    _status=status;
}
void QDenoiser::setImage(QImage* inim){
    QMutexLocker lock(&mutex);
    delete input;
    delete output;
    input= new QImage(*inim);
    output=new QImage(*inim);
    n=input->height();
    m=input->width();
}
void QDenoiser::setTimeSpent(double ms){
    QMutexLocker lock(&mutex);
    time_computing=ms;
}

void QDenoiser::updateTimer(){
    QMutexLocker lock(&mutex);
    QTimer* tmp= (QTimer*) sender();
    time_computing+=(double)tmp->interval()/1000;
}
bool QDenoiser::isRendering(){
    QMutexLocker lock(&mutex);
    return rendering;
}
bool QDenoiser::isPaused(){
    QMutexLocker lock(&mutex);
    return paused;
}
denoiseClass* QDenoiser::getSettings(){
    QMutexLocker lock(&mutex);
    return sets;
}
denoiseClass::method QDenoiser::getDenoisingMethod(){
    QMutexLocker lock(&mutex);
    return denoisingMethod;
}
QImage* QDenoiser::getImage(){
    QMutexLocker lock(&mutex);
    return output;
}
int QDenoiser::getProgress(){
    QMutexLocker lock(&mutex);
    return progress;
}
QString QDenoiser::getStatus(){
    QMutexLocker lock(&mutex);
    return _status;
}
double QDenoiser::getTimeSpent(){
    QMutexLocker lock(&mutex);
    return time_computing;
}

void QDenoiser::startRender(){
    // big function
    setRendering(true);
    setPaused(false);

    if (getDenoisingMethod()==denoiseClass::simple_squares){
        int size=getSettings()->patch_size;
        QtConcurrent::run(this,&QDenoiser::simple_squares,size);
    }
    else if (getDenoisingMethod()==denoiseClass::NLM){
        int pro[1];
        QString settings=" "+QString::number(getSettings()->patch_size)+" "+QString::number(getSettings()->search_window)+" "+QString::number(getSettings()->pow)+
                " "+QString::number(0)+" "+QString::number(n)+" "+QString::number(0)+" "+QString::number(m)+" false ";
        QtConcurrent::run(this,&QDenoiser::NLM,input,output,settings,pro+0);
    }
    else if (getDenoisingMethod()==denoiseClass::NLM_fast){
        int size_m=getSettings()->patch_size;
        int size_b=getSettings()->search_window;
        int h=getSettings()->pow;
        QtConcurrent::run(this,&QDenoiser::NLM_fast,size_m,size_b,h);
    }
    else if (getDenoisingMethod()==denoiseClass::NLM_fast_FFT){
        int size_m=getSettings()->patch_size;
        int size_b=getSettings()->search_window;
        int h=getSettings()->pow;
        QtConcurrent::run(this,&QDenoiser::NLM_fast_FFT,size_m,size_b,h);
    }
    else if (getDenoisingMethod()==denoiseClass::NLM_multyThread){
        QString settings=" "+QString::number(getSettings()->patch_size)+" "+QString::number(getSettings()->search_window)+" "+QString::number(getSettings()->pow)+" ";
        QtConcurrent::run(this,&QDenoiser::NLM_multiThread,input,output,settings);
    }

}


void QDenoiser::pauseRender(){
    setPaused(true);
}

void QDenoiser::resumeRender(){
    setPaused(false);
}

void QDenoiser::cancelRender(){
    setRendering(false);
}

void QDenoiser::popMessageBox(int m1,int n1,int size_b)
{
    QMessageBox::warning(0,QString("Warning, can not allocate memory."),QString("Total available memory (")+QString::number(getAvailableSystemMemory()+getProcMemory())+
                         QString(" MB/")+QString::number(getTotalSystemMemory())+QString(" MB) is less then reqired for denoising this image (")
                         +QString::number((sizeof(rgb_my)*m1*n1*size_b*size_b)/(1024*1024))+QString(" MB)"),QMessageBox::Ok);
}

void QDenoiser::simple_squares(int size){
    setRendering(true);
    setStatus("Status: computing.");    
    emit stoptimer();
    setTimeSpent(0.0);
    emit starttimer();
    int m_size=(size-1)/2;
    QColor q;
    int c,z_red,z_blue,z_green;
    for (int y=0;y<n;y++){
        setProgress((y*100)/(n-1));
        for (int x=0;x<m;x++){
            if (isPaused()){
                emit stoptimer();
                while (isPaused()){
                    if (!isRendering()) setPaused(false);
                    sleep(1);
                }
                emit starttimer();
            }
            if (!isRendering()){
                setRendering(false);
                emit stoptimer();
                return;
            }
            z_red=0;
            z_blue=0;
            z_green=0;
            c=0;
            for (int i=0;i<size;i++){
                for (int j=0;j<size;j++){
                    if ((x-m_size+i>=0)&&(x-m_size+i<m)&&(y-m_size+j>=0)&&(y-m_size+j<n)){
                        q=input->pixel(x-m_size+i,y-m_size+j);
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
            output->setPixel(x,y,q.rgb());

        }
    }
    emit stoptimer();
    setStatus("Status: done.");
    setRendering(false);

}

// single-threaded classic NLM
void QDenoiser::NLM(QImage *inim,QImage *outim,QString settings, int* prog)
{
    // reading input settings
    int size_m=settings.section(" ",1,1).toInt(), size_b=settings.section(" ",2,2).toInt(), h=settings.section(" ",3,3).toInt(),
            beginY=settings.section(" ",4,4).toInt(), endY=settings.section(" ",5,5).toInt(),beginX=settings.section(" ",6,6).toInt(),
            endX=settings.section(" ",7,7).toInt();
    bool silent=true;
    if (settings.section(" ",8,8)=="false") silent=false;
    if (!silent) setRendering(true);
    if (!silent) setStatus("Status: computing.");
    if (!silent) emit stoptimer();
    if (!silent) setTimeSpent(0.0);
    if (!silent) emit starttimer();

    int m_size=(size_m-1)/2;
    int m_size_b=(size_b-1)/2;
    //int c=0;
    QColor p,q,fin,out,noize;
    // weight array and normalizing factor z
    rgb_my Weight[size_b][size_b],z;    
    // main cycle (begin with Y)
    for (int y=beginY;y<endY;y++){
        // visual updates of progress
        if(!silent) setProgress(1+((y-beginY)*100)/(endY-beginY-1));
        else *prog=((y-beginY)*100)/(endY-beginY-1);
        // sub cycle for X
        for (int x=beginX;x<endX;x++){
            // rendering and stop/pause processing
            if (isPaused()){
                if(!silent) emit stoptimer();
                while (isPaused()){
                    if (!isRendering()) setPaused(false);
                    sleep(1);
                }
                if(!silent) emit starttimer();
            }
            if (!isRendering()){
                setRendering(false);
                if(!silent) emit stoptimer();
                return;
            }
            // begin of algorithm; zeroing Z for every pixel
            z.setBlue(0);
            z.setGreen(0);
            z.setRed(0);
            // cycles for finding weights for pixel, going through weight array
            for (int i=0;i<size_b;i++){
                for(int j=0;j<size_b;j++){
                    // Zeroing it out
                    Weight[i][j].setRed(0);
                    Weight[i][j].setBlue(0);
                    Weight[i][j].setGreen(0);
                    // cycle to find value in weight array (looping through neighbourhood)
                    for(int a=0;a<size_m;a++){
                        for(int b=0;b<size_m;b++){
                            //if((x-m_size+a<m)&&(x-m_size+a>=0)&&(y-m_size+b<n)&&(y-m_size+b>-0)){
                                //if ((x-m_size_b-m_size+a+i<m)&&(x-m_size_b-m_size+a+i>=0)&&(y-m_size_b-m_size+b+j<n)&&(y-m_size_b-m_size+b+j>=0)){

                            int x_m=0,y_m=0,x_f=0,y_f=0,x_ff=0,y_ff=0;

                            // measures to ensure, that pixel is not out of bounds of neighbourhood window (surrounding current pixel)

                            if ((x-m_size+a>=endX)||(x-m_size+a<beginX)) x_m=x+m_size-a;
                            else x_m=x-m_size+a;
                            if ((y-m_size+b>=endY)||(y-m_size+b<beginY)) y_m=y+m_size-b;
                            else y_m=y-m_size+b;
                            p=inim->pixel(x_m,y_m);
                            // p - pixel - in the neighbourhood of current pixel

                            // measures to ensure, that pixel is not out of bounds of weight window and neighbourhood (surrounding current pixel in weight window)
                            if ((x-m_size_b+i>=endX)||(x-m_size_b+i<beginX)) x_f=x+m_size_b-i;
                            else x_f=x-m_size_b+i;
                            if ((y-m_size_b+j>=endY)||(y-m_size_b+j<beginY)) y_f=y+m_size_b-j;
                            else y_f=y-m_size_b+j;

                            if ((x_f-m_size+a>=endX)||(x_f-m_size+a<beginX)) x_ff=x_f+m_size-a;
                            else x_ff=x_f-m_size+a;
                            if ((y_f-m_size+b>=endY)||(y_f-m_size+b<beginY)) y_ff=y_f+m_size-b;
                            else y_ff=y_f-m_size+b;

                            q=inim->pixel(x_ff,y_ff);//*/
                            // q - pixel - in the neighbourhood of current pixel in weight window
                            // weight value equals sum of all squared differences of neighbourhood pixels
                            Weight[i][j].setRed(Weight[i][j].red()+pow(p.red()-q.red(),2));
                            Weight[i][j].setBlue(Weight[i][j].blue()+pow(p.blue()-q.blue(),2));
                            Weight[i][j].setGreen(Weight[i][j].green()+pow(p.green()-q.green(),2));
                        }
                    }
                    // getting en exponent of current pixel in weight matrix and divide it by squared h ( denoising power)
                    Weight[i][j].setRed(exp(-Weight[i][j].red()/pow(h,2)));
                    Weight[i][j].setBlue(exp(-Weight[i][j].blue()/pow(h,2)));
                    Weight[i][j].setGreen(exp(-Weight[i][j].green()/pow(h,2)));
                    // counting normalizing factor z
                    z.setRed(z.red()+Weight[i][j].red());
                    z.setBlue(z.blue()+Weight[i][j].blue());
                    z.setGreen(z.green()+Weight[i][j].green());


                }
            }            
            fin.setRed(0);
            fin.setBlue(0);
            fin.setGreen(0);
            outim->setPixel(x,y,fin.rgb());
            //cycle for counting final value of output pixel
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
            // updating image on the screen

        }

    }
    if (!silent) setStatus("Status: done.");
    if (!silent) emit stoptimer();
    if (!silent) setRendering(false);

/*
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  */
}



void QDenoiser::NLM_multiThread(QImage *inim,QImage *outim,QString settings)
{
    emit stoptimer();
    setTimeSpent(0.0);
    emit starttimer();
    int CPUnum=getCPUnum()-1;
    int *progressArr=new int[CPUnum];
    setRendering(true);
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
        QFuture<void> future =QtConcurrent::run(this,&QDenoiser::NLM,in_arr+i,out_arr+i,settings+sets2,progressArr+i);
        results.append(future);
    }
    QElapsedTimer merge;
    setStatus("Status: computing.");
    int finish=0;
    merge.start();


    while (finish!=CPUnum){
        if (isPaused()){
            emit stoptimer();
            while (isPaused()){
                if (!isRendering()) setPaused(false);
                sleep(1);
            }
            emit starttimer();
        }
        if (!isRendering()){
            setRendering(false);
            emit stoptimer();
            delete[] in_arr;
            delete[] out_arr;
            delete[] progressArr;
            return;
        }
        finish=0;
        for (int i=0;i<CPUnum;i++) if (results.at(i).isFinished()) finish++;
        if ((merge.elapsed()>30)&&!isPaused()){
            merge.restart();
            for (int i=0;i<CPUnum;i++)
                for (int n1=(i*(n)/CPUnum);n1<((i+1)*n)/CPUnum;n1++)
                    for (int m1=0;m1<m;m1++)
                        output->setPixel(m1,n1,out_arr[i].pixel(m1,n1));
            //QMetaObject::invokeMethod(this,"updatePixel");
            int percent=0;
            for (int i=0;i<CPUnum;i++) percent+=progressArr[i];
            setProgress(percent/CPUnum);
        }
        sleep(5);
    }
    setStatus("Status: done.");

    delete[] in_arr;

    for (int i=0;i<CPUnum;i++)
        for (int n1=(i*(n)/CPUnum);n1<((i+1)*n)/CPUnum;n1++)
            for (int m1=0;m1<m;m1++)
                output->setPixel(m1,n1,out_arr[i].pixel(m1,n1));
    delete[] out_arr;
    delete[] progressArr;
    emit stoptimer();
    setRendering(false);
}

void QDenoiser::NLM_fast(int size_m,int size_b,int h)
{
    setRendering(true);
    emit stoptimer();
    setTimeSpent(0.0);
    emit starttimer();

    int m_size=(size_m-1)/2;
    int m_size_b=(size_b-1)/2;
    // to avoid running out of RAM, checking for available ram memory

    if ((getAvailableSystemMemory()+getProcMemory())<((long)sizeof(rgb_my)*m*n*size_b*size_b)/(1024*1024)){
        popMessageBox(m,n,size_b);
        setRendering(false);
        return;
    }
    //QMetaObject::invokeMethod(this,"iconPause");

    setStatus("Status: creating 4d-array.");
    setProgress(0);


    //brainblowing creating 4d dynamic array.
    // probably wrap array into something like this
    //rgb_my* Sdx=new rgb_my[size_b*size_b*m*n];
    rgb_my**** Sdx=new rgb_my***[m];
    for (int i=0;i<m;i++){
        setProgress((i*100)/(m-1));
        Sdx[i]=new rgb_my**[n];
        for (int j=0;j<n;j++){
            Sdx[i][j]=new rgb_my *[size_b];
            for (int k=0;k<size_b;k++){
                Sdx[i][j][k]=new rgb_my[size_b];
            }
        }
    }//*/


    QColor p,q;

    setStatus("Status: computing 4d-array.");
    int t1,t2;
    for (int i=0;i<m;i++){
        setProgress((i*100)/(m-1));
        for (int j=0;j<n;j++){
            for (int k=0;k<size_b;k++){
                for (int l=0;l<size_b;l++){
                    t1=i+k-m_size_b;
                    t2=j+l-m_size_b;
                    if ((t1<0)||(t1>=m)) t1=i-k+m_size_b;
                    if ((t2<0)||(t2>=n)) t2=j-l+m_size_b;
                    if ((i==0)&&(j==0)){
                            p=input->pixel(i,j);
                            q=input->pixel(t1,t2);
                            Sdx[i][j][k][l].setRed(pow(p.red()-q.red(),2));
                            Sdx[i][j][k][l].setGreen(pow(p.green()-q.green(),2));
                            Sdx[i][j][k][l].setBlue(pow(p.blue()-q.blue(),2));

                    }
                    if ((i==0)&&(j!=0)){
                            p=input->pixel(i,j);
                            q=input->pixel(t1,t2);
                            Sdx[i][j][k][l].setRed(Sdx[i][j-1][k][l].red()+pow(p.red()-q.red(),2));
                            Sdx[i][j][k][l].setGreen(Sdx[i][j-1][k][l].green()+pow(p.green()-q.green(),2));
                            Sdx[i][j][k][l].setBlue(Sdx[i][j-1][k][l].blue()+pow(p.blue()-q.blue(),2));

                    }
                    if ((j==0)&&(i!=0)){
                            p=input->pixel(i,j);
                            q=input->pixel(t1,t2);
                            Sdx[i][j][k][l].setRed(Sdx[i-1][j][k][l].red()+pow(p.red()-q.red(),2));
                            Sdx[i][j][k][l].setGreen(Sdx[i-1][j][k][l].green()+pow(p.green()-q.green(),2));
                            Sdx[i][j][k][l].setBlue(Sdx[i-1][j][k][l].blue()+pow(p.blue()-q.blue(),2));

                    }
                    if ((j>0)&&(i>0)){
                            p=input->pixel(i,j);
                            q=input->pixel(t1,t2);
                            Sdx[i][j][k][l].setRed(-Sdx[i-1][j-1][k][l].red()+Sdx[i-1][j][k][l].red()+Sdx[i][j-1][k][l].red()+pow(p.red()-q.red(),2));
                            Sdx[i][j][k][l].setGreen(-Sdx[i-1][j-1][k][l].green()+Sdx[i-1][j][k][l].green()+Sdx[i][j-1][k][l].green()+pow(p.green()-q.green(),2));
                            Sdx[i][j][k][l].setBlue(-Sdx[i-1][j-1][k][l].blue()+Sdx[i-1][j][k][l].blue()+Sdx[i][j-1][k][l].blue()+pow(p.blue()-q.blue(),2));


                    }
                }
            }
        }
    }

    setStatus("Status: denoising image.");
    //int c=0;
    int x_p,x_m,y_p,y_m;
    QColor fin,out,noize;
    rgb_my Weight[size_b][size_b],z;

    for (int y=0;y<n;y++){
        setProgress((y*100)/(n-1));
        for (int x=0;x<m;x++){
            if (isPaused()){
                emit stoptimer();
                while (isPaused()){
                    if (!isRendering()) setPaused(false);
                    sleep(1);
                }
                emit starttimer();
            }
            if (!isRendering()){
                setRendering(false);

                setStatus("Status: deleting 4d-array.");
                for (int i=0;i<m;i++){
                    setProgress((i*100)/(m-1));
                    for (int j=0;j<n;j++){
                        for (int k=0;k<size_b;k++){
                            delete[] Sdx[i][j][k];
                        }
                        delete[] Sdx[i][j];
                    }
                    delete[] Sdx[i];

                }
                delete[] Sdx;//*/
                emit stoptimer();
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
            output->setPixel(x,y,fin.rgb());

            for (int ii=0;ii<size_b;ii++){
                for (int jj=0;jj<size_b;jj++){
                    //if ((x+ii-m_size_b<m)&&(x+ii-m_size_b>=0)&&(y+jj-m_size_b<n)&&(y+jj-m_size_b>=0)){
                        if ((x+ii-m_size_b>=m)||(x+ii-m_size_b<0)) if (y+jj-m_size_b<n) if (y+jj-m_size_b>=0)
                            noize=input->pixel(x-ii+m_size_b,y+jj-m_size_b);
                        if (x+ii-m_size_b<m) if (x+ii-m_size_b>=0) if ((y+jj-m_size_b>=n)||(y+jj-m_size_b<=0))
                            noize=input->pixel(x+ii-m_size_b,y-jj+m_size_b);
                        if (((x+ii-m_size_b>=m)||(x+ii-m_size_b<0))&&((y+jj-m_size_b>=n)||(y+jj-m_size_b<0)))
                            noize=input->pixel(x-ii+m_size_b,y-jj+m_size_b);
                        if (x+ii-m_size_b<m) if (x+ii-m_size_b>=0) if (y+jj-m_size_b<n) if (y+jj-m_size_b>=0)
                            noize=input->pixel(x+ii-m_size_b,y+jj-m_size_b);
                        Weight[ii][jj].setRed(Weight[ii][jj].red()*(noize.red())/z.red());
                        Weight[ii][jj].setBlue(Weight[ii][jj].blue()*(noize.blue())/z.blue());
                        Weight[ii][jj].setGreen(Weight[ii][jj].green()*(noize.green())/z.green());
                        fin.setRed(qRound(Weight[ii][jj].red()));
                        fin.setBlue(qRound(Weight[ii][jj].blue()));
                        fin.setGreen(qRound(Weight[ii][jj].green()));
                        out=output->pixel(x,y);
                        out.setRed(out.red()+fin.red());
                        out.setBlue(out.blue()+fin.blue());
                        out.setGreen(out.green()+fin.green());
                        output->setPixel(x,y,out.rgb());
                    //}

                }
            }
        }

    }

    setStatus("Status: deleting 4d-array.");
    for (int i=0;i<m;i++){
        setProgress((i*100)/(m-1));
        for (int j=0;j<n;j++){
            for (int k=0;k<size_b;k++){
                delete[] Sdx[i][j][k];
            }
            delete[] Sdx[i][j];
        }
        delete[] Sdx[i];

    }
    delete[] Sdx;//*/

    setStatus("Status: done.");
    emit stoptimer();
    setRendering(false);

}


void QDenoiser::NLM_fast_FFT(int size_m,int size_b,int h)
{
    emit stoptimer();
    setTimeSpent(0.0);
    emit starttimer();
    setRendering(true);
    int m_size=(size_m-1)/2;
    int m_size_b=(size_b-1)/2;
    //brainblowing creating 4d dynamic array.
    //rgb_my* Sdx=new rgb_my[size_b*size_b*m*n];

    //QMetaObject::invokeMethod(this,"iconPause");
    setStatus("Status: creating 4d-array.");
    setProgress(0);

    rgb_my** Sdx=new rgb_my*[m];
    for (int i=0;i<m;i++){
        setProgress((i*100)/(m-1));
        Sdx[i]=new rgb_my[n];
    }//*/


    QColor p;

    setStatus("Status: computing 2d-array.");
    for (int i=0;i<m;i++){
        setProgress((i*100)/(m-1));
        for (int j=0;j<n;j++){
            p=input->pixel(i,j);
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

    setStatus("Status: denoising image.");
    //int c=0;
    int x_p=0,x_m=0,y_p=0,y_m=0,diff_x=0,diff_y=0;
    rgb_my square_s,square_n;
    QColor fin,out,noize;
    rgb_my Weight[size_b][size_b],z;

    for (int y=0;y<n;y++){
        setProgress((y*100)/(n-1));
        for (int x=0;x<m;x++){
            if (isPaused()){
                emit stoptimer();
                while (isPaused()){
                    if (!isRendering()) setPaused(false);
                    sleep(1);
                }
                emit starttimer();
            }
            if (!isRendering()){
                setRendering(false);
                setStatus("Status: deleting 4d-array.");
                for (int i=0;i<m;i++){
                    setProgress((i*100)/(m-1));
                    delete[] Sdx[i];
                }
                delete[] Sdx;//*/
                emit stoptimer();
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


                    // fft here
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
            output->setPixel(x,y,fin.rgb());

            for (int ii=0;ii<size_b;ii++){
                for (int jj=0;jj<size_b;jj++){
                    //if ((x+ii-m_size_b<m)&&(x+ii-m_size_b>=0)&&(y+jj-m_size_b<n)&&(y+jj-m_size_b>=0)){
                        if ((x+ii-m_size_b>=m)||(x+ii-m_size_b<0)) if (y+jj-m_size_b<n) if (y+jj-m_size_b>=0)
                            noize=input->pixel(x-ii+m_size_b,y+jj-m_size_b);
                        if (x+ii-m_size_b<m) if (x+ii-m_size_b>=0) if ((y+jj-m_size_b>=n)||(y+jj-m_size_b<=0))
                            noize=input->pixel(x+ii-m_size_b,y-jj+m_size_b);
                        if (((x+ii-m_size_b>=m)||(x+ii-m_size_b<0))&&((y+jj-m_size_b>=n)||(y+jj-m_size_b<0)))
                            noize=input->pixel(x-ii+m_size_b,y-jj+m_size_b);
                        if (x+ii-m_size_b<m) if (x+ii-m_size_b>=0) if (y+jj-m_size_b<n) if (y+jj-m_size_b>=0)
                            noize=input->pixel(x+ii-m_size_b,y+jj-m_size_b);
                        Weight[ii][jj].setRed(Weight[ii][jj].red()*(noize.red())/z.red());
                        Weight[ii][jj].setBlue(Weight[ii][jj].blue()*(noize.blue())/z.blue());
                        Weight[ii][jj].setGreen(Weight[ii][jj].green()*(noize.green())/z.green());
                        fin.setRed(qRound(Weight[ii][jj].red()));
                        fin.setBlue(qRound(Weight[ii][jj].blue()));
                        fin.setGreen(qRound(Weight[ii][jj].green()));
                        out=output->pixel(x,y);
                        out.setRed(out.red()+fin.red());
                        out.setBlue(out.blue()+fin.blue());
                        out.setGreen(out.green()+fin.green());
                        output->setPixel(x,y,out.rgb());
                    //}

                }
            }
        }

    }

    setStatus("Status: deleting 4d-array.");

    for (int i=0;i<m;i++){
        setProgress((i*100)/(m-1));
        delete[] Sdx[i];

    }
    delete[] Sdx;//*/
    emit stoptimer();
    setStatus("Status: done.");
    setRendering(false);
}
