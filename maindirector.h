#ifndef MAINDIRECTOR_H
#define MAINDIRECTOR_H

#include <QObject>

class MainDirector : public QObject
{
    Q_OBJECT
public:
    explicit MainDirector(QObject *parent = nullptr);
    
signals:
    
};

#endif // MAINDIRECTOR_H
