#ifndef FILEDIR_H
#define FILEDIR_H

#include <QString>
#include <QDir>

struct file {
    //files
    QString c = "con";//connection_settings
    QString a = "app";//app_settings
    QString u = "cre";//aut data

    //local directory paths
    QString tempsys = QDir::tempPath() + QDir::separator() + "street_metrix";//basic folder
    QString apps = QDir::homePath() + QDir::separator() + ".ASUDD";//basic folder
    QString app = apps + QDir::separator() + "street_metrix";//app
    QString soft = app + QDir::separator() + "app";//app_data
    QString cash = app + QDir::separator() + "cash";//cash_data

    file();
};

#endif // FILEDIR_H
