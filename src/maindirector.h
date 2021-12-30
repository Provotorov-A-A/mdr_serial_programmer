#ifndef MAINDIRECTOR_H
#define MAINDIRECTOR_H

#include <QObject>
#include "argparser.h"
#include "settings.h"

using settings::Settings;

class MainDirector : public QObject
{
    Q_OBJECT
public:
    explicit MainDirector(QObject *parent = nullptr);
    ~MainDirector();
    
    bool ConfigureInterface(const settings::Settings&);
    bool Connect();
    void PrintAvailablePorts();
    
public slots:
    void Run();
    
signals:
    void Finish(int );
    
private:
    bool WaitForApplicationObjectInit();
    void FillSettings(Settings& settings);
    argparser::ArgParser* m_pArgParser = nullptr;
    
    QSerialPort serial;
};

#endif // MAINDIRECTOR_H
