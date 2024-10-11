#ifndef ADDCROSS_H
#define ADDCROSS_H

#include <QWidget>
#include <QLabel>
#include <QShortcut>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QComboBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QPushButton>

#include "common/datareader.h"
#include "common/project.h"
#include "common/rac.h"
extern rac *drac;
extern project *prj;

class AddCross : public QWidget{
    Q_OBJECT

    bool running = false;

    datareader *cd = nullptr;
    QLabel *statusStage = nullptr;

    QGroupBox *gb = nullptr;
    QComboBox *regid = nullptr;
    QComboBox *areid = nullptr;
    QComboBox *subid = nullptr;
    QTableWidget *table = nullptr;

    QVector<int> cr;
    QVector<int> ca;
    QVector<int> cs;
    QVector<int> cc;

    int r;
    int a;
    int s;

public: signals:
    void addCross(racobject r);

public:
    explicit AddCross(QWidget *parent = nullptr);
    void show();

private slots:
    void refreshData();
    void setStage(QString stage);
    void errorMessage(QString error);

    void changeRegion();
    void changeArea();
    void changeSubarea();
    void addCrossToProject();

private:
    void lockrac();

    void toView();
    void toViewCrosses();
    void setCurrentCombos();
    QVector<QString> getRegs();
    QVector<QString> getAres();
    QVector<QString> getSubs();
};

#endif // ADDCROSS_H
