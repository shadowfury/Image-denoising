#include "parser.h"
#include <iostream>
#include <QImage>
#include <QStringList>
#include <QDir>
#include <QFile>

ConsoleParser::ConsoleParser(QStringList line){
    m_line=line;
    ok=true;
    helpSelected=noisingSelected=denoisingSelected=false;
    m_noiseIntensity="30:30:30";
    m_noiseMethod="uniform";
    m_noiseProbability="50:50:50";
    m_denoiseMethod=denoiseClass::NLM_fast;
    m_denoiseValues="7:21:120";

}
ConsoleParser::~ConsoleParser(){
    m_line.clear();
}

void ConsoleParser::parse(){
    for (int i=0;i<m_line.count(); i++){
        QString item=m_line.at(i).toLower();
        if ((item=="-h")||(item=="--help")){
            ok=true;
            helpSelected=true;
            return;
        }
        else if ((item=="-i")||(item=="--input")){
            if (i+1 >= m_line.count()){
                std::cout<<"Input image option is mandatory, check if -i or --input is followed by a correct path to image"<<std::endl;
                ok=false;
            }
            else{
                QImage test(m_line.at(i+1));
                if (!test.isNull()) m_inputPath=m_line.at(i+1);
                else{
                    std::cout<<"Input image option is mandatory, check if -i or --input is followed by a correct path to image"<<std::endl;
                    ok=false;
                }
            }
        }
        else if ((item=="-o")||(item=="--output")){
            if (i+1 >= m_line.count()){
                std::cout<<"Output image option is mandatory, check if -o or --output is followed by a correct path to save file as"<<std::endl;
                ok=false;
            }
            else{
                QImage test=QImage(QSize(1,1), QImage::Format_ARGB32_Premultiplied);
                test.fill(Qt::transparent);
                if ((!QFile(m_line.at(i+1)).exists())){
                    if(!test.save(m_line.at(i+1),0,-1)){
                        std::cout<<"Output image option is mandatory, check if -o or --output is followed by a correct path to save file as"<<std::endl;
                        ok=false;
                    }
                    else{
                        m_outputPath=m_line.at(i+1);
                        QFile(m_line.at(i+1)).remove();
                    }
                }
                else m_outputPath=m_line.at(i+1);
            }
        }
        else if ((item=="-n")||(item=="--noise")){
            noisingSelected=true;
        }
        else if ((item=="-d")||(item=="--denoise")){
            denoisingSelected=true;
        }
        else if ((item=="-nm")||(item=="--noise-method")){
            if (i+1 >= m_line.count()){
                std::cout<<"-nm or --noise-method option is incorrect, check if it is followed by correct value (example: -nm uniform) or exclude this option (-nm uniform is default value)"<<std::endl;
                ok=false;
            }
            else{
                if ((m_line.at(i+1)=="normal")||(m_line.at(i+1)=="uniform")) m_noiseMethod=m_line.at(i+1);
                else{
                    std::cout<<"-nm or --noise-method option is incorrect, check if it is followed by correct value (example: -nm uniform) or exclude this option (-nm uniform is default value)"<<std::endl;
                    ok=false;
                }
            }
        }
        else if ((item=="-ni")||(item=="--noise-intensity")){
            if (i+1 >= m_line.count()){
                std::cout<<"-ni or --noise-intensity option is incorrect, check if it is followed by correct value (example: -ni 30:30:30 (-256<30<256)) or exclude this option (-ni 30:30:30 is default value)"<<std::endl;
                ok=false;
            }
            else{
                QStringList temp=m_line.at(i+1).split(":",QString::SkipEmptyParts);
                bool res=true;
                foreach (QString i,temp) if ((i.toInt()>=256)||(i.toInt()<=-256)||(i.toInt()==0)) res=false;
                if (temp.count()==3 && res) m_noiseIntensity=m_line.at(i+1);
                else{
                    std::cout<<"-ni or --noise-intensity option is incorrect, check if it is followed by correct value (example: -ni 30:30:30 (-256<30<256)) or exclude this option (-ni 30:30;30 is default value)"<<std::endl;
                    ok=false;
                }
            }
        }
        else if ((item=="-np")||(item=="--noise-probability")){
            if (i+1 >= m_line.count()){
                std::cout<<"-np or --noise-probability option is incorrect, check if it is followed by correct value (example: -np 50:50:50 (0<=30<=100)) or exclude this option (-np 50:50:50 is default value)"<<std::endl;
                ok=false;
            }
            else{
                QStringList temp=m_line.at(i+1).split(":",QString::SkipEmptyParts);
                bool res=true;
                foreach (QString i,temp) if ((i.toInt()>100)||(i.toInt()<=0)) res=false;
                if (temp.count()==3 && res) m_noiseProbability=m_line.at(i+1);
                else{
                    std::cout<<"-np or --noise-probability option is incorrect, check if it is followed by correct value (example: -np 50:50:50 (0<=30<=100)) or exclude this option (-np 50:50:50 is default value)"<<std::endl;
                    ok=false;
                }
            }
        }
        else if ((item=="-dm")||(item=="--denoise-method")){
            if (i+1 >= m_line.count()){
                std::cout<<"-dm or --denoise-method option is incorrect, check if it is followed by correct value (example: -dm nlm_fast) or exclude this option (-dm nlm_fast is default value)"<<std::endl;
                ok=false;
            }
            else{
                if (m_line.at(i+1)=="simple_squares")m_denoiseMethod=denoiseClass::simple_squares;
                else if(m_line.at(i+1)=="nlm") m_denoiseMethod=denoiseClass::NLM;
                else if(m_line.at(i+1)=="nlm_fast")m_denoiseMethod=denoiseClass::NLM_fast;
                else if(m_line.at(i+1)=="nlm_fast_fft")m_denoiseMethod=denoiseClass::NLM_fast_FFT;
                else if(m_line.at(i+1)=="nlm_multithread")m_denoiseMethod=denoiseClass::NLM_multiThread;
                else{
                    std::cout<<"-dm or --denoise-method option is incorrect, check if it is followed by correct value (example: -dm nlm_fast) or exclude this option (-dm nlm_fast is default value)"<<std::endl;
                    ok=false;
                }
            }
        }
        else if ((item=="-dv")||(item=="--denoise-values")){
            if (i+1 >= m_line.count()){
                std::cout<<"-dv or --denoise-values option is incorrect, check if it is followed by correct value (example: -dv 7;21;120 or exclude this option (-dv 7:21:120 is default value)"<<std::endl;
                ok=false;
            }
            else{
                QStringList temp=m_line.at(i+1).split(":",QString::SkipEmptyParts);
                bool res=true;
                foreach (QString i,temp) if ((i.toInt()==0)) res=false;
                if (temp.count()==3 && res) m_denoiseValues=m_line.at(i+1);
                else{
                    std::cout<<"-dv or --denoise-values option is incorrect, check if it is followed by correct value (example: -dv 7;21;120 or exclude this option (-dv 7:21:120 is default value)"<<std::endl;
                    ok=false;
                }
            }

        }
    }
}


bool ConsoleParser::isOk(){
    return ok;
}
bool ConsoleParser::isHelpSelected(){
    return helpSelected;
}
bool ConsoleParser::isNoisingSelected(){
    return noisingSelected;
}
bool ConsoleParser::isDenoisingSelected(){
    return denoisingSelected;
}

QString ConsoleParser::inputPath(){
    return m_inputPath;
}
QString ConsoleParser::outputPath(){
    return m_outputPath;
}
QString ConsoleParser::noiseMethod(){
    return m_noiseMethod;
}
QString ConsoleParser::noiseIntensity(){
    return m_noiseIntensity;
}
QString ConsoleParser::noiseProbability(){
    return m_noiseProbability;
}
denoiseClass::method ConsoleParser::denoiseMethod(){
    return m_denoiseMethod;
}
QString ConsoleParser::denoiseValues(){
    return m_denoiseValues;
}
