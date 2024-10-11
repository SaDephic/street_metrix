#ifndef STATISTIC_H
#define STATISTIC_H

#include <QObject>
#include <QMap>
#include <QJsonObject>

struct statistic{
    QMap<QDate,QJsonObject> data;
    void addStatic(QMap<QDate,QJsonObject> q){
        data.insert(q);
    }
};

#endif // STATISTIC_H
