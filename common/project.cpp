#include "project.h"
#include "common/datareader.h"

#include "common/rac.h"
extern rac *drac;

project::project(){}

QJsonObject project::json(){
    QJsonObject ret;
    ret["name"] = (QString)name;
    QJsonArray crs;
    for(int i=0; i<objects.size(); ++i){
        crs.push_back(objects[i]->json());
    }
    ret["objs"] = crs;
    ret["ra"] = (bool)ra;
    ret["bcalc"] = (bool)bcalc;
    ret["ecalc"] = (bool)ecalc;
    ret["tof"] = (QString)of.toString("HH:mm");
    ret["tto"] = (QString)to.toString("HH:mm");
    ret["exec"] = (bool)exec;
    QJsonArray mtr;
    for(int r=0; r<mtx.size(); ++r){
        QJsonArray mtc;
        for(int c=0; c<mtx[r].size(); ++c){
            mtc.push_back(mtx[r][c]);
        }
        mtr.push_back(mtc);
    }
    ret["mtx"] = mtr;
    QJsonArray kk;
    for(int i=0; i<kspk.size(); ++i){
        kk.push_back(kspk[i].json());
    }
    ret["kspk"] = kk;
    return ret;
}

void project::setVal(QJsonObject data){
    name = data["name"].toString();
    ra = data["ra"].toBool();
    bcalc = data["bcalc"].toBool();
    ecalc = data["ecalc"].toBool();
    of = QTime::fromString(data["tof"].toString(),"HH:mm");
    to = QTime::fromString(data["tto"].toString(),"HH:mm");
    exec = data["exec"].toBool();

    mtx.clear();
    QJsonArray mtr = data["mtx"].toArray();
    for(int r=0; r<mtr.size(); ++r){
        QVector<int> val;
        QJsonArray mtc = mtr[r].toArray();
        for(int c=0; c<mtc.size(); ++c){
            val.push_back(mtc[c].toInt());
        }
        mtx.push_back(val);
    }

    kspk.clear();
    QJsonArray kk = data["kspk"].toArray();
    for(int i=0; i<kk.size(); ++i){
        kspk.push_back(KSPK(kk[i].toObject()));
    }

    //read all data crosses
    objs = data["objs"].toArray();

    //reader
    cd = new datareader;
    cd->createNP();
    connect(drac,&rac::loaded,this,&project::loadCrosses);
    cd->updateRegion();

    emit loaded();
}

void project::addObject(racobject r){
    objects.push_back(new cobject(r));
    emit updateTab();
}

bool project::existCross(int r, int a, int s, int c){
    for(int i=0; i<objects.size(); ++i){
        if(objects[i]->data.region==r&&
            objects[i]->data.area==a&&
            objects[i]->data.subarea==s&&
            objects[i]->data.number==c){
            return true;
        }
    }
    return false;
}

void project::loadCrosses(){
    disconnect(drac,&rac::loaded,this,&project::loadCrosses);
    cd->deleteLater();
    emit removeAllObjects();
}

void project::setCrosses(){
    if(drac->cross.isEmpty()){
        for(int i=0; i<objs.size(); ++i){
            QJsonObject o = objs[i].toObject()["rasc"].toObject();
            o.insert("descObject","Создан");
            addObject(racobject(o));
            objects.last()->setVal(objs[i].toObject());
        }
    }else{
        for(int i=0; i<objs.size(); ++i){
            QJsonObject o = objs[i].toObject()["rasc"].toObject();
            addObject(drac->name_cross(o["r"].toInt(),
                                       o["a"].toInt(),
                                       o["s"].toInt(),
                                       o["c"].toInt()));
            objects.last()->setVal(objs[i].toObject());
        }
    }
    emit updateTab();
}

QJsonObject KSPK::json(){
    QJsonObject r;
    r["ks"] = (int)ks;
    r["pk"] = (int)pk;
    r["desc"] = (QString)desc;
    return r;
}

void KSPK::setVal(QJsonObject data){
    ks = data["ks"].toInt();
    pk = data["pk"].toInt();
    desc = data["desc"].toString();
}

KSPK::KSPK(){}

KSPK::KSPK(QJsonObject data){
    setVal(data);
}


