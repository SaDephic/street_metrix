#ifndef VCROSS_H
#define VCROSS_H

#include <QWidget>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QMenuBar>
#include <QTabWidget>
#include <QLabel>

#include <QComboBox>
#include <QPushButton>
#include <QCheckBox>
#include <QGroupBox>
#include <QLineEdit>
#include <QTableWidget>

#include "view/vdateselect.h"
#include "view/vgeneratedata.h"
#include "view/vht.h"
#include "view/vgraph.h"
#include "common/cobject.h"

class vcross : public QWidget{
    Q_OBJECT

    QTableWidget *table = nullptr;//data for calc
    QLineEdit *name = nullptr;//name HT

    vht *r = nullptr;
    vht *a = nullptr;
    vgraph *g = nullptr;

    vdateselect *vds = nullptr;
    vgeneratedata *gd = nullptr;

public:
    cobject *object;

public: signals:
    void exclude_object(racobject o);

public:
    vcross(cobject *obj);

private:
    QMenuBar *createMenu();
    QMenuBar *createMenuData();
    QMenuBar *createMenuCalc();
    QMenuBar *createMenuChanels();
    QWidget *createData();
    QWidget *createHT();
    QWidget *calc();
    QWidget *rays();
    QWidget *areas();
    QWidget *graphs();
    void connectState(bool state);

private slots:
    void exclude();
    void export_settings_file(){
        //save configs to file
        qDebug() << "void export_settings_file(){";
    }
    void generate_data(){
        gd->show();
    }
    void import_data_db();
    void import_data_csv(){
        //get data from file
        qDebug() << "void import_data_csv(){";
    }
    void export_data_csv(){
        //save data to csv
        qDebug() << "void export_data_csv(){";
    }
    void addRowCalc();
    void removeRowCalc();
    void removeDate(){
        //remove one date
        qDebug() << "void removeDate(){";
    }
    void calcRes(){
        //calc result
        qDebug() << "void calcRes(){";
    }
    void showRes(){
        //remove one date
        qDebug() << "void showRes(){";
    }
    //changes
    void changeinout(int r,int c);
    void nameChanged(QString name);
    //loaded
    void updateView();
};

#endif // VCROSS_H

