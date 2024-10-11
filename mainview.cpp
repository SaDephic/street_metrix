#include "mainview.h"

#include "custom/dialog.h"
#include "view/vdbconnection.h"

MainView::MainView(){
    setWindowTitle("Анализатор транспортных потоков");
    setMinimumSize(QSize(450,450));
    createMenu();

    vp = new vproject;
    vc = new vcrosses;

    connect(vc,&vcrosses::unlock,this,&MainView::setEnabledMagistral);

    tw = new QTabWidget;
    tw->setTabPosition(QTabWidget::West);
    tw->addTab(vp, "Основные");
    tw->addTab(vc, "Магистраль");

    setCentralWidget(tw);

    connect(prj,&project::nameProject,this,&MainView::projectTitle);

    QShortcut *closeselect = new QShortcut(this);
    closeselect->setKey(QKeySequence(tr("CTRL+W")));
    connect(closeselect,&QShortcut::activated,this,&QWidget::close);
}

MainView::~MainView(){
    delete se;
    delete envdb;
}

void MainView::createMenu(){
    QMenuBar *mbar = new QMenuBar();

    QMenu *menu = new QMenu("Проект");
    QAction *act = nullptr;
    act = new QAction("Новый");
    act->setShortcut(QKeySequence(tr("F3")));
    connect(act,&QAction::triggered,this,&MainView::newProject);
    menu->addAction(act);
    menu->addSeparator();
    act = new QAction("Открыть");
    act->setShortcut(QKeySequence(tr("F4")));
    connect(act,&QAction::triggered,this,&MainView::openProject);
    menu->addAction(act);
    act = new QAction("Сохранить");
    act->setShortcut(QKeySequence(tr("F5")));
    connect(act,&QAction::triggered,this,&MainView::saveProject);
    menu->addAction(act);
    menu->addSeparator();
    act = new QAction("Выход");
    act->setShortcut(QKeySequence(tr("CTRL+W")));
    connect(act,&QAction::triggered,this,&MainView::close);
    menu->addAction(act);
    mbar->addMenu(menu);

    QAction *settings = new QAction("Настройки");
    settings->setShortcut(QKeySequence(tr("F10")));
    connect(settings,&QAction::triggered,new vdbconnection,&vdbconnection::show);
    mbar->addAction(settings);

    rbar = new QMenuBar;
    QAction *user = new QAction(aut->user);
    connect(user,&QAction::triggered,this,&MainView::showPermissions);
    rbar->addAction(user);

    mbar->setCornerWidget(rbar,Qt::TopRightCorner);

    setMenuBar(mbar);
}

void MainView::showPermissions(){
    w  = new QWidget;
    w->setWindowModality(Qt::ApplicationModal);
    w->setWindowTitle("О пользователе");
    w->setMinimumWidth(300);

    QFormLayout *fl = new QFormLayout(w);
    fl->addRow("Пользователь:",new QLabel(aut->user));
    fl->addRow("Регион:",new QLabel(aut->region == -1 ? "*" : QString::number(aut->region)));
    fl->addRow("Район(ы):",new QLabel(aut->areas_text()));

    QPushButton *ok = new QPushButton("Ок");
    connect(ok,&QPushButton::clicked,w,&QWidget::close);
    fl->addRow(ok);

    QPushButton *swu = new QPushButton("Сменить пользователя");
    connect(swu,&QPushButton::clicked,this,&MainView::swapuser);
    fl->addRow(swu);
    w->show();
}

void MainView::swapuser(){
    w->close();
    emit logout();
}

void MainView::newProject(){
    Dialog *d = new Dialog("Новый", "Создать новый проект?\nДанные будут очищены.",{"Да","Нет"});
    if(!d->exec()){
        return;
    }

    disconnect(prj,&project::nameProject,this,&MainView::projectTitle);
    delete centralWidget();
    delete prj;
    delete rbar;

    rbar = nullptr;
    tw = nullptr;
    vp = nullptr;
    vc = nullptr;

    prj = new project;

    setWindowTitle("Анализатор транспортных потоков");
    setMinimumSize(QSize(450,450));
    createMenu();

    vp = new vproject;
    vc = new vcrosses;

    tw = new QTabWidget;
    tw->setTabPosition(QTabWidget::West);
    tw->addTab(vp, "Основные");
    tw->addTab(vc, "Магистраль");

    setCentralWidget(tw);

    connect(prj,&project::nameProject,this,&MainView::projectTitle);

    QShortcut *closeselect = new QShortcut(this);
    closeselect->setKey(QKeySequence(tr("CTRL+W")));
    connect(closeselect,&QShortcut::activated,this,&QWidget::close);
}

void MainView::closeEvent(QCloseEvent *e){
    Dialog *d = new Dialog("Выход", "Завершить работу с программой?",{"Да","Нет"});
    if(d->exec()){
        e->accept();
    }else{
        e->ignore();
        return;
    }

    QWidget::closeEvent(e);
}
