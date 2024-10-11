#ifndef VAUTH_H
#define VAUTH_H

#include "mainview.h"

#include <QStyle>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QFormLayout>
#include <QToolButton>

#include <QJsonDocument>
#include <QJsonObject>

#include <QtNetworkAuth>
#include <QDesktopServices>
#include <QOAuth2AuthorizationCodeFlow>

#include <QMessageBox>

#include "auth/stateauth.h"
extern stateauth *aut;

#include "custom/sysenv.h"
extern sysenv *se;

class vauth : public QWidget{
    Q_OBJECT

public:
    QLineEdit *u = nullptr;
private:
    QLineEdit *p = nullptr;
    QPushButton *pb = nullptr;

    QString auth = se->host_auth;
    QNetworkAccessManager n;

    MainView *wplase = nullptr;

private:
    QToolButton *button;

public: signals:
    void toApp();

public:
    vauth();
    ~vauth();
    void runApp();

private:
    void lock_buttons(bool lock);

public slots:
    void swapUser();

private slots:
    void getAuth();

private slots:
    void onFinished(QNetworkReply *reply);
    void sslErrors(QNetworkReply *reply, const QList<QSslError> & errors);
};

#endif // VAUTH_H
