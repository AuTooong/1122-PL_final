#include <ncurses.h>
#include "menu.cpp"

int main() {
    std::setlocale(LC_ALL, ""); // set locale to support unicode characters
                                
    // init ncurses
    initscr();           // init ncurses
    noecho();            // do not echo input
    curs_set(FALSE);     // do not display cursor
    keypad(stdscr, TRUE);// enable keypad

    // start game
    menu();

    endwin();            // end ncurses
    return 0;
}
