/**
 * @file cli.cpp
 * @brief Основной файл для запуска игр в консоли.
 */
#include "cli.h"

int main() {
  srand(time(NULL));
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, true);
  timeout(100);
  int c = 0;
  while (c != 'e' && c != 'E') {
    clear();
    mvprintw(4, 3, "Tetris - Enter ");
    addch('T' | A_BOLD);
    mvprintw(6, 3, "Snake  - Enter ");
    addch('S' | A_BOLD);
    mvprintw(8, 3, "Exit   - Enter ");
    addch('E' | A_BOLD);
    c = getch();
    if (c == 't' || c == 'T') {
      clear();
      game_tetris();
    } else if (c == 's' || c == 'S') {
      clear();
      s21::SnakeModel m{};
      s21::SnakeController c{&m};
      s21::SnakeView v{&c};
      v.game_snake();
    }
  }
  endwin();
  return 0;
}
