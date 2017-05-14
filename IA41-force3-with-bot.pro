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


SOURCES += src/main.cpp \
    src/force3.cpp \
    src/gameboard.cpp \
	src/mainmenu.cpp \
	src/messagehandler.cpp \
    src/gamesquare.cpp \
	src/settingsmenu.cpp \
    src/gamestate.cpp \
    src/boardstate.cpp \
    src/alphabeta.cpp \
    src/heuristic.cpp \
    src/movewrapper.cpp

HEADERS  += \
    src/force3.hpp \
    src/gameboard.hpp \
	src/mainmenu.hpp \
	src/messagehandler.hpp \
    src/gamesquare.hpp \
    src/settingsmenu.hpp \
	src/boardsquare.hpp \
    src/move.hpp \
    src/gamestate.hpp \
    src/boardstate.hpp \
    src/alphabeta.hpp \
    src/tree.hpp \
    src/movewrapper.hpp \
    src/heuristic.hpp

FORMS    += \
    ui/force3.ui \
    ui/gameboard.ui \
    ui/mainmenu.ui \
    ui/settingsmenu.ui
