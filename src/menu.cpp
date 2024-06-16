#include <ncurses.h>
#include <fstream>
#include "game.cpp"

void gameLoop();
void loadGame();

void menu() {
    clear();
    mvprintw(0, 2, " ██████╗  ██████╗ ██╗  ██╗ █████╗");
    mvprintw(1, 2, " ╚════██╗██╔═████╗██║  ██║██╔══██╗");
    mvprintw(2, 2, "  █████╔╝██║██╔██║███████║╚█████╔╝");
    mvprintw(3, 2, " ██╔═══╝ ████╔╝██║╚════██║██╔══██╗");
    mvprintw(4, 2, " ███████╗╚██████╔╝     ██║╚█████╔╝");
    mvprintw(5, 2, " ╚══════╝ ╚═════╝      ╚═╝ ╚════╝");

    mvprintw(7, 2, "1. Start Game");
    mvprintw(8, 2, "2. Load Last Game");
    mvprintw(9, 2, "3. Quit");
    refresh();
    int ch = getch();
    switch (ch) {
        case '1':
            initGame();
            gameLoop();
            break;
        case '2':
            loadGame();
            gameLoop();
            break;
        case '3':
            return;
    }
}

void gameLoop() {
    while (true) {
        printGrid();
        int ch = getch();
        bool moved = false;
        switch (ch) {
            case 'a':
            case KEY_LEFT:
                moved = moveLeft();
                break;
            case 'd':
            case KEY_RIGHT:
                moved = moveRight();
                break;
            case 'w':
            case KEY_UP:
                moved = moveUp();
                break;
            case 's':
            case KEY_DOWN:
                moved = moveDown();
                break;
            case 'q':
                saveGame();
                menu();
                return;
        }
        if (moved) {
            ++moves;
            generateNewTile();
        }
    }
}

void loadGame() {
    std::ifstream inFile(saveFile);
    if (inFile.is_open()) {
        inFile >> score >> bestScore >> moves;
        for (int i = 0; i < GRID_SIZE; ++i) {
            for (int j = 0; j < GRID_SIZE; ++j) {
                inFile >> grid[i][j];
            }
        }
        inFile.close();
    }
}
