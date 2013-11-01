#ifndef CONSOLEPARSER_H
#define CONSOLEPARSER_H

#include <QObject>
#include <QStringList>
#include "../denoise/Denoise.h"

class ConsoleParser : public QObject
{
    Q_OBJECT
    bool ok,helpSelected,noisingSelected,denoisingSelected;
    QString m_inputPath,m_outputPath,m_noiseMethod,m_noiseIntensity,m_noiseProbability,m_denoiseValues;
    denoiseClass::method m_denoiseMethod;
    QStringList m_line;
;
public:
    explicit ConsoleParser(QStringList line);
    ~ConsoleParser();

signals:

public slots:
    void parse();

    bool isOk();
    bool isHelpSelected();
    bool isNoisingSelected();
    bool isDenoisingSelected();

    QString inputPath();
    QString outputPath();
    QString noiseMethod();
    QString noiseIntensity();
    QString noiseProbability();

    denoiseClass::method denoiseMethod();
    QString denoiseValues();

};

#endif // CONSOLEPARSER_H
