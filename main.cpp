#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>

#include "auth/vauth.h"

#include "auth/stateauth.h"
#include "custom/sysenv.h"
#include "db/dbdcon.h"
#include "common/rac.h"
#include "common/project.h"
sysenv *se = nullptr;
stateauth *aut = nullptr;
dbdcon *envdb = nullptr;
rac *drac = nullptr;
project *prj = nullptr;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    envdb = new dbdcon;//read in *se
    se = new sysenv;//variables system
    aut = new stateauth;//when server is connected
    drac = new rac;//data from server and manage
    prj = new project;//all project

    QTranslator qtTranslator;
    if(qtTranslator.load("qt_" + QLocale::system().name(), QLibraryInfo::path(QLibraryInfo::TranslationsPath))){
        a.installTranslator(&qtTranslator);
    }

    vauth va;
    va.u->setText(se->user_login);
    va.show();

    return a.exec();
}
