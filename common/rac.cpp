#include "rac.h"

bool racobject::operator ==(const racobject &o){
    return region == o.region && area == o.area && number == o.number;
}

QString racobject::getNameHT(){
    return QString::number(region) + "."
           + QString::number(area) + "."
           + QString::number(subarea) + "."
           + QString::number(number);
}

QJsonObject racobject::json(){
    QJsonObject r;
    r["r"] = (int)region;
    r["a"] = (int)area;
    r["s"] = (int)subarea;
    r["c"] = (int)number;
    return r;
}

void racobject::setVal(QJsonObject data){
    region = data["r"].toInt();
    area = data["a"].toInt();
    subarea = data["s"].toInt();
    number = data["c"].toInt();
    descObject = data["descObject"].toString();
}

racobject::racobject(){}

racobject::racobject(QJsonObject data){
    setVal(data);
}

rac::rac(){}

QVector<int> rac::getRegions(){
    QVector<int> r;
    for(int i=0; i<cross.size(); ++i){
        if(!r.contains(cross[i].region))
            r.push_back(cross[i].region);
    }
    std::sort(r.begin(),r.end());
    return r;
}

QVector<int> rac::getAreas(int r){
    QVector<int> a;
    for(int i=0; i<cross.size(); ++i){
        if(cross[i].region == r){
            if(!a.contains(cross[i].area))
                a.push_back(cross[i].area);
        }
    }
    std::sort(a.begin(),a.end());
    return a;
}

QVector<int> rac::getSubarea(int r, int a){
    QVector<int> s;
    for(int i=0; i<cross.size(); ++i){
        if(cross[i].region == r && cross[i].area == a){
            if(!s.contains(cross[i].subarea)){
                s.push_back(cross[i].subarea);
            }
        }
    }
    std::sort(s.begin(),s.end());
    return s;
}

QVector<int> rac::getCrosses(int r, int a, int s){
    QVector<int> c;
    for(int i=0; i<cross.size(); ++i){
        if(cross[i].region == r && cross[i].area == a && cross[i].subarea == s){
            c.push_back(cross[i].number);
        }
    }
    std::sort(c.begin(),c.end());
    return c;
}

QString rac::name_region(int r){
    for(int i=0; i<cross.size(); ++i){
        if(cross[i].region == r)
            return cross[i].descRegion;
    }
    return "Без имени";
}

QString rac::name_area(int r, int a){
    for(int i=0; i<cross.size(); ++i){
        if(cross[i].region == r && cross[i].area == a)
            return cross[i].descArea;
    }
    return "Без имени";
}

QString rac::name_subarea(int r, int a, int s){
    for(int i=0; i<cross.size(); ++i){
        if(cross[i].region == r && cross[i].area == a && cross[i].subarea == s)
            return cross[i].descSubarea;
    }
    return "Без имени";
}

racobject rac::name_cross(int r, int a, int s, int c){
    for(int i=0; i<cross.size(); ++i){
        if(cross[i].region == r && cross[i].area == a && cross[i].subarea == s && cross[i].number == c)
            return cross[i];
    }
    return racobject();
}

void rac::setRegion(QVector<QMap<QString, dbvalue> > data){
    cross.clear();
    for(int i=0; i<data.size(); ++i){
        QMap<QString,dbvalue> c = data[i];
        racobject dcross;
        dcross.number = c["id"].value_int();
        dcross.descObject = c["describ"].value_string();
        dcross.subarea = c["subarea"].value_int();
        dcross.descSubarea = QString::number(dcross.subarea);
        dcross.region = c["region"].value_int();
        dcross.descRegion = c["nameregion"].value_string();
        dcross.area = c["area"].value_int();
        dcross.descArea = c["namearea"].value_string();
        dcross.period = QJsonDocument::fromJson(c["interval"].value_json()).object().value("lvs").toArray();
        cross.push_back(dcross);
    }
    emit loaded();
}
