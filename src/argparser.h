#ifndef ARGPARSER_H
#define ARGPARSER_H

#include <QCommandLineParser>
#include "settings.h"

namespace argparser
{

using settings::Settings;

class ArgParser : private QCommandLineParser
{
public:
    enum CommandLineParseResult
    {
        CommandLineOk,
        CommandLineError,
        CommandLineVersionRequested,
        CommandLineHelpRequested,
        CommandLineAvailablePortsRequested
    };
    
    ArgParser();
    CommandLineParseResult Parse(const QStringList& args);
    settings::Settings Settings();
    QString ErrorText();
    QString HelpText();
    QString VersionText();
    
private:
    // Extracts option value of type T that corresponds to specified string literal (actually - an option value from command line )
    // Map should contain one key for illegal value equal to Settings::COMMON_ILLEGAL_SETTING_ENUM_VALUE
    template <typename T> 
    T ExtractOptionValue(QMap<T, QString> valueStringMap, const QString& token)
    {
        for (const auto key : valueStringMap.keys())
        {
            if (0 == token.compare(valueStringMap.value(key), Qt::CaseInsensitive)) return key;
        }
        return static_cast<T>(Settings::COMMON_ILLEGAL_SETTING_ENUM_VALUE);    
    }
    
    QString m_error;    
    settings::Settings m_settings;
};

} // namespace argparser

#endif // ARGPARSER_H
