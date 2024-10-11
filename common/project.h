#ifndef PROJECT_H
#define PROJECT_H

#include <QObject>
#include "common/cobject.h"
#include "common/datareader.h"
#include <QMessageBox>

struct KSPK{
    int ks;
    int pk;
    QString desc;
    QJsonObject json();
    void setVal(QJsonObject data);
    KSPK();
    KSPK(QJsonObject data);
};

class project : public QObject{
    Q_OBJECT

public: signals:
    void updateTab();
    void loaded();
    void nameProject();
    void removeAllObjects();

public:
    QJsonArray objs;
    datareader *cd = nullptr;

    QVector<cobject*> objects;

    QString name;
    bool ra = true;
    bool bcalc = true;
    bool ecalc = false;
    QTime of;
    QTime to;
    bool exec = false;
    QVector<QVector<int>> mtx;
    QVector<KSPK> kspk;

public:
    project();

    QJsonObject json();
    void setVal(QJsonObject data);

public slots:
    void addObject(racobject r);
    bool existCross(int r, int a, int s, int c);

    void setCrosses();
private slots:
    void loadCrosses();
};

#endif // PROJECT_H
