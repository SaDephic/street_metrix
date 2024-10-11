#include "vpicht.h"

#include <QPainter>

rotatedText::rotatedText(QWidget *parent) : QLabel(parent){}
rotatedText::rotatedText(const QString &text, QWidget *parent) : QLabel(text, parent){}

void rotatedText::paintEvent(QPaintEvent*){
    QPainter painter(this);
    painter.translate(0,sizeHint().height());
    painter.rotate(270);
    painter.drawText(QRect (QPoint(0,0),QLabel::sizeHint()),Qt::AlignCenter,text());
}

QSize rotatedText::minimumSizeHint() const {
    QSize s = QLabel::minimumSizeHint();
    return QSize(s.height(), s.width());
}

QSize rotatedText::sizeHint() const {
    QSize s = QLabel::sizeHint();
    return QSize(s.height(), s.width());
}
