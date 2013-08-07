#ifndef QDENOISER_H
#define QDENOISER_H
#include "Denoise.h"
#include "src/RgbMy.h"
#include <math.h>
#include <QImage>
#include <QColor>

#include <QMessageBox>
#include <QFile>
#include <QTextStream>

#include <QElapsedTimer>
#include <QTimer>

#include <QObject>
#include <QFuture>
#include <QtConcurrentRun>

#include <QMutex>
#include <QMutexLocker>

class QDenoiser: public QObject
{
    Q_OBJECT

private:

    bool rendering,paused;                  // private variables, states
    QImage *input,*output;                  // input and output images
    QString _status;                        // holds status, what is going now
    denoiseClass* sets;                     // denoising settings
    int progress,m,n;                       // private variables, holds progress, and input/output images size
    double time_computing;                  // time spent computing variable
    denoiseClass::method denoisingMethod;   // holds what method is selected
    QTimer *computingTime;                  // timer, counts computing time
    QMutex mutex;                           // to make class more thread safe

public:
    QDenoiser();                            // default constructor
    QDenoiser(QImage* in,denoiseClass* settings);
    ~QDenoiser();
private slots:
    /* denoising methods */
    void simple_squares(int size);
    void NLM(QImage *inim,QImage *outim,QString settings, int* prog);
    void NLM_multiThread(QImage *inim,QImage *outim,QString settings);
    void NLM_fast(int size_m,int size_b,int h);
    void NLM_fast_FFT(int size_m,int size_b,int h);
    /* pops Message box if not enough RAM */
    void popMessageBox(int m1,int n1,int size_b);
    /* sets progress */
    void setProgress(int prog);
    void setTimeSpent(double ms=0.0);
    void updateTimer();

signals:
    void stoptimer();
    void starttimer();

public slots:
    /* setter functions, set corresponding private variables */
    void setRendering(bool state);
    void setPaused(bool state);
    void setStatus(QString status);
    void setImage(QImage* inim);
    void setSettings(denoiseClass* settings);
    void setDenoisingMethod(denoiseClass::method val);

    /* get functiong, return corresponding private variables */
    denoiseClass::method getDenoisingMethod();
    denoiseClass* getSettings();
    QImage* getImage();
    int getProgress();
    QString getStatus();
    bool isRendering();
    bool isPaused();
    double getTimeSpent();


    /* control methods - start, pause, resume and end rendering */
    void startRender();
    void pauseRender();
    void resumeRender();
    void cancelRender();
};

#endif // QDENOISER_H
