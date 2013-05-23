/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Thu May 23 10:56:28 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QCommandLinkButton>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QScrollArea>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QWidget>
#include "droparea.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionMethod1;
    QAction *actionNon_local_means;
    QAction *actionLocal_means;
    QAction *actionWinner_s_method;
    QAction *actionInterpolation_method;
    QAction *actionNlm_fast_FFT;
    QAction *actionAbout;
    QAction *actionNlm_classic_with_multi_threads;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QLabel *statusLabel;
    QLabel *elapsedLabel;
    QProgressBar *progressBar;
    QSpacerItem *horizontalSpacer;
    QPushButton *cancelButton;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGridLayout *gridLayout_5;
    QPushButton *openButton;
    QSpacerItem *horizontalSpacer_2;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents_3;
    QGridLayout *gridLayout_6;
    DropArea *originalLabel;
    QWidget *tab_2;
    QGridLayout *gridLayout_2;
    QScrollArea *scrollArea_2;
    QWidget *scrollAreaWidgetContents;
    QGridLayout *gridLayout_7;
    QLabel *noisedLabel;
    QPushButton *noiseButton;
    QPushButton *noiseDetailsButton;
    QPushButton *blurButton;
    QWidget *tab_3;
    QGridLayout *gridLayout_3;
    QPushButton *denoiseButton;
    QScrollArea *scrollArea_3;
    QWidget *scrollAreaWidgetContents_2;
    QGridLayout *gridLayout_8;
    QLabel *denoisedLabel;
    QPushButton *denoiseDetailsButton;
    QWidget *tab_4;
    QGridLayout *gridLayout_4;
    QPushButton *pushButton_6;
    QPushButton *pushButton_5;
    QLineEdit *lineEdit_20;
    QLineEdit *lineEdit_19;
    QLabel *label_28;
    QLineEdit *lineEdit_18;
    QScrollArea *scrollArea_4;
    QWidget *scrollAreaWidgetContents_4;
    QGridLayout *gridLayout_9;
    QLabel *label_9;
    QLabel *label_29;
    QLabel *label_24;
    QWidget *tab_5;
    QGridLayout *gridLayout_10;
    QPushButton *pushButton_12;
    QScrollArea *scrollArea_5;
    QWidget *scrollAreaWidgetContents_5;
    QGridLayout *gridLayout_11;
    QLabel *label_15;
    QCommandLinkButton *commandLinkButton_2;
    QLineEdit *lineEdit_7;
    QCommandLinkButton *commandLinkButton;
    QWidget *tab_10;
    QLabel *label_7;
    QGridLayout *gridLayout_12;
    QSlider *horizontalSlider_2;
    QLabel *label_18;
    QLineEdit *lineEdit_10;
    QLabel *label_14;
    QLineEdit *lineEdit_9;
    QLabel *label_17;
    QComboBox *comboBox_2;
    QPushButton *pushButton_7;
    QScrollArea *scrollArea_6;
    QWidget *scrollAreaWidgetContents_6;
    QGridLayout *gridLayout_13;
    QLabel *label_16;
    QCommandLinkButton *commandLinkButton_4;
    QLineEdit *lineEdit_8;
    QCommandLinkButton *commandLinkButton_3;
    QLineEdit *lineEdit_11;
    QWidget *tab_11;
    QGridLayout *gridLayout_14;
    QPushButton *pushButton_14;
    QLabel *label_25;
    QLineEdit *lineEdit_16;
    QLabel *label_26;
    QLineEdit *lineEdit_17;
    QLabel *label_27;
    QScrollArea *scrollArea_7;
    QWidget *scrollAreaWidgetContents_7;
    QGridLayout *gridLayout_15;
    QLabel *label_19;
    QCommandLinkButton *commandLinkButton_6;
    QLineEdit *lineEdit_12;
    QCommandLinkButton *commandLinkButton_5;
    QWidget *tab_12;
    QGridLayout *gridLayout_16;
    QPushButton *pushButton_15;
    QPushButton *pushButton_16;
    QLabel *label_21;
    QLineEdit *lineEdit_13;
    QLabel *label_22;
    QLineEdit *lineEdit_14;
    QLabel *label_23;
    QLineEdit *lineEdit_15;
    QScrollArea *scrollArea_8;
    QWidget *scrollAreaWidgetContents_8;
    QGridLayout *gridLayout_17;
    QLabel *label_20;
    QStatusBar *statusBar;
    QMenuBar *menuBar;
    QMenu *menuItem;
    QMenu *menuHelp;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(680, 603);
        actionMethod1 = new QAction(MainWindow);
        actionMethod1->setObjectName(QString::fromUtf8("actionMethod1"));
        actionNon_local_means = new QAction(MainWindow);
        actionNon_local_means->setObjectName(QString::fromUtf8("actionNon_local_means"));
        actionLocal_means = new QAction(MainWindow);
        actionLocal_means->setObjectName(QString::fromUtf8("actionLocal_means"));
        actionWinner_s_method = new QAction(MainWindow);
        actionWinner_s_method->setObjectName(QString::fromUtf8("actionWinner_s_method"));
        actionInterpolation_method = new QAction(MainWindow);
        actionInterpolation_method->setObjectName(QString::fromUtf8("actionInterpolation_method"));
        actionNlm_fast_FFT = new QAction(MainWindow);
        actionNlm_fast_FFT->setObjectName(QString::fromUtf8("actionNlm_fast_FFT"));
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionNlm_classic_with_multi_threads = new QAction(MainWindow);
        actionNlm_classic_with_multi_threads->setObjectName(QString::fromUtf8("actionNlm_classic_with_multi_threads"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        statusLabel = new QLabel(centralWidget);
        statusLabel->setObjectName(QString::fromUtf8("statusLabel"));
        statusLabel->setMaximumSize(QSize(16777215, 23));
        statusLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(statusLabel, 1, 0, 1, 1);

        elapsedLabel = new QLabel(centralWidget);
        elapsedLabel->setObjectName(QString::fromUtf8("elapsedLabel"));
        elapsedLabel->setMinimumSize(QSize(160, 0));
        elapsedLabel->setMaximumSize(QSize(16777215, 23));

        gridLayout->addWidget(elapsedLabel, 1, 1, 1, 2);

        progressBar = new QProgressBar(centralWidget);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setMaximumSize(QSize(16777215, 23));
        progressBar->setValue(0);

        gridLayout->addWidget(progressBar, 2, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(5, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 2, 1, 1, 1);

        cancelButton = new QPushButton(centralWidget);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setMinimumSize(QSize(150, 0));
        cancelButton->setMaximumSize(QSize(250, 23));

        gridLayout->addWidget(cancelButton, 2, 2, 1, 1);

        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setStyleSheet(QString::fromUtf8("QTabWidget::pane { /* The tab widget frame */\n"
"     border-top: 2px solid #C2C7CB;\n"
"	 border-bottom: 2px solid #C2C7CB;\n"
"	 border-left: 2px solid #C2C7CB;\n"
"     border-right:2px solid #C2C7CB;\n"
" }\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
""));
        tabWidget->setTabPosition(QTabWidget::North);
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        gridLayout_5 = new QGridLayout(tab);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        openButton = new QPushButton(tab);
        openButton->setObjectName(QString::fromUtf8("openButton"));
        openButton->setMaximumSize(QSize(16777215, 16777215));
        openButton->setAutoDefault(false);
        openButton->setDefault(false);

        gridLayout_5->addWidget(openButton, 0, 0, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(501, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_5->addItem(horizontalSpacer_2, 0, 1, 1, 1);

        scrollArea = new QScrollArea(tab);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(scrollArea->sizePolicy().hasHeightForWidth());
        scrollArea->setSizePolicy(sizePolicy);
        scrollArea->setMinimumSize(QSize(0, 0));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents_3 = new QWidget();
        scrollAreaWidgetContents_3->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_3"));
        scrollAreaWidgetContents_3->setGeometry(QRect(0, 0, 638, 399));
        sizePolicy.setHeightForWidth(scrollAreaWidgetContents_3->sizePolicy().hasHeightForWidth());
        scrollAreaWidgetContents_3->setSizePolicy(sizePolicy);
        scrollAreaWidgetContents_3->setMinimumSize(QSize(0, 0));
        gridLayout_6 = new QGridLayout(scrollAreaWidgetContents_3);
        gridLayout_6->setSpacing(6);
        gridLayout_6->setContentsMargins(11, 11, 11, 11);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        originalLabel = new DropArea(scrollAreaWidgetContents_3);
        originalLabel->setObjectName(QString::fromUtf8("originalLabel"));
        sizePolicy.setHeightForWidth(originalLabel->sizePolicy().hasHeightForWidth());
        originalLabel->setSizePolicy(sizePolicy);
        originalLabel->setMinimumSize(QSize(0, 0));
        originalLabel->setFocusPolicy(Qt::NoFocus);
        originalLabel->setScaledContents(false);
        originalLabel->setAlignment(Qt::AlignCenter);

        gridLayout_6->addWidget(originalLabel, 1, 0, 1, 1);

        scrollArea->setWidget(scrollAreaWidgetContents_3);

        gridLayout_5->addWidget(scrollArea, 1, 0, 1, 2);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        gridLayout_2 = new QGridLayout(tab_2);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        scrollArea_2 = new QScrollArea(tab_2);
        scrollArea_2->setObjectName(QString::fromUtf8("scrollArea_2"));
        scrollArea_2->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 638, 399));
        sizePolicy.setHeightForWidth(scrollAreaWidgetContents->sizePolicy().hasHeightForWidth());
        scrollAreaWidgetContents->setSizePolicy(sizePolicy);
        gridLayout_7 = new QGridLayout(scrollAreaWidgetContents);
        gridLayout_7->setSpacing(6);
        gridLayout_7->setContentsMargins(11, 11, 11, 11);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        noisedLabel = new QLabel(scrollAreaWidgetContents);
        noisedLabel->setObjectName(QString::fromUtf8("noisedLabel"));
        sizePolicy.setHeightForWidth(noisedLabel->sizePolicy().hasHeightForWidth());
        noisedLabel->setSizePolicy(sizePolicy);

        gridLayout_7->addWidget(noisedLabel, 0, 0, 1, 1);

        scrollArea_2->setWidget(scrollAreaWidgetContents);

        gridLayout_2->addWidget(scrollArea_2, 1, 0, 1, 8);

        noiseButton = new QPushButton(tab_2);
        noiseButton->setObjectName(QString::fromUtf8("noiseButton"));
        noiseButton->setMaximumSize(QSize(16777215, 16777215));

        gridLayout_2->addWidget(noiseButton, 0, 0, 1, 1);

        noiseDetailsButton = new QPushButton(tab_2);
        noiseDetailsButton->setObjectName(QString::fromUtf8("noiseDetailsButton"));
        noiseDetailsButton->setMaximumSize(QSize(16777215, 16777215));

        gridLayout_2->addWidget(noiseDetailsButton, 0, 2, 1, 1);

        blurButton = new QPushButton(tab_2);
        blurButton->setObjectName(QString::fromUtf8("blurButton"));

        gridLayout_2->addWidget(blurButton, 0, 1, 1, 1);

        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        gridLayout_3 = new QGridLayout(tab_3);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        denoiseButton = new QPushButton(tab_3);
        denoiseButton->setObjectName(QString::fromUtf8("denoiseButton"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(denoiseButton->sizePolicy().hasHeightForWidth());
        denoiseButton->setSizePolicy(sizePolicy1);
        denoiseButton->setMaximumSize(QSize(16777215, 16777215));

        gridLayout_3->addWidget(denoiseButton, 4, 0, 1, 1);

        scrollArea_3 = new QScrollArea(tab_3);
        scrollArea_3->setObjectName(QString::fromUtf8("scrollArea_3"));
        scrollArea_3->setMinimumSize(QSize(0, 0));
        scrollArea_3->setWidgetResizable(true);
        scrollAreaWidgetContents_2 = new QWidget();
        scrollAreaWidgetContents_2->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_2"));
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 638, 399));
        sizePolicy.setHeightForWidth(scrollAreaWidgetContents_2->sizePolicy().hasHeightForWidth());
        scrollAreaWidgetContents_2->setSizePolicy(sizePolicy);
        gridLayout_8 = new QGridLayout(scrollAreaWidgetContents_2);
        gridLayout_8->setSpacing(6);
        gridLayout_8->setContentsMargins(11, 11, 11, 11);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        denoisedLabel = new QLabel(scrollAreaWidgetContents_2);
        denoisedLabel->setObjectName(QString::fromUtf8("denoisedLabel"));
        sizePolicy.setHeightForWidth(denoisedLabel->sizePolicy().hasHeightForWidth());
        denoisedLabel->setSizePolicy(sizePolicy);
        denoisedLabel->setMinimumSize(QSize(0, 0));

        gridLayout_8->addWidget(denoisedLabel, 1, 0, 1, 1);

        scrollArea_3->setWidget(scrollAreaWidgetContents_2);

        gridLayout_3->addWidget(scrollArea_3, 6, 0, 1, 9);

        denoiseDetailsButton = new QPushButton(tab_3);
        denoiseDetailsButton->setObjectName(QString::fromUtf8("denoiseDetailsButton"));
        denoiseDetailsButton->setMaximumSize(QSize(16777215, 16777215));

        gridLayout_3->addWidget(denoiseDetailsButton, 4, 1, 1, 1);

        tabWidget->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        gridLayout_4 = new QGridLayout(tab_4);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        pushButton_6 = new QPushButton(tab_4);
        pushButton_6->setObjectName(QString::fromUtf8("pushButton_6"));
        pushButton_6->setMaximumSize(QSize(16777215, 16777215));

        gridLayout_4->addWidget(pushButton_6, 0, 0, 1, 1);

        pushButton_5 = new QPushButton(tab_4);
        pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(pushButton_5->sizePolicy().hasHeightForWidth());
        pushButton_5->setSizePolicy(sizePolicy2);
        pushButton_5->setMaximumSize(QSize(16777215, 16777215));

        gridLayout_4->addWidget(pushButton_5, 0, 1, 1, 1);

        lineEdit_20 = new QLineEdit(tab_4);
        lineEdit_20->setObjectName(QString::fromUtf8("lineEdit_20"));
        lineEdit_20->setMaximumSize(QSize(60, 23));

        gridLayout_4->addWidget(lineEdit_20, 0, 3, 1, 1);

        lineEdit_19 = new QLineEdit(tab_4);
        lineEdit_19->setObjectName(QString::fromUtf8("lineEdit_19"));
        lineEdit_19->setMaximumSize(QSize(60, 23));

        gridLayout_4->addWidget(lineEdit_19, 0, 6, 1, 1);

        label_28 = new QLabel(tab_4);
        label_28->setObjectName(QString::fromUtf8("label_28"));

        gridLayout_4->addWidget(label_28, 0, 7, 1, 1);

        lineEdit_18 = new QLineEdit(tab_4);
        lineEdit_18->setObjectName(QString::fromUtf8("lineEdit_18"));
        lineEdit_18->setMaximumSize(QSize(60, 23));

        gridLayout_4->addWidget(lineEdit_18, 0, 8, 1, 1);

        scrollArea_4 = new QScrollArea(tab_4);
        scrollArea_4->setObjectName(QString::fromUtf8("scrollArea_4"));
        scrollArea_4->setWidgetResizable(true);
        scrollAreaWidgetContents_4 = new QWidget();
        scrollAreaWidgetContents_4->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_4"));
        scrollAreaWidgetContents_4->setGeometry(QRect(0, 0, 638, 399));
        sizePolicy.setHeightForWidth(scrollAreaWidgetContents_4->sizePolicy().hasHeightForWidth());
        scrollAreaWidgetContents_4->setSizePolicy(sizePolicy);
        gridLayout_9 = new QGridLayout(scrollAreaWidgetContents_4);
        gridLayout_9->setSpacing(6);
        gridLayout_9->setContentsMargins(11, 11, 11, 11);
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        label_9 = new QLabel(scrollAreaWidgetContents_4);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        sizePolicy.setHeightForWidth(label_9->sizePolicy().hasHeightForWidth());
        label_9->setSizePolicy(sizePolicy);

        gridLayout_9->addWidget(label_9, 0, 0, 1, 1);

        scrollArea_4->setWidget(scrollAreaWidgetContents_4);

        gridLayout_4->addWidget(scrollArea_4, 1, 0, 1, 9);

        label_29 = new QLabel(tab_4);
        label_29->setObjectName(QString::fromUtf8("label_29"));

        gridLayout_4->addWidget(label_29, 0, 5, 1, 1);

        label_24 = new QLabel(tab_4);
        label_24->setObjectName(QString::fromUtf8("label_24"));

        gridLayout_4->addWidget(label_24, 0, 2, 1, 1);

        tabWidget->addTab(tab_4, QString());
        tab_5 = new QWidget();
        tab_5->setObjectName(QString::fromUtf8("tab_5"));
        gridLayout_10 = new QGridLayout(tab_5);
        gridLayout_10->setSpacing(6);
        gridLayout_10->setContentsMargins(11, 11, 11, 11);
        gridLayout_10->setObjectName(QString::fromUtf8("gridLayout_10"));
        pushButton_12 = new QPushButton(tab_5);
        pushButton_12->setObjectName(QString::fromUtf8("pushButton_12"));
        sizePolicy2.setHeightForWidth(pushButton_12->sizePolicy().hasHeightForWidth());
        pushButton_12->setSizePolicy(sizePolicy2);
        pushButton_12->setMaximumSize(QSize(141, 23));
        pushButton_12->setAutoDefault(false);
        pushButton_12->setDefault(false);

        gridLayout_10->addWidget(pushButton_12, 0, 0, 1, 1);

        scrollArea_5 = new QScrollArea(tab_5);
        scrollArea_5->setObjectName(QString::fromUtf8("scrollArea_5"));
        sizePolicy.setHeightForWidth(scrollArea_5->sizePolicy().hasHeightForWidth());
        scrollArea_5->setSizePolicy(sizePolicy);
        scrollArea_5->setMinimumSize(QSize(0, 0));
        scrollArea_5->setWidgetResizable(true);
        scrollAreaWidgetContents_5 = new QWidget();
        scrollAreaWidgetContents_5->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_5"));
        scrollAreaWidgetContents_5->setGeometry(QRect(0, 0, 638, 403));
        sizePolicy.setHeightForWidth(scrollAreaWidgetContents_5->sizePolicy().hasHeightForWidth());
        scrollAreaWidgetContents_5->setSizePolicy(sizePolicy);
        scrollAreaWidgetContents_5->setMinimumSize(QSize(0, 0));
        gridLayout_11 = new QGridLayout(scrollAreaWidgetContents_5);
        gridLayout_11->setSpacing(6);
        gridLayout_11->setContentsMargins(11, 11, 11, 11);
        gridLayout_11->setObjectName(QString::fromUtf8("gridLayout_11"));
        label_15 = new QLabel(scrollAreaWidgetContents_5);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        sizePolicy.setHeightForWidth(label_15->sizePolicy().hasHeightForWidth());
        label_15->setSizePolicy(sizePolicy);
        label_15->setMinimumSize(QSize(0, 0));

        gridLayout_11->addWidget(label_15, 0, 0, 1, 3);

        commandLinkButton_2 = new QCommandLinkButton(scrollAreaWidgetContents_5);
        commandLinkButton_2->setObjectName(QString::fromUtf8("commandLinkButton_2"));
        commandLinkButton_2->setMaximumSize(QSize(35, 35));

        gridLayout_11->addWidget(commandLinkButton_2, 1, 0, 1, 1);

        lineEdit_7 = new QLineEdit(scrollAreaWidgetContents_5);
        lineEdit_7->setObjectName(QString::fromUtf8("lineEdit_7"));
        lineEdit_7->setEnabled(false);
        lineEdit_7->setMaximumSize(QSize(16777215, 35));
        lineEdit_7->setAlignment(Qt::AlignCenter);

        gridLayout_11->addWidget(lineEdit_7, 1, 1, 1, 1);

        commandLinkButton = new QCommandLinkButton(scrollAreaWidgetContents_5);
        commandLinkButton->setObjectName(QString::fromUtf8("commandLinkButton"));
        commandLinkButton->setMaximumSize(QSize(35, 35));

        gridLayout_11->addWidget(commandLinkButton, 1, 2, 1, 1);

        scrollArea_5->setWidget(scrollAreaWidgetContents_5);

        gridLayout_10->addWidget(scrollArea_5, 1, 0, 2, 2);

        tabWidget->addTab(tab_5, QString());
        tab_10 = new QWidget();
        tab_10->setObjectName(QString::fromUtf8("tab_10"));
        label_7 = new QLabel(tab_10);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(120, 110, 211, 191));
        gridLayout_12 = new QGridLayout(tab_10);
        gridLayout_12->setSpacing(6);
        gridLayout_12->setContentsMargins(11, 11, 11, 11);
        gridLayout_12->setObjectName(QString::fromUtf8("gridLayout_12"));
        horizontalSlider_2 = new QSlider(tab_10);
        horizontalSlider_2->setObjectName(QString::fromUtf8("horizontalSlider_2"));
        horizontalSlider_2->setMaximumSize(QSize(16777215, 23));
        horizontalSlider_2->setMinimum(-255);
        horizontalSlider_2->setMaximum(255);
        horizontalSlider_2->setTracking(true);
        horizontalSlider_2->setOrientation(Qt::Horizontal);

        gridLayout_12->addWidget(horizontalSlider_2, 0, 0, 1, 1);

        label_18 = new QLabel(tab_10);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        sizePolicy2.setHeightForWidth(label_18->sizePolicy().hasHeightForWidth());
        label_18->setSizePolicy(sizePolicy2);

        gridLayout_12->addWidget(label_18, 0, 1, 1, 1);

        lineEdit_10 = new QLineEdit(tab_10);
        lineEdit_10->setObjectName(QString::fromUtf8("lineEdit_10"));
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(51);
        sizePolicy3.setVerticalStretch(23);
        sizePolicy3.setHeightForWidth(lineEdit_10->sizePolicy().hasHeightForWidth());
        lineEdit_10->setSizePolicy(sizePolicy3);
        lineEdit_10->setMaximumSize(QSize(41, 23));

        gridLayout_12->addWidget(lineEdit_10, 0, 2, 1, 1);

        label_14 = new QLabel(tab_10);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        sizePolicy2.setHeightForWidth(label_14->sizePolicy().hasHeightForWidth());
        label_14->setSizePolicy(sizePolicy2);

        gridLayout_12->addWidget(label_14, 0, 3, 1, 1);

        lineEdit_9 = new QLineEdit(tab_10);
        lineEdit_9->setObjectName(QString::fromUtf8("lineEdit_9"));
        sizePolicy2.setHeightForWidth(lineEdit_9->sizePolicy().hasHeightForWidth());
        lineEdit_9->setSizePolicy(sizePolicy2);
        lineEdit_9->setMaximumSize(QSize(41, 23));

        gridLayout_12->addWidget(lineEdit_9, 0, 4, 1, 1);

        label_17 = new QLabel(tab_10);
        label_17->setObjectName(QString::fromUtf8("label_17"));

        gridLayout_12->addWidget(label_17, 0, 5, 1, 1);

        comboBox_2 = new QComboBox(tab_10);
        comboBox_2->setObjectName(QString::fromUtf8("comboBox_2"));
        comboBox_2->setMinimumSize(QSize(110, 23));
        comboBox_2->setMaximumSize(QSize(16777215, 23));

        gridLayout_12->addWidget(comboBox_2, 0, 8, 1, 1);

        pushButton_7 = new QPushButton(tab_10);
        pushButton_7->setObjectName(QString::fromUtf8("pushButton_7"));
        pushButton_7->setMaximumSize(QSize(16777215, 23));

        gridLayout_12->addWidget(pushButton_7, 0, 9, 1, 1);

        scrollArea_6 = new QScrollArea(tab_10);
        scrollArea_6->setObjectName(QString::fromUtf8("scrollArea_6"));
        sizePolicy.setHeightForWidth(scrollArea_6->sizePolicy().hasHeightForWidth());
        scrollArea_6->setSizePolicy(sizePolicy);
        scrollArea_6->setMinimumSize(QSize(0, 0));
        scrollArea_6->setWidgetResizable(true);
        scrollAreaWidgetContents_6 = new QWidget();
        scrollAreaWidgetContents_6->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_6"));
        scrollAreaWidgetContents_6->setGeometry(QRect(0, 0, 638, 403));
        sizePolicy.setHeightForWidth(scrollAreaWidgetContents_6->sizePolicy().hasHeightForWidth());
        scrollAreaWidgetContents_6->setSizePolicy(sizePolicy);
        scrollAreaWidgetContents_6->setMinimumSize(QSize(0, 0));
        gridLayout_13 = new QGridLayout(scrollAreaWidgetContents_6);
        gridLayout_13->setSpacing(6);
        gridLayout_13->setContentsMargins(11, 11, 11, 11);
        gridLayout_13->setObjectName(QString::fromUtf8("gridLayout_13"));
        label_16 = new QLabel(scrollAreaWidgetContents_6);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        sizePolicy.setHeightForWidth(label_16->sizePolicy().hasHeightForWidth());
        label_16->setSizePolicy(sizePolicy);
        label_16->setMinimumSize(QSize(0, 0));

        gridLayout_13->addWidget(label_16, 0, 0, 1, 3);

        commandLinkButton_4 = new QCommandLinkButton(scrollAreaWidgetContents_6);
        commandLinkButton_4->setObjectName(QString::fromUtf8("commandLinkButton_4"));
        sizePolicy2.setHeightForWidth(commandLinkButton_4->sizePolicy().hasHeightForWidth());
        commandLinkButton_4->setSizePolicy(sizePolicy2);
        commandLinkButton_4->setMaximumSize(QSize(35, 35));

        gridLayout_13->addWidget(commandLinkButton_4, 1, 0, 1, 1);

        lineEdit_8 = new QLineEdit(scrollAreaWidgetContents_6);
        lineEdit_8->setObjectName(QString::fromUtf8("lineEdit_8"));
        lineEdit_8->setEnabled(false);
        lineEdit_8->setMinimumSize(QSize(0, 35));
        lineEdit_8->setAlignment(Qt::AlignCenter);

        gridLayout_13->addWidget(lineEdit_8, 1, 1, 1, 1);

        commandLinkButton_3 = new QCommandLinkButton(scrollAreaWidgetContents_6);
        commandLinkButton_3->setObjectName(QString::fromUtf8("commandLinkButton_3"));
        sizePolicy2.setHeightForWidth(commandLinkButton_3->sizePolicy().hasHeightForWidth());
        commandLinkButton_3->setSizePolicy(sizePolicy2);
        commandLinkButton_3->setMaximumSize(QSize(35, 35));

        gridLayout_13->addWidget(commandLinkButton_3, 1, 2, 1, 1);

        scrollArea_6->setWidget(scrollAreaWidgetContents_6);

        gridLayout_12->addWidget(scrollArea_6, 1, 0, 1, 10);

        lineEdit_11 = new QLineEdit(tab_10);
        lineEdit_11->setObjectName(QString::fromUtf8("lineEdit_11"));
        sizePolicy2.setHeightForWidth(lineEdit_11->sizePolicy().hasHeightForWidth());
        lineEdit_11->setSizePolicy(sizePolicy2);
        lineEdit_11->setMinimumSize(QSize(0, 0));
        lineEdit_11->setMaximumSize(QSize(41, 23));

        gridLayout_12->addWidget(lineEdit_11, 0, 6, 1, 1);

        tabWidget->addTab(tab_10, QString());
        tab_11 = new QWidget();
        tab_11->setObjectName(QString::fromUtf8("tab_11"));
        gridLayout_14 = new QGridLayout(tab_11);
        gridLayout_14->setSpacing(6);
        gridLayout_14->setContentsMargins(11, 11, 11, 11);
        gridLayout_14->setObjectName(QString::fromUtf8("gridLayout_14"));
        pushButton_14 = new QPushButton(tab_11);
        pushButton_14->setObjectName(QString::fromUtf8("pushButton_14"));
        QSizePolicy sizePolicy4(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(pushButton_14->sizePolicy().hasHeightForWidth());
        pushButton_14->setSizePolicy(sizePolicy4);
        pushButton_14->setMaximumSize(QSize(71, 23));

        gridLayout_14->addWidget(pushButton_14, 0, 0, 1, 1);

        label_25 = new QLabel(tab_11);
        label_25->setObjectName(QString::fromUtf8("label_25"));
        label_25->setMaximumSize(QSize(16777215, 23));

        gridLayout_14->addWidget(label_25, 0, 1, 1, 1);

        lineEdit_16 = new QLineEdit(tab_11);
        lineEdit_16->setObjectName(QString::fromUtf8("lineEdit_16"));
        lineEdit_16->setMaximumSize(QSize(10000, 23));

        gridLayout_14->addWidget(lineEdit_16, 0, 2, 1, 1);

        label_26 = new QLabel(tab_11);
        label_26->setObjectName(QString::fromUtf8("label_26"));
        label_26->setMaximumSize(QSize(16777215, 23));

        gridLayout_14->addWidget(label_26, 0, 3, 1, 1);

        lineEdit_17 = new QLineEdit(tab_11);
        lineEdit_17->setObjectName(QString::fromUtf8("lineEdit_17"));
        lineEdit_17->setMaximumSize(QSize(10000, 23));

        gridLayout_14->addWidget(lineEdit_17, 0, 4, 1, 1);

        label_27 = new QLabel(tab_11);
        label_27->setObjectName(QString::fromUtf8("label_27"));
        label_27->setMaximumSize(QSize(16777215, 23));

        gridLayout_14->addWidget(label_27, 0, 5, 1, 1);

        scrollArea_7 = new QScrollArea(tab_11);
        scrollArea_7->setObjectName(QString::fromUtf8("scrollArea_7"));
        sizePolicy.setHeightForWidth(scrollArea_7->sizePolicy().hasHeightForWidth());
        scrollArea_7->setSizePolicy(sizePolicy);
        scrollArea_7->setMinimumSize(QSize(0, 0));
        scrollArea_7->setWidgetResizable(true);
        scrollAreaWidgetContents_7 = new QWidget();
        scrollAreaWidgetContents_7->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_7"));
        scrollAreaWidgetContents_7->setGeometry(QRect(0, 0, 638, 403));
        sizePolicy.setHeightForWidth(scrollAreaWidgetContents_7->sizePolicy().hasHeightForWidth());
        scrollAreaWidgetContents_7->setSizePolicy(sizePolicy);
        scrollAreaWidgetContents_7->setMinimumSize(QSize(0, 0));
        gridLayout_15 = new QGridLayout(scrollAreaWidgetContents_7);
        gridLayout_15->setSpacing(6);
        gridLayout_15->setContentsMargins(11, 11, 11, 11);
        gridLayout_15->setObjectName(QString::fromUtf8("gridLayout_15"));
        label_19 = new QLabel(scrollAreaWidgetContents_7);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        sizePolicy.setHeightForWidth(label_19->sizePolicy().hasHeightForWidth());
        label_19->setSizePolicy(sizePolicy);
        label_19->setMinimumSize(QSize(0, 0));

        gridLayout_15->addWidget(label_19, 0, 0, 1, 3);

        commandLinkButton_6 = new QCommandLinkButton(scrollAreaWidgetContents_7);
        commandLinkButton_6->setObjectName(QString::fromUtf8("commandLinkButton_6"));
        commandLinkButton_6->setMaximumSize(QSize(35, 35));

        gridLayout_15->addWidget(commandLinkButton_6, 1, 0, 1, 1);

        lineEdit_12 = new QLineEdit(scrollAreaWidgetContents_7);
        lineEdit_12->setObjectName(QString::fromUtf8("lineEdit_12"));
        lineEdit_12->setEnabled(false);
        lineEdit_12->setMinimumSize(QSize(0, 35));
        lineEdit_12->setAlignment(Qt::AlignCenter);

        gridLayout_15->addWidget(lineEdit_12, 1, 1, 1, 1);

        commandLinkButton_5 = new QCommandLinkButton(scrollAreaWidgetContents_7);
        commandLinkButton_5->setObjectName(QString::fromUtf8("commandLinkButton_5"));
        commandLinkButton_5->setMaximumSize(QSize(35, 35));

        gridLayout_15->addWidget(commandLinkButton_5, 1, 2, 1, 1);

        scrollArea_7->setWidget(scrollAreaWidgetContents_7);

        gridLayout_14->addWidget(scrollArea_7, 1, 0, 1, 6);

        tabWidget->addTab(tab_11, QString());
        tab_12 = new QWidget();
        tab_12->setObjectName(QString::fromUtf8("tab_12"));
        gridLayout_16 = new QGridLayout(tab_12);
        gridLayout_16->setSpacing(6);
        gridLayout_16->setContentsMargins(11, 11, 11, 11);
        gridLayout_16->setObjectName(QString::fromUtf8("gridLayout_16"));
        pushButton_15 = new QPushButton(tab_12);
        pushButton_15->setObjectName(QString::fromUtf8("pushButton_15"));
        pushButton_15->setMaximumSize(QSize(71, 23));

        gridLayout_16->addWidget(pushButton_15, 0, 0, 1, 1);

        pushButton_16 = new QPushButton(tab_12);
        pushButton_16->setObjectName(QString::fromUtf8("pushButton_16"));
        sizePolicy2.setHeightForWidth(pushButton_16->sizePolicy().hasHeightForWidth());
        pushButton_16->setSizePolicy(sizePolicy2);
        pushButton_16->setMaximumSize(QSize(71, 23));

        gridLayout_16->addWidget(pushButton_16, 0, 1, 1, 1);

        label_21 = new QLabel(tab_12);
        label_21->setObjectName(QString::fromUtf8("label_21"));
        label_21->setMaximumSize(QSize(16777215, 23));

        gridLayout_16->addWidget(label_21, 0, 2, 1, 1);

        lineEdit_13 = new QLineEdit(tab_12);
        lineEdit_13->setObjectName(QString::fromUtf8("lineEdit_13"));
        lineEdit_13->setMaximumSize(QSize(16777215, 23));

        gridLayout_16->addWidget(lineEdit_13, 0, 3, 1, 1);

        label_22 = new QLabel(tab_12);
        label_22->setObjectName(QString::fromUtf8("label_22"));
        label_22->setMaximumSize(QSize(16777215, 23));

        gridLayout_16->addWidget(label_22, 0, 4, 1, 1);

        lineEdit_14 = new QLineEdit(tab_12);
        lineEdit_14->setObjectName(QString::fromUtf8("lineEdit_14"));
        lineEdit_14->setMaximumSize(QSize(16777215, 23));

        gridLayout_16->addWidget(lineEdit_14, 0, 5, 1, 1);

        label_23 = new QLabel(tab_12);
        label_23->setObjectName(QString::fromUtf8("label_23"));
        label_23->setMaximumSize(QSize(16777215, 23));

        gridLayout_16->addWidget(label_23, 0, 6, 1, 1);

        lineEdit_15 = new QLineEdit(tab_12);
        lineEdit_15->setObjectName(QString::fromUtf8("lineEdit_15"));
        lineEdit_15->setMaximumSize(QSize(16777215, 23));

        gridLayout_16->addWidget(lineEdit_15, 0, 7, 1, 1);

        scrollArea_8 = new QScrollArea(tab_12);
        scrollArea_8->setObjectName(QString::fromUtf8("scrollArea_8"));
        sizePolicy.setHeightForWidth(scrollArea_8->sizePolicy().hasHeightForWidth());
        scrollArea_8->setSizePolicy(sizePolicy);
        scrollArea_8->setMinimumSize(QSize(0, 0));
        scrollArea_8->setWidgetResizable(true);
        scrollAreaWidgetContents_8 = new QWidget();
        scrollAreaWidgetContents_8->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_8"));
        scrollAreaWidgetContents_8->setGeometry(QRect(0, 0, 638, 403));
        sizePolicy.setHeightForWidth(scrollAreaWidgetContents_8->sizePolicy().hasHeightForWidth());
        scrollAreaWidgetContents_8->setSizePolicy(sizePolicy);
        scrollAreaWidgetContents_8->setMinimumSize(QSize(0, 0));
        gridLayout_17 = new QGridLayout(scrollAreaWidgetContents_8);
        gridLayout_17->setSpacing(6);
        gridLayout_17->setContentsMargins(11, 11, 11, 11);
        gridLayout_17->setObjectName(QString::fromUtf8("gridLayout_17"));
        label_20 = new QLabel(scrollAreaWidgetContents_8);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        sizePolicy.setHeightForWidth(label_20->sizePolicy().hasHeightForWidth());
        label_20->setSizePolicy(sizePolicy);
        label_20->setMinimumSize(QSize(0, 0));

        gridLayout_17->addWidget(label_20, 0, 0, 1, 1);

        scrollArea_8->setWidget(scrollAreaWidgetContents_8);

        gridLayout_16->addWidget(scrollArea_8, 1, 0, 1, 8);

        tabWidget->addTab(tab_12, QString());

        gridLayout->addWidget(tabWidget, 0, 0, 1, 3);

        MainWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 680, 25));
        menuItem = new QMenu(menuBar);
        menuItem->setObjectName(QString::fromUtf8("menuItem"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuItem->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuItem->addAction(actionMethod1);
        menuItem->addAction(actionNon_local_means);
        menuItem->addAction(actionLocal_means);
        menuItem->addAction(actionNlm_fast_FFT);
        menuItem->addAction(actionNlm_classic_with_multi_threads);
        menuHelp->addAction(actionAbout);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Denoising", 0, QApplication::UnicodeUTF8));
        actionMethod1->setText(QApplication::translate("MainWindow", "Simple squares method", 0, QApplication::UnicodeUTF8));
        actionNon_local_means->setText(QApplication::translate("MainWindow", "Non-local means method", 0, QApplication::UnicodeUTF8));
        actionLocal_means->setText(QApplication::translate("MainWindow", "Non-local means fast method", 0, QApplication::UnicodeUTF8));
        actionWinner_s_method->setText(QApplication::translate("MainWindow", "Wiener's method", 0, QApplication::UnicodeUTF8));
        actionInterpolation_method->setText(QApplication::translate("MainWindow", "Lagrange-like interpolation method", 0, QApplication::UnicodeUTF8));
        actionNlm_fast_FFT->setText(QApplication::translate("MainWindow", "Nlm-fast (FFT)", 0, QApplication::UnicodeUTF8));
        actionAbout->setText(QApplication::translate("MainWindow", "About", 0, QApplication::UnicodeUTF8));
        actionNlm_classic_with_multi_threads->setText(QApplication::translate("MainWindow", "Nlm (classic with multi-threads)", 0, QApplication::UnicodeUTF8));
        statusLabel->setText(QApplication::translate("MainWindow", "Status:", 0, QApplication::UnicodeUTF8));
        elapsedLabel->setText(QApplication::translate("MainWindow", "Time elapsed:     ", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("MainWindow", "Cancel", 0, QApplication::UnicodeUTF8));
        openButton->setText(QApplication::translate("MainWindow", "Open file", 0, QApplication::UnicodeUTF8));
        originalLabel->setText(QApplication::translate("MainWindow", "drop image here or click to \n"
" invoke file open dialog", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "Original", 0, QApplication::UnicodeUTF8));
        noisedLabel->setText(QString());
        noiseButton->setText(QApplication::translate("MainWindow", "Add Noise", 0, QApplication::UnicodeUTF8));
        noiseDetailsButton->setText(QApplication::translate("MainWindow", "Details", 0, QApplication::UnicodeUTF8));
        blurButton->setText(QApplication::translate("MainWindow", "Add Blur", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MainWindow", "Noised", 0, QApplication::UnicodeUTF8));
        denoiseButton->setText(QApplication::translate("MainWindow", "Denoise", 0, QApplication::UnicodeUTF8));
        denoisedLabel->setText(QString());
        denoiseDetailsButton->setText(QApplication::translate("MainWindow", "Details", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("MainWindow", "Denoised", 0, QApplication::UnicodeUTF8));
        pushButton_6->setText(QApplication::translate("MainWindow", "Difference", 0, QApplication::UnicodeUTF8));
        pushButton_5->setText(QApplication::translate("MainWindow", "Save results", 0, QApplication::UnicodeUTF8));
        label_28->setText(QApplication::translate("MainWindow", "Error blue=", 0, QApplication::UnicodeUTF8));
        label_9->setText(QString());
        label_29->setText(QApplication::translate("MainWindow", "Error green=", 0, QApplication::UnicodeUTF8));
        label_24->setText(QApplication::translate("MainWindow", "Error red=", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("MainWindow", "Comparison", 0, QApplication::UnicodeUTF8));
        pushButton_12->setText(QApplication::translate("MainWindow", "Select original images folder", 0, QApplication::UnicodeUTF8));
        label_15->setText(QString());
        commandLinkButton_2->setText(QString());
        commandLinkButton->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tab_5), QApplication::translate("MainWindow", "w_or", 0, QApplication::UnicodeUTF8));
        label_7->setText(QString());
        label_18->setText(QApplication::translate("MainWindow", "Red", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("MainWindow", "Green", 0, QApplication::UnicodeUTF8));
        label_17->setText(QApplication::translate("MainWindow", "Blue", 0, QApplication::UnicodeUTF8));
        comboBox_2->clear();
        comboBox_2->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Uniform distribution", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Normal distribution", 0, QApplication::UnicodeUTF8)
        );
        pushButton_7->setText(QApplication::translate("MainWindow", "Add Noise", 0, QApplication::UnicodeUTF8));
        label_16->setText(QString());
        commandLinkButton_4->setText(QString());
        commandLinkButton_3->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tab_10), QApplication::translate("MainWindow", "w_no", 0, QApplication::UnicodeUTF8));
        pushButton_14->setText(QApplication::translate("MainWindow", "Denoise", 0, QApplication::UnicodeUTF8));
        label_25->setText(QApplication::translate("MainWindow", "Denoise image nubmer:", 0, QApplication::UnicodeUTF8));
        label_26->setText(QApplication::translate("MainWindow", "Using", 0, QApplication::UnicodeUTF8));
        label_27->setText(QApplication::translate("MainWindow", "first images in the seria", 0, QApplication::UnicodeUTF8));
        label_19->setText(QString());
        commandLinkButton_6->setText(QString());
        commandLinkButton_5->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tab_11), QApplication::translate("MainWindow", "w_de", 0, QApplication::UnicodeUTF8));
        pushButton_15->setText(QApplication::translate("MainWindow", "Difference", 0, QApplication::UnicodeUTF8));
        pushButton_16->setText(QApplication::translate("MainWindow", "Save results", 0, QApplication::UnicodeUTF8));
        label_21->setText(QApplication::translate("MainWindow", "Error red=", 0, QApplication::UnicodeUTF8));
        label_22->setText(QApplication::translate("MainWindow", "Error green=", 0, QApplication::UnicodeUTF8));
        label_23->setText(QApplication::translate("MainWindow", "Error blue=", 0, QApplication::UnicodeUTF8));
        label_20->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tab_12), QApplication::translate("MainWindow", "w_co", 0, QApplication::UnicodeUTF8));
        menuItem->setTitle(QApplication::translate("MainWindow", "Select method", 0, QApplication::UnicodeUTF8));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
