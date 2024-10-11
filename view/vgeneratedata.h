#ifndef VGENERATEDATA_H
#define VGENERATEDATA_H

#include <QWidget>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QGroupBox>
#include <QLineEdit>
#include <QLabel>
#include <QSpinBox>
#include <QDateEdit>
#include <QShortcut>

class vgeneratedata : public QWidget{
    Q_OBJECT

    QLineEdit *td = nullptr;
    QLineEdit *iq = nullptr;
    QLineEdit *iht = nullptr;
    QDateEdit *date = nullptr;
    QSpinBox *chc = nullptr;
    QSpinBox *ofv = nullptr;
    QSpinBox *tov = nullptr;

public:
    vgeneratedata(){
        setWindowTitle("Генерация данных");

        td = new QLineEdit;
        iq = new QLineEdit;
        iht = new QLineEdit;
        date = new QDateEdit;
        chc = new QSpinBox;
        ofv = new QSpinBox;
        ofv->setMinimumWidth(80);
        tov = new QSpinBox;
        tov->setMinimumWidth(80);

        QGroupBox *sp = new QGroupBox("Параметры данных");
        QFormLayout *fl = new QFormLayout(sp);
        fl->addRow("Тип данных:",td);
        fl->addRow("Интервал опроса:",iq);
        fl->addRow("Интервал ХТ:",iht);
        fl->addRow("Дата:",date);
        fl->addRow("Количество каналов:",chc);

        QGroupBox *vp = new QGroupBox("Значения");
        QHBoxLayout *hb = new QHBoxLayout(vp);
        hb->addStretch();
        hb->addWidget(new QLabel("От:"));
        hb->addWidget(ofv);
        hb->addWidget(new QLabel("До:"));
        hb->addWidget(tov);

        QVBoxLayout *vl = new QVBoxLayout(this);
        vl->addWidget(sp);
        vl->addWidget(vp);

        QPushButton *gen = new QPushButton("Ок");
        connect(gen,&QPushButton::clicked,this,&vgeneratedata::generateData);

        QPushButton *cancel = new QPushButton("Отмена");
        connect(cancel,&QPushButton::clicked,this,&QWidget::close);

        QHBoxLayout *hbl = new QHBoxLayout;
        hbl->addWidget(cancel);
        hbl->addStretch();
        hbl->addWidget(gen);
        vl->addLayout(hbl);

        QShortcut *closeselect = new QShortcut(this);
        closeselect->setKey(QKeySequence(tr("CTRL+W")));
        connect(closeselect,&QShortcut::activated,this,&QWidget::close);
    }
private slots:
    void generateData(){
        qDebug() << "void generateData(){";
    }
};

#endif // VGENERATEDATA_H
