/**
 * @file tetrisQt.h
 * @brief Заголовочный файл интерфейса игры Тетрис на Qt.
 */
#ifndef TETRISQT_H
#define TETRISQT_H

#include <QApplication>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QMessageBox>
#include <QPainter>
#include <QPoint>
#include <QPushButton>
#include <QRandomGenerator>
#include <QTime>
#include <QTimer>
#include <QVBoxLayout>
#include <QVector>
#include <QWidget>

#ifdef __cplusplus
extern "C" {
#endif
#include "../../brick_game/tetris/tetris.h"
#ifdef __cplusplus
}
#endif
/**
 * @class TetrisQT
 * @brief Класс интерфейса игры Тетрис для Qt.
 */
class TetrisQT : public QWidget {
  Q_OBJECT
 public:
  TetrisQT();
  ~TetrisQT();

 private:
  /**
   * @brief Отрисовка всего поля.
   */
  void paintEvent(QPaintEvent *event);
  /**
   * @brief Обработка нажатия клавиш.
   */
  void keyPressEvent(QKeyEvent *event);
  /**
   * @brief Обновление состояния игры.
   */
  void timerEvent(QTimerEvent *event);
  QTimer *timer;
  GameInfo_t G;
  Figure currentFigure, nextFigure;
  void addLabel(QVBoxLayout *info, QLabel *label, const QFont &font);
  void layout();
  /**
   * @brief Печать дополнительных сведений об игре.
   */
  void printInformation();
  /**
   * @brief Отрисовка фигур.
   */
  void drawFigures();
  /**
   * @brief Обновление игры и проверка победы/поражения.
   */
  void updateGame();
  int timerId = 0;
  int check(Figure *figure, GameInfo_t *g);
  /**
   * @brief Конец игры.
   */
  void gameOver();
  /**
   * @brief Инициализация игры.
   */
  void initGame();
  QLabel *levelLabel;
  QLabel *scoreLabel;
  QLabel *speedLabel;
  QLabel *highScoreLabel;
  QLabel *pause;
  QLabel *victory;
  QLabel *next;
  int tempSpeed;
  int DOT_SIZE = 30;
};

#endif
