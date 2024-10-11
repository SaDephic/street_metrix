#include "dialog.h"

Dialog::Dialog(QString title, QString message, QString kk, QString cc, QList<QString> buttons){
    setWindowModality(Qt::ApplicationModal);

    setWindowTitle(title);
    setWindowFlag(Qt::WindowStaysOnTopHint);

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSizeConstraint(QLayout::SetFixedSize);
    vbox->addWidget(new QLabel(message));

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addStretch();

    if(!buttons.isEmpty()){
        QPushButton *acceptData = new QPushButton(kk);
        connect(acceptData,SIGNAL(clicked()),this,SLOT(accept()));
        hbox->addWidget(acceptData);
    }

    if(!buttons.isEmpty()){
        QPushButton *rejectData = new QPushButton(cc);
        connect(rejectData,SIGNAL(clicked()),this,SLOT(reject()));
        hbox->addWidget(rejectData);
    }

    if(!buttons.isEmpty()){
        foreach(QString cur, buttons){
            QPushButton *enother = new QPushButton(cur);
            hbox->addWidget(enother);
            listActions.append(enother);
        }
    }
    vbox->addLayout(hbox);
    setLayout(vbox);
}

Dialog::Dialog(QString title, QString message, QList<QString> buttons, bool link){
    setWindowModality(Qt::ApplicationModal);

    setWindowTitle(title);
    setWindowFlag(Qt::WindowStaysOnTopHint);

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSizeConstraint(QLayout::SetFixedSize);
    vbox->addWidget(new QLabel(message));

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addStretch();

    if(!buttons.isEmpty() && link){
        QString firstAction = buttons.takeFirst();
        QPushButton *acceptData = new QPushButton(firstAction);
        connect(acceptData,SIGNAL(clicked()),this,SLOT(accept()));
        hbox->addWidget(acceptData);
    }

    if(!buttons.isEmpty() && link){
        QString secondAction = buttons.takeFirst();
        QPushButton *rejectData = new QPushButton(secondAction);
        connect(rejectData,SIGNAL(clicked()),this,SLOT(reject()));
        hbox->addWidget(rejectData);
    }

    if(!buttons.isEmpty()){
        foreach(QString cur, buttons){
            QPushButton *enother = new QPushButton(cur);
            hbox->addWidget(enother);
            listActions.append(enother);
        }
    }
    vbox->addLayout(hbox);
    setLayout(vbox);
}

Dialog::Dialog(QString title, QString message, QString redsegment, QList<QString> buttons){
    setWindowModality(Qt::ApplicationModal);

    setWindowTitle(title);
    setWindowFlag(Qt::WindowStaysOnTopHint);

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSizeConstraint(QLayout::SetFixedSize);

    QLabel *redlabel = new QLabel(redsegment);
    redlabel->setStyleSheet("color:red");
    vbox->addWidget(redlabel);
    vbox->addWidget(new QLabel(message));

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addStretch();

    if(!buttons.isEmpty()){
        QString firstAction = buttons.takeFirst();
        QPushButton *acceptData = new QPushButton(firstAction);
        connect(acceptData,SIGNAL(clicked()),this,SLOT(accept()));
        hbox->addWidget(acceptData);
    }

    if(!buttons.isEmpty()){
        QString secondAction = buttons.takeFirst();
        QPushButton *rejectData = new QPushButton(secondAction);
        connect(rejectData,SIGNAL(clicked()),this,SLOT(reject()));
        hbox->addWidget(rejectData);
    }

    if(!buttons.isEmpty()){
        foreach(QString cur, buttons){
            QPushButton *enother = new QPushButton(cur);
            hbox->addWidget(enother);
            listActions.append(enother);
        }
    }
    vbox->addLayout(hbox);
    setLayout(vbox);
}

Dialog::Dialog(QString title, QString message, QList<QString> data, QList<QString> buttons){
    setWindowModality(Qt::ApplicationModal);

    setWindowTitle(title);
    setWindowFlag(Qt::WindowStaysOnTopHint);

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSizeConstraint(QLayout::SetFixedSize);
    vbox->addWidget(new QLabel(message));

    QListWidget *l = new QListWidget;
    l->addItems(data);
    vbox->addWidget(l);

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addStretch();

    if(!buttons.isEmpty()){
        QString firstAction = buttons.takeFirst();
        QPushButton *acceptData = new QPushButton(firstAction);
        connect(acceptData,SIGNAL(clicked()),this,SLOT(accept()));
        hbox->addWidget(acceptData);
    }

    if(!buttons.isEmpty()){
        QString secondAction = buttons.takeFirst();
        QPushButton *rejectData = new QPushButton(secondAction);
        connect(rejectData,SIGNAL(clicked()),this,SLOT(reject()));
        hbox->addWidget(rejectData);
    }

    if(!buttons.isEmpty()){
        foreach(QString cur, buttons){
            QPushButton *enother = new QPushButton(cur);
            hbox->addWidget(enother);
            listActions.append(enother);
        }
    }
    vbox->addLayout(hbox);
    setLayout(vbox);
}

QPushButton *Dialog::getButton(QString name){
    if(!listActions.isEmpty()){
        foreach(QPushButton *cur, listActions){
            if(cur->text() == name)
                return cur;
        }
    }
    return new QPushButton;
}

Dialog::~Dialog(){
    while(listActions.size()>0){
        delete listActions.takeAt(0);
    }
}
