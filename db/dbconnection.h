#ifndef DBCONNECTION_H
#define DBCONNECTION_H

#include "dbdcon.h"
#include "dbvalue.h"

#include <QFile>
#include <QThread>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlResult>
#include <QSqlRecord>

#include "auth/stateauth.h"
extern stateauth *aut;

struct status{
    status(int val = -1, QString error = QString());
    int v = -1;
    QString err = QString();
};

class dbconnection : public QObject{
    Q_OBJECT

    QString q;
    QVector<QDate> dtl;

public:
    QString addq;
    QString racstr;

    dbdcon *env;

signals:
    void statusBase(status v);
    void finished();//for unlinked thread
    void answer(QVector<QMap<QString,dbvalue>> res);//ask data massive
    void errorConnection(QString error);//message error execution
    void currentStage(QString info);

public:
    dbconnection();
    dbconnection(QString query, QVector<QDate> lt);
    dbconnection(QString query);
    ~dbconnection();

    void setEnv(dbdcon *enviroments);

    void disconnectBase();

private:
    status checkLink();
    QVector<QMap<QString,dbvalue>> getRecordData();
    QVector<QMap<QString,dbvalue>> getMonth();

    //read file
    QByteArray readDataFromFile(QString filename);
    void writeFile(QString name, QByteArray data);

public slots:
    void check();
    void askDataRegionCross();
    void askDataMonth();
};

#endif // DBCONNECTION_H
