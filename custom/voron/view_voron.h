#ifndef VIEW_VORON_H
#define VIEW_VORON_H

#include <QScrollBar>
#include <QWidget>
#include <QWheelEvent>

#include "voron_item.h"

class view_voron : public QWidget{
    Q_OBJECT
public: signals:
    void current_position_data(QString pos);

public:
    view_voron();

private slots:
    void set_current_position(QPointF cur_pos);

public:
    voron_item *vor_item = nullptr;

    bool mousePressed;
    int _panStartX;
    int _panStartY;
};

#endif // VIEW_VORON_H
