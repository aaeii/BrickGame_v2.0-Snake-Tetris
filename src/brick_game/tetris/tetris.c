/**
 * @file tetris.c
 * @brief Основные функции игры
 */

#include "tetris.h"

#include "../../gui/cli/interface/tetris_interface.h"
int timer = 800000;
struct timeval before, now;
Figure figures_array[7] = {
    {(int *[]){(int[4]){0, 0, 0, 0}, (int[4]){0, 1, 1, 0}, (int[4]){1, 1, 0, 0},
               (int[4]){0, 0, 0, 0}},
     0, 0},
    {(int *[]){(int[4]){0, 0, 0, 0}, (int[4]){1, 1, 0, 0}, (int[4]){0, 1, 1, 0},
               (int[4]){0, 0, 0, 0}},
     0, 0},
    {(int *[]){(int[4]){0, 0, 0, 0}, (int[4]){0, 1, 0, 0}, (int[4]){1, 1, 1, 0},
               (int[4]){0, 0, 0, 0}},
     0, 0},
    {(int *[]){(int[4]){0, 0, 0, 0}, (int[4]){0, 0, 1, 0}, (int[4]){1, 1, 1, 0},
               (int[4]){0, 0, 0, 0}},
     0, 0},
    {(int *[]){(int[4]){0, 0, 0, 0}, (int[4]){0, 1, 0, 0}, (int[4]){0, 1, 1, 1},
               (int[4]){0, 0, 0, 0}},
     0, 0},
    {(int *[]){(int[4]){0, 0, 0, 0}, (int[4]){0, 1, 1, 0}, (int[4]){0, 1, 1, 0},
               (int[4]){0, 0, 0, 0}},
     0, 0},
    {(int *[]){(int[4]){0, 0, 0, 0}, (int[4]){1, 1, 1, 1}, (int[4]){0, 0, 0, 0},
               (int[4]){0, 0, 0, 0}},
     0, 0}};

Figure copy_figure(Figure *figure) {
  Figure new_figure = *figure;
  new_figure.array = (int **)malloc(ARRAY_SIZE * sizeof(int *));
  for (int i = 0; i < ARRAY_SIZE; i++) {
    new_figure.array[i] = (int *)malloc(ARRAY_SIZE * sizeof(int));
    memcpy(new_figure.array[i], figure->array[i], ARRAY_SIZE * sizeof(int));
  }
  return new_figure;
}

void delete_figure(Figure *f) {
  for (int i = 0; i < ARRAY_SIZE; i++) {
    free(f->array[i]);
  }
  free(f->array);
}

int check_position(Figure *figure, GameInfo_t *g) {
  for (int i = 0; i < ARRAY_SIZE; i++) {
    for (int j = 0; j < ARRAY_SIZE; j++) {
      if ((figure->x + j < 0 || figure->x + j >= WIDTH ||
           figure->y + i >= HEIGHT)) {
        if (figure->array[i][j]) return 0;
      } else if (g->field[figure->y + i][figure->x + j] && figure->array[i][j])
        return 0;
    }
  }
  return 1;
}

void stopping_figure(Figure *current, GameInfo_t *g) {
  for (int i = 0; i < ARRAY_SIZE; i++) {
    for (int j = 0; j < ARRAY_SIZE; j++) {
      if (current->array[i][j]) {
        g->field[current->y + i][current->x + j] = current->array[i][j];
      }
    }
  }
}

void deleting_lines(GameInfo_t *g) {
  int sum, count = 0;
  for (int i = 0; i < HEIGHT; i++) {
    sum = 0;
    for (int j = 0; j < WIDTH; j++) {
      sum += g->field[i][j];
    }
    if (sum == WIDTH) {
      count++;
      for (int k = i; k >= 1; k--)
        for (int l = 0; l < WIDTH; l++) {
          g->field[k][l] = g->field[k - 1][l];
        }
      for (int j = 0; j < WIDTH; j++) {
        g->field[0][j] = 0;
      }
    }
  }

  change_level_speed_score(g, count);
}

void rotate_figure(Figure *figure) {
  Figure temp = copy_figure(figure);
  for (int i = 0; i < ARRAY_SIZE; i++) {
    for (int j = 0, k = ARRAY_SIZE - 1; j < ARRAY_SIZE; j++, k--) {
      figure->array[i][j] = temp.array[k][i];
    }
  }
  delete_figure(&temp);
}

void generating_next_figure(Figure *next) {
  int n1 = rand() % 7;
  if (next->array) {
    delete_figure(next);
  }
  *next = copy_figure(&figures_array[n1]);
}

void generating_current_figure(Figure *current, Figure *next, GameInfo_t *g) {
  if (current->array) {
    delete_figure(current);
  }
  *current = copy_figure(next);
  current->x = rand() % (WIDTH - ARRAY_SIZE) + 1;
  current->y = 0;
  generating_next_figure(next);
  if (!check_position(current, g)) {
    g->pause = 2;
  }
}

void userInput(UserAction_t action, Figure *current, Figure *next,
               GameInfo_t *g) {
  Figure temp = copy_figure(current);
  switch (action) {
    case Left:
      temp.x--;
      if (check_position(&temp, g)) current->x--;
      break;
    case Right:
      temp.x++;
      if (check_position(&temp, g)) current->x++;
      break;
    case Down:
      temp.y++;
      if (check_position(&temp, g))
        current->y++;
      else {  // figure has reached the end
        stopping_figure(current, g);
        deleting_lines(g);
        generating_current_figure(current, next, g);
      }
      break;
    case Up:
      break;
    case Action:
      if (g->level <= FINAL_LEVEL) {
        rotate_figure(&temp);
        if (check_position(&temp, g)) rotate_figure(current);
      }
      break;
    case Pause:
      g->pause = !g->pause;
      break;
    case Terminate:
      write_score_to_file(g);
      g->pause = 3;
      break;
    case Start:
      g->pause = 1;
      break;
  }
  delete_figure(&temp);
}

GameInfo_t updateCurrentState() {
  GameInfo_t state = {0};
  set_start(&state);
  return state;
}
void delete_field(GameInfo_t *game) {
  for (int i = 0; i < HEIGHT; i++) {
    free(game->field[i]);
  }
  free(game->field);
}
void set_start(GameInfo_t *game) {
  timer = 800000;
  game->pause = 2;
  game->high_score = find_high_score();
  game->score = 0;
  game->speed = timer;
  game->level = 1;
  if (game->field != NULL) {
    delete_field(game);
  }
  game->field = (int **)malloc(HEIGHT * sizeof(int *));
  for (int i = 0; i < HEIGHT; i++) {
    game->field[i] = (int *)calloc(WIDTH, sizeof(int));
  }
}

void change_level_speed_score(GameInfo_t *g, int count) {
  int decrease = 50000;
  switch (count) {
    case 1:
      g->score += 100;
      break;
    case 2:
      g->score += 300;
      break;
    case 3:
      g->score += 700;
      break;
    case 4:
      g->score += 1500;
      break;
    default:
      break;
  }
  if (g->level <= FINAL_LEVEL) {
    int level = g->score / 600 + 1;
    if (g->high_score < g->score) {
      g->high_score = g->score;
    }
    if (level != g->level) {
      g->level = level;
      if (g->speed > 150000) {
        g->speed -= decrease;
      }
    }
  }
  timer = g->speed;
}
int find_high_score() {
  FILE *f = fopen("tetris_score.txt", "r");
  if (!f) {
    return 0;
  }
  int max_score = 0, score = 0;
  while (fscanf(f, "%d ", &score) == 1) {
    if (score > max_score) {
      max_score = score;
    }
  }
  fclose(f);
  return max_score;
}
void write_score_to_file(GameInfo_t *g) {
  char h_score[16];
  sprintf(h_score, "%d ", g->score);
  FILE *in = fopen("tetris_score.txt", "a");
  if (in) {
    fputs(h_score, in);
    fclose(in);
  }
}