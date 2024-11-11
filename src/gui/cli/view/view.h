/**
 * @file view.h
 * @brief Заголовочный файл интерфейса игры змейка.
 */
#ifndef VIEW_H_
#define VIEW_H_
#include "../../../brick_game/snake/controller/controller.h"

namespace s21 {
/**
 * @class SnakeView
 * @brief Класс интерфейса игры Змейка.
 */
class SnakeView {
 private:
  SnakeController *controller;
  /**
   * @brief Печать сведений о победе.
   */
  void victory();
  /**
   * @brief Отрисовка поля.
   */
  void draw();
  /**
   * @brief Печать сведений о проигрыше.
   */
  void gameOver();
  /**
   * @brief Печать дополнительных сведений об игре.
   */
  void information();
  /**
   * @brief Рестарт игры.
   */
  void restart();
  /**
   * @brief Обработка полученного сигнала.
   */
  UserAction_t get_signal(int user_input);

 public:
  /**
   * @brief Конструктор.
   * @param Указатель на объект SnakeController.
   */
  SnakeView(SnakeController *controller);
  /**
   * @brief Деструктор.
   */
  ~SnakeView();
  /**
   * @brief Основная функция.
   */
  void game_snake();
};
}  // namespace s21

#endif