CC=gcc -std=c11
CPP=g++ -std=c++17
CFLAGS=-Wall -Werror -Wextra 
LIBSFLAGS = -lcheck -lpthread -lm -lgcov -lncurses 

all: style install

install: unistall clean cli desktop

snake.a:
	$(CPP) $(CFLAGS) -c brick_game/snake/model/*.cpp brick_game/snake/controller/*.cpp
	ar rc snake.a *.o
	rm -rf *.o

tetris.a:
	$(CC) $(CFLAGS) -c brick_game/tetris/*.c
	ar rc tetris.a *.o
	rm -rf *.o

cli: tetris.a snake.a
	mkdir BrickGame_v2
	$(CC) $(CFLAGS) -c gui/cli/interface/*.c 
	$(CPP)  $(CFLAGS) -c gui/cli/*.cpp gui/cli/view/*.cpp
	cd BrickGame_v2 && $(CPP)  $(CFLAGS) -o cli ../*.o $(LIBSFLAGS) ../snake.a ../tetris.a
	rm -rf *.o *.a

desktop:
	rm -rf temp
	mkdir temp
	cd temp && qmake ../gui/desktop
	cd temp && make
	mv temp/desktop BrickGame_v2
	rm -rf temp

cli_start:
	./BrickGame_v2/cli
	
desktop_start:
	./BrickGame_v2/desktop

unistall:
	rm -rf BrickGame_v2

dvi:
	doxygen
	open doxygen/html/index.html

dist: clean 
	mkdir BrickGame-2.0
	cp -r ./Makefile ./tests ./gui ./brick_game ./Doxyfile BrickGame-2.0
	tar -czf BrickGame-2.0.tar.gz BrickGame-2.0/
	rm -rf BrickGame-2.0


tests: clean
	$(CPP) $(CFLAGS) brick_game/snake/model/*.cpp brick_game/snake/controller/*.cpp tests/*.cpp --coverage -o test -lgtest
	./test
	rm -rf snake_score.txt

gcov_report: tests
	lcov -c -d . --no-external -o test.info
	genhtml -o report test.info
	open ./report/index.html
	rm -rf test.info *.gcda *.gcno

leaks:
	valgrind --tool=memcheck --leak-check=yes --log-file=log.txt ./test

clang:
	clang-format -n brick_game/tetris/*  brick_game/snake/model/* brick_game/snake/*.h \
	brick_game/snake/controller/*  gui/cli/*.cpp gui/cli/*.h gui/cli/interface/* gui/cli/view/* \
	gui/desktop/*.cpp gui/desktop/*.h tests/*

style:
	clang-format -i brick_game/tetris/*  brick_game/snake/model/* brick_game/snake/*.h \
	brick_game/snake/controller/*  gui/cli/*.cpp gui/cli/*.h gui/cli/interface/* gui/cli/view/* \
	gui/desktop/*.cpp gui/desktop/*.h tests/*

clean:
	rm -rf *.txt *.o *.a *.info  test *.gcda *.gcno report/ doxygen/ *.gz *.tar
