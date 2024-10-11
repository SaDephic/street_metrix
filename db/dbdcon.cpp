#include "dbdcon.h"
#include "common/filedir.h"

dbdcon::dbdcon(){}

dbdcon::dbdcon(QString name, QString ip, int port, QString databasename, QString username, QString userpassword){
    this->name = name;
    this->ip = ip;
    this->port = port;
    this->databasename = databasename;
    this->username = username;
    this->userpassword = userpassword;
}

dbdcon::~dbdcon(){
    QFile f(file().soft + QDir::separator() + file().c);
    if(f.open(QFile::WriteOnly)){
        f.write(QJsonDocument(json()).toJson());
        f.close();
    }
}

void dbdcon::setjson(QJsonObject data){
    name = data["name"].toString();
    ip = data["ip"].toString();
    port = data["port"].toInt();
    databasename = data["databasename"].toString();
    username = data["username"].toString();
    userpassword = data["userpassword"].toString();
}

QJsonObject dbdcon::json(){
    QJsonObject data;
    data["name"] = (QString)name;
    data["ip"] = (QString)ip;
    data["port"] = (int)port;
    data["databasename"] = (QString)databasename;
    data["username"] = (QString)username;
    data["userpassword"] = (QString)userpassword;
    return data;
}
