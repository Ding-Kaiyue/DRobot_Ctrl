QT       += core gui opengl network printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

LIBS +=-lGL -lGLU

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    armgl.cpp \
    cmdsend.cpp \
    developermode.cpp \
    jointpos.cpp \
    jointposfed.cpp \
    kinematics.cpp \
    main.cpp \
    mainwindow.cpp \
    posctrl.cpp \
    rotctrl.cpp \
    rrglwidget.cpp \
    stlfileloader.cpp \
    versioninfo.cpp

HEADERS += \
    armgl.h \
    cmdsend.h \
    developermode.h \
    jointpos.h \
    jointposfed.h \
    kinematics.h \
    mainwindow.h \
    posctrl.h \
    rotctrl.h \
    rrglwidget.h \
    stlfileloader.h \
    versioninfo.h

FORMS += \
    cmdsend.ui \
    developermode.ui \
    jointpos.ui \
    jointposfed.ui \
    mainwindow.ui \
    posctrl.ui \
    rotctrl.ui \
    versioninfo.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
