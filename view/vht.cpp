#include "vht.h"

vht::vht(cobject *obj, bool ryar){
    object = obj;
    ra = ryar;
    setview();
}

void vht::updateViewHT(){
    connectState(false);
    if(ra){
        for(int i=0; i<object->htparams.ray.size(); ++i){
            params->insertRow(params->rowCount());
            params->setItem(i,0,new QTableWidgetItem(QString::number(object->htparams.ray[i].L)));
            params->setItem(i,1,new QTableWidgetItem(QString::number(object->htparams.ray[i].R)));
            params->setItem(i,2,new QTableWidgetItem(QString::number(object->htparams.ray[i].PlanL)));
            params->setItem(i,3,new QTableWidgetItem(QString::number(object->htparams.ray[i].PlanM)));
            params->setItem(i,4,new QTableWidgetItem(QString::number(object->htparams.ray[i].PlanR)));
            params->setItem(i,5,new QTableWidgetItem(QString::number(object->htparams.ray[i].Fl)));
            params->setItem(i,6,new QTableWidgetItem(QString::number(object->htparams.ray[i].Fr)));
            params->setItem(i,7,new QTableWidgetItem(object->htparams.ray[i].desc));
        }
    }else{
        for(int i=0; i<object->htparams.area.size(); ++i){
            params->insertRow(params->rowCount());
            params->setItem(i,0,new QTableWidgetItem(QString::number(object->htparams.area[i].L)));
            params->setItem(i,1,new QTableWidgetItem(QString::number(object->htparams.area[i].R)));
            params->setItem(i,2,new QTableWidgetItem(QString::number(object->htparams.area[i].Plan)));
            params->setItem(i,3,new QTableWidgetItem(object->htparams.area[i].desc));
        }
    }
    connectState(true);
    emit updateView();
}

void vht::setview(){
    QHBoxLayout *hl = new QHBoxLayout(this);
    hl->setMenuBar(createMenu());

    params = new QTableWidget;
    if(ra){
        params->setColumnCount(8);
        params->setHorizontalHeaderLabels({"Прямой","Обратный","КС-П","КС-С","КС-О","Луч-П","Луч-О","Описание"});
    }else{
        params->setColumnCount(4);
        params->setHorizontalHeaderLabels({"Прямой","Обратный","КС","Описание"});
    }
    params->resizeRowsToContents();
    params->resizeColumnsToContents();

    hl->addWidget(params);
    v = new vpicht(object,ra);
    connect(this,&vht::updateView,v,&vpicht::updateView);
    hl->addWidget(v);
}

QMenuBar *vht::createMenu(){
    QMenuBar *mbar = new QMenuBar;
    QAction *act = nullptr;
    act = new QAction("Добавить");
    connect(act,&QAction::triggered,this,&vht::addrow);
    mbar->addAction(act);
    act = new QAction("Удалить");
    connect(act,&QAction::triggered,this,&vht::removerow);
    mbar->addAction(act);
    act = new QAction("Клонировать");
    connect(act,&QAction::triggered,this,&vht::clone);
    mbar->addAction(act);
    return mbar;
}

void vht::addRow(){
    connectState(false);
    int insrow = params->rowCount();
    params->insertRow(insrow);
    if(ra){
        params->setItem(insrow,0,new QTableWidgetItem(QString::number(object->htparams.ray.last().L)));
        params->setItem(insrow,1,new QTableWidgetItem(QString::number(object->htparams.ray.last().R)));
        params->setItem(insrow,2,new QTableWidgetItem(QString::number(object->htparams.ray.last().PlanL)));
        params->setItem(insrow,3,new QTableWidgetItem(QString::number(object->htparams.ray.last().PlanM)));
        params->setItem(insrow,4,new QTableWidgetItem(QString::number(object->htparams.ray.last().PlanR)));
        params->setItem(insrow,5,new QTableWidgetItem(QString::number(object->htparams.ray.last().Fl)));
        params->setItem(insrow,6,new QTableWidgetItem(QString::number(object->htparams.ray.last().Fr)));
        params->setItem(insrow,7,new QTableWidgetItem(object->htparams.ray.last().desc));
    }else{
        params->setItem(insrow,0,new QTableWidgetItem(QString::number(object->htparams.area.last().L)));
        params->setItem(insrow,1,new QTableWidgetItem(QString::number(object->htparams.area.last().R)));
        params->setItem(insrow,2,new QTableWidgetItem(QString::number(object->htparams.area.last().Plan)));
        params->setItem(insrow,3,new QTableWidgetItem(object->htparams.area.last().desc));
    }
    connectState(true);
    emit updateView();
}

void vht::removeRow(){
    connectState(false);
    params->removeRow(params->rowCount()-1);
    connectState(true);
    emit updateView();
}

void vht::connectState(bool state){
    if(state){
        connect(this,&vht::updateView,v,&vpicht::updateView);
        connect(params,&QTableWidget::cellChanged,this,&vht::cellchanged);

    }else{
        disconnect(this,&vht::updateView,v,&vpicht::updateView);
        disconnect(params,&QTableWidget::cellChanged,this,&vht::cellchanged);
    }
}

void vht::addrow(){
    ra ? object->htparams.addRay() : object->htparams.addArea();
    addRow();
}

void vht::removerow(){
    if(ra){
        if(!object->htparams.ray.isEmpty())
            object->htparams.ray.removeLast();
        else
            return;
    }else{
        if(!object->htparams.area.isEmpty())
            object->htparams.area.removeLast();
        else
            return;
    }
    removeRow();
}

void vht::cellchanged(int r, int c){
    if(ra){
        if(c<5){
            if(c == 0)
                object->htparams.ray[r].L = params->item(r,c)->text().toInt();
            if(c == 1)
                object->htparams.ray[r].R = params->item(r,c)->text().toInt();
            if(c == 2)
                object->htparams.ray[r].PlanL = params->item(r,c)->text().toInt();
            if(c == 3)
                object->htparams.ray[r].PlanM = params->item(r,c)->text().toInt();
            if(c == 4)
                object->htparams.ray[r].PlanR = params->item(r,c)->text().toInt();
        }
        if(c>4 && c<7){
            connectState(false);
            QString text = params->item(r,c)->text();
            text.replace(":",".");
            text.replace("-",".");
            text.replace(",",".");
            params->item(r,c)->setText(text);
            connectState(true);
            if(c == 5)
                object->htparams.ray[r].Fl = text.toDouble();
            if(c == 6)
                object->htparams.ray[r].Fr = text.toDouble();
        }
        if(c==7){
            if(c==7){
                object->htparams.ray[r].desc = params->item(r,c)->text();
                return;
            }
        }
    }else{
        if(c==0)
            object->htparams.area[r].L = params->item(r,c)->text().toInt();
        if(c==1)
            object->htparams.area[r].R = params->item(r,c)->text().toInt();
        if(c==2)
            object->htparams.area[r].Plan = params->item(r,c)->text().toInt();
        if(c==3){
            object->htparams.area[r].desc = params->item(r,c)->text();
            return;
        }
    }
    emit updateView();
}
