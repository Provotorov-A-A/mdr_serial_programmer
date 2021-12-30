#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QSerialPort>
#include "serial_settings.h"

namespace settings 
{



class Settings
{
public:
    static const int COMMON_ILLEGAL_SETTING_ENUM_VALUE = -1;
    
    enum OperationType
    {
        OperationType_ERASE,
        OperationType_READ,
        OperationType_WRITE,
        OperationType_ERASE_WRITE,
        OperationType_WRITE_VERIFY,
        OperationType_ERASE_WRITE_VERIFY,
        
        OperationType_SHOW_PORTS,
        OperationType_FAKE_WRITE,
        
        OperationType_Unspecified = COMMON_ILLEGAL_SETTING_ENUM_VALUE
    } ;
    
    enum FileFormat
    {
        FileFormat_BIN,
        FileFormat_HEX,
        FileFormat_ELF,
        
        FileFormat_Unspecified = COMMON_ILLEGAL_SETTING_ENUM_VALUE
    } ;
    
public:
    void Print();
    
    // Behaviour settings
    OperationType m_operation = OperationType_READ;
    bool m_verbose = false;
    SerialSettings m_serialSettings;
    
    // Data settings
    QString m_dataFile;
    FileFormat m_fileFormat = FileFormat_HEX;
    QString m_outputFile;
    
private:
    bool m_isEmpty = true;
};

} // namespace settings 


#endif // SETTINGS_H
