/**
 * @file controller.cpp
 * @brief Реализация класса SnakeController.
 */
#include "controller.h"

namespace s21 {
SnakeController::SnakeController(SnakeModel *model) : model{model} {}
SnakeController::~SnakeController() {}
void SnakeController::constantMovement() {
  switch (model->direction) {
    case 3:
      model->snakeHead.x--;
      break;
    case 4:
      model->snakeHead.x++;
      break;
    case 5:
      model->snakeHead.y--;
      break;
    case 6:
      model->snakeHead.y++;
      break;
  }
}
void SnakeController::userInput(UserAction_t action, bool hold) {
  if (hold) {
    switch (action) {
      case Left:
        if (model->direction != 4) {
          model->direction = 3;
        }
        break;
      case Right:
        if (model->direction != 3) {
          model->direction = 4;
        }
        break;
      case Up:
        if (model->direction != 6) {
          model->direction = 5;
        }
        break;
      case Down:
        if (model->direction != 5) {
          model->direction = 6;
        }
        break;
      case Terminate:
        model->writingFile();
        model->game.pause = 3;
        break;
      case Action:
        if (model->game.speed > 150000) {
          model->game.speed -= 50000;
        }
        break;
      case Pause:
        model->game.pause = !model->game.pause;
        break;
      case Start:
        model->game.pause = 1;
        break;
    }
  }
}

}  // namespace s21