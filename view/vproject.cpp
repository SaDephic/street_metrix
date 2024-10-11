#include "vproject.h"

vproject::vproject(){
    connect(project,&project::loaded,this,&vproject::updateView);

    nameproject = new QLineEdit;
    connect(nameproject,&QLineEdit::textChanged,this,&vproject::nameprojectChanged);

    matrix = new QTableWidget;
    matrix->setColumnCount(3);
    for(int i=0; i<4; ++i){
        matrix->insertRow(matrix->rowCount());
        for(int c=0; c<matrix->columnCount(); ++c){
            matrix->setItem(i,c,new QTableWidgetItem("0"));
        }
    }
    matrix->resizeColumnsToContents();
    matrix->resizeRowsToContents();

    int c0=2,c1=1,c2=3;
    for(int i=1; i<matrix->rowCount(); ++i){
        matrix->item(i,0)->setText(QString::number(c0));
        matrix->item(i,1)->setText(QString::number(c1));
        matrix->item(i,2)->setText(QString::number(c2));
        c0+=3; c1+=3; c2+=3;
    }
    mtxchanged(0,0);
    connect(matrix,&QTableWidget::cellChanged,this,&vproject::mtxchanged);

    kspk = new QTableWidget;
    kspk->setColumnCount(3);
    kspk->setHorizontalHeaderLabels({"КС","ПК","Примечание"});
    for(int i=0; i<12; ++i){
        kspk->insertRow(kspk->rowCount());
        kspk->setItem(i,0,new QTableWidgetItem("0"));
        kspk->setItem(i,1,new QTableWidgetItem("0"));
        kspk->setItem(i,2,new QTableWidgetItem());
    }
    kspk->resizeColumnsToContents();
    kspk->resizeRowsToContents();
    connect(kspk,&QTableWidget::cellChanged,this,&vproject::kspkchanged);

    mtxchanged(0,0);
    kspkchanged(0,0);

    into = new QComboBox;
    intht = new QComboBox;

    ray = new QRadioButton("Лучи");
    connect(ray,&QRadioButton::clicked,this,&vproject::tcalcChanged);

    area = new QRadioButton("Области");
    connect(area,&QRadioButton::clicked,this,&vproject::tcalcChanged);

    ray->setChecked(true);

    c = new QCheckBox("Расчет ХТ");
    c->setCheckState(Qt::Checked);
    connect(c,&QCheckBox::stateChanged,this,&vproject::techModeCalc);

    e = new QCheckBox("Исполнение ХТ");
    connect(e,&QCheckBox::stateChanged,this,&vproject::techModeExec);

    bsp = new QTimeEdit; bsp->setMinimumWidth(100);
    bsp->setTime(QTime(0,0));
    connect(bsp,&QTimeEdit::timeChanged,this,&vproject::oftime);

    esp = new QTimeEdit; esp->setMinimumWidth(100);
    esp->setTime(QTime(2,0));
    connect(esp,&QTimeEdit::timeChanged,this,&vproject::totime);

    //init data
    oftime(QTime(0,0));
    totime(QTime(2,0));

    exec = new QCheckBox("Выполнять");
    connect(exec,&QCheckBox::stateChanged,this,&vproject::timeexec);

    QHBoxLayout *hl = new QHBoxLayout(this);
    //***********************************************************
    QGroupBox *gbMTX = new QGroupBox("Матрица приоритетов");
    QFormLayout *f0 = new QFormLayout(gbMTX);
    f0->addWidget(matrix);
    QGroupBox *gbPC = new QGroupBox("Привязка КС к ПК");
    QFormLayout *f1 = new QFormLayout(gbPC);
    f1->addWidget(kspk);

    //***********************************************************
    QGroupBox *gbPCHT = new QGroupBox("Параметры расчета ХТ");
    QFormLayout *f2 = new QFormLayout(gbPCHT);;
    f2->addRow("Интервал опроса",into);
    f2->addRow("Интервал ХТ",intht);

    QGroupBox *gbMC = new QGroupBox("Способ расчета");
    QVBoxLayout *mtc = new QVBoxLayout(gbMC);
    mtc->addWidget(ray);
    mtc->addWidget(area);
    mtc->setMenuBar(createMenu());

    QGroupBox *gbTM = new QGroupBox("Технический режим");
    QVBoxLayout * vlb = new QVBoxLayout(gbTM);
    vlb->addWidget(c);
    vlb->addWidget(e);

    QGroupBox *gbWYNS = new QGroupBox("Работа по НК СК");
    QHBoxLayout *hb = new QHBoxLayout(gbWYNS);

    hb->addWidget(new QLabel("От:"));
    hb->addWidget(bsp);

    hb->addWidget(new QLabel("До:"));
    hb->addWidget(esp);

    hb->addWidget(exec);
    hb->addStretch();

    QVBoxLayout *vl = new QVBoxLayout;
    QFormLayout *fl = new QFormLayout;
    fl->addRow("Имя проекта:",nameproject);
    vl->addLayout(fl);
    vl->addWidget(gbMC);
    vl->addWidget(gbPCHT);
    vl->addWidget(gbTM);
    vl->addWidget(gbWYNS);
    vl->addStretch();

    hl->addLayout(vl);
    hl->addWidget(gbMTX);
    hl->addWidget(gbPC);
    hl->addStretch();
}

QMenuBar *vproject::createMenu(){
    QMenuBar * mbar = new QMenuBar;
    QAction *calc = new QAction("Расчитать");
    connect(calc, &QAction::triggered,this,&vproject::calcProject);
    mbar->addAction(calc);
    return mbar;
}

void vproject::connectState(bool state){
    if(state){
        connect(nameproject,&QLineEdit::textChanged,this,&vproject::nameprojectChanged);
        connect(matrix,&QTableWidget::cellChanged,this,&vproject::mtxchanged);
        connect(kspk,&QTableWidget::cellChanged,this,&vproject::kspkchanged);
        connect(ray,&QRadioButton::clicked,this,&vproject::tcalcChanged);
        connect(area,&QRadioButton::clicked,this,&vproject::tcalcChanged);
        connect(c,&QCheckBox::stateChanged,this,&vproject::techModeCalc);
        connect(e,&QCheckBox::stateChanged,this,&vproject::techModeExec);
        connect(bsp,&QTimeEdit::timeChanged,this,&vproject::oftime);
        connect(esp,&QTimeEdit::timeChanged,this,&vproject::totime);
        connect(exec,&QCheckBox::stateChanged,this,&vproject::timeexec);
    }else{
        disconnect(nameproject,&QLineEdit::textChanged,this,&vproject::nameprojectChanged);
        disconnect(matrix,&QTableWidget::cellChanged,this,&vproject::mtxchanged);
        disconnect(kspk,&QTableWidget::cellChanged,this,&vproject::kspkchanged);
        disconnect(ray,&QRadioButton::clicked,this,&vproject::tcalcChanged);
        disconnect(area,&QRadioButton::clicked,this,&vproject::tcalcChanged);
        disconnect(c,&QCheckBox::stateChanged,this,&vproject::techModeCalc);
        disconnect(e,&QCheckBox::stateChanged,this,&vproject::techModeExec);
        disconnect(bsp,&QTimeEdit::timeChanged,this,&vproject::oftime);
        disconnect(esp,&QTimeEdit::timeChanged,this,&vproject::totime);
        disconnect(exec,&QCheckBox::stateChanged,this,&vproject::timeexec);
    }
}

void vproject::calcProject(){
    qDebug() << "void calcProject(){";
}

void vproject::nameprojectChanged(QString name){
    project->name = name;
    emit project->nameProject();
}

void vproject::tcalcChanged(){
    if(ray->isChecked())
        project->ra = true;
    if(area->isChecked())
        project->ra = false;
}

void vproject::techModeCalc(int){
    c->checkState() == Qt::Checked ? project->bcalc=true : project->bcalc=false;
}

void vproject::techModeExec(int){
    e->checkState() == Qt::Checked ? project->ecalc=true : project->ecalc=false;
}

void vproject::oftime(QTime time){
    project->of = time;
}

void vproject::totime(QTime time){
    project->to = time;
}

void vproject::timeexec(int){
    exec->checkState() == Qt::Checked ? project->exec=true : project->exec=false;
}

void vproject::mtxchanged(int, int){
    QVector<QVector<int>> vall;
    for(int r=0; r<matrix->rowCount(); ++r){
        QVector<int> v;
        for(int c=0; c<matrix->columnCount(); ++c){
            v.push_back(matrix->item(r,c)->text().toInt());
        }
        vall.push_back(v);
    }
    project->mtx = vall;
}

void vproject::kspkchanged(int, int){
    QVector<KSPK> vall;
    for(int r=0; r<kspk->rowCount(); ++r){
        KSPK v;
        v.ks = kspk->item(r,0)->text().toInt();
        v.pk = kspk->item(r,1)->text().toInt();
        v.desc = kspk->item(r,2)->text();
        vall.push_back(v);
    }
    project->kspk = vall;
}

void vproject::updateView(){
    connectState(false);
    //title project
    nameproject->setText(project->name);
    emit project->nameProject();
    //set matrix
    for(int r=0; r<project->mtx.size(); ++r){
        for(int c=0; c<project->mtx[r].size(); ++c){
            matrix->item(r,c)->setText(QString::number(project->mtx[r][c]));
        }
    }
    //set kspk
    for(int i=0; i<project->kspk.size(); ++i){
        kspk->item(i,0)->setText(QString::number(project->kspk[i].ks));
        kspk->item(i,1)->setText(QString::number(project->kspk[i].pk));
        kspk->item(i,2)->setText(project->kspk[i].desc);
    }
    //times
    bsp->setTime(project->of);
    esp->setTime(project->to);
    project->exec ? exec->setCheckState(Qt::Checked) : exec->setCheckState(Qt::Unchecked);
    project->ra ? ray->setChecked(true) : area->setChecked(true);
    project->bcalc ? c->setCheckState(Qt::Checked) : c->setCheckState(Qt::Unchecked);
    project->ecalc ? e->setCheckState(Qt::Checked) : e->setCheckState(Qt::Unchecked);
    connectState(true);
}
