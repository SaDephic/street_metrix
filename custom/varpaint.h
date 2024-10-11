#ifndef VARPAINT_H
#define VARPAINT_H

#include <QJsonArray>
#include <QRandomGenerator>
#include <QJsonObject>
#include <QPointF>
#include <QString>
#include <QVector>
#include <QMap>
#include <QRectF>

struct chan{
    QVector<int> f;
    QVector<int> b;
    QString desc;
    chan(QVector<int> fw,QVector<int> bw,QString desct);
    chan(QJsonObject data);
    QString fin();
    QString bin();
    bool operator ==(const chan &o);
    QJsonObject json();
    void setVal(QJsonObject data);
};

struct htparams{
    int L;
    int R;
    int Plan;
    int PlanL;
    int PlanM;
    int PlanR;
    double Fl;
    double Fr;
    QString desc;
    htparams(int l, int r, int p0, int p1, int p2, int p3, double fl, double fe, QString text);
    htparams(QJsonObject data);
    bool operator ==(const htparams &o);
    static bool compare_rays(htparams &st1, htparams &st2);
    static bool compare_area(htparams &st1, htparams &st2);
    QJsonObject json();
    void setVal(QJsonObject data);
};

class Point{
public:
    Point(int w, int h);

    QString name;

    bool bear(int w, int h);
    Point mediana(int nw, int nh);
    ulong distanceSqrd(int w,int h);
    Point summ(int nw, int nh,int bw, int bh);
    Point simetr(float f);
    bool isGood();

    int w,h;
    int w_,h_;

    bool operator==(const Point &other) const;
};

class PlanMap{
public:
    PlanMap(int w,int h);
    ~PlanMap();

    void SetPlan(int x, int y, int planl);
    int width();
    int height();
    int get(int x,int y);
    QVector<QVector<int>> map;

private:
    bool nopaint = false;
    int width_,height_;
};

class tcHT{
public:
    QString time;
    QString date;
    int forward;
    int backward;
    double coef;
    int csht;
    QString desc;

    void multData(double value);
    void setKS(int value);

private:
    void coefCalc();
};

class Voronoi {
public:
    QVector<htparams> sts;

    QVector<Point> createPoints(int count);
    QVector<Point> points;
    QVector<Point> coords;

    QVector<Point> sprayes;
    QVector<Point> bogoses;

    QVector<int> plans;
    PlanMap *planMap;
    QMap<int, QList<QPoint>> listPlans;

    float delW;
    float delH;

private:
    int width;
    int height;

public:
    Voronoi(int width,int height);
    ~Voronoi(){}

    QRectF get_rect_area();
    int RandomBetween(int low, int high);

    void addRays(htparams st);
    void addArea(htparams st);

    void addBogoses(QVector<Point> bogs);
    void addSpray(Point point,QString name);

    void clearSpray();

    void makeDiagramm();
    void makeAreals();

    int GetPlan(int w,int h);

    static bool compare(const QPoint& first, const QPoint& second);
};

#endif // VARPAINT_H
