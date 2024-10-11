#include "dbvalue.h"

int dbvalue::value_int(){
    return data.toInt();
}
QByteArray dbvalue::value_byte(){
    return data.toByteArray();
}
QByteArray dbvalue::value_json(){
    return data.toByteArray();
}
QString dbvalue::value_string(){
    return data.toString();
}
