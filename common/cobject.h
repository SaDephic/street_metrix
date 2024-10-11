#ifndef COBJECT_H
#define COBJECT_H

#include <QObject>
#include "common/ht.h"
#include "common/rac.h"
#include "common/statistic.h"

class cobject : public QObject{
    Q_OBJECT

public: signals:
    void loaded();

public:
    racobject data;
    ht htparams;
    statistic statis;

public:
    cobject(racobject d){
        data = d;
    }

    QJsonObject json();
    void setVal(QJsonObject data);

};

#endif // COBJECT_H
