#ifndef VCROSSES_H
#define VCROSSES_H

#include <QWidget>
#include <QVBoxLayout>
#include <QTabWidget>

#include "custom/customtabstyle.h"
#include "view/addcross.h"

#include <view/vcross.h>

#include "common/project.h"
extern project *prj;

class vcrosses : public QWidget{
    Q_OBJECT

    QVector<cobject*> *objs = &prj->objects;

    QTabWidget *tabs;
    QVector<vcross*> tabcross;

public: signals:
    void loadObjects();
    void unlock();
public:
    vcrosses(){
        connect(prj,&project::removeAllObjects,this,&vcrosses::removeAllObjects);
        connect(this,&vcrosses::loadObjects,prj,&project::setCrosses);

        tabs = new QTabWidget;
        tabs->setTabPosition(QTabWidget::West);
        tabs->tabBar()->setStyle(new CustomTabStyle);
        QVBoxLayout *vl = new QVBoxLayout(this);

        QMenuBar *mbar = new QMenuBar;

        QMenu *cross = new QMenu("Перекресток");
        QAction *addCross = new QAction("Добавить");
        addCross->setShortcut(QKeySequence(tr("CTRL+S")));
        connect(addCross,&QAction::triggered,new AddCross,&AddCross::show);
        cross->addAction(addCross);
        mbar->addMenu(cross);

        vl->setMenuBar(mbar);

        vl->addWidget(tabs);
        connect(prj,&project::updateTab,this,&vcrosses::updateCrosses);

        vl->setContentsMargins(0, 0, 0, 0);
        vl->setSpacing(0);
    }

    void updateCrosses(){
        QVector<int> cc;
        for(int i=0; i<tabs->count(); ++i){
            cc.push_back(tabs->tabText(i).toInt());
        }
        for(int i=0; i<objs->size(); ++i){
            if(!cc.contains(objs->value(i)->data.number)){
                vcross *v = new vcross(objs->value(i));
                tabcross.push_back(v);
                connect(v,&vcross::exclude_object,this,&vcrosses::removeCross);
                tabs->addTab(v,QString::number(objs->value(i)->data.number));
            }
        }
        emit unlock();
    }

private slots:
    void removeCross(racobject o){
        for(int i=0; i<tabcross.size(); ++i){
            if(tabcross[i]->object->data == o){
                objs->removeOne(tabcross[i]->object);//remove data obj
                for(int c=0; c<tabs->count(); ++c){
                    if(tabs->tabText(c).toInt() == o.number){
                        tabs->removeTab(c);//remove tab
                        tabcross[i]->deleteLater();
                        tabcross.removeOne(tabcross[i]);
                    }
                }
            }
        }
    }
    void removeAllObjects(){
        while(!objs->isEmpty()){
            removeCross(objs->first()->data);
        }
        emit loadObjects();
    }
};

#endif // VCROSSES_H
