/**
 * @file snake.h
 * @brief Основной заголовочный файл
 */
#ifndef SNAKE_H
#define SNAKE_H

#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>

namespace s21 {
#define HEIGHT 20
#define WIDTH 10
#define ESCAPE 27
#define ENTER_KEY 10
#define KEY_SPACE ' '
#define FINAL_LEVEL 10
#define FINAL_LENGTH 200
/**
 * @enum UserAction_t
 * @brief Действия пользователя в игре.
 */
typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;
/**
 * @struct GameInfo_t
 * @brief Структура, содержащая информацию о текущем состоянии игры.
 */
typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;
/**
 * @struct Position
 * @brief Структура, содержащая информацию о координатах.
 */
typedef struct {
  int x;
  int y;
} Position;
}  // namespace s21
#endif