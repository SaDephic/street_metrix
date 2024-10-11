#ifndef VPICHT_H
#define VPICHT_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

#include "common/cobject.h"
#include "../custom/voron/view_voron.h"

class rotatedText : public QLabel{
    Q_OBJECT
public:
    explicit rotatedText(QWidget *parent=0);
    explicit rotatedText(const QString &text, QWidget *parent=0);
protected:
    void paintEvent(QPaintEvent*);
    QSize sizeHint() const ;
    QSize minimumSizeHint() const;
};

class vpicht : public QWidget{
    Q_OBJECT

    bool ra;
    cobject *object;

    view_voron *voronov = nullptr;
    QLabel *position = nullptr;

    rotatedText *osxF = nullptr;
    QLabel *osyB = nullptr;

public:
    vpicht(cobject *obj, bool ryar){
        object = obj;
        ra = ryar;

        compact();
    }

    void compact(){
        position = new QLabel;
        voronov = new view_voron;
        osxF = new rotatedText("Прямой");
        osyB = new QLabel("Обратный");

        QHBoxLayout *hp = new QHBoxLayout(this);

        //for
        QVBoxLayout *v = new QVBoxLayout;
        v->addWidget(osxF);
        v->addStretch();

        //full
        hp->addLayout(v);

        //back
        QVBoxLayout *vp = new QVBoxLayout;
        QHBoxLayout *vpp = new QHBoxLayout;
        vpp->addWidget(position);
        vpp->addStretch();
        vp->addLayout(vpp);
        vp->addWidget(voronov);//picture
        QHBoxLayout *dvp = new QHBoxLayout;
        dvp->addStretch();
        dvp->addWidget(osyB);
        vp->addLayout(dvp);

        //full
        hp->addLayout(vp);
    }

private:
    int getMaximumForward(){
        int max = 0;
        foreach(htparams cur, ra?object->htparams.ray:object->htparams.area)
            if(cur.L>max)
                max=cur.L;
        return max;
    }

    int getMaximumBackward(){
        int max = 0;
        foreach(htparams cur, ra?object->htparams.ray:object->htparams.area)
            if(cur.R>max)
                max=cur.R;
        return max;
    }

public slots:
    void updateView(){
        qDebug() << object->data.json() << ra << sender()->metaObject()->className();
        /*Voronoi *vor = new Voronoi(getMaximumForward(),getMaximumBackward());
        QVector<htparams> val = ra?object->htparams.ray:object->htparams.area;
        for(int i=0; i<val.size(); i++){
            ra?vor->addRays(val[i]):vor->addArea(val[i]);
        }
        ra?vor->makeDiagramm():vor->makeAreals();
        voronov->vor_item->setNewVoron(vor);*/
    }
};

#endif // VPICHT_H
