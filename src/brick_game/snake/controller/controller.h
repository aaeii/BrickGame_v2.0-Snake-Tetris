/**
 * @file controller.h
 * @brief Определение класса контроллера.
 *
 * Контроллер осуществляет модификацию модели.
 */
#ifndef CONTROLLER_H_
#define CONTROLLER_H_
#include "../model/model.h"

namespace s21 {
/**
 * @class SnakeController
 * @brief Класс контроллера игры Змейка.
 */
class SnakeController {
 public:
  SnakeModel *model;
  /**
   * @brief Конструктор.
   * @param Указатель на объект SnakeModel.
   */
  SnakeController(SnakeModel *model);
  /**
   * @brief Деструктор.
   */
  ~SnakeController();
  /**
   * @brief Управление вводом.
   */
  void userInput(UserAction_t action, bool hold);
  /**
   * @brief Перемещение змейки.
   */
  void constantMovement();
};
}  // namespace s21

#endif