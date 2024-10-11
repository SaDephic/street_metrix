#ifndef SYSENV_H
#define SYSENV_H

#include <QFile>
#include <QDir>
#include <QObject>
#include <QProcessEnvironment>

#include <QJsonDocument>
#include <QJsonObject>

class sysenv : public QObject{

public:
    QString host_auth;
    QString last_folder;
    QString user_login;
    sysenv();
    ~sysenv();
};

#endif // SYSENV_H
