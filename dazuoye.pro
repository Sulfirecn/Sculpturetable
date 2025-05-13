QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    bullet.cpp \
    coverwindow.cpp \
    inspirationsystem.cpp \
    levelselectwindow.cpp \
    main.cpp \
    gamewindow.cpp \
    player.cpp \
    recordwallwindow.cpp \
    upgradeselectwindow.cpp \
    zombie.cpp

HEADERS += \
    bullet.h \
    constants.h \
    coverwindow.h \
    gamewindow.h \
    inspirationsystem.h \
    levelselectwindow.h \
    player.h \
    recordwallwindow.h \
    upgradeselectwindow.h \
    zombie.h

FORMS += \
    gamewindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc \
    res1.qrc
