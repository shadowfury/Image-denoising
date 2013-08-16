#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "denoise/noisewidget.h"
#include "denoise/denoisewidget.h"
#include "denoise/qdenoiser.h"
#include "blur/blurwidget.h"
#include "droparea.h"
#include "RgbMy.h"
#include "settingwidget.h"

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    settingWidget *sw;
    noiseWidget* nw;
    denoiseWidget* dw;
    blurWidget* bw;
    DropArea *dropArea;

    QImage *Orig,*Noise,*Output,*Comparison;
    noiseClass* noiseSettings;
    denoiseClass* denoiseSettings;
    blurClass* blurSettings;
    QDenoiser *denoiser;
    bool isPaused,isRendering;
    
private:
    Ui::MainWindow *ui;
public slots:
    void on_openButton_clicked();
    void image_dropped();

    void on_noiseButton_clicked();
    void on_noiseDetailsButton_clicked();
    void on_denoiseButton_clicked();
    void on_denoiseDetailsButton_clicked();
    void on_cancelButton_clicked();

    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();

    void popMessageBox(QString line);
    void timeout_slot();


private slots:

    void add_noise_n(QImage *original,QImage *noised,noiseClass* noiseSettings);
    void add_noise(QImage *original,QImage *noised,noiseClass* noiseSettings);
    QImage blurred(const QImage& image, const QRect& rect, int radius, bool alphaOnly=false);
    int noisePixelplus(int noiseVal,int pixVal,int prob);
    int noisePixelminus(int noiseVal,int pixVal,int prob);
    float RandNorm( int num );


    void closeEvent(QCloseEvent* ev);
    void select_m(QAction*);
    void denoise_select_m();
    void activeChanged(int comboIndex);

    void finished(double diff);








    void on_blurButton_clicked();
};

#endif // MAINWINDOW_H
