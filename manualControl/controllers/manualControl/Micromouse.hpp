#ifndef MICROMOUSE_HPP
#define MICROMOUSE_HPP

#include <webots/Robot.hpp>
#include <webots/Motor.hpp>
#include <webots/DistanceSensor.hpp>
#include <webots/PositionSensor.hpp>
#include <webots/Compass.hpp>
#include <webots/Keyboard.hpp>

using namespace webots;

class Micromouse : public Robot {
  Compass* compass;
  Motor* leftMotor;
  Motor* rightMotor;
  PositionSensor *leftEncoder;
  PositionSensor *rightEncoder;
  DistanceSensor* leftProx;
  DistanceSensor* frontProx;
  DistanceSensor* rightProx;
  int row;
  int col;
  int heading;
public:
  Keyboard* keyboard;
  int timeStep;
  Micromouse();
  ~Micromouse();
  void setRowColHeading(int r, int c, int h);
  void turnLeft();
  void turnRight();
  void goStraight();
  void printState(int step);
  int mystep();
private:
  void finishTask();
  void turnTo(int ogtargetHeading);
  double getBearing();
  char getHeadingChar(int heading);
  char checkWallProx(DistanceSensor* prox);
};

#endif // MICROMOUSE_HPP