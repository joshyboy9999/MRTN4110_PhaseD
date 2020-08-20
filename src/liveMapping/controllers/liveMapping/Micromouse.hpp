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
public:
  DistanceSensor* leftProx;
  DistanceSensor* frontProx;
  DistanceSensor* rightProx;
  int row;
  int col;
  int heading;
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
  bool getWallFromProx(DistanceSensor* prox);
  char getHeadingChar(int heading);
private:
  void finishTask();
  void turnTo(int ogtargetHeading);
  double getBearing();
};

#endif // MICROMOUSE_HPP