#include "ConsoleDisplayer.hpp"


/**************************************************/
/*             constructor, destructor            */
/**************************************************/

const int win_total_h = 20;
const int win_total_w = 72;

const int win_game_h = 20;
const int win_game_w = 40;

const int win_side1_h = 14;
const int win_side1_w = 30;

const int win_side2_h = 5;
const int win_side2_w = 30;

ConsoleDisplayer::ConsoleDisplayer()
{
    // ncurses methods
    initscr();
    noecho();
    cbreak();
    timeout(250);
    curs_set(0);
    refresh();

    // create windows
    int height, width;
    getmaxyx(stdscr, height, width);

    m_win_game = newwin(win_game_h, win_game_w, (height - win_total_h)/2, (width - win_total_w)/2);
    m_win_side1 = newwin(win_side1_h, win_side1_w, (height - win_total_h)/2, (width - win_total_w)/2 + win_game_w + 2);
    m_win_side2 = newwin(win_side2_h, win_side2_w, (height - win_total_h)/2 + win_side1_h + 1, (width - win_total_w)/2 + win_game_w + 2);
    m_win_total = newwin(win_total_h, win_total_w, (height - win_total_h)/2, (width - win_total_w)/2);

}

ConsoleDisplayer::~ConsoleDisplayer()
{
    endwin();
}


/**************************************************/
/*                  public methods                */
/**************************************************/

void ConsoleDisplayer::displayFullScreen(const std::string &string_to_display)
{

    mvwprintw(m_win_total, 0, 0, string_to_display.c_str());
    wrefresh(m_win_total);
}

void ConsoleDisplayer::displayGameSection(const std::string &string_to_display)
{
    mvwprintw(m_win_game, 0, 0, string_to_display.c_str());
    wrefresh(m_win_game);
}

void ConsoleDisplayer::addCharToGameSectionAt(int y, int x, char character)
{
    mvwaddch(m_win_game, y, x, character);
    wrefresh(m_win_game);
}

void ConsoleDisplayer::removeCharFromGameSectionAt(int y, int x)
{
    mvwaddch(m_win_game, y, x, ' ');
    wrefresh(m_win_game);
}

void ConsoleDisplayer::displaySide1Section(const std::string &string_to_display)
{
    std::string s(14*30 - string_to_display.length(), ' ');
    mvwprintw(m_win_side1, 0, 0, (string_to_display  + s).c_str());
    wrefresh(m_win_side1);
}

void ConsoleDisplayer::displaySide2Section(const std::string &string_to_display)
{
    std::string s(5*30 - string_to_display.length(), ' ');
    mvwprintw(m_win_side2, 0, 0, (string_to_display  + s).c_str());
    wrefresh(m_win_side2);
}

void ConsoleDisplayer::emptyFullScreen()
{
    std::string s(1440, ' ');
    mvwprintw(m_win_total, 0, 0, s.c_str());
    wrefresh(m_win_total);
}
