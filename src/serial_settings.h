#ifndef SERIAL_SETTINGS_H
#define SERIAL_SETTINGS_H

#include <QString>
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>

// Serial port settings
struct SerialSettings
{
    QString m_portName;
    QSerialPort::BaudRate   m_baudRate = QSerialPort::Baud9600;
    QSerialPort::Parity     m_parity = QSerialPort::NoParity;
    QSerialPort::DataBits   m_dataBits = QSerialPort::Data8;
    QSerialPort::StopBits   m_stopBits = QSerialPort::OneStop;
    
    void SetDefault()
    {
        m_portName = QString();
        m_baudRate = QSerialPort::Baud9600;
        m_parity = QSerialPort::NoParity;
        m_dataBits = QSerialPort::Data8;
        m_stopBits = QSerialPort::OneStop;
    }
    
    static void Print(const SerialSettings& s)
    {
        qDebug() << "portName: " << s.m_portName;
        qDebug() << "baudRate: " << static_cast<int>(s.m_baudRate);
        qDebug() << "parity: " << 
                    ( (QSerialPort::NoParity == s.m_parity)? "NoParity" :
                    (QSerialPort::EvenParity == s.m_parity)? "EvenParity" :
                    (QSerialPort::OddParity == s.m_parity)? "OddParity" :
                    (QSerialPort::SpaceParity == s.m_parity)? "SpaceParity" :
                    (QSerialPort::MarkParity == s.m_parity)? "MarkParity" : "UnknownParity");
        qDebug() << "m_dataBits: " << 
                    ( (QSerialPort::Data5 == s.m_dataBits)? "Data5" :
                    (QSerialPort::Data6 == s.m_dataBits)? "Data6" :
                    (QSerialPort::Data7 == s.m_dataBits)? "Data7" :
                    (QSerialPort::Data8 == s.m_dataBits)? "Data8" : "UnknownDataBits");
        qDebug() << "m_stopBits: " << 
                    ( (QSerialPort::OneStop == s.m_stopBits)? "OneStop" :
                    (QSerialPort::OneAndHalfStop == s.m_stopBits)? "OneAndHalfStop" :
                    (QSerialPort::TwoStop == s.m_stopBits)? "TwoStop" : "UnknownStopBits");
        
    }
    
};

#endif // SERIAL_SETTINGS_H
