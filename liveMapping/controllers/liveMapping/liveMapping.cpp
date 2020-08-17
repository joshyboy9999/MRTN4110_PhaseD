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
  
  std::cout << "Done\n";
  delete micromouse;
  return 0;
  
}

