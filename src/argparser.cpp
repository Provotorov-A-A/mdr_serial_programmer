#include "cassert"
#include <QString>
#include <QCommandLineOption>
#include <QMap>

#include "argparser.h"

namespace argparser
{

static const QString appDescrString = "Serial programmer application for Milandr MCU's 1986 series";

enum CommandLineOptionToken
{
//  OptionToken_show_help,              // Implemented implicitly by qt parser
//  OptionToken_show_version,           // Implemented implicitly by qt parser
    OptionToken_optype,
    OptionToken_f_verbose,
    OptionToken_serial_port,
    OptionToken_serial_baudrate,
    OptionToken_file_format,
    OptionToken_input_file,
    OptionToken_output_file,
    
    OptionToken_TOTAL,
};

static const QMap<CommandLineOptionToken, QString> optionTokenMap =
{
    {OptionToken_optype,            "optype"},
    {OptionToken_f_verbose,         "verbose"},
    {OptionToken_serial_port,       "port"},
    {OptionToken_serial_baudrate,   "baudrate"},
    {OptionToken_file_format,       "format"},
    {OptionToken_input_file,        "infile"},
    {OptionToken_output_file,       "outfile"},
};
        
static const QMap<Settings::OperationType, QString> operationTypeMap =
{
    {Settings::OperationType_READ,                  "R"},
    {Settings::OperationType_ERASE,                 "E"},
    {Settings::OperationType_WRITE,                 "W"},
    {Settings::OperationType_WRITE_VERIFY,          "WV"},
    {Settings::OperationType_ERASE_WRITE,           "EW"},
    {Settings::OperationType_ERASE_WRITE_VERIFY,    "EWV"},
    {Settings::OperationType_SHOW_PORTS,            "S"},
    {Settings::OperationType_FAKE_WRITE,            "F"},
};

static const QMap<Settings::FileFormat, QString> fileFormatMap =
{
    {Settings::FileFormat_BIN,                      "bin"},
    {Settings::FileFormat_HEX,                      "hex"},
    {Settings::FileFormat_ELF,                      "elf"},
};

ArgParser::ArgParser()
{
    setApplicationDescription(appDescrString);
}

ArgParser::CommandLineParseResult ArgParser::Parse(const QStringList & args)
{
    setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);
    //-----------------------------------------------------
    // Set possible options
    const QCommandLineOption helpOption = addHelpOption();
    const QCommandLineOption versionOption = addVersionOption();    

    bool parserInitOk = true;
    // --- Define long options with values (not flags) - add not empty string as third argument
    QCommandLineOption operationTypeOption(QStringList() << optionTokenMap.value(OptionToken_optype),
            QCoreApplication::translate("main", "Operation type"), "M", "S");
    parserInitOk = parserInitOk && addOption(operationTypeOption); 
    
    QCommandLineOption serialPortNameOption(QStringList() << optionTokenMap.value(OptionToken_serial_port),
            QCoreApplication::translate("main", "Serial port name"), "M");
    parserInitOk = parserInitOk && addOption(serialPortNameOption);
    
    QCommandLineOption serialBaudrateOption(QStringList() << optionTokenMap.value(OptionToken_serial_baudrate),
            QCoreApplication::translate("main", "Serial port baudrate"), "M");
    parserInitOk = parserInitOk && addOption(serialBaudrateOption); 
    
    QCommandLineOption inputFileOption(QStringList() << optionTokenMap.value(OptionToken_input_file),
            QCoreApplication::translate("main", "Input filename"), "M");
    parserInitOk = parserInitOk && addOption(inputFileOption); 

    QCommandLineOption outputFileOption(QStringList() << optionTokenMap.value(OptionToken_output_file),
            QCoreApplication::translate("main", "Output filename"), "M");
    parserInitOk = parserInitOk && addOption(outputFileOption); 
    
    QCommandLineOption fileFormatOption(QStringList() << optionTokenMap.value(OptionToken_file_format),
            QCoreApplication::translate("main", "File format"), "M");
    parserInitOk = parserInitOk && addOption(fileFormatOption); 
    
    // --- Define short options with no values (flags)
    QCommandLineOption verboseOption(QStringList() << optionTokenMap.value(OptionToken_f_verbose),
            QCoreApplication::translate("main", "Verbose output"));
    parserInitOk = parserInitOk && addOption(verboseOption);

    assert(parserInitOk);
    
    //-----------------------------------------------------
    // Basic parsing
    bool commonParseOk = parse(args);
    if (!commonParseOk)
    {
        m_error = errorText();
        return CommandLineError;
    }
    
    //-----------------------------------------------------
    // Specific checks
    auto optype = ExtractOptionValue<Settings::OperationType>(operationTypeMap, value(operationTypeOption));
    
    if (isSet(versionOption))
    {
        return CommandLineVersionRequested;
    }
    
    if (isSet(helpOption))
    {
        return CommandLineHelpRequested;
    }
    
    if (settings::Settings::OperationType_SHOW_PORTS == optype)
    {
        return CommandLineAvailablePortsRequested;
    }
    
    
    //-----------------------------------------------------
    // Fill Settings structure
    bool optionTypeOk = false;
    
    
    m_settings.m_operation = optype;
    m_settings.m_verbose = isSet(verboseOption);
    m_settings.m_serialSettings.m_portName = value(serialPortNameOption);

    if (isSet(serialBaudrateOption))
    {
        int baud = value(serialBaudrateOption).toInt(&optionTypeOk); 
        if (optionTypeOk)
        {
            m_settings.m_serialSettings.m_baudRate = static_cast<QSerialPort::BaudRate>(baud);
        }
        else
        {
            m_error = QString("Illegal value for option '%0'").arg(optionTokenMap.value(OptionToken_serial_baudrate));
            return CommandLineError;
        }
    }
    
    auto fileFormat = ExtractOptionValue<Settings::FileFormat>(fileFormatMap, value(fileFormatOption));
    m_settings.m_fileFormat = fileFormat;
    m_settings.m_dataFile = value(inputFileOption);
    m_settings.m_outputFile = value(outputFileOption);
    
    return CommandLineOk;
}

settings::Settings ArgParser::Settings()
{
    return m_settings;
}

QString ArgParser::ErrorText()
{
    return m_error;
}

QString ArgParser::HelpText()
{
    return helpText();
}

QString ArgParser::VersionText()
{
    return QCoreApplication::instance()->applicationVersion();
}

} //namespace argparser
