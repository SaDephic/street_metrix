#include "voron_item.h"

Viron::Viron(){
    setAcceptHoverEvents(true);
}

void Viron::hoverEnterEvent(QGraphicsSceneHoverEvent *event){
    Q_UNUSED(event)
    setToolTip(QVariant(property("area").toInt()%100).toString());
    QGraphicsItem::hoverEnterEvent(event);
}

void Viron::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
    Q_UNUSED(event)
    QGraphicsItem::hoverLeaveEvent(event);
}


voron_item::voron_item(){
    isprayes = new QGraphicsPathItem(this);
    isprayes->setZValue(5);
    isprayes->setBrush(Qt::black);
    isprayes->setPen(QPen(Qt::black));

    ibogoses = new QGraphicsPathItem(this);
    ibogoses->setZValue(5);
    ibogoses->setBrush(Qt::black);

    //текст
    itext = new QGraphicsPathItem(this);
    itext->setZValue(5);
    itext->setBrush(Qt::black);

    //точки
    ipoints = new QGraphicsPathItem(this);
    ipoints->setZValue(5);
    ipoints->setPen(QPen(Qt::black));
    ipoints->setBrush(Qt::blue);
}

void voron_item::setNewVoron(Voronoi *v){
    QVector<Point> _spr;

    if(this->v)
        _spr = this->v->sprayes;


    Voronoi *_v_ = this->v;

    this->v = v;
    emit reset_area_rect(v->get_rect_area());

    if(_v_){
        this->v->sprayes = _spr;
        delete _v_;
    }

    createColor();
    vorUpdate();
    scene()->update();
}

void voron_item::createColor(){
    //!!!
    foreach(QGraphicsPathItem *path, listPlans){
        listPlans.removeOne(dynamic_cast<Viron*>(path));
        delete path;
    }
    for(int i=0; i<v->listPlans.size(); i++){
        QColor randcolor = QColor(qRgb(rand() % 200 + 50, rand() % 200 + 55, rand() % 200 + 50));
        Viron *it = new Viron;
        it->setParentItem(this);
        it->setZValue(-1);
        it->setBrush(randcolor);
        it->setPen(randcolor);
        listPlans.append(it);
    }

    color[0]=qRgb(255,255,255);
    if(v->sts.size()!=0){
        foreach (auto s,v->sts ) {
            color[s.PlanL]=(qRgb(rand() % 200 + 50, rand() % 200 + 55, rand() % 200 + 50));
            color[s.PlanR]=(qRgb(rand() % 200 + 50, rand() % 200 + 55, rand() % 200 + 50));
            color[s.PlanM]=(qRgb(rand() % 200 + 50, rand() % 200 + 55, rand() % 200 + 50));
        }
    }
    if(v->plans.size()!=0){
        foreach (auto p, v->plans) {
            color[p]=(qRgb(rand() % 200 + 50, rand() % 200 + 55, rand() % 200 + 50));
        }
    }
}

void voron_item::clearItemVoron(){
    foreach(QGraphicsPathItem *path, listPlans){
        listPlans.removeOne(dynamic_cast<Viron*>(path));
        delete path;
    }
    ipoints->setPath(QPainterPath());
    ibogoses->setPath(QPainterPath());
    isprayes->setPath(QPainterPath());
    itext->setPath(QPainterPath());

    if(v){
        v->sts.clear();

        v->listPlans.clear();
        v->bogoses.clear();
    }
}

void voron_item::clearPointsItemVoron(){
    ipoints->setPath(QPainterPath());
    ibogoses->setPath(QPainterPath());
    isprayes->setPath(QPainterPath());
    itext->setPath(QPainterPath());
    if(v){
        v->points.clear();
        v->coords.clear();
        v->sprayes.clear();
        v->bogoses.clear();
    }
}

void voron_item::clear_added_sprays(){
    v->sprayes.clear();
    vorUpdate();
}

void voron_item::updateDataHT(QVector<tcHT> data){
    if(v){
        for(int i=0; i<data.size(); i++){
            if((data[i].forward != 0)||(data[i].backward != 0)){
                v->addSpray(Point(data[i].backward,data[i].forward),data[i].time + "[" + QVariant(data[i].csht).toString() + "] " +
                            QString::asprintf("%.2f",data[i].coef) + " " + data[i].date);
            }
        }
        vorUpdate();
    }
}

void voron_item::vorUpdate(){
    if(!v->listPlans.isEmpty()){
        QList<int> kv = v->listPlans.keys();
        for(int i=0; i<kv.size(); i++){
            QPainterPath _tmpA = QPainterPath();
            if(!v->listPlans.value(kv[i]).isEmpty()){
                _tmpA.moveTo(v->listPlans.value(kv[i]).first());
                for(int p=0; p<v->listPlans.value(kv[i]).size(); p++){
                    _tmpA.lineTo(v->listPlans.value(kv[i])[p]);
                }
                _tmpA.lineTo(v->listPlans.value(kv[i]).first());
                if(!listPlans.isEmpty()){
                    listPlans[i]->setPath(_tmpA);
                    listPlans[i]->setProperty("area",QVariant(kv[i]).toString());
                }
            }
        }
    }

    QPainterPath _ttmp = QPainterPath();
    QPainterPath _tmp = QPainterPath();
    foreach(auto p, v->points){
        int w = p.w, h = v->planMap->height() - p.h;
        QString ww=QString::asprintf("(%d,%d)",p.h_,p.w_);
        _ttmp.addText(QPointF(w-30,h-10),QFont("san-serif",-1,8),ww);
        _tmp.addEllipse(w-2,h-2,4,4);
    }
    ipoints->setPath(_tmp);

    _tmp = QPainterPath();
    foreach(auto p, v->bogoses){
        int w = p.w, h = v->planMap->height() - p.h;
        QString ww=QString::asprintf("(%d,%d)",p.h_,p.w_);
        _ttmp.addText(QPointF(w-30,h-10),QFont("san-serif",-1,8),ww);
        _tmp.addEllipse(w-2,h-2,4,4);
    }
    ibogoses->setPath(_tmp);

    _tmp = QPainterPath();
    foreach(auto p, v->sprayes){
        int w = p.w, h = v->planMap->height() - p.h;
        _tmp.addEllipse(w-0.5,h-0.5,1,1);
    }
    isprayes->setPath(_tmp);

    itext->setPath(_ttmp);
}
