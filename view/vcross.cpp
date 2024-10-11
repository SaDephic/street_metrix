#include "vcross.h"

#include "view/vgraph.h"
#include "view/vdata.h"
#include "custom/dep.h"

vcross::vcross(cobject *obj){
    object = obj;
    connect(object,&cobject::loaded,this,&vcross::updateView);

    vds = new vdateselect(obj);
    gd = new vgeneratedata;

    QVBoxLayout *v = new QVBoxLayout(this);
    v->setMenuBar(createMenu());

    QTabWidget *tab = new QTabWidget;
    tab->addTab(createData(),"Данные");
    tab->addTab(createHT(),"ХТ");
    v->addWidget(tab);

    name->setText(object->data.getNameHT());

    v->setContentsMargins(0, 0, 0, 0);
    v->setSpacing(0);
}

QMenuBar *vcross::createMenu(){
    QAction *act = nullptr;

    QMenuBar *mbar = new QMenuBar;

    act = new QAction("Исключить");
    connect(act,&QAction::triggered,this,&vcross::exclude);
    mbar->addAction(act);

    act = new QAction(object->data.descObject);
    act->setEnabled(false);
    mbar->addAction(act);

    return mbar;
}

QMenuBar *vcross::createMenuData(){
    QMenuBar *mbar = new QMenuBar;

    QAction *act = nullptr;
    QMenu *manage = new QMenu("Управление");

    act = new QAction("Экспорт параметров в файл");
    connect(act,&QAction::triggered,this,&vcross::export_settings_file);
    manage->addAction(act);
    manage->addSeparator();

    act = new QAction("Сгенерировать");
    act->setShortcut(QKeySequence("CTRL+G"));
    connect(act,&QAction::triggered,this,&vcross::generate_data);
    manage->addAction(act);
    manage->addSeparator();

    act = new QAction("Импорт из БД");
    act->setShortcut(QKeySequence("CTRL+A"));
    connect(act,&QAction::triggered,this,&vcross::import_data_db);
    manage->addAction(act);

    act = new QAction("Импорт из *.csv");
    connect(act,&QAction::triggered,this,&vcross::import_data_csv);
    manage->addAction(act);
    manage->addSeparator();

    act = new QAction("Экспорт в *.csv");
    connect(act,&QAction::triggered,this,&vcross::export_data_csv);
    manage->addAction(act);
    manage->addSeparator();

    mbar->addMenu(manage);

    return mbar;
}

QMenuBar *vcross::createMenuCalc(){
    QMenuBar *mbar = new QMenuBar;
    QAction *act = nullptr;
    act = new QAction("Расчитать");
    connect(act,&QAction::triggered,this,&vcross::calcRes);
    mbar->addAction(act);
    act = new QAction("Отобразить");
    connect(act,&QAction::triggered,this,&vcross::showRes);
    mbar->addAction(act);
    return mbar;
}

QMenuBar *vcross::createMenuChanels(){
    QMenuBar *mbar = new QMenuBar;
    QAction *act = nullptr;
    act = new QAction("Добавить");
    connect(act,&QAction::triggered,this,&vcross::addRowCalc);
    mbar->addAction(act);
    act = new QAction("Удалить");
    connect(act,&QAction::triggered,this,&vcross::removeRowCalc);
    mbar->addAction(act);
    return mbar;
}

QWidget *vcross::createData(){
    QHBoxLayout *h = nullptr;

    QWidget *w = new QWidget;
    QHBoxLayout *hl = new QHBoxLayout(w);
    hl->setMenuBar(createMenuData());

    QVBoxLayout *vl = new QVBoxLayout;

    QComboBox *dates = new QComboBox;
    dates->setMinimumWidth(450);
    dates->setMaximumWidth(450);

    QPushButton *remdate = new QPushButton("Удалить");
    connect(remdate,&QPushButton::clicked,this,&vcross::removeDate);
    remdate->setMaximumWidth(70);

    h = new QHBoxLayout;
    h->addWidget(dates);
    h->addWidget(remdate);
    h->addStretch();
    vl->addLayout(h);

    QCheckBox *summ = new QCheckBox("Суммировать");
    QComboBox *cmb = new QComboBox;
    cmb->setMinimumWidth(200);

    h = new QHBoxLayout;
    h->addWidget(summ);
    h->addWidget(cmb);
    h->addStretch();
    vl->addLayout(h);

    vl->addWidget(new vdata(object));
    hl->addLayout(vl);

    return w;
}

QWidget *vcross::createHT(){
    QWidget *w = new QWidget;

    QTabWidget *tab = new QTabWidget;
    tab->addTab(calc(),"Расчет");
    tab->addTab(rays(),"Лучи");
    tab->addTab(areas(),"Области");
    tab->addTab(graphs(),"Графики");
    tab->setTabPosition(QTabWidget::West);

    QHBoxLayout *h = new QHBoxLayout(w);
    h->addWidget(tab);

    h->setContentsMargins(0, 0, 0, 0);
    h->setSpacing(0);

    return w;
}

QWidget *vcross::calc(){
    QWidget *w = new QWidget;
    QHBoxLayout *hb = new QHBoxLayout(w);

    QGroupBox *gb = new QGroupBox("Параметры");
    QComboBox *params = new QComboBox;
    name = new QLineEdit;
    connect(name,&QLineEdit::textChanged,this,&vcross::nameChanged);

    QFormLayout *fl = new QFormLayout(gb);
    fl->addRow("Выбор даты:",params);
    fl->addRow("Имя ХТ:",name);
    fl->setMenuBar(createMenuCalc());

    QGroupBox *gbt = new QGroupBox("Каналы");
    QHBoxLayout *hl = new QHBoxLayout(gbt);
    hl->setMenuBar(createMenuChanels());
    table = new QTableWidget;
    connect(table,&QTableWidget::cellChanged,this,&vcross::changeinout);
    table->setColumnCount(3);
    table->setHorizontalHeaderLabels({"Каналы 'Прямой'","Каналы 'Обратный'","Примечание"});
    table->resizeRowsToContents();
    table->resizeColumnsToContents();
    hl->addWidget(table);
    hl->setContentsMargins(0, 0, 0, 0);
    hl->setSpacing(0);
    fl->addRow(gbt);

    QTableWidget *rescalc = new QTableWidget;
    rescalc->setColumnCount(5);
    rescalc->setHorizontalHeaderLabels({"Прямой","Обратный","КС-расчитаная","Отношение","Примечание"});
    rescalc->resizeRowsToContents();
    rescalc->resizeColumnsToContents();

    hb->addWidget(gb);
    hb->addWidget(rescalc);

    return w;
}

QWidget *vcross::rays(){
    r = new vht(object,true);
    return r;
}

QWidget *vcross::areas(){
    a = new vht(object,false);
    return a;
}

QWidget *vcross::graphs(){
    g = new vgraph();
    return g;
}

void vcross::connectState(bool state){
    if(state){
        connect(table,&QTableWidget::cellChanged,this,&vcross::changeinout);
        connect(object,&cobject::loaded,this,&vcross::updateView);
    }else{
        disconnect(table,&QTableWidget::cellChanged,this,&vcross::changeinout);
        disconnect(object,&cobject::loaded,this,&vcross::updateView);
    }
}

void vcross::exclude(){
    emit exclude_object(object->data);
}

void vcross::import_data_db(){
    vds->show();
}

void vcross::addRowCalc(){
    object->htparams.addChanel();
    table->insertRow(table->rowCount());

    QTableWidgetItem *vif = new QTableWidgetItem;
    table->setItem(table->rowCount()-1,0,vif);//values
    QTableWidgetItem *vib = new QTableWidgetItem;
    table->setItem(table->rowCount()-1,1,vib);//values
    table->setItem(table->rowCount()-1,2,new QTableWidgetItem(""));//text
}

void vcross::removeRowCalc(){
    disconnect(table,&QTableWidget::cellChanged,this,&vcross::changeinout);
    if(!object->htparams.chanels.isEmpty()){
        object->htparams.chanels.removeLast();
        if(table->rowCount()>0)
            table->removeRow(table->rowCount()-1);
    }
    connect(table,&QTableWidget::cellChanged,this,&vcross::changeinout);
}

void vcross::changeinout(int r, int c){
    QString text = table->item(r,c)->text();
    if(!text.isEmpty()){
        if(c==0 || c==1){
            disconnect(table,&QTableWidget::cellChanged,this,&vcross::changeinout);
            text.replace(".",";");
            text.replace(":",";");
            text.replace("-",";");
            text.replace(",",";");
            table->item(r,c)->setText(text);
            if(c==0) object->htparams.chanels[r].f = dep::fromChansString(text,';');
            if(c==1) object->htparams.chanels[r].b = dep::fromChansString(text,';');
            connect(table,&QTableWidget::cellChanged,this,&vcross::changeinout);
        }
        if(c==2) object->htparams.chanels[r].desc = text;
    }else{
        if(c==0) object->htparams.chanels[r].f = QVector<int>();
        if(c==1) object->htparams.chanels[r].b = QVector<int>();
        if(c==2) object->htparams.chanels[r].desc = text;
    }
}

void vcross::nameChanged(QString name){
    object->htparams.name = name;
}

void vcross::updateView(){
    connectState(false);
    name->setText(object->htparams.name);
    //chanels
    for(int i=0; i<object->htparams.chanels.size(); ++i){
        table->insertRow(table->rowCount());
        table->setItem(i,0,new QTableWidgetItem(object->htparams.chanels[i].fin()));
        table->setItem(i,1,new QTableWidgetItem(object->htparams.chanels[i].bin()));
        table->setItem(i,2,new QTableWidgetItem(object->htparams.chanels[i].desc));
    }
    r->updateViewHT();
    a->updateViewHT();
    connectState(true);
}
