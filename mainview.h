#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QAction>
#include <QWidget>
#include <QLabel>
#include <QFormLayout>
#include <QPushButton>
#include <QCloseEvent>
#include <QFileDialog>

#include "view/vcrosses.h"
#include "view/vproject.h"

#include "auth/stateauth.h"
#include "common/project.h"
#include "custom/sysenv.h"
extern stateauth *aut;
extern sysenv *se;
extern project *prj;

class MainView : public QMainWindow{
    Q_OBJECT

    QWidget *w = nullptr;
    QMenuBar *rbar = nullptr;

    QTabWidget *tw = nullptr;
    vproject *vp = nullptr;
    vcrosses *vc = nullptr;

public: signals:
    void logout();

public:
    MainView();
    ~MainView();

    void createMenu();

private slots:
    void showPermissions();
    void swapuser();
    void newProject();
    void openProject(){
        QString val = QFileDialog::getOpenFileName(nullptr,"Открыть проект:", se->last_folder,tr("Проект (*.estb)"));
        if(!val.isEmpty()){
            QFile f(val);
            QDir d(val); d.cdUp(); se->last_folder = d.path();
            if(f.open(QFile::ReadOnly)){
                prj->setVal(QJsonDocument::fromJson(f.readAll()).object());
                vc->setEnabled(false);
                f.close();
            }
        }
    }
    void saveProject(){
        QString val = QFileDialog::getSaveFileName(nullptr,"Сохранить как:", se->last_folder,tr("Проект (*.estb)"));
        if(!val.isEmpty()){
            QFile f(val);
            QDir d(val); d.cdUp(); se->last_folder = d.path();
            if(f.open(QFile::WriteOnly)){
                f.write(QJsonDocument(prj->json()).toJson(/*QJsonDocument::Compact*/));
                f.close();
            }
        }
    }
    void setEnabledMagistral(){
        vc->setEnabled(true);
    }
    void projectTitle(){
        if(prj->name.isEmpty())
            setWindowTitle("Анализатор транспортных потоков");
        else
            setWindowTitle("Анализатор транспортных потоков - " + prj->name);
    }

protected:
    void closeEvent(QCloseEvent *e);
};
#endif // MAINVIEW_H
