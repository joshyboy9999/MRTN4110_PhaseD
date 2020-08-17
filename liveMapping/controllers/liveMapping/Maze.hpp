#ifndef MAZE_HPP
#define MAZE_HPP

#define ROW_COUNT 5
#define COL_COUNT 9
#define WALL_UNKNOWN -1
#define WALL_GAP 0
#define WALL_THERE 1

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
  int horiWalls[ROW_COUNT-1][COL_COUNT] = {{0}};
  int vertWalls[ROW_COUNT][COL_COUNT-1] = {{0}};
  int robotCol;
  int robotRow;
  int robotDir;
  char robotCardinal;
  char robotArrow;
  void initialiseWallAsUnknown();
  void declareHoriWallAs(int wallRow, int col, int wallType);
  void declareVertWallAs(int row, int wallCol, int wallType));
  void setRobotPose(int row, int col, char ch);
  std::vector<std::string> getWallText();
  std::vector<std::string> addRobotArrowToText(std::vector<std::string> text);
  void printText(std::vector<std::string> text);
  void printWalls();
  void printMaze();
  void printPath(const Path path);
  void printPaths(const Paths paths);
  bool noWallFromCellInDir(const Cell cell, const int dir);
  bool isCellInPath(Path path, Cell cell);
  Paths expandPaths(const Paths paths);
  bool doesAnyPathReachCell(const Paths paths, const Cell dest);
  Paths getPathsReachCell(const Paths paths, const Cell dest);
  Paths findShortestPaths(const Cell endCell);
  std::vector<std::string> stringsFromPaths(Paths paths);
  int findShortestStringIdx(std::vector<std::string> strs);

} typedef Maze;

#endif // MAZE_HPP