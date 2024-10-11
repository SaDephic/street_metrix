#ifndef VDBCONNECTION_H
#define VDBCONNECTION_H

#include <QWidget>
#include <QTabWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include <QAction>
#include <QToolButton>
#include <QStyle>

#include <QFormLayout>
#include <QVBoxLayout>

#include <QThread>
#include <QShortcut>
#include <QCloseEvent>

#include <QJsonDocument>

#include "db/dbdcon.h"
extern dbdcon *envdb;
#include "db/dbconnection.h"

class vdbconnection : public QWidget{
    Q_OBJECT

    QLabel *conncheck = nullptr;
    QLineEdit *ip = nullptr;
    QLineEdit *port = nullptr;
    QLineEdit *databasename = nullptr;
    QLineEdit *username = nullptr;
    QLineEdit *userpassword = nullptr;
    QPushButton *apply = nullptr;

    QToolButton *button = nullptr;
    QToolButton *dbutton = nullptr;

    dbdcon *envtest = nullptr;
    QThread *thread = nullptr;

public:
    vdbconnection();

public:
    void toView();
    void show();
    void setEditsEnabled(bool state);
    void setApplyEnabled();
    void apply_settings();
    void setStatus(status v);

protected:
    void closeEvent(QCloseEvent *e);
};

#endif // VDBCONNECTION_H
