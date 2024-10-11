#include "dbconnection.h"
#include "common/filedir.h"
#include "common/querys.h"

status::status(int val, QString error){
    v = val;
    err = error;
}

dbconnection::dbconnection(){}

dbconnection::dbconnection(QString query, QVector<QDate> lt){
    q = query;
    dtl = lt;
}

dbconnection::dbconnection(QString query){
    q = query;
}
dbconnection::~dbconnection(){
    disconnectBase();
}

void dbconnection::setEnv(dbdcon *enviroments){
    env = enviroments;
}

void dbconnection::disconnectBase(){
    if(QSqlDatabase::contains(env->name)){
        {
            QSqlDatabase::database(env->name).close();
        }
        QSqlDatabase::removeDatabase(env->name);
    }
}

status dbconnection::checkLink(){
    status v;
    {
        disconnectBase();
        QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL",env->name);
        db.setHostName(env->ip);
        db.setPort(env->port);
        db.setDatabaseName(env->databasename);
        bool o = db.open(env->username,env->userpassword);
        if(o){
            if(db.isOpen()){
                v = status(1);
            }else{
                v = status(0,db.lastError().text());
            }
        }else{
            v = status(0,db.lastError().text());
        }
    }
    disconnectBase();
    return v;
}

QVector<QMap<QString, dbvalue>> dbconnection::getRecordData(){
    QVector<QMap<QString,dbvalue>> data;
    {
        disconnectBase();
        QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL",env->name);
        db.setHostName(env->ip);
        db.setPort(env->port);
        db.setDatabaseName(env->databasename);
        if(db.open(env->username,env->userpassword)){
            if(db.isOpen()){
                QSqlQuery qr(db);
                qr.exec(querys().a_region);
                while(qr.next()){
                    QMap<QString,dbvalue> recmem;
                    for(int i=0; i<qr.record().count(); ++i){
                        recmem.insert(qr.record().fieldName(i),{qr.record().value(i)});
                    }

                    //check awaible
                    if(aut->region != -1){
                        if(recmem["region"].value_int() != aut->region){
                            continue;
                        }
                    }
                    if(!aut->areas.contains(-1)){
                        if(!aut->areas.contains(recmem["area"].value_int())){
                            continue;
                        }
                    }

                    emit currentStage(qr.record().value("region").toString() + "/" + qr.record().value("area").toString());

                    QSqlQuery qc(db);
                    qc.exec(querys().a_cro_per.arg(QString::number(recmem["region"].value_int()),QString::number(recmem["area"].value_int())));

                    QMap<QString,dbvalue> mem;
                    while(qc.next()){
                        for(int i=0; i<qc.record().count(); ++i){
                            mem.insert(qc.record().fieldName(i),{qc.record().value(i)});
                        }
                        mem.insert("nameregion",{qr.record().value("nameregion")});
                        mem.insert("namearea",{qr.record().value("namearea")});
                        data.push_back(mem);
                    }
                }
            }else{
                emit errorConnection(db.lastError().text());
            }
        }else{
            emit errorConnection(db.lastError().text());
        }
    }
    disconnectBase();
    return data;
}

QVector<QMap<QString, dbvalue> > dbconnection::getMonth(){
    QVector<QMap<QString,dbvalue>> data;
    {
        disconnectBase();
        QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL",env->name);
        db.setHostName(env->ip);
        db.setPort(env->port);
        db.setDatabaseName(env->databasename);
        bool o = db.open(env->username,env->userpassword);
        if(o){
            if(db.isOpen()){
                QVector<QMap<QString,dbvalue>> dates;

                QSqlQuery qr(db);
                qr.exec(q);
                while(qr.next()){
                    QMap<QString,dbvalue> recmem;
                    for(int i=0; i<qr.record().count(); ++i){
                        recmem.insert(qr.record().fieldName(i),{qr.record().value(i)});
                    }
                    dates.push_back(recmem);
                }

                //get stat data
                for(int i=0; i<dates.size(); ++i){
                    QDate b = QDate::fromString(dates[i]["date"].value_string(),"yyyy-MM-dd");
                    QDate e = b.addDays(1);

                    //load from cash
                    QFile f(file().cash + QDir::separator() + racstr + "_" + b.toString("yyyy-MM-dd"));
                    if(f.exists()){
                        QMap<QString,dbvalue> rf;
                        rf.insert("stat",{readDataFromFile(f.fileName())});
                        rf.insert("date",{b.toString("yyyy-MM-dd")});
                        data.push_back(rf);
                        continue;
                    }

                    qr.exec(addq.arg(b.toString("yyyy-MM-dd")).arg(e.toString("yyyy-MM-dd")));
                    while(qr.next()){
                        QMap<QString,dbvalue> recmem;
                        for(int i=0; i<qr.record().count(); ++i){
                            recmem.insert(qr.record().fieldName(i),{qr.record().value(i)});
                        }
                        recmem.insert("date",{b.toString("yyyy-MM-dd")});
                        data.push_back(recmem);

                        //cash data
                        writeFile(file().cash + QDir::separator() + racstr + "_" + b.toString("yyyy-MM-dd"),recmem["stat"].value_byte());
                    }
                }
            }else{
                emit errorConnection(db.lastError().text());
            }
        }else{
            emit errorConnection(db.lastError().text());
        }
    }
    disconnectBase();
    return data;
}

QByteArray dbconnection::readDataFromFile(QString filename){
    QByteArray data;
    QFile f(filename);
    if(f.open(QFile::ReadOnly)){
        data = f.readAll();
        f.close();
    }
    return data;
}

void dbconnection::writeFile(QString name, QByteArray data){
    QFile f(name);
    if(f.open(QFile::WriteOnly)){
        f.write(data);
        f.close();
    }
}

void dbconnection::check(){
    emit statusBase(checkLink());
    emit finished();
}

void dbconnection::askDataRegionCross(){
    emit answer(getRecordData());
    emit finished();
}

void dbconnection::askDataMonth(){
    emit answer(getMonth());
    emit finished();
}
