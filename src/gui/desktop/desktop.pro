TEMPLATE = app 
TARGET = desktop 
INCLUDEPATH +=.
QT += widgets 
LIBS += -lncurses

HEADERS += snakeQt.h tetrisQt.h ../../brick_game/tetris/tetris.h ../../brick_game/snake/model/model.h ../../brick_game/snake/controller/controller.h ../../brick_game/snake/snake.h 
SOURCES += main.cpp snakeQt.cpp tetrisQt.cpp ../../brick_game/tetris/tetris.c ../../brick_game/snake/model/model.cpp ../../brick_game/snake/controller/controller.cpp
RESOURCES += img.qrc
