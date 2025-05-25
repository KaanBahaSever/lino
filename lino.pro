QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

RESOURCES += lino.qrc

# If you use an .ico file in lino.rc, make sure it exists in the project directory.
# For example, if you reference "letter-l.ico" in lino.rc, the file must be present.
# If you do not have the icon, either:
# 1. Place letter-l.ico in c:\Users\BAHA\Desktop\lino\
# 2. Or comment/remove the icon line from lino.rc to avoid the build error.

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
