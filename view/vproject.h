#ifndef VPROJECT_H
#define VPROJECT_H

#include <QWidget>
#include <QLabel>
#include <QGroupBox>
#include <QRadioButton>
#include <QComboBox>
#include <QPushButton>
#include <QCheckBox>
#include <QTimeEdit>
#include <QTableWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QMenuBar>
#include <QAction>

#include "common/project.h"
extern project *prj;

class vproject : public QWidget{
    Q_OBJECT

    project *project = prj;

    QLineEdit *nameproject = nullptr;
    QTableWidget *matrix = nullptr;
    QTableWidget *kspk = nullptr;
    QComboBox *into = nullptr;
    QComboBox *intht = nullptr;
    QRadioButton *ray = nullptr;
    QRadioButton *area = nullptr;
    QCheckBox *c = nullptr;
    QCheckBox *e = nullptr;
    QTimeEdit *bsp = nullptr;
    QTimeEdit *esp = nullptr;
    QCheckBox *exec = nullptr;

public:
    vproject();

private:
    QMenuBar *createMenu();
    void connectState(bool state);

private slots:
    void calcProject();
    //changes
    void nameprojectChanged(QString name);
    void tcalcChanged();
    void techModeCalc(int);
    void techModeExec(int);
    void oftime(QTime time);
    void totime(QTime time);
    void timeexec(int);
    void mtxchanged(int,int);
    void kspkchanged(int,int);
    //loaded
    void updateView();
};

#endif // VPROJECT_H

