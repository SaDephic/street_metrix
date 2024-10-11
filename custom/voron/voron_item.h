#ifndef VORON_ITEM_H
#define VORON_ITEM_H

#include <QObject>
#include <QGraphicsPathItem>
#include <QGraphicsScene>

#include "../varpaint.h"

class Viron : public QObject, public QGraphicsPathItem{
    Q_OBJECT
public:
    Viron();
    ~Viron(){}
protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
};

class voron_item : public QObject, public QGraphicsPathItem{
    Q_OBJECT
public: signals:
    void reset_area_rect(QRectF rect);

private:
    QMap<uint8_t,QColor> color;

    QGraphicsPathItem *ipoints;
    QGraphicsPathItem *ibogoses;
    QGraphicsPathItem *isprayes;
    QGraphicsPathItem *itext;

    QList<Viron *> listPlans;

public:
    Voronoi *v = nullptr;

public:
    voron_item();
    ~voron_item(){}

    void vorUpdate();
    void setNewVoron(Voronoi *v);
    void createColor();
    void clearItemVoron();
    void clearPointsItemVoron();

    void clear_added_sprays();

public slots:
    void updateDataHT(QVector<tcHT> data);//update point in view
};

#endif // VORON_ITEM_H
