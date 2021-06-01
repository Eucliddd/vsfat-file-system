QT       += core gui
DEFINES += QT_DEPRECATED_WARNINGS
QT += charts
TARGET = PieChart
TEMPLATE = app

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
#CONFIG+= console
# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Access.cpp \
    FILE_SYSTEM.cpp \
    Files.cpp \
    Folder.cpp \
    Super.cpp \
    acc.cpp \
    changepass.cpp \
    chdir.cpp \
    logindialog.cpp \
    main.cpp \
    mainwindow.cpp \
    mybutton.cpp \
    newfolder.cpp \
    other.cpp \
    piechart.cpp \
    potential_figure.cpp \
    register.cpp \
    text.cpp

HEADERS += \
    acc.h \
    all.h \
    changepass.h \
    chdir.h \
    logindialog.h \
    mainwindow.h \
    mybutton.h \
    newfolder.h \
    piechart.h \
    potential_figure.h \
    register.h \
    text.h

FORMS += \
    acc.ui \
    changepass.ui \
    chdir.ui \
    logindialog.ui \
    mainwindow.ui \
    newfolder.ui \
    piechart.ui \
    potential_figure.ui \
    register.ui \
    text.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    disk.txt \
    index.txt
