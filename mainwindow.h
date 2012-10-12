#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "noisewidget.h"
#include "denoisewidget.h"
#include "RgbMy.h"

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    int index;
    noiseWidget* nw;
    denoiseWidget* dw;
    
private:
    Ui::MainWindow *ui;
public slots:
    void on_openButton_clicked();
    void on_noiseButton_clicked();
    void on_noiseDetailsButton_clicked();
    void on_denoiseButton_clicked();
    void on_denoiseDetailsButton_clicked();
    void on_cancelButton_clicked();

    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();

    void simple_squares_method(int size);
    void non_local_means_method(int size_m,int size_b,int h);
    void non_local_means_method_fast(int size_m,int size_b,int h);
    void Nlm_fast_FFT(int size_m,int size_b,int h);



private slots:

    void add_noise_n(QImage *original,QImage *noised,noiseClass* noiseSettings);
    void add_noise(QImage *original,QImage *noised,noiseClass* noiseSettings);
    int noisePixelplus(int noiseVal,int pixVal,int prob);
    int noisePixelminus(int noiseVal,int pixVal,int prob);
    void closeEvent(QCloseEvent* ev);
    void select_m(QAction*);
    void denoise_select_m();
    void activeChanged(int comboIndex);

    void finished(double diff);
    void updateStatus(QString status);
    void updateProgress(int progress);
    void updatePixel();
    void popMessageBox(int m1,int n1,int size_b);
    void iconPause();







};

#endif // MAINWINDOW_H
