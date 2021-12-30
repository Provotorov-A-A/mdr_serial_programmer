#include <QDebug>
#include <QTimer>
#include <QCoreApplication>
#include <QtSerialPort/QSerialPortInfo>

#include "maindirector.h"

using argparser::ArgParser;

enum class ProtocolId : uint8_t
{
    CMD_SYNC = 0x00,
    CMD_CR = 0x0D,
    CMD_BAUD = 0x42,
    CMD_LOAD = 0x4C,
    CMD_VFY = 0x59,
    CMD_RUN = 0x52,
};
        
enum ApplicationExitCode
{
  ApplicationExitCode_OK = EXIT_SUCCESS,
  ApplicationExitCode_INTERNAL_ERROR,
  ApplicationExitCode_INVALID_ARGS,
  ApplicationExitCode_INVALID_ARGS_SET,
  ApplicationExitCode_UNKNOWN_ERROR,
};

MainDirector::MainDirector(QObject *parent) : QObject(parent)
{
    m_pArgParser = new ArgParser();
}

MainDirector::~MainDirector()
{
    if (m_pArgParser)
    {
        delete m_pArgParser;
        m_pArgParser = nullptr;
    }
}

bool MainDirector::ConfigureInterface(const settings::Settings& s)
{
    serial.setPortName(s.m_serialSettings.m_portName);
    serial.setFlowControl(QSerialPort::NoFlowControl);
//    serial.setBaudRate(s.m_serialSettings.m_baudRate);
    serial.setBaudRate(QSerialPort::Baud9600, QSerialPort::AllDirections);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    bool openOk = serial.open(QIODevice::ReadWrite);
    if (!openOk)
    {
        qDebug() << "Cannot open port " << s.m_serialSettings.m_portName;
        return false;
    }
    serial.clear();
    
    qDebug() << "Port " << s.m_serialSettings.m_portName << "is open";
}

bool MainDirector::Connect()
{
    const int timeout = 2000;
    
    bool r = false;
    QByteArray indata;
    char syncByte = static_cast<char>(ProtocolId::CMD_SYNC);
    
    QEventLoop loop;
    QTimer to;
    
    to.setSingleShot(true);
    to.setInterval(timeout);
    
    bool hasData = false;
    
//    for (int i = 0; i < 100; ++i)
//    {
//        serial.write(&syncByte, 1);
//    }
    
    qDebug() << "Wait sync...";
    to.start();
    do
    {
        serial.write(&syncByte, 1);
        loop.processEvents();
        indata.append(serial.readAll());
        
        if (indata.size() >= 3)
        {
            to.stop();
            hasData = true;
        }
    } while(to.isActive());
        
    if (!hasData)
    {
        qDebug() << "Sync error";
    }
    else
    {
        qDebug() << "Bytes was read: " << indata.size();
        QByteArray validAnswer("\r\n>");
        if (indata.compare(validAnswer))
        {
            qDebug() << "Sync answer is invalid:";
            qDebug() << indata;
        }
        else
        {
            qDebug() << "Sync OK";
            r = true;
        }        
    }
    
    return r;
}

void MainDirector::PrintAvailablePorts()
{
    QList<QSerialPortInfo> serialList = QSerialPortInfo::availablePorts();
    for (const auto s : serialList)
    {
        qDebug() << s.portName();
    }    
}

bool MainDirector::WaitForApplicationObjectInit()
{
    QCoreApplication* g_app = QCoreApplication::instance();
    if (g_app == nullptr)
    {
        return false;
    }
    
    QTimer waitTimer;
    waitTimer.setSingleShot(true);
    waitTimer.setTimerType(Qt::VeryCoarseTimer);
    waitTimer.start(1000);
    
    bool appObjectOk = false;
    while(waitTimer.isActive())
    {
        if (!g_app->startingUp())
        {
            appObjectOk = true;
            waitTimer.stop();
            break;
        }
        QCoreApplication::processEvents();
    }
    
    if (!appObjectOk)
    {
        return false;
    }    
    
    return true;
}

void MainDirector::Run()
{
    bool appObjectOk = WaitForApplicationObjectInit();
    if (!appObjectOk)
    {
        qFatal("Core Application object was not intantiated");
        emit Finish(ApplicationExitCode_INTERNAL_ERROR);
        return;
    }
    
    QCoreApplication* app = QCoreApplication::instance();
    
    ArgParser::CommandLineParseResult parsingResult = m_pArgParser->Parse(app->arguments());
    switch (parsingResult)
    {
    case ArgParser::CommandLineParseResult::CommandLineError:
        qDebug() << "Argument parsing error:" << m_pArgParser->ErrorText();
        app->exit(ApplicationExitCode_INVALID_ARGS);
        return;
        
    case ArgParser::CommandLineParseResult::CommandLineHelpRequested:
        qDebug() << m_pArgParser->HelpText();
        app->exit(ApplicationExitCode_OK);
        return;

    case ArgParser::CommandLineParseResult::CommandLineVersionRequested:
        qDebug() << m_pArgParser->VersionText();
        app->exit(ApplicationExitCode_OK);
        return;
    
    case ArgParser::CommandLineParseResult::CommandLineAvailablePortsRequested:
        PrintAvailablePorts();
        app->exit(ApplicationExitCode_OK);
        return;    
        
    default:
        break;
    }
    
    Settings settings = m_pArgParser->Settings();
//    settings.Print();
    
    
    bool cfgOk = ConfigureInterface(settings);
    if (!cfgOk)
    {
        emit Finish(ApplicationExitCode_OK);
        return;
    }
    
    bool connectOk = Connect();
    if (!connectOk)
    {
        emit Finish(ApplicationExitCode_OK);
        return;
    }
    
    emit Finish(ApplicationExitCode_OK);
    app->exit(ApplicationExitCode_OK);
    
    
}
