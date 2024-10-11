#ifndef DEP_H
#define DEP_H

#include <QString>
#include <QPointF>

#include <QJsonObject>
#include <QJsonArray>

class dep{
public:
    static QString posToString(QPointF p);
    static QPointF posFromString(QString sp);
    static QVector<int> fromJsonValInt(QJsonArray a);
    static QVector<QString> fromJsonValString(QJsonArray a);
    static QVector<int> fromVecString(QVector<QString> val);
    static QVector<int> fromChansString(QString text, QChar spl);
};

#endif // DEP_H
