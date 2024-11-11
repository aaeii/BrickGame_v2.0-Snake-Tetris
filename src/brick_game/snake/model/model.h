/**
 * @file model.h
 * @brief Определение класса модели.
 *
 * Модель хранит и осуществляет доступ к основным данным.
 */
#ifndef MODEL_H_
#define MODEL_H_
#include <cstring>

#include "../snake.h"

namespace s21 {
/**
 * @class SnakeModel
 * @brief Класс модели игры Змейка.
 */
class SnakeModel {
 public:
  /**
   * @brief Конструктор.
   */
  SnakeModel();
  /**
   * @brief Деструктор.
   */
  ~SnakeModel();
  GameInfo_t game;
  Position snakeHead;
  Position fruit;
  int tailX[201], tailY[201], nTail, direction;
  /**
   * @brief Инициализация игры.
   */
  void initializeGame();
  /**
   * @brief Проверка на столкновения.
   */
  int checkCollision();
  /**
   * @brief Рвзмещение фрукта.
   */
  void placeFruit();
  /**
   * @brief Столкновение с фруктом.
   */
  void collisionWithFruit();
  /**
   * @brief Перемещение тела змейки.
   */
  void moveTail();
  /**
   * @brief Запись в файл high_score.
   */
  void writingFile();
  /**
   * @brief Получение из файла high_score.
   */
  int findHighScore();
  /**
   * @brief Обновление уровня.
   */
  int updateLvl();
};
}  // namespace s21
#endif