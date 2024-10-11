#include "dep.h"

QString dep::posToString(QPointF p){
    return QString::number(p.x()) + ":" + QString::number(p.y());
}

QPointF dep::posFromString(QString sp){
    auto a = sp.split(":");
    if(a.size() < 2 || a.size() > 2)
        return QPointF();
    return QPointF(a[0].toDouble(),a[1].toDouble());
}

QVector<int> dep::fromJsonValInt(QJsonArray a){
    QVector<int> r;
    for(int i=0; i<a.size(); ++i){
        r.push_back(a[i].toInt());
    }
    return r;
}

QVector<QString> dep::fromJsonValString(QJsonArray a){
    QVector<QString> r;
    for(int i=0; i<a.size(); ++i){
        r.push_back(a[i].toString());
    }
    return r;
}

QVector<int> dep::fromVecString(QVector<QString> val){
    QVector<int> r;
    for(int i=0; i<val.size(); ++i){
        r.push_back(val[i].toInt());
    }
    return r;
}

QVector<int> dep::fromChansString(QString text, QChar spl){
    auto aspl = text.split(spl);
    QVector<int> r;
    for(int i=0; i<aspl.size(); ++i){
        if(aspl[i].toInt() == 0)
            continue;
        r.push_back(aspl[i].toInt());
    }
    return r;
}
