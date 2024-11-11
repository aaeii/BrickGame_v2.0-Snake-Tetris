/**
 * @file main.cpp
 * @brief Основной файл для запуска игр на Qt.
 */

#include "snakeQt.h"
#include "tetrisQt.h"

void setupButton(QPushButton *btn, const QFont &f) {
  btn->setFixedSize(250, 100);
  btn->setFont(f);
}
int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  QWidget w1;
  w1.setWindowTitle("Menu");
  w1.setFixedSize(400, 500);
  QVBoxLayout *verticalLayout = new QVBoxLayout(&w1);
  verticalLayout->setAlignment(Qt::AlignCenter);
  QPushButton *snakeBtn = new QPushButton("Snake");
  QPushButton *tetrisBtn = new QPushButton("Tetris");
  QPushButton *exitBtn = new QPushButton("Exit");

  QFont f1("Arial", 18, QFont::Bold);
  setupButton(snakeBtn, f1);
  setupButton(tetrisBtn, f1);
  setupButton(exitBtn, f1);

  QVBoxLayout *btn = new QVBoxLayout;
  btn->addWidget(snakeBtn);
  btn->addWidget(tetrisBtn);
  btn->addWidget(exitBtn);
  verticalLayout->addLayout(btn);

  s21::SnakeModel m{};
  s21::SnakeController c{&m};
  s21::SnakeQT v{&c};
  QObject::connect(snakeBtn, &QPushButton::clicked, [&]() { v.show(); });

  TetrisQT tetris;
  QObject::connect(tetrisBtn, &QPushButton::clicked, [&]() { tetris.show(); });
  QObject::connect(exitBtn, &QPushButton::clicked, qApp, &QApplication::quit);
  w1.show();

  return app.exec();
}
