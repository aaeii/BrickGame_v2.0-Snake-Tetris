/**
 * @file snakeQt.cpp
 * @brief Файл интерфейса игры Змейка на Qt.
 */
#include "snakeQt.h"

namespace s21 {
SnakeQT::SnakeQT(SnakeController *controller) : controller_(controller) {
  setWindowTitle("Snake");
  resize(DOT_SIZE * WIDTH * 2, DOT_SIZE * HEIGHT + 2);
  timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &SnakeQT::updateGame);
  initGame();
  layout();
}
SnakeQT::~SnakeQT() {
  killTimer(timerId);
  delete timer;
}
void SnakeQT::addLabel(QVBoxLayout *info, QLabel *label, const QFont &font) {
  info->addWidget(label);
  label->setFont(font);
}
void SnakeQT::layout() {
  QFont f1("Arial", 14, QFont::Bold);
  QFont bigFont("Arial", 18, QFont::Bold);
  QHBoxLayout *layout = new QHBoxLayout(this);
  QWidget *infoWidget = new QWidget(this);
  QVBoxLayout *info = new QVBoxLayout(infoWidget);
  levelLabel = new QLabel(this);
  scoreLabel = new QLabel(this);
  speedLabel = new QLabel(this);
  highScoreLabel = new QLabel(this);
  pause = new QLabel(this);
  victory = new QLabel(this);
  addLabel(info, levelLabel, f1);
  addLabel(info, scoreLabel, f1);
  addLabel(info, speedLabel, f1);
  addLabel(info, highScoreLabel, f1);
  addLabel(info, pause, f1);
  addLabel(info, victory, bigFont);
  layout->addWidget(infoWidget);
}

void SnakeQT::information() {
  highScoreLabel->setGeometry(WIDTH * DOT_SIZE + 5, 0, 150, 40);
  scoreLabel->setGeometry(WIDTH * DOT_SIZE + 5, 40, 150, 40);
  levelLabel->setGeometry(WIDTH * DOT_SIZE + 5, 80, 150, 40);
  speedLabel->setGeometry(WIDTH * DOT_SIZE + 5, 120, 150, 40);
  pause->setGeometry(WIDTH * DOT_SIZE + 5, 160, 150, 40);
  highScoreLabel->setText(
      QString("High score: %1").arg(controller_->model->game.high_score));
  scoreLabel->setText(QString("Score: %1").arg(controller_->model->game.score));
  if (controller_->model->game.level <= FINAL_LEVEL) {
    levelLabel->setText(
        QString("Level: %1").arg(controller_->model->game.level));
  } else if (controller_->model->game.level == FINAL_LEVEL + 1) {
    levelLabel->setText(QString("Level: %1").arg(FINAL_LEVEL));
  }
  speedLabel->setText(QString("Speed: %1").arg(controller_->model->game.speed));
  if (controller_->model->game.pause == 0) {
    pause->setText(QString("Pause"));
  } else if (controller_->model->game.pause == 1) {
    pause->setText(QString(" "));
  }
}
void SnakeQT::initGame() {
  controller_->model->initializeGame();
  if (timerId) {
    killTimer(timerId);
  }
  tempSpeed = controller_->model->game.speed / 1000;
  timerId = startTimer(tempSpeed);
}
void SnakeQT::fruitsImg(QImage &apple, QImage &banana, QImage &orange,
                        QImage &lemon, QImage &peach) {
  apple = apple.scaled(DOT_SIZE, DOT_SIZE, Qt::KeepAspectRatio,
                       Qt::SmoothTransformation);
  banana = banana.scaled(DOT_SIZE, DOT_SIZE, Qt::KeepAspectRatio,
                         Qt::SmoothTransformation);
  orange = orange.scaled(DOT_SIZE, DOT_SIZE, Qt::KeepAspectRatio,
                         Qt::SmoothTransformation);
  lemon = lemon.scaled(DOT_SIZE, DOT_SIZE, Qt::KeepAspectRatio,
                       Qt::SmoothTransformation);
  peach = peach.scaled(DOT_SIZE, DOT_SIZE, Qt::KeepAspectRatio,
                       Qt::SmoothTransformation);
}
void SnakeQT::drawFruitsImg(QPainter *painter, int i, int j, int x, int y,
                            QImage apple, QImage banana, QImage orange,
                            QImage lemon, QImage peach) {
  if (controller_->model->game.field[i][j] == -1) {
    painter->drawImage(x, y, apple);
  }
  if (controller_->model->game.field[i][j] == -2) {
    painter->drawImage(x, y, banana);
  }
  if (controller_->model->game.field[i][j] == -3) {
    painter->drawImage(x, y, orange);
  }
  if (controller_->model->game.field[i][j] == -4) {
    painter->drawImage(x, y, lemon);
  }
  if (controller_->model->game.field[i][j] == -5) {
    painter->drawImage(x, y, peach);
  }
}
void SnakeQT::draw() {
  QPainter painter(this);
  QPen pen;
  pen.setWidth(1);
  pen.setBrush(Qt::black);
  painter.setPen(pen);
  painter.drawRect(0, 0, 300, 600);
  painter.setBrush(Qt::white);
  painter.drawRect(0, 0, 300, 600);
  QImage apple(":/img/apple.png");
  QImage banana(":/img/banana.png");
  QImage orange(":/img/orange.png");
  QImage lemon(":/img/lemon.png");
  QImage peach(":/img/peach.png");
  fruitsImg(apple, banana, orange, lemon, peach);
  for (int i = 0; i < HEIGHT + 2; ++i) {
    for (int j = 0; j < WIDTH + 2; ++j) {
      int x = DOT_SIZE * j - DOT_SIZE, y = DOT_SIZE * i - DOT_SIZE;
      if (i == controller_->model->snakeHead.y &&
          j == controller_->model->snakeHead.x) {
        pen.setWidth(2);
        pen.setBrush(Qt::darkGreen);
        painter.setPen(pen);
        painter.setBrush(Qt::darkGreen);
        painter.drawRect(x, y, DOT_SIZE, DOT_SIZE);
      } else {
        bool isTail = false;
        for (int k = 0; k < controller_->model->nTail; k++) {
          if (controller_->model->tailX[k] == j &&
              controller_->model->tailY[k] == i) {
            isTail = true;
            pen.setWidth(2);
            pen.setBrush(Qt::darkGreen);
            painter.setPen(pen);
            painter.setBrush(Qt::green);
            painter.drawRect(x, y, DOT_SIZE, DOT_SIZE);
          }
        }
        if (!isTail)
          drawFruitsImg(&painter, i, j, x, y, apple, banana, orange, lemon,
                        peach);
      }
    }
  }
}
void SnakeQT::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);
  if (controller_->model->checkCollision() == 0) {
    draw();
    if (controller_->model->updateLvl() == 0) {
      victory->setText(QString(""));
    } else if (controller_->model->updateLvl() == 1) {
      victory->setGeometry(DOT_SIZE * WIDTH / 5, 200, 450, 40);
      victory->setText(QString("Victory! You have completed the game!"));
    }
  } else if (controller_->model->checkCollision() == 1) {
    controller_->model->writingFile();
    gameOver();
  }
  information();
}

void SnakeQT::keyPressEvent(QKeyEvent *e) {
  UserAction_t act = Start;
  if (e->key() == Qt::Key_Down) {
    act = Down;
  } else if (e->key() == Qt::Key_Up) {
    act = Up;
  } else if (e->key() == Qt::Key_Left) {
    act = Left;
  } else if (e->key() == Qt::Key_Right) {
    act = Right;
  } else if (e->key() == Qt::Key_Enter) {
    act = Start;
  } else if (e->key() == Qt::Key_Escape) {
    act = Terminate;
    initGame();
  } else if (e->key() == ' ') {
    act = Pause;
  } else if (e->key() == Qt::Key_Backspace) {
    act = Action;
  }
  controller_->userInput(act, 1);
  repaint();
  if (act == Terminate) {
    QWidget::close();
  }
}

void SnakeQT::timerEvent(QTimerEvent *event) {
  Q_UNUSED(event);
  if (controller_->model->game.pause == 1 &&
      controller_->model->updateLvl() == 0) {
    controller_->constantMovement();
    controller_->model->moveTail();
    controller_->model->collisionWithFruit();
    if (tempSpeed != controller_->model->game.speed / 1000) {
      killTimer(timerId);
      tempSpeed = controller_->model->game.speed / 1000;
      timerId = startTimer(tempSpeed);
    }

    repaint();
  }
}

void SnakeQT::updateGame() {
  update();
  if (controller_->model->game.pause == 2) {
    killTimer(timerId);
  }
}

void SnakeQT::gameOver() {
  QMessageBox msgb;
  msgb.setWindowTitle("Game Over");
  QPushButton *restartBtn = msgb.addButton("Restart", QMessageBox::AcceptRole);
  QPushButton *exitBtn = msgb.addButton("Exit", QMessageBox::DestructiveRole);
  QFont f1("Arial", 12, QFont::Bold);
  restartBtn->setFixedSize(100, 50);
  restartBtn->setFont(f1);
  exitBtn->setFixedSize(100, 50);
  exitBtn->setFont(f1);

  QHBoxLayout *centerLayout = new QHBoxLayout();
  centerLayout->addStretch();
  centerLayout->addWidget(restartBtn);
  centerLayout->addWidget(exitBtn);
  centerLayout->addStretch();

  QWidget *btnW = new QWidget();
  btnW->setLayout(centerLayout);
  msgb.layout()->addWidget(btnW);

  msgb.exec();
  initGame();
  if (msgb.clickedButton() == restartBtn) {
  } else if (msgb.clickedButton() == exitBtn) {
    QWidget::close();
  }
}
}  // namespace s21
