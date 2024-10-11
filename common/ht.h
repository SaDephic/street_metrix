#ifndef HT_H
#define HT_H

#include <QObject>
#include "custom/varpaint.h"

class ht : public QObject{
    Q_OBJECT

public:
    QString name;
    QVector<htparams> ray;
    QVector<htparams> area;
    QVector<chan> chanels;

public:
    ht();

    QJsonObject json();
    void setVal(QJsonObject data);

    void addRay();
    void addArea();
    void addChanel();
};

#endif // HT_H
