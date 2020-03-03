#pragma once

#include <iostream>
#include <ncurses.h>

class ConsoleDisplayer
{
public:
    ConsoleDisplayer();
    ~ConsoleDisplayer();
    void displayFullScreen(const std::string &string_to_display);
    void displayGameSection(const std::string &string_to_display);
    void displaySide1Section(const std::string &string_to_display);
    void displaySide2Section(const std::string &string_to_display);
    void addCharToGameSectionAt(int y, int x, char character);
    void removeCharFromGameSectionAt(int y, int x);
    void emptyFullScreen();

private:
    WINDOW* m_win_total;
    WINDOW* m_win_game;
    WINDOW* m_win_side1;
    WINDOW* m_win_side2;
};
