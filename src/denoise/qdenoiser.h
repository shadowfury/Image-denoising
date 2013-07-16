#ifndef QDENOISER_H
#define QDENOISER_H
#include "Denoise.h"
#include "src/RgbMy.h"
#include <math.h>
#include <QImage>
#include <QMessageBox>
#include <QFuture>
#include <QtConcurrentRun>
#include <QFile>
#include <QElapsedTimer>
#include <QTextStream>
#include <QColor>

class QDenoiser
{
private:
    bool rendering,paused;                  // private variables, states
    QImage *input,*output;                  // input and output images
    QString _status;                        // holds status, what is going now
    denoiseClass* sets;                     // denoising settings
    int progress,m,n;                       // private variables, holds progress, and input/output images size
    denoiseClass::method denoisingMethod;   // holds what method is selected

public:
    QDenoiser();                            // default constructor
    QDenoiser(QImage* in,denoiseClass* settings);
private slots:
    /* denoising methods */
    void simple_squares(int size);
    void NLM(QImage *inim,QImage *outim,QString settings, int* progress);
    void NLM_multyThread(QImage *inim,QImage *outim,QString settings);
    void NLM_fast(int size_m,int size_b,int h);
    void NLM_fast_FFT(int size_m,int size_b,int h);
    /* pops Message box if not enough RAM */
    void popMessageBox(int m1,int n1,int size_b);
    /* sets progress */
    void setProgress(int prog);

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

    /* control methods - start, pause, resume and end rendering */
    void startRender();
    void pauseRender();
    void resumeRender();
    void cancelRender();
};

#endif // QDENOISER_H
