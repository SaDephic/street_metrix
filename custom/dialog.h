#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QSizePolicy>

class Dialog : public QDialog{
    Q_OBJECT

private:
    QVector<QPushButton*> listActions;

public:
    Dialog(QString title, QString message, QString kk, QString cc, QList<QString> buttons);
    Dialog(QString title, QString message, QList<QString> buttons, bool link = true);
    Dialog(QString title, QString message, QString redsegment, QList<QString> buttons);
    Dialog(QString title, QString message, QList<QString> data, QList<QString> buttons);
    QPushButton *getButton(QString name);
    ~Dialog();
};

#endif // DIALOG_H
