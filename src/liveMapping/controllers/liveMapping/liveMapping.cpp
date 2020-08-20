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
#include "Maze.hpp"

// All the webots classes are defined in the "webots" namespace
using namespace webots;

void updateMazeFromMouse(Maze* maze, Micromouse* mouse) {
  bool leftWall = mouse->getWallFromProx(mouse->leftProx);
  bool frontWall = mouse->getWallFromProx(mouse->frontProx);
  bool rightWall = mouse->getWallFromProx(mouse->rightProx);
  int leftWallType = leftWall ? WALL_THERE : WALL_GAP;
  int frontWallType = frontWall ? WALL_THERE : WALL_GAP;
  int rightWallType = rightWall ? WALL_THERE : WALL_GAP;
  int row = mouse->row;
  int col = mouse->col;
  int heading = mouse->heading;
  if (heading == 0) {
    if (row > 0)            maze->setHoriWall(row-1, col, leftWallType);
    if (col < COL_COUNT-1)  maze->setVertWall(row, col, frontWallType);
    if (row < ROW_COUNT-1)  maze->setHoriWall(row, col, rightWallType);
  
  } else if (heading == 1) {
    if (col > 0)            maze->setVertWall(row, col-1, leftWallType);
    if (row > 0)            maze->setHoriWall(row-1, col, frontWallType);
    if (col < COL_COUNT-1)  maze->setVertWall(row, col, rightWallType);
  
  } else if (heading == 2) {
    if (row < ROW_COUNT-1)  maze->setHoriWall(row, col, leftWallType);
    if (col > 0)            maze->setVertWall(row, col-1, frontWallType);
    if (row > 0)            maze->setHoriWall(row-1, col, rightWallType);
  
  } else if (heading == 3) {
    if (col < COL_COUNT-1)  maze->setVertWall(row, col, leftWallType);
    if (row < ROW_COUNT-1)  maze->setHoriWall(row, col, frontWallType);
    if (col > 0)            maze->setVertWall(row, col-1, rightWallType);
  }
  maze->setRobotPose(row, col, mouse->getHeadingChar(mouse->heading));
}

void printCurrentKnownMaze(Maze* maze, Micromouse* mouse) {
  
  maze->printMaze();
}

int main(int argc, char **argv) {
  Micromouse* mouse = new Micromouse();
  mouse->setRowColHeading(0,0,3);
  Maze* maze = new Maze();
  maze->initialiseWallsAsUnknown();

  mouse->step(mouse->timeStep);
  updateMazeFromMouse(maze, mouse);
  maze->printMaze();
  
  while (mouse->step(mouse->timeStep) != -1) {
    auto key = mouse->keyboard->getKey();
    if (key > 0) {
      switch (key) {
        case Keyboard::UP:
          // std::cout << "Go forwards\n";
          mouse->goStraight();
          updateMazeFromMouse(maze, mouse);
          maze->printMaze();
          break;
        case Keyboard::RIGHT:
          // std::cout << "Turn right\n";
          mouse->turnRight();
          updateMazeFromMouse(maze, mouse);
          maze->printMaze();
          break;
        case Keyboard::LEFT:
          // std::cout << "Turn left\n";
          mouse->turnLeft();
          updateMazeFromMouse(maze, mouse);
          maze->printMaze();
          break;
      }
    }
  }
  
  std::cout << "Done\n";
  delete mouse;
  delete maze;
  return 0;
  
}

