#ifndef CONSOLEFORMATTER_H
#define CONSOLEFORMATTER_H
#include <QObject>
#include <QList>
#include <QPair>
#include <QString>

class ConsoleFormatter: public QObject
{
    Q_OBJECT

    QList <QPair<QString,QString> >*m_commands;
    int m_nameSize,m_descrSize;

public:
    ConsoleFormatter(int name_size=30, int descr_size=50);
    ~ConsoleFormatter();
public slots:
    void printCommands();
    void addCommand(QString name,QString description);
    void _printDescr(QString _descr);

};

#endif // CONSOLEFORMATTER_H
