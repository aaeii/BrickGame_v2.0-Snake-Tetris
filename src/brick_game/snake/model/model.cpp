/**
 * @file model.cpp
 * @brief Реализация класса SnakeModel.
 */
#include "model.h"

namespace s21 {
SnakeModel::SnakeModel() {
  game.field = NULL;
  game.field = new int *[HEIGHT + 2];
  for (int i = 0; i < HEIGHT + 2; ++i) {
    game.field[i] = new int[WIDTH + 2];
  }
}

SnakeModel::~SnakeModel() {
  for (int i = 0; i < HEIGHT + 2; ++i) {
    delete[] game.field[i];
  }
  delete[] game.field;
}

void SnakeModel::initializeGame() {
  if (game.field != NULL) {
    for (int i = 0; i < HEIGHT + 2; ++i) {
      delete[] game.field[i];
    }
    delete[] game.field;
  }

  game.field = new int *[HEIGHT + 2];
  for (int i = 0; i < HEIGHT + 2; ++i) {
    game.field[i] = new int[WIDTH + 2];
  }
  snakeHead = {WIDTH / 2 + 1, HEIGHT / 2};
  game.next = NULL;
  game.high_score = findHighScore();
  game.score = 0;
  game.level = 1;
  game.speed = 800000;
  game.pause = 2;
  nTail = 4;
  direction = 6;
  memset(tailX, 0, sizeof(tailX));
  memset(tailY, 0, sizeof(tailY));

  for (int i = 0; i < nTail; i++) {
    tailX[i] = snakeHead.x;
    tailY[i] = snakeHead.y - i;
  }
  for (int i = 0; i < HEIGHT + 2; i++) {
    for (int j = 0; j < WIDTH + 2; j++) {
      if (i == 0 || i == HEIGHT + 1 || j == 0 || j == WIDTH + 1) {
        game.field[i][j] = 1;
      } else {
        game.field[i][j] = 0;
      }
    }
  }
  placeFruit();
}

void SnakeModel::writingFile() {
  std::ofstream out;
  out.open("snake_score.txt", std::ios::app);
  if (out.is_open()) {
    out << game.high_score << " ";
  }
  out.close();
}

int SnakeModel::findHighScore() {
  int ch = 0;
  std::ifstream f("snake_score.txt");
  if (!f.is_open()) {
    return 0;
  }
  f >> ch;
  while (!f.eof()) {
    int temp_ch = 0;
    f >> temp_ch;
    if (temp_ch > ch) {
      ch = temp_ch;
    }
  }

  return ch;
}

void SnakeModel::moveTail() {
  int prevx = tailX[0], prevy = tailY[0];
  int prev2x, prev2y;
  tailX[0] = snakeHead.x;
  tailY[0] = snakeHead.y;
  for (int i = 1; i < nTail; i++) {
    prev2x = tailX[i];
    prev2y = tailY[i];
    tailX[i] = prevx;
    tailY[i] = prevy;
    prevx = prev2x;
    prevy = prev2y;
  }
}

void SnakeModel::placeFruit() {
  int emptyPlace = 0;
  while (!emptyPlace) {
    fruit.x = rand() % WIDTH + 1;
    fruit.y = rand() % HEIGHT + 1;
    int tailPlace = 0;
    for (int i = 0; i < nTail; i++) {
      if (tailX[i] == fruit.x && tailY[i] == fruit.y) {
        tailPlace = 1;
        break;
      }
    }
    if (!tailPlace) {
      game.field[fruit.y][fruit.x] = -(rand() % 5 + 1);
      emptyPlace = 1;
    }
  }
}

void SnakeModel::collisionWithFruit() {
  if (game.field[snakeHead.y][snakeHead.x] <= -1) {
    game.score++;
    nTail++;
    game.field[fruit.y][fruit.x] = 0;
    if (updateLvl() == 0) {
      placeFruit();
    }
  }
}

int SnakeModel::checkCollision() {
  int flag = 0;
  if (game.field[snakeHead.y][snakeHead.x] == 1) {
    flag = 1;
  }
  for (int i = 1; i < nTail; i++) {
    if (snakeHead.x == tailX[i] && snakeHead.y == tailY[i]) {
      flag = 1;
      break;
    }
  }

  return flag;
}
int SnakeModel::updateLvl() {
  int res = 0;
  if (game.level <= FINAL_LEVEL && nTail != FINAL_LENGTH) {
    int level = game.score / 5 + 1;
    if (game.high_score < game.score) {
      game.high_score = game.score;
    }
    if (level != game.level) {
      game.level = level;
      if (game.speed > 150000) {
        game.speed -= 50000;
      } else if (game.speed <= 150000) {
        game.speed = 150000;
      }
    }
  } else {
    res = 1;
  }
  return res;
}

}  // namespace s21
