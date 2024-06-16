#include <ncurses.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <fstream>
#include <algorithm>

// init game grid size
const int GRID_SIZE = 4;
int grid[GRID_SIZE][GRID_SIZE];

int score = 0;
int bestScore = 0;
int moves = 0;

int initX = 7;
int initY = 4;

const char *saveFile = "2048_save";

void initGame() {
    std::srand(std::time(nullptr));
    score = 0;
    moves = 0;
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            grid[i][j] = 0;
        }
    }
    // generate two random num in grid
    for (int i = 0; i < 2; ++i) {
        int x = std::rand() % GRID_SIZE;
        int y = std::rand() % GRID_SIZE;
        while (grid[x][y] != 0) {
            x = std::rand() % GRID_SIZE;
            y = std::rand() % GRID_SIZE;
        }
        grid[x][y] = (std::rand() % 2 + 1) * 2;
    }
}

void saveGame() {
    std::ofstream outFile(saveFile);
    if (outFile.is_open()) {
        outFile << score << ' ' << bestScore << ' ' << moves << '\n';
        for (int i = 0; i < GRID_SIZE; ++i) {
            for (int j = 0; j < GRID_SIZE; ++j) {
                outFile << grid[i][j] << ' ';
            }
            outFile << '\n';
        }
        outFile.close();
        mvprintw(initX + GRID_SIZE * 2 + 7, 0, "Game saved!");
    } else {
        mvprintw(initX + GRID_SIZE * 2 + 7, 0, "Failed to save the game.");
    }
    refresh();
}

void printGrid() {
    clear();


    mvprintw(0, 2, " ██████╗  ██████╗ ██╗  ██╗ █████╗");
    mvprintw(1, 2, " ╚════██╗██╔═████╗██║  ██║██╔══██╗");
    mvprintw(2, 2, "  █████╔╝██║██╔██║███████║╚█████╔╝");
    mvprintw(3, 2, " ██╔═══╝ ████╔╝██║╚════██║██╔══██╗");
    mvprintw(4, 2, " ███████╗╚██████╔╝     ██║╚█████╔╝");
    mvprintw(5, 2, " ╚══════╝ ╚═════╝      ╚═╝ ╚════╝");

    mvprintw(initX+0, initY, "┌───────────────────────────┐");
    mvprintw(initX+1, initY, "│ SCORE: %18d │", score);
    mvprintw(initX+2, initY, "│ BEST SCORE: %13d │", bestScore);    mvprintw(initX+3, initY, "│ MOVES: %18d │", moves);
    mvprintw(initX+4, initY, "└───────────────────────────┘");

    mvprintw(initX+6, initY, "┌──────┬──────┬──────┬──────┐");
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            if (grid[i][j] == 0) {
                mvprintw(initX + 7 + i * 2, initY + 1 + j * 7, "      ");
            } else {
                mvprintw(initX + 7 + i * 2, initY + 1 + j * 7, "%4d", grid[i][j]);
            }
            if (j < GRID_SIZE) {
                mvprintw(initX + 7 + i * 2, initY + 7 + j * 7, "│");
            }
        }
        mvprintw(initX + 7 + i * 2, initY, "│");
        if (i < GRID_SIZE - 1) {
            mvprintw(initX + 8 + i * 2, initY, "├──────┼──────┼──────┼──────┤");
        }
    }
    mvprintw(initX + 14, initY, "└──────┴──────┴──────┴──────┘");

    mvprintw(initX + 17, 2, "W or ↑ => Up");
    mvprintw(initX + 18, 2, "A or ← => Left");
    mvprintw(initX + 19, 2, "S or ↓ => Down");
    mvprintw(initX + 20, 2, "D or → => Right");
    mvprintw(initX + 21, 2, "q => Save and Quit");
    refresh();
}

bool combineRow(int row[GRID_SIZE]) {
    bool moved = false;
    for (int i = 0; i < GRID_SIZE - 1; ++i) {
        if (row[i] == row[i + 1] && row[i] != 0) {
            row[i] *= 2;
            row[i + 1] = 0;
            score += row[i];
            if (score > bestScore) {
                bestScore = score;
            }
            moved = true;
        }
    }
    return moved;
}

bool moveRow(int row[GRID_SIZE]) {
    bool moved = false;
    for (int i = 0; i < GRID_SIZE - 1; ++i) {
        if (row[i] == 0 && row[i + 1] != 0) {
            row[i] = row[i + 1];
            row[i + 1] = 0;
            moved = true;
        }
    }
    return moved;
}

bool moveLeft() {
    bool moved = false;
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE - 1; ++j) {
            moved |= moveRow(grid[i]);
        }
        moved |= combineRow(grid[i]);
        for (int j = 0; j < GRID_SIZE - 1; ++j) {
            moved |= moveRow(grid[i]);
        }
    }
    return moved;
}

bool moveRight() {
    bool moved = false;
    for (int i = 0; i < GRID_SIZE; ++i) {
        std::reverse(grid[i], grid[i] + GRID_SIZE);
        for (int j = 0; j < GRID_SIZE - 1; ++j) {
            moved |= moveRow(grid[i]);
        }
        moved |= combineRow(grid[i]);
        for (int j = 0; j < GRID_SIZE - 1; ++j) {
            moved |= moveRow(grid[i]);
        }
        std::reverse(grid[i], grid[i] + GRID_SIZE);
    }
    return moved;
}

bool moveUp() {
    bool moved = false;
    for (int j = 0; j < GRID_SIZE; ++j) {
        int column[GRID_SIZE];
        for (int i = 0; i < GRID_SIZE; ++i) {
            column[i] = grid[i][j];
        }
        for (int i = 0; i < GRID_SIZE - 1; ++i) {
            moved |= moveRow(column);
        }
        moved |= combineRow(column);
        for (int i = 0; i < GRID_SIZE - 1; ++i) {
            moved |= moveRow(column);
        }
        for (int i = 0; i < GRID_SIZE; ++i) {
            grid[i][j] = column[i];
        }
    }
    return moved;
}

bool moveDown() {
    bool moved = false;
    for (int j = 0; j < GRID_SIZE; ++j) {
        int column[GRID_SIZE];
        for (int i = 0; i < GRID_SIZE; ++i) {
            column[i] = grid[i][j];
        }
        std::reverse(column, column + GRID_SIZE);
        for (int i = 0; i < GRID_SIZE - 1; ++i) {
            moved |= moveRow(column);
        }
        moved |= combineRow(column);
        for (int i = 0; i < GRID_SIZE - 1; ++i) {
            moved |= moveRow(column);
        }
        std::reverse(column, column + GRID_SIZE);
        for (int i = 0; i < GRID_SIZE; ++i) {
            grid[i][j] = column[i];
        }
    }
    return moved;
}

void generateNewTile() {
    std::vector<std::pair<int, int>> emptyTiles;
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            if (grid[i][j] == 0) {
                emptyTiles.emplace_back(i, j);
            }
        }
    }
    if (!emptyTiles.empty()) {
        int index = std::rand() % emptyTiles.size();
        grid[emptyTiles[index].first][emptyTiles[index].second] = (std::rand() % 2 + 1) * 2;
    }
}
