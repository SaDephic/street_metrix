#ifndef DATAREADER_H
#define DATAREADER_H

#include "db/dbconnection.h"

#include <QThread>
#include <QMessageBox>

#include "common/rac.h"
extern rac *drac;

class datareader : public QObject{
    Q_OBJECT

public:
    QThread *thread = nullptr;
    dbconnection *db = nullptr;

public:
    datareader();

    void createNP();
    void cretateSD(racobject rasc, QVector<QDate> l);
    void cretateMD(racobject rasc, int MM, int YY);

    void updateRegion();
    void askDates();
};

#endif // DATAREADER_H
