#ifndef VDATA_H
#define VDATA_H

#include "common/cobject.h"
#include <QWidget>
#include <QTableWidget>
#include <QGroupBox>
#include <QHBoxLayout>

class vdata : public QWidget{
    Q_OBJECT

    cobject *object;

    QTableWidget *data = nullptr;

public:
    vdata(cobject *obj){
        object = obj;

        QHBoxLayout *hl = new QHBoxLayout(this);
        data = new QTableWidget;
        data->setColumnCount(16);
        data->resizeColumnsToContents();
        data->resizeRowsToContents();

        QGroupBox *gb = new QGroupBox("Каналы");
        hl->addWidget(data);
        hl->addWidget(gb);
    }
    ~vdata(){}
};

#endif // VDATA_H
