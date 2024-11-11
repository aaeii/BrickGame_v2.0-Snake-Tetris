#include <gtest/gtest.h>

#include "../brick_game/snake/controller/controller.h"
TEST(model, test1) {
  s21::SnakeModel m{};
  m.initializeGame();
  m.snakeHead.y = 0;
  m.snakeHead.x = 0;
  int res = m.checkCollision();
  EXPECT_EQ(res, 1);
  m.snakeHead.y = 2;
  m.snakeHead.x = 2;
  m.tailX[1] = m.snakeHead.x;
  m.tailY[1] = m.snakeHead.y;
  res = m.checkCollision();
  EXPECT_EQ(res, 1);
}

TEST(model, test2) {
  s21::SnakeModel model{};
  model.initializeGame();
  model.snakeHead.y = model.fruit.y;
  model.snakeHead.x = model.fruit.x;
  model.tailX[1] = model.fruit.x;
  model.tailY[1] = model.fruit.y;
  model.placeFruit();
  model.collisionWithFruit();
  EXPECT_EQ(model.nTail, 5);
  EXPECT_EQ(model.game.score, 1);
  model.initializeGame();
  model.tailX[1] = model.fruit.x;
  model.tailY[1] = model.fruit.y;
  model.placeFruit();
}
TEST(model, test3) {
  s21::SnakeModel m{};
  m.game.high_score = 20;
  m.writingFile();
  m.game.high_score = 30;
  m.writingFile();
  int res = m.findHighScore();
  EXPECT_EQ(res, 30);
}

TEST(model, test4) {
  s21::SnakeModel m{};
  m.initializeGame();
  m.game.score = 5;
  m.game.high_score = 3;
  int res = m.updateLvl();
  EXPECT_EQ(m.game.level, 2);
  EXPECT_EQ(m.game.speed, 750000);

  EXPECT_EQ(res, 0);
  m.game.level = FINAL_LEVEL + 1;
  res = m.updateLvl();
  EXPECT_EQ(res, 1);
}

TEST(model, test5) {
  s21::SnakeModel m{};
  m.initializeGame();
  m.moveTail();
  EXPECT_EQ(m.tailX[0], m.snakeHead.x);
  EXPECT_EQ(m.tailY[0], m.snakeHead.y);
}

TEST(controller, test1) {
  s21::SnakeModel m{};
  s21::SnakeController c{&m};
  m.initializeGame();
  EXPECT_EQ(m.game.pause, 2);
  c.userInput(s21::Start, 1);
  EXPECT_EQ(m.game.pause, 1);
  c.userInput(s21::Left, 1);
  EXPECT_EQ(m.direction, 3);
  c.userInput(s21::Right, 1);
  EXPECT_EQ(m.direction, 3);
  c.userInput(s21::Up, 1);
  EXPECT_EQ(m.direction, 5);
  c.userInput(s21::Down, 1);
  EXPECT_EQ(m.direction, 5);
  c.userInput(s21::Right, 1);
  c.userInput(s21::Pause, 1);
  EXPECT_EQ(m.game.pause, 0);
  c.userInput(s21::Pause, 1);
  EXPECT_EQ(m.game.pause, 1);
  EXPECT_EQ(m.direction, 4);
  c.userInput(s21::Down, 1);
  EXPECT_EQ(m.direction, 6);
  c.userInput(s21::Action, 1);
  EXPECT_EQ(m.game.speed, 750000);
  c.userInput(s21::Terminate, 1);
  EXPECT_EQ(m.game.pause, 3);
}

TEST(controller, test3) {
  s21::SnakeModel m{};
  s21::SnakeController c{&m};
  m.initializeGame();
  m.direction = 3;
  c.constantMovement();
  EXPECT_EQ(m.snakeHead.x, 5);
  m.snakeHead.x = 0;
  m.direction = 4;
  c.constantMovement();
  EXPECT_EQ(m.snakeHead.x, 1);
  m.snakeHead.y = 4;
  m.direction = 5;
  c.constantMovement();
  EXPECT_EQ(m.snakeHead.y, 3);
  m.snakeHead.y = 2;
  m.direction = 6;
  c.constantMovement();
  EXPECT_EQ(m.snakeHead.y, 3);
}

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}