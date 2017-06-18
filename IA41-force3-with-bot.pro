#-------------------------------------------------
#
# Project created by QtCreator 2017-04-11T11:56:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = IA41-force3-with-bot
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD/src/

HEADERS += \
    src/common/boardsquare.hpp \
    src/common/messagehandler.hpp \
    src/common/move.hpp \
    src/common/movewrapper.hpp \
    src/common/tree.hpp \
    src/logic/ai.hpp \
    src/logic/boardstate.hpp \
    src/logic/gamestate.hpp \
    src/logic/heuristic.hpp \
    src/ui/force3.hpp \
    src/ui/gameboard.hpp \
    src/ui/gamesquare.hpp \
    src/ui/mainmenu.hpp \
    src/ui/settingsmenu.hpp

SOURCES += \
    src/common/messagehandler.cpp \
    src/logic/ai.cpp \
    src/logic/boardstate.cpp \
    src/logic/gamestate.cpp \
    src/logic/heuristic.cpp \
    src/ui/force3.cpp \
    src/ui/gameboard.cpp \
    src/ui/gamesquare.cpp \
    src/ui/mainmenu.cpp \
    src/ui/settingsmenu.cpp \
    src/main.cpp

FORMS    += \
    ui/force3.ui \
    ui/gameboard.ui \
    ui/mainmenu.ui \
    ui/settingsmenu.ui
