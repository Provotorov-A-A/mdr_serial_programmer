#include "settings.h"

namespace settings 
{

void Settings::Print()
{
    qDebug() << "------- Application behaviour settings ";
    qDebug() << "verbose: " << m_verbose;
    qDebug() << "operation: " << 
                (
                (m_operation == OperationType_ERASE)? "OperationType_ERASE" : 
                (m_operation == OperationType_READ)? "OperationType_READ" : 
                (m_operation == OperationType_WRITE)? "OperationType_WRITE" : 
                (m_operation == OperationType_ERASE_WRITE)? "OperationType_ERASE_WRITE" : 
                (m_operation == OperationType_WRITE_VERIFY)? "OperationType_WRITE_VERIFY" : 
                (m_operation == OperationType_ERASE_WRITE_VERIFY)? "OperationType_ERASE_WRITE_VERIFY" : 
                (m_operation == OperationType_SHOW_PORTS)? "OperationType_SHOW_PORTS" : 
                (m_operation == OperationType_FAKE_WRITE)? "Fake Write" : "Unspecified"
                );
    
    qDebug() << "------- Serial port settings ";
    SerialSettings::Print(m_serialSettings);
    
    qDebug() << "------- Data settings";
    qDebug() << "dataFile: " << m_dataFile;
    qDebug() << "fileFormat: " << 
                ((FileFormat_HEX == m_fileFormat)? "Intel Hex" : 
                (FileFormat_BIN == m_fileFormat)? "Binary" : 
                (FileFormat_ELF == m_fileFormat)? "Elf" : "Unspecified");
    qDebug() << "m_fakeOutputFile: " << m_outputFile;
}

} // namespace settings 
