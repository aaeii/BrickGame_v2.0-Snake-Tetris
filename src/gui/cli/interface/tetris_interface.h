/**
 * @file tetris_interface.h
 * @brief Заголовочный файл интерфейса игры тетрис.
 */

#ifndef INTERFACE_H
#define INTERFACE_H
#include <math.h>

#include "../../../brick_game/tetris/tetris.h"
/**
 * @brief Печать дополнительных сведений об игре.
 * @param Указатель на структуру GameInfo_t.
 */
void print_information(GameInfo_t *g);
extern int timer;
/**
 * @brief Печать рамки вокруг игрового поля.
 * @param Указатель на структуру GameInfo_t.
 */
void print_frame();
/**
 * @brief Печать игрового поля.
 * @param Current указатель на структуру Figure (текущая фигура), next указатель
 * на структуру Figure (следующая фигура), g указатель на структуру GameInfo_t.
 */
void PrintTable(Figure *current, Figure *next, GameInfo_t *g);
/**
 * @brief Конец игры.
 * После проигрыша ожидает ввода, чтобы перезапустить игру, либо завершить.
 * @param Указатель на структуру GameInfo_t.
 */
void game_over(Figure *current, GameInfo_t *g);
/**
 * @brief Получение сигнала.
 * @return Возвращает полученный сигнал.
 */
UserAction_t get_signal(int user_input);
void game_tetris();
void victory(GameInfo_t *g);
void restart(GameInfo_t *g);
#endif