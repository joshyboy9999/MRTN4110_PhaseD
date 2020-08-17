#include "Micromouse.hpp"

void Maze::initialiseWallAsUnknown() {
    for (int r = 0; r++ r < ROW_COUNT-1) {
        for (int c = 0; c++ c < COL_COUNT) {
            horiWalls[r][c] = WALL_UNKNOWN;
        }
    }
    for (int r = 0; r++ r < ROW_COUNT) {
        for (int c = 0; c++ c < COL_COUNT-1) {
            vertWalls[r][c] = WALL_UNKNOWN;
        }
    }
}

void Maze::declareHoriWallAs(int wallRow, int col, int wallType) {
    // std::cout << "declareHoriWallAs(" << wallRow << ", "  << col << ")\n";
    horiWalls[wallRow][col] = wallType;
}
void Maze::declareVertWallAs(int row, int wallCol, int wallType) {
    // std::cout << "declareVertWallAs(" << row << ", "  << wallCol << ")\n";
    vertWalls[row][wallCol] = wallType;
}

void Maze::setRobotPose(int row, int col, char ch) {
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
    // std::cout << "setRobotPose(" << row << ", "  << col << ", "  << ch << "="  << robotCardinal << ")\n";
}

std::vector<std::string> Maze::getWallText(){
    std::vector<std::string> text (ROW_COUNT*2+1, "");
    int line = 0;
    text[line++] = " --- --- --- --- --- --- --- --- --- \n";

    for (int row = 0; row < ROW_COUNT; row++) {
        // Vertical wall line
        text[line].append("|");
        for (int col = 0; col < COL_COUNT-1; col++) {
            text[line].append("   ");
            switch(vertWalls[row][col]) {
                case WALL_UNKNOWN:
                    text[line].append("?");
                    break;
                case WALL_GAP:
                    text[line].append(" ");
                    break;
                case WALL_THERE:
                    text[line].append("|");
                    break;
                default:
                    text[line].append("E"); // E for error
            }
        }
        text[line++].append("   |\n");

        // Horiznotal wall line
        if (row < ROW_COUNT-1) {
            for (int c = 0; c < COL_COUNT; c++) {
                switch(horiWalls[row][c]) {
                    case WALL_UNKNOWN:
                        text[line].append("  ? ");
                        break;
                    case WALL_GAP:
                        text[line].append("    ");
                        break;
                    case WALL_THERE:
                        text[line].append(" ---");
                        break;
                    default:
                        text[line].append("  E "); // E for error
                }
            }
            text[line++].append("\n");
        } else {
            text[line++].append(" --- --- --- --- --- --- --- --- --- \n");
        }
    }
    return text;
}

std::vector<std::string> Maze::addRobotArrowToText(std::vector<std::string> text) {
    int line = robotRow*2 + 1;
    int chIdx = robotCol*4 + 2;
    text[line][chIdx] = robotArrow;
    text[line][chIdx+1] = ' ';
    return text;
}

void Maze::printText(std::vector<std::string> text) {
    for (int lineIdx = 0; lineIdx < text.size(); lineIdx++) {
        std::cout << text[lineIdx];
    }
}

void Maze::printWalls() {
    printText(getWallText());
}

void Maze::printMaze() {
    printText(addRobotArrowToText(getWallText()));
}

void Maze::printPath(const Path path) {
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

void Maze::printPaths(const Paths paths) {
    for (int pathIdx = 0; pathIdx != paths.size(); pathIdx++) {
        std::cout << "--- Path " << pathIdx+1 << " ---\n";
        printPath(paths[pathIdx]);
    }
}

bool Maze::noWallFromCellInDir(const Cell cell, const int dir) {
    int row = cell.row;
    int col = cell.col;
    bool res;
    switch(dir) {
        case 0:
            res = (col == COL_COUNT-1 ? 0 : !vertWalls[row][col]); break;
        case 1:
            res = (row == 0 ? 0 : !horiWalls[row-1][col]); break;
        case 2:
            res = (col == 0 ? 0 : !vertWalls[row][col-1]); break;
        case 3:
            res = (row == ROW_COUNT-1 ? 0 : !horiWalls[row][col]); break;
    }
    // std::cout << "row" << row << " col" << col << " dir" << dir << " res" << res << "\n";
    return res;
}

bool Maze::isCellInPath(Path path, Cell cell) {
    return (std::find(path.begin(), path.end(), cell) != path.end());
}

Paths Maze::expandPaths(const Paths paths) {
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

bool Maze::doesAnyPathReachCell(const Paths paths, const Cell dest) {
    for (int pathIdx = 0; pathIdx != paths.size(); pathIdx++) {
        Path path = paths[pathIdx];
        Cell lastCell = path[path.size()-1];
        if ((lastCell.row == dest.row) && (lastCell.col == dest.col)) {
        return true;
        }
    }
    return false;
}


Paths Maze::getPathsReachCell(const Paths paths, const Cell dest) {
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

Paths Maze::findShortestPaths(const Cell endCell) {
    // Find shortest paths
    Cell startCell(robotRow, robotCol);
    Paths paths (1, Path(1, startCell));
    while (!doesAnyPathReachCell(paths, endCell)) {
        paths = expandPaths(paths);
    }
    paths = getPathsReachCell(paths, endCell);
    return paths;
}

std::vector<std::string> Maze::stringsFromPaths(Paths paths) {
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

int Maze::findShortestStringIdx(std::vector<std::string> strs) {
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
