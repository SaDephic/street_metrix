#ifndef VHT_H
#define VHT_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QMenuBar>

#include "common/cobject.h"
#include "view/vpicht.h"

class vht : public QWidget{
    Q_OBJECT

    bool ra = false;
    cobject *object;

    vpicht *v = nullptr;//voronov_view
    QTableWidget *params = nullptr;//params_data

public: signals:
    void updateView();

public:
    vht(cobject *obj, bool ryar);
    void updateViewHT();

private:
    void setview();
    QMenuBar *createMenu();
    void addRow();
    void removeRow();
    void connectState(bool state);

private slots:
    void addrow();
    void removerow();
    void cellchanged(int r, int c);
    void clone(){
        qDebug() << "void clone(){" << object->data.number;
    }
};

#endif // VHT_H
