QT       += core gui sql network networkauth

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TARGET = StreetMetrix

MOC_DIR = common\build\moc
RCC_DIR = common\build\rcc
OBJECTS_DIR = coomon\build\obj

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    auth/stateauth.cpp \
    auth/vauth.cpp \
    common/cobject.cpp \
    common/datareader.cpp \
    common/filedir.cpp \
    common/ht.cpp \
    common/project.cpp \
    common/querys.cpp \
    common/rac.cpp \
    common/statistic.cpp \
    custom/customtabstyle.cpp \
    custom/dep.cpp \
    custom/dialog.cpp \
    custom/htpaint.cpp \
    custom/sysenv.cpp \
    custom/varpaint.cpp \
    custom/voron/view_voron.cpp \
    custom/voron/voron_item.cpp \
    db/dbconnection.cpp \
    db/dbdcon.cpp \
    db/dbvalue.cpp \
    main.cpp \
    mainview.cpp \
    view/addcross.cpp \
    view/vcross.cpp \
    view/vcrosses.cpp \
    view/vdata.cpp \
    view/vdateselect.cpp \
    view/vdbconnection.cpp \
    view/vgeneratedata.cpp \
    view/vgraph.cpp \
    view/vht.cpp \
    view/vpicht.cpp \
    view/vproject.cpp

HEADERS += \
    auth/stateauth.h \
    auth/vauth.h \
    common/cobject.h \
    common/datareader.h \
    common/filedir.h \
    common/ht.h \
    common/project.h \
    common/querys.h \
    common/rac.h \
    common/statistic.h \
    custom/customtabstyle.h \
    custom/dep.h \
    custom/dialog.h \
    custom/htpaint.h \
    custom/sysenv.h \
    custom/varpaint.h \
    custom/voron/view_voron.h \
    custom/voron/voron_item.h \
    db/dbconnection.h \
    db/dbdcon.h \
    db/dbvalue.h \
    mainview.h \
    view/addcross.h \
    view/vcross.h \
    view/vcrosses.h \
    view/vdata.h \
    view/vdateselect.h \
    view/vdbconnection.h \
    view/vgeneratedata.h \
    view/vgraph.h \
    view/vht.h \
    view/vpicht.h \
    view/vproject.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
