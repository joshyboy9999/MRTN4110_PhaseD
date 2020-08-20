#include "Micromouse.hpp"
#include <filesystem>

#define TURN_DIS (3.14159 / 2 * 56.6 / 2 / 20.0)
#define STRAIGHT_DIS (165.0 / 20.0)
#define SPEED (6.28 * 0.4)

using namespace webots;

Micromouse::Micromouse() {
  timeStep = (int)getBasicTimeStep();

  compass = getCompass("compass");
  compass->enable(timeStep);

  // Initialise motors
  leftMotor = getMotor("left wheel motor");
  rightMotor = getMotor("right wheel motor");
  leftMotor->setVelocity(SPEED);
  rightMotor->setVelocity(SPEED);
  leftMotor->setPosition(0.0);
  rightMotor->setPosition(0.0);

  // Initialise encoders
  leftEncoder = getPositionSensor("left wheel sensor");
  rightEncoder = getPositionSensor("right wheel sensor");
  leftEncoder->enable(timeStep);
  rightEncoder->enable(timeStep);

  // Initialise proximity sensors for wall detection
  leftProx = getDistanceSensor("left prox");
  frontProx = getDistanceSensor("front prox");
  rightProx = getDistanceSensor("right prox");
  leftProx->enable(timeStep);
  frontProx->enable(timeStep);
  rightProx->enable(timeStep);
}

Micromouse::~Micromouse() {}

void Micromouse::finishTask() {
  int s = step(timeStep);
  double leftEncVal = leftEncoder->getValue();
  double rightEncVal = rightEncoder->getValue();
  double prevLeftEncVal;
  double prevRightEncVal;

  do {
    prevLeftEncVal = leftEncVal;
    prevRightEncVal = rightEncVal;
    s = step(timeStep);
    leftEncVal = leftEncoder->getValue();
    rightEncVal = rightEncoder->getValue();
  } while (pow(leftEncVal - prevLeftEncVal, 2) + pow(rightEncVal - prevRightEncVal, 2) > 0 && s != -1);
}

double Micromouse::getBearing() {
  const double *cval = compass->getValues();
  double rad = atan2(-cval[2], cval[0]);
  double bear = rad / M_PI * 180.0;
  bear = fmod(bear + 360, 360);
  return bear;
}

void Micromouse::setRowColHeading(int r, int c, int h) {
  row = r;
  col = c;
  heading = h;
}

void Micromouse::turnLeft() {
  turnTo(heading + 1);
}

void Micromouse::turnRight() {
  turnTo(heading - 1);
}

void Micromouse::turnTo(int targetHeading) {
  // Enter velocity control
  leftMotor->setPosition(INFINITY);
  rightMotor->setPosition(INFINITY);

  // Make targetHeading within [0,3]
  targetHeading = (targetHeading + 4) % 4;

  double targetBearing = (90 * (1 - targetHeading) + 360) % 360;
  double currentBreaing = getBearing();
  int s = 0;
  while (abs(targetBearing - currentBreaing) > 1 && s != -1) {
    double diff = targetBearing - currentBreaing;
    double speed = SPEED * pow(abs(diff) / 180, 0.2);
    if (fmod(diff + 360, 360) > 180) {
      // Turn left
      leftMotor->setVelocity(-speed);
      rightMotor->setVelocity(speed);
    } else {
      // Turn right
      leftMotor->setVelocity(speed);
      rightMotor->setVelocity(-speed);
    }

    s = step(timeStep);
    currentBreaing = getBearing();
  }
  // Stop motors and enter position control
  leftMotor->setPosition(leftEncoder->getValue());
  rightMotor->setPosition(rightEncoder->getValue());
  leftMotor->setVelocity(SPEED);
  rightMotor->setVelocity(SPEED);
  step(timeStep);
  heading = targetHeading;
}

void Micromouse::goStraight() {
  leftMotor->setPosition(leftEncoder->getValue() + STRAIGHT_DIS);
  rightMotor->setPosition(rightEncoder->getValue() + STRAIGHT_DIS);

  switch (heading) {
  case 0:
    col++;
    break;
  case 1:
    row--;
    break;
  case 2:
    col--;
    break;
  case 3:
    row++;
    break;
  }
  finishTask();
}

void Micromouse::printState(int step) {
  std::cout << "Step: " << std::setfill('0') << std::setw(2) << step
            << ", Row: " << row
            << ", Column: " << col
            << ", Heading: " << getHeadingChar(heading)
            << ", Left Wall: " << checkWallProx(leftProx)
            << ", Front Wall: " << checkWallProx(frontProx)
            << ", Right Wall: " << checkWallProx(rightProx)
            << "\n";
}

int Micromouse::mystep() {
  return step(timeStep);
}

char Micromouse::getHeadingChar(int heading) {
  char ch = '-';
  switch (heading) {
  case 0:
    ch = 'E';
    break;
  case 1:
    ch = 'N';
    break;
  case 2:
    ch = 'W';
    break;
  case 3:
    ch = 'S';
    break;
  }
  return ch;
}

char Micromouse::checkWallProx(DistanceSensor *prox) {
  return (prox->getValue() < 900) ? 'Y' : 'N';
}
