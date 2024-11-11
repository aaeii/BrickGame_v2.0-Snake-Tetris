/**
 * @file view.cpp
 * @brief Реализация класса SnakeView.
 */
#include "view.h"
namespace s21 {
SnakeView::SnakeView(SnakeController *c) : controller{c} {};
SnakeView::~SnakeView(){};
void SnakeView::game_snake() {
  int c;
  curs_set(0);
  controller->model->game.pause = 2;
  while (controller->model->game.pause != 3) {
    mvprintw(10, 4, "Press ENTER to start the game");
    refresh();
    while (controller->model->game.pause == 2) {
      c = getch();
      if (c == ENTER_KEY) {
        controller->model->initializeGame();
        controller->model->game.pause = 1;
        clear();
      } else if (c == ESCAPE) {
        controller->model->game.pause = 3;
        break;
      }
    }
    while (controller->model->game.pause != 3) {
      int key = getch();
      if (key != ERR) {
        UserAction_t action = get_signal(key);
        controller->userInput(action, key != ERR);
        if (controller->model->game.pause == 0) {
          mvprintw(10, WIDTH + 4, "Pause");
          refresh();
        }
      }
      if (controller->model->game.pause == 1) {
        draw();
        controller->constantMovement();
        controller->model->collisionWithFruit();
        controller->model->moveTail();

        if (controller->model->checkCollision() == 1) {
          gameOver();
          break;
        }
        usleep(controller->model->game.speed);
      }
    }
  }
}
void SnakeView::restart() {
  int c = 0;
  while ((c = getch()) != ENTER_KEY && c != ESCAPE) {
    // waiting for action
  }
  if (c == ENTER_KEY) {
    clear();
    controller->model->initializeGame();
    controller->model->game.pause = 2;
  } else if (c == ESCAPE) {
    clear();
    controller->model->game.pause = 3;
  }
}
void SnakeView::victory() {
  if (controller->model->game.level == FINAL_LEVEL + 1 ||
      controller->model->nTail == FINAL_LENGTH) {
    controller->model->game.pause = 2;
    mvprintw(7, 4, "Victory! You have completed the game!");
    mvprintw(8, 4, "High score: %d", controller->model->game.high_score);
    mvprintw(9, 4, "Score: %d", controller->model->game.score);
    refresh();
    controller->model->writingFile();
  }
}

void SnakeView::draw() {
  clear();
  for (int i = 0; i < HEIGHT + 2; i++) {
    for (int j = 0; j < WIDTH + 2; j++) {
      if (i == controller->model->snakeHead.y &&
          j == controller->model->snakeHead.x) {
        printw("O");
      } else {
        bool isTail = false;
        for (int k = 0; k < controller->model->nTail; k++) {
          if (controller->model->tailX[k] == j &&
              controller->model->tailY[k] == i) {
            isTail = true;
            printw("o");
          }
        }
        if (!isTail) {
          if (controller->model->game.field[i][j] <= -1) {
            printw("@");
          } else if (controller->model->game.field[i][j] == 1) {
            printw("#");
          } else if (controller->model->game.field[i][j] == 0) {
            printw(" ");
          }
        }
      }
    }
    printw("\n");
  }
  if (controller->model->updateLvl() == 1) {
    victory();
  }
  information();
}
void SnakeView::information() {
  mvprintw(1, WIDTH + 3, "High score: %d", controller->model->game.high_score);
  mvprintw(2, WIDTH + 3, "Score: %d", controller->model->game.score);
  mvprintw(3, WIDTH + 3, "Speed: %d", controller->model->game.speed);
  if (controller->model->game.level <= FINAL_LEVEL) {
    mvprintw(4, WIDTH + 3, "Level: %d", controller->model->game.level);
  } else if (controller->model->game.level == FINAL_LEVEL + 1) {
    mvprintw(4, WIDTH + 3, "Level: %d", FINAL_LEVEL);
  }
  mvprintw(HEIGHT - 2, WIDTH + 3, "Pause - SPACE");
  mvprintw(HEIGHT - 1, WIDTH + 3, "Exit - ESC");
  mvprintw(HEIGHT, WIDTH + 3, "Action - ENTER");
  refresh();
}
void SnakeView::gameOver() {
  controller->model->writingFile();
  mvprintw(10, 4, "Game over. Press ENTER to restart.");
  mvprintw(11, 4, "High score: %d", controller->model->game.high_score);
  mvprintw(12, 4, "Score: %d", controller->model->game.score);
  refresh();
  restart();
}
UserAction_t SnakeView::get_signal(int user_input) {
  UserAction_t sign;
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
}  // namespace s21