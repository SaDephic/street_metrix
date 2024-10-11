#include "vdbconnection.h"
#include "common/filedir.h"

vdbconnection::vdbconnection(){
    setWindowTitle("Настройки");
    setWindowModality(Qt::ApplicationModal);
    setMinimumSize(480,320);

    ip = new QLineEdit;
    port = new QLineEdit;

    databasename = new QLineEdit();
    databasename->setEchoMode(QLineEdit::Password);
    QAction *action = databasename->addAction(style()->standardIcon(QStyle::SP_ArrowLeft), QLineEdit::TrailingPosition);
    dbutton = qobject_cast<QToolButton *>(action->associatedWidgets().last());
    dbutton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(dbutton, &QToolButton::pressed, databasename, [this](){
        databasename->setEchoMode(QLineEdit::Normal);
        dbutton->setIcon(style()->standardIcon(QStyle::SP_ArrowUp));
    });
    connect(dbutton, &QToolButton::released, databasename, [this](){
        databasename->setEchoMode(QLineEdit::Password);
        dbutton->setIcon(style()->standardIcon(QStyle::SP_ArrowLeft));
    });

    username = new QLineEdit;
    userpassword = new QLineEdit;
    userpassword->setEchoMode(QLineEdit::Password);
    action = userpassword->addAction(style()->standardIcon(QStyle::SP_ArrowLeft), QLineEdit::TrailingPosition);
    button = qobject_cast<QToolButton *>(action->associatedWidgets().last());
    button->setCursor(QCursor(Qt::PointingHandCursor));
    connect(button, &QToolButton::pressed, userpassword, [this](){
        userpassword->setEchoMode(QLineEdit::Normal);
        button->setIcon(style()->standardIcon(QStyle::SP_ArrowUp));
    });
    connect(button, &QToolButton::released, userpassword, [this](){
        userpassword->setEchoMode(QLineEdit::Password);
        button->setIcon(style()->standardIcon(QStyle::SP_ArrowLeft));
    });

    QVBoxLayout *vgl = new QVBoxLayout(this);
    QTabWidget *tab = new QTabWidget;
    vgl->addWidget(tab);

    QWidget *conn = new QWidget;
    QVBoxLayout *vlay = new QVBoxLayout(conn);

    QFormLayout *flay = new QFormLayout;
    flay->addRow("IP:",ip);
    flay->addRow("Port:",port);
    flay->addRow("Имя базы данных:",databasename);
    flay->addRow("Имя пользователя:",username);
    flay->addRow("Пароль:",userpassword);
    vlay->addLayout(flay);

    conncheck = new QLabel();
    vlay->addWidget(conncheck);

    apply = new QPushButton("Проверить");
    connect(apply,&QPushButton::clicked,this,&vdbconnection::apply_settings);

    QHBoxLayout *hl = new QHBoxLayout;
    hl->addStretch();
    hl->addWidget(apply);
    vlay->addLayout(hl);

    tab->addTab(conn,"База данных");

    QShortcut *closeselect = new QShortcut(this);
    closeselect->setKey(QKeySequence(tr("CTRL+W")));
    connect(closeselect,&QShortcut::activated,this,&QWidget::close);

    QFile f(file().soft + QDir::separator() + file().c);
    if(f.open(QFile::ReadOnly)){
        envdb->setjson(QJsonDocument::fromJson(f.readAll()).object());
        f.close();
    }
    toView();
}

void vdbconnection::toView(){
    ip->setText(envdb->ip);
    port->setText(QString::number(envdb->port));
    databasename->setText(envdb->databasename);
    username->setText(envdb->username);
    userpassword->setText(envdb->userpassword);
}

void vdbconnection::show(){
    toView();
    QWidget::show();
}

void vdbconnection::setEditsEnabled(bool state){
    ip->setEnabled(state);
    port->setEnabled(state);
    databasename->setEnabled(state);
    username->setEnabled(state);
    userpassword->setEnabled(state);
    apply->setEnabled(state);
}

void vdbconnection::setApplyEnabled(){
    setEditsEnabled(true);
}

void vdbconnection::apply_settings(){
    setCursor(Qt::BusyCursor);
    setEditsEnabled(false);
    conncheck->clear();
    {
        envtest = new dbdcon("localhost",
                             ip->text(),
                             port->text().toInt(),
                             databasename->text(),
                             username->text(),
                             userpassword->text());


        thread = new QThread;
        dbconnection *db = new dbconnection();//object
        db->setEnv(envtest);
        connect(db,&dbconnection::statusBase,this,&vdbconnection::setStatus);//link set to view status
        db->moveToThread(thread);//object in thread
        //delete instanses after executed
        connect(thread,&QThread::started, db,&dbconnection::check);
        connect(db,&dbconnection::finished, thread,&QThread::quit);
        connect(thread,&QThread::finished,thread,&QThread::deleteLater);
        connect(db,&dbconnection::finished,db,&dbconnection::deleteLater);
        thread->start();
    }
}

void vdbconnection::setStatus(status v){
    switch (v.v) {
    case 0:
        conncheck->setStyleSheet("QLabel { color : red; }");
        conncheck->setText("Не удалось установить соединение!\n" + v.err);
        break;
    default:
        conncheck->setStyleSheet("QLabel { color : green; }");
        conncheck->setText("Соединение установлено!");
        envdb = envtest;//save current connection
        break;
    }

    thread = nullptr;

    setCursor(Qt::ArrowCursor);
    setApplyEnabled();
}

void vdbconnection::closeEvent(QCloseEvent *e){
    if(dynamic_cast<QThread *>(thread)){
        if(!thread->isFinished()){
            conncheck->setStyleSheet("QLabel { color : red; }");
            conncheck->setText("Прервано! Выполняется...");
        }
    }else{
        conncheck->setText(QString());
    }
    QWidget::closeEvent(e);
}
