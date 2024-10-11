#include "filedir.h"

file::file(){
    if(!QDir(apps).exists()){
        QDir().mkdir(apps);
    }
    if(!QDir(app).exists()){
        QDir().mkdir(app);
    }
    if(!QDir(cash).exists()){
        QDir().mkdir(cash);
    }
    if(!QDir(soft).exists()){
        QDir().mkdir(soft);
    }
    if(!QDir(tempsys).exists()){
        QDir().mkdir(tempsys);
    }
}
