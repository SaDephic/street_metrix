#ifndef DBVALUE_H
#define DBVALUE_H

#include <QVariant>
#include <QByteArray>
#include <QString>

struct dbvalue
{
    QVariant data;
    int value_int();
    QByteArray value_byte();
    QByteArray value_json();
    QString value_string();
};

#endif // DBVALUE_H
