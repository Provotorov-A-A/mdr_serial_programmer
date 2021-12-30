#include <QCoreApplication>
#include <QLocale>
#include <QTranslator>
#include <QTimer>

#include "maindirector.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("mdr_serial_programmer");
    QCoreApplication::setApplicationVersion("1.0");
    
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "mdr_serial_programmer_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }
    
    MainDirector mainDirector;
    mainDirector.Run();
    
//    QObject::connect(&mainDirector, SIGNAL(Finish(int )), &app, SLOT(quit( )), Qt::QueuedConnection);
//    QTimer::singleShot(0, &mainDirector, SLOT(Run()));
//    return app.exec();
}
