#ifndef STATEAUTH_H
#define STATEAUTH_H

#include <QObject>

class stateauth : public QObject{
    Q_OBJECT

public:
    QString user;
    bool state;
    QVector<QString> permissions;
    int region;
    QVector<int> areas;

public:
    stateauth(){}
    ~stateauth(){}
    void setUser(QString _user){
        user = _user;
    }
    void setStatesAuth(bool _state){
        state = _state;
    }
    void setPrevileges(QVector<QString> _permissions){
        permissions = _permissions;
    }
    void setRegion(int _region){
        region = _region;
    }
    void setAreas(QVector<int> _areas){
        areas = _areas;
    }

    QString areas_text(){
        if(areas.isEmpty())
            return QString();
        if(areas.contains(-1)){
            return "*";
        }
        QString r;
        for(int i=0; i<areas.size(); ++i){
            r += QString::number(areas[i]) + " ";
        }
        return r;
    }
    QString permissions_text(){
        if(permissions.isEmpty())
            return QString();
        QString r;
        for(int i=0; i<permissions.size()-1; ++i){
            r += permissions[i] + "\n";
        }
        return r;
    }
    QString uperm(){
        return user + "\n" +
               QString::number(region) + "\n" +
               areas_text() + "\n" +
               permissions_text();
    }
};

#endif // STATEAUTH_H
