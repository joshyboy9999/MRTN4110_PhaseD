// File: z5164829_MTRN4110_PhaseB.cpp
// Date: 23/6/2020
// Description: Shortest path finder
// Author: Tobin Smit

#include <fstream>
#include <filesystem>
#include <iostream>
#include <vector>
#include <string>

#define MAP_FILE_NAME "../../Map.txt"
#define rowCount 5
#define colCount 9

#define centreRow (rowCount/2)
#define centreCol (colCount/2)

struct cell {
  int row;
  int col;
  cell() {
    row = -1;
    col = -1;
  }
  cell(int r, int c) {
    row = r;
    col = c;
  }

  bool operator==(const cell& other) {
    return (this->row == other.row && this->col == other.col);
  }

} typedef Cell;

std::vector<Cell> typedef Path;
std::vector<Path > typedef Paths;


struct maze {
  bool horiWalls[rowCount-1][colCount] = {{0}};
  bool vertWalls[rowCount][colCount-1] = {{0}};
  int robotCol;
  int robotRow;
  int robotDir;
  char robotCardinal;
  char robotArrow;
  
  void addHoriWall(int wallRow, int col) {
    // std::cout << "addHoriWall(" << wallRow << ", "  << col << ")\n";
    horiWalls[wallRow][col] = 1;
  }
  void addVertWall(int row, int wallCol) {
    // std::cout << "addVertWall(" << row << ", "  << wallCol << ")\n";
    vertWalls[row][wallCol] = 1;
  }

  void addRobot(int row, int col, char ch) {
    robotRow = row;
    robotCol = col;
    robotArrow = ch;
    switch (ch) {
      case '>':
        robotDir = 0;
        robotCardinal = 'E'; break;
      case '^':
        robotDir = 1;
        robotCardinal = 'N'; break;
      case '<':
        robotDir = 2;
        robotCardinal = 'W'; break;
      case 'v':
        robotDir = 3;
        robotCardinal = 'S'; break;
    }
    // std::cout << "addRobot(" << row << ", "  << col << ", "  << ch << "="  << robotCardinal << ")\n";
  }

  std::vector<std::string> getWallText(){
    std::vector<std::string> text (rowCount*2+1, "");
    int line = 0;
    text[line++] = " --- --- --- --- --- --- --- --- --- \n";

    for (int row = 0; row < rowCount; row++) {
      // Vertical wall line
      text[line].append("|");
      for (int col = 0; col < colCount-1; col++) {
        text[line].append(vertWalls[row][col] ? "   |" : "    ");
      }
      text[line++].append("   |\n");

      // Horiznotal wall line
      if (row < rowCount-1) {
        for (int c = 0; c < colCount; c++) {
          text[line].append(horiWalls[row][c] ? " ---" : "    ");
        }
        text[line++].append("\n");
      } else {
        text[line++].append(" --- --- --- --- --- --- --- --- --- \n");
      }
    }
    return text;
  }

  std::vector<std::string> addRobotArrowToText(std::vector<std::string> text) {
      int line = robotRow*2 + 1;
      int chIdx = robotCol*4 + 2;
      text[line][chIdx] = robotArrow;
      text[line][chIdx+1] = ' ';
      return text;
  }

  void printText(std::vector<std::string> text) {
    for (int lineIdx = 0; lineIdx < text.size(); lineIdx++) {
      std::cout << text[lineIdx];
    }
  }

  void printWalls() {
    printText(getWallText());
  }

  void printMaze() {
    printText(addRobotArrowToText(getWallText()));
  }

  void printPath(const Path path) {
    std::vector<std::string> text = getWallText();
    for (int cellIdx = 0; cellIdx != path.size(); cellIdx++) {
      int row = path[cellIdx].row;
      int col = path[cellIdx].col;
      int line = row*2 + 1;
      int chIdx = col*4 + 2;
      std::string label = std::to_string(path.size() - 1 - cellIdx);
      text[line].replace(chIdx, label.size(), label);
      // std::cout << "row" << row << " col" << col << "\n";
    }
    text = addRobotArrowToText(text);
    printText(text);
  }

  void printPaths(const Paths paths) {
    for (int pathIdx = 0; pathIdx != paths.size(); pathIdx++) {
      std::cout << "--- Path " << pathIdx+1 << " ---\n";
      printPath(paths[pathIdx]);
    }
  }

  bool noWallFromCellInDir(const Cell cell, const int dir) {
    int row = cell.row;
    int col = cell.col;
    bool res;
    switch(dir) {
      case 0:
        res = (col == colCount-1 ? 0 : !vertWalls[row][col]); break;
      case 1:
        res = (row == 0 ? 0 : !horiWalls[row-1][col]); break;
      case 2:
        res = (col == 0 ? 0 : !vertWalls[row][col-1]); break;
      case 3:
        res = (row == rowCount-1 ? 0 : !horiWalls[row][col]); break;
    }
    // std::cout << "row" << row << " col" << col << " dir" << dir << " res" << res << "\n";
    return res;
  }

  bool isCellInPath(Path path, Cell cell) {
    return (std::find(path.begin(), path.end(), cell) != path.end());
  }

  Paths expandPaths(const Paths paths) {
    Paths newPaths;
    for (int pathIdx = 0; pathIdx != paths.size(); pathIdx++) {
      Path path = paths[pathIdx];
      Cell currentCell = path[path.size()-1];
      // Cell lastCell;
      // if (path.size() > 1) 
      //   lastCell = path[path.size()-2];

      for (int dir = 0; dir < 4; dir++) {
        if (noWallFromCellInDir(currentCell, dir)) {
          Cell nextCell;
          switch(dir) {
            case 0:
              nextCell = Cell(currentCell.row, currentCell.col+1); break;
            case 1:
              nextCell = Cell(currentCell.row-1, currentCell.col); break;
            case 2:
              nextCell = Cell(currentCell.row, currentCell.col-1); break;
            case 3:
              nextCell = Cell(currentCell.row+1, currentCell.col); break;
          }
          if (!isCellInPath(path, nextCell)) {
            newPaths.push_back(path);
            newPaths[newPaths.size()-1].push_back(nextCell); 
          }
        }
      } 
    }
    return newPaths;
  }

  bool doesAnyPathReachCell(const Paths paths, const Cell dest) {
    for (int pathIdx = 0; pathIdx != paths.size(); pathIdx++) {
      Path path = paths[pathIdx];
      Cell lastCell = path[path.size()-1];
      if ((lastCell.row == dest.row) && (lastCell.col == dest.col)) {
        return true;
      }
    }
    return false;
  }


  Paths getPathsReachCell(const Paths paths, const Cell dest) {
    Paths reachingPaths;
    for (int pathIdx = 0; pathIdx != paths.size(); pathIdx++) {
      Path path = paths[pathIdx];
      Cell lastCell = path[path.size()-1];
      if ((lastCell.row == dest.row) && (lastCell.col == dest.col)) {
        reachingPaths.push_back(path);
      }
    }
    return reachingPaths;
  }

  Paths findShortestPaths(const Cell endCell) {
    // Find shortest paths
    Cell startCell(robotRow, robotCol);
    Paths paths (1, Path(1, startCell));
    while (!doesAnyPathReachCell(paths, endCell)) {
      paths = expandPaths(paths);
    }
    paths = getPathsReachCell(paths, endCell);
    return paths;
  }

  std::vector<std::string> stringsFromPaths(Paths paths) {
    std::vector<std::string> pathStrings;
    for (int pathIdx = 0; pathIdx != paths.size(); pathIdx++) {
      Path path = paths[pathIdx];
      std::string str = std::to_string(robotRow) + std::to_string(robotCol) + robotCardinal;
      int lastDir = robotDir;
      Cell lastCell = path[0];
      for (int cellIdx = 1; cellIdx != path.size(); cellIdx++) {
        Cell currCell = path[cellIdx];
        int rowDiff = currCell.row - lastCell.row;
        int colDiff = currCell.col - lastCell.col;
        int newDir = 0;
        if (colDiff == 1) {
          newDir = 0;
        } else if (rowDiff == -1) {
          newDir = 1;
        } else if (colDiff == -1) {
          newDir = 2;
        } else if (rowDiff == 1) {
          newDir = 3;
        }
        int dirDiff = newDir - lastDir;
        dirDiff = (dirDiff + 8) % 4;
        if (dirDiff == 1) {
          str += "L";
        } else if (dirDiff == 2) {
          str += "LL";
        } else if (dirDiff == 3) {
          str += "R";
        }
        str += "F";
        lastDir = newDir;
        lastCell = currCell;
      }
      pathStrings.push_back(str);
    }
    return pathStrings;
  }

  int findShortestStringIdx(std::vector<std::string> strs) {
    int shortestIdx = -1;
    int shortestLength = 0;
    for (int strIdx = 0; strIdx != strs.size(); strIdx++) {
      int strLength = strs[strIdx].size();
      if ((shortestIdx == -1) || strLength < shortestLength) {
        shortestIdx = strIdx;
        shortestLength = strLength;
      }
    }
    return shortestIdx;
  }

} typedef Maze;

int main(int argc, char **argv) {
  // Read maze file
  Maze* maze = new Maze();
  // std::cout << "Start - Reading map:\n";
  std::cout << "--- Task 1 ---\n";
  std::ifstream ifs (MAP_FILE_NAME);
  std::string line;
  size_t lineIdx = 0;
  while (std::getline(ifs, line)) {
    std::cout << line << "\n";
   if (lineIdx > 0 && lineIdx < 2*rowCount) {
      for(int chIdx = 0; chIdx < line.size(); chIdx++) {
        char ch = line[chIdx];
        if (ch != ' ' && chIdx < 4*colCount) {
          // Not space
          if (lineIdx % 2 == 0) {
            // Horizontal wall line
            if (chIdx % 4 == 2) {
              // Horizontal wall spot
              maze->addHoriWall(lineIdx/2-1, (chIdx-2)/4);
            }
          } else {
            // Vertical wall line
            if (chIdx % 4 == 0 && chIdx > 0) {
              maze->addVertWall((lineIdx-1)/2, chIdx/4-1);
            } else if (chIdx % 4 == 2) {
              // Middle of cell
              maze->addRobot((lineIdx-1)/2, chIdx/4, ch);
            }
          }
        }
      }
    }
    lineIdx++;
  }
  // std::cout << "Done - Map read!\n";
  // maze->printMaze();

  // Find shortest paths
  std::cout << "--- Task 2 ---\n";
  // std::cout << "Start - Finding shortest paths:\n";
  Cell endCell (2,4);
  Paths paths = maze->findShortestPaths(endCell);
  maze->printPaths(paths);
  // std::cout << "Done - " << paths.size() << " shortest paths found!\n";

  // Find shortest paths with least turns
  std::cout << "--- Task 3 ---\n";
  // std::cout << "Start - Finding shortest path with least turns:\n";
  std::vector<std::string> strs = maze->stringsFromPaths(paths);
  int shortestIdx = maze->findShortestStringIdx(strs);
  // std::cout << "shortestIdx: " << shortestIdx << "\n";
  maze->printPath(paths[shortestIdx]);
  std::string shortestString = strs[shortestIdx];
  std::cout << "Steps: " << shortestString.size()-3 << "\n";
  std::cout << "Path: " << shortestString << "\n";
  // std::cout << "Done - Shortest path with least turns found!\n";

  std::cout << "--- Task 4 ---\n";
  std::cout << "File: ../../PathPlanFound.txt\n";
  std::cout << "Path: " << shortestString << "\n";
  std::ofstream myfile;
  myfile.open("../../PathPlanFound.txt");
  myfile << shortestString;
  myfile.close();

  delete maze;
  return 0;
}
