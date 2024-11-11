/**
 * @file snakeQt.h
 * @brief Заголовочный файл интерфейса игры Змейка на Qt.
 */
#ifndef SNAKEQT_H
#define SNAKEQT_H

#include <QApplication>
#include <QHBoxLayout>
#include <QImage>
#include <QKeyEvent>
#include <QLabel>
#include <QMessageBox>
#include <QPainter>
#include <QPen>
#include <QPoint>
#include <QPushButton>
#include <QRandomGenerator>
#include <QTime>
#include <QTimer>
#include <QVBoxLayout>
#include <QVector>
#include <QWidget>

#include "../../brick_game/snake/controller/controller.h"

namespace s21 {
/**
 * @class SnakeQT
 * @brief Класс интерфейса игры Змейка для Qt.
 */
class SnakeQT : public QWidget {
  Q_OBJECT

 public:
  SnakeController *controller_;
  /**
   * @brief Конструктор.
   * @param controller Указатель на объект контроллера игры.
   */
  SnakeQT(SnakeController *controller);
  ~SnakeQT();

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
  /**
   * @brief Печать дополнительных сведений об игре.
   */
  void information();
  void layout();
  /**
   * @brief Отрисовка игрового поля.
   */
  void draw();
  /**
   * @brief Инициализация игры.
   */
  void initGame();
  QTimer *timer;
  int timerId = 0;
  int DOT_SIZE = 30;
  /**
   * @brief Обновление игры (перерисовывание поля), вызывается когда срабатывает
   * таймер.
   */
  void updateGame();
  /**
   * @brief Конец игры.
   */
  void gameOver();
  QLabel *levelLabel;
  QLabel *scoreLabel;
  QLabel *speedLabel;
  QLabel *highScoreLabel;
  QLabel *pause;
  QLabel *victory;
  int tempSpeed;
  /**
   * @brief Создание QLabel для отображения текста.
   */
  void addLabel(QVBoxLayout *info, QLabel *label, const QFont &font);
  void drawFruitsImg(QPainter *painter, int i, int j, int x, int y,
                     QImage apple, QImage banana, QImage orange, QImage lemon,
                     QImage peach);
  void fruitsImg(QImage &apple, QImage &banana, QImage &orange, QImage &lemon,
                 QImage &peach);
};
}  // namespace s21

#endif  // SNAKEQT_H
