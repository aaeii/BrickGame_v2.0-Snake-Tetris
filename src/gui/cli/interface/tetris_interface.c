/**
 * @file interface.c
 * @brief Интерфейс игры
 */
#include "tetris_interface.h"
void game_tetris() {
  int c = 0;
  curs_set(0);
  start_color();
  scrollok(stdscr, TRUE);
  gettimeofday(&before, NULL);
  GameInfo_t G = {0};
  while (G.pause != 3) {
    Figure Current = {0}, Next = {0};
    G = updateCurrentState();
    mvprintw(HEIGHT / 3, 4, "Press ENTER to start the game");
    refresh();
    while (G.pause == 2) {
      c = getch();
      if (c == ENTER_KEY) {
        G.pause = 1;
        clear();
      } else if (c == ESCAPE) {
        G.pause = 3;
        break;
      }
    }
    generating_next_figure(&Next);
    generating_current_figure(&Current, &Next, &G);
    PrintTable(&Current, &Next, &G);
    while (G.pause != 2 && G.pause != 3 && c != ESCAPE) {
      c = getch();
      game_actions(c, &Current, &Next, &G);
      PrintTable(&Current, &Next, &G);
      refresh();
      game_over(&Current, &G);
    }
    delete_figure(&Current);
    delete_figure(&Next);
  }
  delete_field(&G);
}
void victory(GameInfo_t *g) {
  if (g->level == FINAL_LEVEL + 1) {
    mvprintw(HEIGHT / 2, 15, "Victory! You have completed the game!");
    refresh();
    write_score_to_file(g);
  }
}

void print_information(GameInfo_t *g) {
  int i = 1;
  mvprintw(i++, WIDTH + 3, "Hight Score: %d", g->high_score);
  mvprintw(i++, WIDTH + 3, "Score: %d", g->score);
  if (g->level <= FINAL_LEVEL) {
    mvprintw(i++, WIDTH + 3, "Level: %d", g->level);
  } else if (g->level == FINAL_LEVEL + 1) {
    mvprintw(i++, WIDTH + 3, "Level: %d", FINAL_LEVEL);
  }
  mvprintw(i++, WIDTH + 3, "Speed: %d", g->speed);
  mvprintw(i++, WIDTH + 3, "Next:");
  mvprintw(HEIGHT - 2, WIDTH + 3, "Pause - SPACE");
  mvprintw(HEIGHT - 1, WIDTH + 3, "Exit - ESC");
  mvprintw(HEIGHT, WIDTH + 3, "Move - ENTER");
}
void print_frame() {
  mvaddch(0, 0, ACS_ULCORNER);
  mvhline(0, 1, ACS_HLINE, WIDTH);
  mvaddch(0, WIDTH + 1, ACS_URCORNER);

  mvaddch(HEIGHT + 1, 0, ACS_LLCORNER);
  mvhline(HEIGHT + 1, 1, ACS_HLINE, WIDTH);
  mvaddch(HEIGHT + 1, WIDTH + 1, ACS_LRCORNER);
  for (int i = 1; i < HEIGHT + 1; i++) {
    mvaddch(i, 0, ACS_VLINE);
    mvaddch(i, WIDTH + 1, ACS_VLINE);
  }
}
void PrintTable(Figure *current, Figure *next, GameInfo_t *g) {
  init_pair(1, COLOR_WHITE, COLOR_BLACK);
  init_pair(3, COLOR_BLACK, COLOR_BLACK);
  init_pair(6, COLOR_GREEN, COLOR_GREEN);
  int Buffer[HEIGHT][WIDTH] = {0};
  for (int i = 0; i < ARRAY_SIZE; i++) {
    for (int j = 0; j < ARRAY_SIZE; j++) {
      if (current->array[i][j])
        Buffer[current->y + i][current->x + j] = current->array[i][j];
    }
  }
  for (int i = 0; i < ARRAY_SIZE; i++) {
    for (int j = 0; j < ARRAY_SIZE; j++) {
      int color = 3;
      if (next->array[i][j] != 0) {
        color = 6;
      }
      attron(COLOR_PAIR(color));
      mvaddch(i + 6, j + WIDTH + 4, ' ');
      attroff(COLOR_PAIR(color));
    }
  }
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      int color = 6;
      if (g->field[i][j] + Buffer[i][j] != 0) {
        color = 3;
      }
      attron(COLOR_PAIR(color));
      mvaddch(i + 1, j + 1, ' ');
      attroff(COLOR_PAIR(color));
    }
  }
  print_frame();
  attron(COLOR_PAIR(1));
  print_information(g);
  refresh();
  attroff(COLOR_PAIR(1));
}
void restart(GameInfo_t *g) {
  int c = 0;
  while ((c = getch()) != ENTER_KEY && c != ESCAPE) {
    // waiting for action
  }
  if (c == ENTER_KEY) {
    clear();
    g->pause = 2;
  } else if (c == ESCAPE) {
    g->pause = 3;
  }
}
void game_over(Figure *current, GameInfo_t *g) {
  if (check_position(current, g) == 0) {
    mvprintw(HEIGHT / 2 - 1, 4, "Game over. Press ENTER to restart.");
    mvprintw(HEIGHT / 2, 4, "High score: %d", g->high_score);
    mvprintw(HEIGHT / 2 + 1, 4, "Score: %d", g->score);
    refresh();
    write_score_to_file(g);
    restart(g);
  }
}

void game_actions(int sign, Figure *curr, Figure *n, GameInfo_t *g) {
  while (g->pause != 3 && g->pause != 2) {
    if ((sign = getch())) {
      UserAction_t action = get_signal(sign);
      userInput(action, curr, n, g);
      PrintTable(curr, n, g);
    }
    if (g->pause == 1 && g->level <= FINAL_LEVEL) {
      gettimeofday(&now, NULL);
      if (((suseconds_t)(now.tv_sec * 1000000 + now.tv_usec) -
           ((suseconds_t)before.tv_sec * 1000000 + before.tv_usec)) > timer) {
        userInput(Down, curr, n, g);
        PrintTable(curr, n, g);
        gettimeofday(&before, NULL);
      }
    }
    victory(g);
  }
}

UserAction_t get_signal(int user_input) {
  UserAction_t sign = -1;
  if (user_input == KEY_UP)
    sign = Up;
  else if (user_input == KEY_DOWN)
    sign = Down;
  else if (user_input == KEY_LEFT)
    sign = Left;
  else if (user_input == KEY_RIGHT)
    sign = Right;
  else if (user_input == ESCAPE)
    sign = Terminate;
  else if (user_input == ENTER_KEY)
    sign = Action;
  else if (user_input == KEY_SPACE)
    sign = Pause;

  return sign;
}