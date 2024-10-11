#include "vauth.h"
#include "custom/dep.h"

vauth::vauth(){
    setMinimumWidth(300);
    setWindowTitle("Авторизация");

    u = new QLineEdit;
    p = new QLineEdit;
    p->setEchoMode(QLineEdit::Password);

    QAction *action = p->addAction(style()->standardIcon(QStyle::SP_ArrowLeft), QLineEdit::TrailingPosition);
    button = qobject_cast<QToolButton *>(action->associatedWidgets().last());
    button->setCursor(QCursor(Qt::PointingHandCursor));
    connect(button, &QToolButton::pressed, p, [this](){
        p->setEchoMode(QLineEdit::Normal);
        button->setIcon(style()->standardIcon(QStyle::SP_ArrowUp));
    });
    connect(button, &QToolButton::released, p, [this](){
        p->setEchoMode(QLineEdit::Password);
        button->setIcon(style()->standardIcon(QStyle::SP_ArrowLeft));
    });

    QFormLayout *fl = new QFormLayout(this);
    fl->addRow("Логин:",u);
    fl->addRow("Пароль:",p);

    //********************
    p->setText("162747");
    //********************

    pb = new QPushButton("Авторизоваться");
    pb->setShortcut(QKeySequence("Return"));
    connect(pb,&QPushButton::clicked,this,&vauth::getAuth);
    fl->addRow(pb);

    connect(&n,&QNetworkAccessManager::finished,this,&vauth::onFinished);
    connect(&n, &QNetworkAccessManager::sslErrors, this, &vauth::sslErrors);
}

vauth::~vauth(){
    delete wplase;
}

void vauth::getAuth(){
    lock_buttons(false);

    QNetworkRequest request(auth);
    request.setHeader(QNetworkRequest::ContentTypeHeader,QStringLiteral("application/json"));

    QSslConfiguration cert = QSslConfiguration::defaultConfiguration();
    cert.setSslOption(QSsl::SslOptionDisableSessionTickets,true);
    request.setSslConfiguration(cert);

    QJsonObject a;
    QJsonObject c;
    c["username"] = u->text();
    c["password"] = p->text();
    a["auth"] = c;

    aut->setUser(u->text());

    QJsonDocument doc(a);
    QByteArray data = doc.toJson(QJsonDocument::Indented);

    n.post(request,data);
}

void vauth::runApp(){
    this->setVisible(false);
    wplase = new MainView();
    p->clear();
    se->user_login = u->text();
    connect(wplase, &MainView::logout, this, &vauth::swapUser);
    wplase->show();
}

void vauth::lock_buttons(bool lock){
    u->setEnabled(lock);
    p->setEnabled(lock);
    pb->setEnabled(lock);
}

void vauth::swapUser(){
    wplase->close();
    this->setVisible(true);
}


void vauth::onFinished(QNetworkReply *reply){
    QMessageBox msg;
    msg.setWindowModality(Qt::ApplicationModal);

    QByteArray f = reply->readAll();
    reply->deleteLater();

    if(reply->error() == QNetworkReply::AuthenticationRequiredError){
        msg.setIcon(QMessageBox::Warning);
        msg.setWindowTitle("Внимание");
        msg.setText(QString::fromUtf8(f.data(),f.size()));
        msg.exec();
        lock_buttons(true);
        return;
    }

    if(reply->error() == QNetworkReply::NoError){
        QJsonDocument doc = QJsonDocument::fromJson(f);
        if(!doc.isEmpty()){
            QJsonObject r = doc.object();
            aut->setStatesAuth(r["status"].toBool());
            QString res_reg = r["region"].toString();
            if(res_reg == "*")
                aut->setRegion(-1);
            else
                aut->setRegion(res_reg.toInt());
            QVector<QString> res_are = dep::fromJsonValString(r["areas"].toArray());
            if(res_are.contains("*"))
                aut->setAreas({-1});
            else
                aut->setAreas(dep::fromVecString(res_are));
            aut->setPrevileges(dep::fromJsonValString(r["privileges"].toArray()));

            runApp();
        }else{
            msg.setIcon(QMessageBox::Warning);
            msg.setWindowTitle("Ошибка входа");
            msg.exec();
        }
    }else{
        msg.setIcon(QMessageBox::Critical);
        msg.setWindowTitle("Ошибка");
        msg.setText(reply->errorString());
        msg.exec();
    }
    lock_buttons(true);
}

void vauth::sslErrors(QNetworkReply *reply, const QList<QSslError> &errors){
    reply->ignoreSslErrors(errors);
}
