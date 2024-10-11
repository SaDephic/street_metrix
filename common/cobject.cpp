#include "cobject.h"

QJsonObject cobject::json(){
    QJsonObject r;
    r["rasc"] = data.json();
    r["params"] = htparams.json();
    return r;
}

void cobject::setVal(QJsonObject data){
    htparams.setVal(data["params"].toObject());
    emit loaded();
}
