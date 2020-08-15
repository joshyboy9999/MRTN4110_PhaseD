// File:          z5164829_controller.cpp
// Date:
// Description:
// Author:
// Modifications:

// You may need to add webots include files such as
// <webots/DistanceSensor.hpp>, <webots/Motor.hpp>, etc.
// and/or to add some other includes
#include <webots/Robot.hpp>
#include <webots/Motor.hpp>
#include <webots/DistanceSensor.hpp>
#include <webots/PositionSensor.hpp>
#include <webots/Compass.hpp>
#include <fstream>
#include <filesystem>
#include <webots/Keyboard.hpp>

#include "Micromouse.hpp"

// All the webots classes are defined in the "webots" namespace
using namespace webots;

#define PATH_PLAN_FILE_NAME "../../PathPlan.txt"

std::string getPathPlanString(std::string path) { 
  std::ifstream ifs (path);
  std::string str;
  std::getline(ifs, str);
  return str;
}

int main(int argc, char **argv) {
  Micromouse* micromouse = new Micromouse();
  micromouse->setRowColHeading(0,0,3);
  
  while (micromouse->step(micromouse->timeStep) != -1) {
    auto key = micromouse->keyboard->getKey();
    if (key > 0) {
      std::cout << key << "\n";
      switch (key) {
        case Keyboard::UP:
          std::cout << "Go forwards\n";
          micromouse->goStraight();
          break;
        case Keyboard::RIGHT:
          std::cout << "Turn right\n";
          micromouse->turnRight();
          break;
        case Keyboard::LEFT:
          std::cout << "Turn left\n";
          micromouse->turnLeft();
          break;
      }
    }
  }
  
  // std::cout << "Start - Read path plan from " << PATH_PLAN_FILE_NAME << ":\n";
  // std::string pathPlanString = getPathPlanString(PATH_PLAN_FILE_NAME);
  // std::cout << pathPlanString << "\n";
  // std::cout << "Done - Path plan read!\n";

  // int step = 0;
  // int startRow = pathPlanString.at(0) - '0';
  // int startCol = pathPlanString.at(1) - '0';
  // char headingLetter = pathPlanString.at(2);
  // int startHeading = 0;
  // switch (headingLetter) {
    // case 'E':
      // startHeading = 0; break;
    // case 'N':
      // startHeading = 1; break;
    // case 'W':
      // startHeading = 2; break;
    // case 'S':
      // startHeading = 3; break;
  // }
  // micromouse->setRowColHeading(startRow,startCol,startHeading);

  // std::cout << "Start - Execute path plan!\n";
  // micromouse->step(micromouse->timeStep);
  // for (int i = 3; i < pathPlanString.length(); i++) {
    // micromouse->printState(step);
    // switch (pathPlanString.at(i)) {
      // case 'L':
        // micromouse->turnLeft(); break;
      // case 'F':
        // micromouse->goStraight(); break;
      // case 'R':
        // micromouse->turnRight(); break;
    // }
    // finishTask(robot, timeStep, leftEncoder, rightEncoder, frontProx);
    // step++;
  // }
  // micromouse->printState(step);
  std::cout << "Done\n";
  delete micromouse;
  return 0;
  
}

