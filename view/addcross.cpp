#include "addcross.h"

AddCross::AddCross(QWidget *parent) : QWidget{parent}{
    setWindowTitle("Добавить объекты");
    setWindowModality(Qt::ApplicationModal);
    setMinimumSize(QSize(600,400));

    connect(this,&AddCross::addCross,prj,&project::addObject);

    cd = new datareader;
    connect(drac,&rac::loaded,this,&AddCross::toView);

    statusStage = new QLabel;
    regid = new QComboBox;
    areid = new QComboBox;
    subid = new QComboBox;
    table = new QTableWidget;
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionMode(QAbstractItemView::NoSelection);
    table->setColumnCount(4);
    table->setHorizontalHeaderLabels({"#","Имя перекрестка","","Интервал опроса"});

    QVBoxLayout *vl = new QVBoxLayout(this);
    vl->addWidget(statusStage);

    gb = new QGroupBox("Объекты");
    gb->setVisible(false);

    QFormLayout *flsel = new QFormLayout(gb);
    flsel->addRow("Регион:",regid);
    flsel->addRow("Район",areid);
    flsel->addRow("Подрайон:",subid);
    flsel->addWidget(table);
    vl->addWidget(gb);

    QHBoxLayout *end = new QHBoxLayout;
    QPushButton *conf = new QPushButton("Принять");
    conf->setMaximumWidth(80);
    connect(conf,&QPushButton::clicked,this,&QWidget::close);
    end->addStretch();
    end->addWidget(conf);
    vl->addLayout(end);


    QShortcut *closeselect = new QShortcut(this);
    closeselect->setKey(QKeySequence(tr("CTRL+W")));
    connect(closeselect,&QShortcut::activated,this,&QWidget::close);
}

void AddCross::show(){
    if(!running)
        refreshData();
    QWidget::show();
}

void AddCross::refreshData(){
    running = true;

    statusStage->setVisible(true);
    gb->setVisible(false);

    cd->createNP();
    connect(cd->db,&dbconnection::currentStage,this,&AddCross::setStage);
    connect(cd->db,&dbconnection::errorConnection,this,&AddCross::errorMessage);
    cd->updateRegion();
}

void AddCross::setStage(QString stage){
    setCursor(Qt::BusyCursor);

    gb->setVisible(false);

    statusStage->setText("Читаю: " + stage);
}

void AddCross::errorMessage(QString error){
    QMessageBox msg;
    msg.setIcon(QMessageBox::Critical);
    msg.setWindowTitle("Ошибка");
    msg.setText(error);
    msg.exec();

    statusStage->setText(error);
    setCursor(Qt::ArrowCursor);
}

void AddCross::changeRegion(){
    disconnect(areid,&QComboBox::currentIndexChanged,this,&AddCross::changeArea);
    disconnect(subid,&QComboBox::currentIndexChanged,this,&AddCross::changeSubarea);

    if(!drac->cross.isEmpty()){
        r = cr[regid->currentIndex()];
        ca = drac->getAreas(r);
        if(!ca.isEmpty()){
            a = ca.first();
            cs = drac->getSubarea(r,a);
            if(!cs.isEmpty()){
                s = cs.first();
                cc = drac->getCrosses(r,a,s);
            }
        }
    }

    areid->clear();
    subid->clear();
    areid->addItems(getAres());
    subid->addItems(getSubs());

    connect(areid,&QComboBox::currentIndexChanged,this,&AddCross::changeArea);
    connect(subid,&QComboBox::currentIndexChanged,this,&AddCross::changeSubarea);
    toViewCrosses();
}

void AddCross::changeArea(){
    disconnect(subid,&QComboBox::currentIndexChanged,this,&AddCross::changeSubarea);

    if(!drac->cross.isEmpty()){
        if(!ca.isEmpty()){
            a = ca[areid->currentIndex()];
            cs = drac->getSubarea(r,a);
            if(!cs.isEmpty()){
                s = cs.first();
                cc = drac->getCrosses(r,a,s);
            }
        }
    }

    subid->clear();
    subid->addItems(getSubs());

    connect(subid,&QComboBox::currentIndexChanged,this,&AddCross::changeSubarea);
    toViewCrosses();
}

void AddCross::changeSubarea(){
    if(!drac->cross.isEmpty()){
        s = cs[subid->currentIndex()];
        cc = drac->getCrosses(r,a,s);
    }
    toViewCrosses();
}

void AddCross::addCrossToProject(){
    QPushButton *pb = dynamic_cast<QPushButton*>(sender());
    int _r = pb->property("r").toInt();
    int _a = pb->property("a").toInt();
    int _s = pb->property("s").toInt();
    int _c = pb->property("c").toInt();
    pb->setEnabled(false);
    emit addCross(drac->name_cross(_r,_a,_s,_c));
    lockrac();
}

void AddCross::lockrac(){
    if(prj->objects.isEmpty()){
        regid->setEnabled(true);
        areid->setEnabled(true);
        subid->setEnabled(true);
    }else{
        regid->setEnabled(false);
        areid->setEnabled(false);
        subid->setEnabled(false);
    }
}

void AddCross::toView(){
    statusStage->setVisible(false);
    gb->setVisible(true);

    lockrac();

    if(!drac->cross.isEmpty()){
        cr = drac->getRegions();
        if(!cr.isEmpty()){
            r = cr.first();
            ca = drac->getAreas(r);
            if(!ca.isEmpty()){
                a = ca.first();
                cs = drac->getSubarea(r,a);
                if(!cs.isEmpty()){
                    s = cs.first();
                    cc = drac->getCrosses(r,a,s);
                }
            }
        }

        setCurrentCombos();

    }else{
        statusStage->setText("Ошибка чтения данных или данные отсутствуют!");
    }

    setCursor(Qt::ArrowCursor);
    running = false;
}

void AddCross::toViewCrosses(){
    while(table->rowCount()>0){
        table->removeRow(0);
    }

    QVector<racobject> crac;
    for(int i=0; i<cc.size(); ++i){
        crac.push_back(drac->name_cross(r,a,s,cc[i]));
    }

    if(!crac.isEmpty()){
        for(int i=0; i<crac.size(); ++i){
            table->insertRow(table->rowCount());            
            table->setItem(i,0,new QTableWidgetItem(QString::number(crac[i].number)));            
            table->setItem(i,1,new QTableWidgetItem(crac[i].descObject));
            QPushButton *addCrossTo = new QPushButton("Добавить");
            if(prj->existCross(crac[i].region,crac[i].area,crac[i].subarea,crac[i].number))
                addCrossTo->setEnabled(false);
            addCrossTo->setMaximumHeight(20);
            connect(addCrossTo,&QPushButton::clicked,this,&AddCross::addCrossToProject);
            addCrossTo->setProperty("r",crac[i].region);
            addCrossTo->setProperty("a",crac[i].area);
            addCrossTo->setProperty("s",crac[i].subarea);
            addCrossTo->setProperty("c",crac[i].number);

            table->setCellWidget(i,2,addCrossTo);
            QComboBox *interval = new QComboBox;
            racobject ro = drac->name_cross(crac[i].region,crac[i].area,crac[i].subarea,crac[i].number);
            QVector<QString> ints;
            QJsonArray ia = ro.period;
            for(int c=0; c<ia.size(); ++c){
                ints.push_back(QString::number(ia[c].toObject()["period"].toInt()));
            }
            interval->addItems(ints);
            table->setCellWidget(i,3,interval);
        }
    }

    table->resizeRowsToContents();
    table->resizeColumnsToContents();
}

void AddCross::setCurrentCombos(){
    disconnect(regid,&QComboBox::currentIndexChanged,this,&AddCross::changeRegion);
    disconnect(areid,&QComboBox::currentIndexChanged,this,&AddCross::changeArea);
    disconnect(subid,&QComboBox::currentIndexChanged,this,&AddCross::changeSubarea);

    regid->clear();
    areid->clear();
    subid->clear();

    regid->addItems(getRegs());
    areid->addItems(getAres());
    subid->addItems(getSubs());

    toViewCrosses();

    connect(regid,&QComboBox::currentIndexChanged,this,&AddCross::changeRegion);
    connect(areid,&QComboBox::currentIndexChanged,this,&AddCross::changeArea);
    connect(subid,&QComboBox::currentIndexChanged,this,&AddCross::changeSubarea);
}

QVector<QString> AddCross::getRegs(){
    QVector<QString> t;
    for(int i=0; i<cr.size(); ++i){
        t.push_back(drac->name_region(cr[i]));
    }
    return t;
}

QVector<QString> AddCross::getAres(){
    QVector<QString> t;
    for(int i=0; i<ca.size(); ++i){
        t.push_back(drac->name_area(r,ca[i]));
    }
    return t;
}

QVector<QString> AddCross::getSubs(){
    QVector<QString> t;
    for(int i=0; i<cs.size(); ++i){
        t.push_back(drac->name_subarea(r,a,cs[i]));
    }
    return t;
}

