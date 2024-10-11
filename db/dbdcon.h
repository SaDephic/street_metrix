#ifndef DBDCON_H
#define DBDCON_H

#include <QJsonObject>
#include <QString>
#include <QJsonDocument>
#include <QFile>
#include <QDir>

struct dbdcon{
    QString name;
    QString ip;
    int port;
    QString databasename;
    QString username;
    QString userpassword;

    dbdcon();
    dbdcon(QString name,
           QString ip, int port, QString databasename,
           QString username, QString userpassword);
    ~dbdcon();

    void setjson(QJsonObject data);
    QJsonObject json();
};

#endif // DBDCON_H
