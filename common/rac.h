#ifndef RAC_H
#define RAC_H

#include <QJsonArray>
#include <QObject>
#include "db/dbvalue.h"

#include "db/dbdcon.h"
extern dbdcon *envdb;

struct racobject{
    int region = 0;//id region
    int area = 0;//id area
    int subarea = 0;//id subarea
    int number = 0;//id number
    QString descRegion = QString();//string of region
    QString descArea = QString();//string of area
    QString descSubarea = QString();//string of subarea
    QString descObject = QString();//string of object
    QJsonArray period = QJsonArray();//periods
    bool operator == (const racobject& o);//equal
    QString getNameHT();
    QJsonObject json();
    void setVal(QJsonObject data);
    racobject();
    racobject(QJsonObject data);
};

class rac : public QObject{
    Q_OBJECT

public:
    QVector<racobject> cross;

public: signals:
    void loaded();
    void loadDataSelectedCross();

public:
    rac();
    QVector<int> getRegions();
    QVector<int> getAreas(int r);
    QVector<int> getSubarea(int r, int a);
    QVector<int> getCrosses(int r, int a, int s);
    QString name_region(int r);
    QString name_area(int r, int a);
    QString name_subarea(int r, int a, int s);
    racobject name_cross(int r, int a, int s, int c);

public slots:
    void setRegion(QVector<QMap<QString,dbvalue>> data);
};

#endif // RAC_H
