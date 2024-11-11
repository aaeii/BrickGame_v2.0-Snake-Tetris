/**
 * @file tetrisQt.cpp
 * @brief Файл интерфейса игры Тетрис на Qt.
 */
#include "tetrisQT.h"

TetrisQT::TetrisQT() {
  setWindowTitle("Tetris");
  resize(DOT_SIZE * WIDTH * 2, DOT_SIZE * (HEIGHT) + 2);
  timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &TetrisQT::updateGame);
  initGame();
  layout();
}
TetrisQT::~TetrisQT() {
  killTimer(timerId);
  delete_figure(&currentFigure);
  delete_figure(&nextFigure);
  delete_field(&G);
  delete timer;
}
void TetrisQT::addLabel(QVBoxLayout *info, QLabel *label, const QFont &font) {
  info->addWidget(label);
  label->setFont(font);
}
void TetrisQT::layout() {
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
  next = new QLabel(this);
  addLabel(info, levelLabel, f1);
  addLabel(info, scoreLabel, f1);
  addLabel(info, speedLabel, f1);
  addLabel(info, highScoreLabel, f1);
  addLabel(info, pause, f1);
  addLabel(info, next, f1);
  addLabel(info, victory, bigFont);
  layout->addWidget(infoWidget);
}
void TetrisQT::initGame() {
  G = updateCurrentState();
  if (timerId > 0) {
    killTimer(timerId);
  }
  tempSpeed = G.speed / 1000;
  timerId = startTimer(tempSpeed);

  nextFigure = {nullptr, 0, 0};
  currentFigure = {nullptr, 0, 0};
  generating_next_figure(&nextFigure);
  generating_current_figure(&currentFigure, &nextFigure, &G);
}
void TetrisQT::paintEvent(QPaintEvent *e) {
  Q_UNUSED(e)
  if (check_position(&currentFigure, &G) != 0) {
    drawFigures();
    if (G.level <= FINAL_LEVEL) {
      if (G.pause == 0) {
        pause->setText(QString("Pause"));
      } else if (G.pause == 1) {
        pause->setText(QString(" "));
      }
      victory->setText(QString(""));
    } else if (G.level == FINAL_LEVEL + 1) {
      victory->setGeometry(DOT_SIZE * 2, 200, 450, 40);
      victory->setText(QString("Victory! You have completed the game!"));
    }
  }

  else if (check_position(&currentFigure, &G) == 0) {
    write_score_to_file(&G);
    gameOver();
  }

  printInformation();
}
void TetrisQT::drawFigures() {
  QPainter p(this);
  p.drawRect(0, 0, 300, 600);
  pause->setGeometry(WIDTH * DOT_SIZE + 5, 300, 150, 40);
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      if (G.field[i][j]) {
        p.setBrush(Qt::green);
      } else {
        p.setBrush(Qt::white);
      }
      p.drawRect(DOT_SIZE * j, DOT_SIZE * i, DOT_SIZE, DOT_SIZE);
    }
  }

  for (int i = 0; i < ARRAY_SIZE; i++) {
    for (int j = 0; j < ARRAY_SIZE; j++) {
      if (currentFigure.array[i][j]) {
        p.setBrush(Qt::green);
        p.drawRect(DOT_SIZE * (currentFigure.x + j),
                   DOT_SIZE * (currentFigure.y + i), DOT_SIZE, DOT_SIZE);
      }
    }
  }

  for (int i = 0; i < ARRAY_SIZE; i++) {
    for (int j = 0; j < ARRAY_SIZE; j++) {
      if (nextFigure.array[i][j]) {
        p.setBrush(Qt::darkGreen);
        p.drawRect((WIDTH + 1 + j) * DOT_SIZE, (6 + i) * DOT_SIZE, DOT_SIZE,
                   DOT_SIZE);
      }
    }
  }
}
void TetrisQT::printInformation() {
  highScoreLabel->setGeometry(WIDTH * DOT_SIZE + 5, 0, 190, 40);
  scoreLabel->setGeometry(WIDTH * DOT_SIZE + 5, 40, 150, 40);
  levelLabel->setGeometry(WIDTH * DOT_SIZE + 5, 80, 150, 40);
  speedLabel->setGeometry(WIDTH * DOT_SIZE + 5, 120, 150, 40);
  next->setGeometry(WIDTH * DOT_SIZE + 5, 160, 150, 40);
  highScoreLabel->setText(QString("High score: %1").arg(G.high_score));
  scoreLabel->setText(QString("Score: %1").arg(G.score));
  levelLabel->setText(QString("Level: %1").arg(G.level));
  speedLabel->setText(QString("Speed: %1").arg(G.speed));
  next->setText(QString("Next:"));
}

void TetrisQT::updateGame(void) {
  update();
  if (G.pause == 0) {
    killTimer(timerId);
  }
}
void TetrisQT::timerEvent(QTimerEvent *event) {
  Q_UNUSED(event);
  if (check_position(&currentFigure, &G) && G.level <= FINAL_LEVEL) {
    if (check(&currentFigure, &G)) {
      if (G.pause == 1) {
        currentFigure.y++;
      }
    } else {
      stopping_figure(&currentFigure, &G);
      deleting_lines(&G);
      generating_current_figure(&currentFigure, &nextFigure, &G);
      if (tempSpeed != G.speed / 1000) {
        killTimer(timerId);
        tempSpeed = G.speed / 1000;
        timerId = startTimer(tempSpeed);
      }
    }
    repaint();
  }
}

int TetrisQT::check(Figure *figure, GameInfo_t *g) {
  int flag = 1;
  for (int i = 0; i < ARRAY_SIZE; i++) {
    for (int j = 0; j < ARRAY_SIZE; j++) {
      if (figure->array[i][j]) {
        int x = figure->x + j, y = figure->y + i + 1;
        if (y >= HEIGHT || g->field[y][x]) {
          flag = 0;
        }
      }
    }
  }
  return flag;
}
void TetrisQT::keyPressEvent(QKeyEvent *e) {
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
  userInput(act, &currentFigure, &nextFigure, &G);
  repaint();
  if (act == Terminate) {
    QWidget::close();
  }
}

void TetrisQT::gameOver() {
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
