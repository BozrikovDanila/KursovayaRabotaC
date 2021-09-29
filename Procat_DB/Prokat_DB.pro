QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    MainWindow.cpp \
    Classes.cpp \
    MyList.cpp \
    manual.cpp \
    reference.cpp

HEADERS += \
    MainWindow.h \
    Classes.h \
    MyList.h \
    manual.h \
    reference.h

FORMS += \
    mainwindow.ui \
    manual.ui \
    reference.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons_res.qrc
