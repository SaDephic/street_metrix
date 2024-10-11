#include "ht.h"

ht::ht(){}

QJsonObject ht::json(){
    QJsonObject r;

    r["name"] = (QString)name;

    QJsonArray ra;
    for(int i=0; i<ray.size(); ++i){
        ra.push_back(ray[i].json());
    }
    r["ray"] = ra;

    QJsonArray ar;
    for(int i=0; i<area.size(); ++i){
        ar.push_back(area[i].json());
    }
    r["area"] = ar;

    QJsonArray cn;
    for(int i=0; i<chanels.size(); ++i){
        cn.push_back(chanels[i].json());
    }
    r["chanel"] = cn;

    return r;
}

void ht::setVal(QJsonObject data){
    name = data["name"].toString();

    ray.clear();
    QJsonArray r = data["ray"].toArray();
    for(int i=0; i<r.size(); ++i){
        ray.push_back(htparams(r[i].toObject()));
    }
    area.clear();
    QJsonArray a = data["area"].toArray();
    for(int i=0; i<a.size(); ++i){
        area.push_back(htparams(a[i].toObject()));
    }
    chanels.clear();
    QJsonArray c = data["chanel"].toArray();
    for(int i=0; i<c.size(); ++i){
        chanels.push_back(chan(c[i].toObject()));
    }
}

void ht::addRay(){
    ray.push_back(htparams(0,0,0,0,0,0,1.0,1.0,"НОВЫЙ"));
}

void ht::addArea(){
    area.push_back(htparams(0,0,0,0,0,0,0.0,0.0,"НОВЫЙ"));
}

void ht::addChanel(){
    chanels.push_back(chan(QVector<int>(),QVector<int>(),QString()));
}
