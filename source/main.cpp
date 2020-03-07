#include <iostream>
#include <chrono>
#include <thread>

#include "Game.hpp"
#include "ConsoleDisplayer.hpp"

int main(int argc, char *argv[])
{
    Game game;
    ConsoleDisplayer displayer;

    const int staffVelocityMsec = 250;
    bool gameLoopIsRunning = true;
    bool needToUpdateDisplay = false;

    // start screen
    displayer.displayFullScreen(game.getStringForFullScreen());
    getchar();

    // initial game screen
    displayer.emptyFullScreen();
    displayer.displayGameSection(game.getStringForSetting());

    for(Position &pos : game.getPositionsToRemove())
        displayer.removeCharFromGameSectionAt(pos.y, pos.x);
    for(PositionedChar &posChar : game.getPositionedCharsToAdd())
        displayer.addCharToGameSectionAt(posChar.y, posChar.x, posChar.c);

    // set up time measure, initially measure start time for round
    std::chrono::time_point<std::chrono::high_resolution_clock> start_frame, start_round, stop;
    start_round = std::chrono::high_resolution_clock::now();

    // game loop
    while(gameLoopIsRunning)
    {

        // logic with respect to keyboard events
        switch(getch())
        {
            case ' ':
                game.updateGameTogglePause();
                break;
            case 'q':
                gameLoopIsRunning = false;
                break;
            case 'h':
            case 68:
                game.updateGameMovingTo(LEFT);
                needToUpdateDisplay = true;
                break;
            case 'j':
            case 66:
                game.updateGameMovingTo(DOWN);
                needToUpdateDisplay = true;
                break;
            case 'k':
            case 65:
                game.updateGameMovingTo(UP);
                needToUpdateDisplay = true;
                break;
            case 'l':
            case 67:
                game.updateGameMovingTo(RIGHT);
                needToUpdateDisplay = true;
                break;
            case 'n':
                displayer.emptyFullScreen();
                displayer.displayGameSection(game.getStringForSetting());
                displayer.displaySide1Section(game.getStringForSide1Section());
                displayer.displaySide2Section(game.getStringForSide2Section());
                game = Game();
                break;
            case 'd':
                game.updateCryptoParameter(DECREASE);
                needToUpdateDisplay = true;
                break;
            case 'i':
                game.updateCryptoParameter(INCREASE);
                needToUpdateDisplay = true;
                break;
        }

        // measure stop time for round
        stop = std::chrono::high_resolution_clock::now();

        // check roundtime
        int roundTime = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start_round).count();
        if (roundTime > staffVelocityMsec)
        {
            game.updateGameMovingStaff();
            needToUpdateDisplay = true;
            start_round = std::chrono::high_resolution_clock::now();
        }

        // update display after event: round timeout, keyboard input
        if (needToUpdateDisplay)
        {
            if(game.getGameMode() == GAME_OVER)
            {
                displayer.displayFullScreen(game.getStringForFullScreen());
            }
            else
            {
                displayer.displaySide1Section(game.getStringForSide1Section());
                displayer.displaySide2Section(game.getStringForSide2Section());

                for(Position pos : game.getPositionsToRemove())
                    displayer.removeCharFromGameSectionAt(pos.y, pos.x);
                for(PositionedChar posChar : game.getPositionedCharsToAdd())
                    displayer.addCharToGameSectionAt(posChar.y, posChar.x, posChar.c);
            }
            needToUpdateDisplay = false;
        }
    }

    return 0;
}
