#include "formatter.h"
#include <iostream>

ConsoleFormatter::ConsoleFormatter(int name_size, int descr_size)
{
    m_commands = new QList<QPair<QString, QString> >;
    m_nameSize=name_size;
    m_descrSize=descr_size;
}

ConsoleFormatter::~ConsoleFormatter(){
    delete m_commands;
}

void ConsoleFormatter::printCommands(){
    std::cout<<std::endl;
    for (int i=0;i<m_commands->count();i++){
        QString name=m_commands->at(i).first;
        QString descr=m_commands->at(i).second;
        std::cout<<name.leftJustified(m_nameSize,' ').toStdString()<<" "
                <<descr.leftJustified(m_descrSize,' ').left(m_descrSize).toStdString()<<std::endl;
        if (descr.count() > m_descrSize) _printDescr(descr.remove(descr.left(m_descrSize)));
    }
    std::cout<<std::endl;
}

void ConsoleFormatter::_printDescr(QString _descr){
    std::cout<<QString(" ").repeated(m_nameSize+1).toStdString()<<_descr.leftJustified(m_descrSize,' ').left(m_descrSize).toStdString()<<std::endl;
    if ( _descr.count()> m_descrSize) _printDescr(_descr.remove(_descr.left(m_descrSize)));
}

void ConsoleFormatter::addCommand(QString name,QString description){
    m_commands->append(qMakePair(name,description));
}
