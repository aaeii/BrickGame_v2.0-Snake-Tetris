/**
 * @file tetris.h
 * @brief Основной заголовочный файл
 */
#ifndef TETRIS_H
#define TETRIS_H

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#define HEIGHT 20
#define WIDTH 10
#define ESCAPE 27
#define ENTER_KEY 10
#define ARRAY_SIZE 4
#define KEY_SPACE ' '
#define FINAL_LEVEL 10

/**
 * @struct Figure
 * @brief Структура, содержащая информацию о фигуре.
 */
typedef struct {
  /// @brief Массив для хранения фигуры.
  int **array;
  /// @brief Координата.
  int x;
  /// @brief Координата.
  int y;
} Figure;

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
 * @brief Структура представления времени с большой точностью.
 *
 *  tv_sec - количество целых секунд, extern - описание структуры.
 */
extern struct timeval before, now;
/**
 * @brief Копирование заданной фигуры.
 * @param Указатель на структуру Figure, которую необходимо скопировать.
 */
Figure copy_figure(Figure *figure);
/**
 * @brief Удаление заданной фигуры.
 * @param Указатель на структуру Figure, которую необходимо удалить.
 */
void delete_figure(Figure *figure);
/**
 * @brief Проверка положения фигуры.
 * @param figure указатель на структуру Figure (текущая фигура), g указатель на
 * структуру GameInfo_t, которая содержит общую информацию об игре.
 */
int check_position(Figure *figure, GameInfo_t *g);
/**
 * @brief Остановка фигуры при касании нижней границы.
 * @param figure указатель на структуру Figure (текущая фигура), g указатель на
 * структуру GameInfo_t, которая содержит общую информацию об игре.
 */
void stopping_figure(Figure *, GameInfo_t *g);
/**
 * @brief Изменение скорости, уровня, очков.
 * @param game указатель на структуру GameInfo_t, количество линий.
 */
void change_level_speed_score(GameInfo_t *game, int count);
/**
 * @brief Удаление заполненных линий
 * @param Указатель на структуру GameInfo_t.
 */
void deleting_lines(GameInfo_t *);
/**
 * @brief Поворот заданной фигуры.
 * @param Указатель на структуру Figure, которую необходимо повернуть.
 */
void rotate_figure(Figure *figure);
/**
 * @brief Генерация следующей фигуры.
 * @param Указатель на структуру Figure, которую необходимо сгенерировать.
 */
void generating_next_figure(Figure *next);
/**
 * @brief Генерация текущей фигуры.
 * @param Current указатель на структуру Figure (текущая фигура), next указатель
 * на структуру Figure (следующая фигура), g указатель на структуру GameInfo_t,
 * которая содержит общую информацию об игре.
 */
void generating_current_figure(Figure *current, Figure *next, GameInfo_t *g);
/**
 * @brief Управление клавишами.
 */
void userInput(UserAction_t action, Figure *, Figure *, GameInfo_t *);
/**
 * @brief Игровые действия.
 */
void game_actions(int sign, Figure *, Figure *, GameInfo_t *g);
/**
 * @brief Ввод начальных данных.
 */
void set_start(GameInfo_t *game);
/**
 * @brief Обновление состояния игры.
 */
GameInfo_t updateCurrentState();
/**
 * @brief Удаление информации о состоянии игры.
 * @param game указатель на структуру GameInfo_t.
 */
void delete_field(GameInfo_t *game);
/**
 * @brief Поиск high_score.
 * @return Возвращает high_score если найден.
 */
int find_high_score();
/**
 * @brief Запись high_score в файл.
 * @param Указатель на структуру GameInfo_t.
 */
void write_score_to_file(GameInfo_t *g);
#endif