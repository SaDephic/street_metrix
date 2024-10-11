#include "datareader.h"
#include "common/querys.h"

datareader::datareader(){}

void datareader::createNP(){
    thread = new QThread;
    db = new dbconnection();
}

void datareader::cretateSD(racobject rasc, QVector<QDate> l){
    thread = new QThread;
    db = new dbconnection(querys().a_statistic
                              .arg(QString::number(rasc.region))
                              .arg(QString::number(rasc.area))
                              .arg(QString::number(rasc.number))
                              .arg("%1"), l);
}

void datareader::cretateMD(racobject rasc, int MM, int YY){
    thread = new QThread;
    db = new dbconnection(querys().a_monthstat
                              .arg(QString::number(rasc.region))
                              .arg(QString::number(rasc.area))
                              .arg(QString::number(rasc.number))
                              .arg(QString::number(MM))
                              .arg(QString::number(YY)));
    //additional query
    db->addq = querys().a_monthstatdata
                   .arg(QString::number(rasc.region))
                   .arg(QString::number(rasc.area))
                   .arg(QString::number(rasc.number))
                   .arg("%1")
                   .arg("%2");
    db->racstr = QString::number(rasc.region) + "." +
                 QString::number(rasc.area) + "." +
                 QString::number(rasc.number);
}

void datareader::updateRegion(){
    connect(db,&dbconnection::answer,drac,&rac::setRegion);
    db->setEnv(envdb);
    db->moveToThread(thread);//object in thread
    //delete instanses after executed
    connect(thread,&QThread::started, db,&dbconnection::askDataRegionCross);
    connect(db,&dbconnection::finished, thread,&QThread::quit);
    connect(thread,&QThread::finished,thread,&QThread::deleteLater);
    connect(db,&dbconnection::finished,db,&dbconnection::deleteLater);
    thread->start();
}

void datareader::askDates(){
    db->setEnv(envdb);
    db->moveToThread(thread);//object in thread
    //delete instanses after executed
    connect(thread,&QThread::started, db,&dbconnection::askDataMonth);
    connect(db,&dbconnection::finished, thread,&QThread::quit);
    connect(thread,&QThread::finished,thread,&QThread::deleteLater);
    connect(db,&dbconnection::finished,db,&dbconnection::deleteLater);
    thread->start();
}
