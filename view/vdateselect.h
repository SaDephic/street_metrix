#ifndef VDATESELECT_H
#define VDATESELECT_H

#include "common/cobject.h"
#include "common/datareader.h"
#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QCalendarWidget>
#include <QTableWidget>
#include <QCheckBox>
#include <QHeaderView>
#include <QShortcut>

class vdateselect : public QWidget{
    Q_OBJECT

    bool running = false;

    datareader *cd = nullptr;

    QCalendarWidget *calen = nullptr;
    QTableWidget *selectedItems = nullptr;
    QPushButton *pb = nullptr;
    QPushButton *pc = nullptr;

public:
    cobject *object;        

public:
    vdateselect(cobject *obj){
        setMinimumWidth(300);

        object = obj;

        setWindowTitle("Импорт данных из БД");
        cd = new datareader;

        QVBoxLayout *vl = new QVBoxLayout(this);

        QHBoxLayout *caltab = new QHBoxLayout;

        calen = new QCalendarWidget;
        connect(calen,&QCalendarWidget::currentPageChanged,this,&vdateselect::mothchandged);

        selectedItems = new QTableWidget;
        selectedItems->setColumnCount(2);
        selectedItems->setHorizontalHeaderLabels({"Дата","Заполненность,%"});
        selectedItems->setSelectionMode(QAbstractItemView::NoSelection);
        selectedItems->verticalHeader()->hide();
        selectedItems->resizeColumnsToContents();
        selectedItems->resizeRowsToContents();

        caltab->addWidget(calen);
        caltab->addWidget(selectedItems);

        QHBoxLayout *hb = new QHBoxLayout;

        pb = new QPushButton("Импортировать");
        connect(pb,&QPushButton::clicked,this,&vdateselect::addStatistic);

        pc = new QPushButton("Отмена");
        connect(pc,&QPushButton::clicked,this,&vdateselect::close);

        hb->addWidget(pc);
        hb->addStretch();
        hb->addWidget(pb);

        vl->addLayout(caltab);
        vl->addLayout(hb);

        QShortcut *closeselect = new QShortcut(this);
        closeselect->setKey(QKeySequence(tr("CTRL+W")));
        connect(closeselect,&QShortcut::activated,this,&QWidget::close);
    }
    void show(){
        if(!running)
            readMothses();
        QWidget::show();
    }

private:
    void lockActs(bool state){
        pb->setEnabled(state);
        pc->setEnabled(state);
        calen->setEnabled(state);
        selectedItems->setEnabled(state);
    }
    void readMothses(){
        setCursor(Qt::BusyCursor);
        running = true;

        lockActs(false);
        cd->cretateMD(object->data,calen->monthShown(),calen->yearShown());
        connect(cd->db,&dbconnection::answer,this,&vdateselect::setDates);
        cd->askDates();
    }
    void setDates(QVector<QMap<QString,dbvalue>> res){
        while(selectedItems->rowCount()>0){
            selectedItems->removeRow(selectedItems->rowCount()-1);
        }
        for(int i=0; i<res.size(); ++i){
            selectedItems->insertRow(selectedItems->rowCount());
            selectedItems->setCellWidget(i,0,new QCheckBox(res[i]["date"].value_string()));
        }

        selectedItems->resizeColumnsToContents();
        selectedItems->resizeRowsToContents();
        lockActs(true);

        running = false;
        setCursor(Qt::ArrowCursor);
    }

private slots:
    void addStatistic(){

    }
    void mothchandged(int y, int m){
        setCursor(Qt::BusyCursor);
        lockActs(false);
        cd->cretateMD(object->data,m,y);
        connect(cd->db,&dbconnection::answer,this,&vdateselect::setDates);
        cd->askDates();
    }
};

#endif // VDATESELECT_H
