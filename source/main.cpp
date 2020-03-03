#include <iostream>
#include <chrono>
#include <thread>

#include "Game.hpp"
#include "ConsoleDisplayer.hpp"

int main(int argc, char *argv[])
{

    const int framesPerSecond = 60;
    const int minimalDelayMsec = 1000 / framesPerSecond;
    const int staffVelocityMsec = 250;
    bool gameLoopIsRunning = true;
    bool needToUpdateDisplay = false;
    Game game;
    ConsoleDisplayer displayer;

    displayer.displayFullScreen(game.getStringForFullScreen());
    getchar();

    // initial display
    displayer.emptyFullScreen();
    displayer.displayGameSection(game.getStringForSetting());
    displayer.displaySide1Section(game.getStringForSide1Section());
    displayer.displaySide2Section(game.getStringForSide2Section());

    for(Position &pos : game.getPositionsToRemove())
        displayer.removeCharFromGameSectionAt(pos.y, pos.x);
    for(PositionedCharacter &posChar : game.getPositionedCharactersToAdd())
        displayer.addCharToGameSectionAt(posChar.y, posChar.x, posChar.c);

    // set up time measure, initially measure start time for round
    std::chrono::time_point<std::chrono::high_resolution_clock> start_frame, start_round, stop;
    start_round = std::chrono::high_resolution_clock::now();

    // game loop
    while(gameLoopIsRunning)
    {
        // reset update variable for this loop
        needToUpdateDisplay = false;

        // measure start time for frame
        start_frame = std::chrono::high_resolution_clock::now();

        // logic for keyboard events
        switch(getch())
        {
            case ' ':
                if (game.getGameMode() == WALKING)
                    game.setGameMode(WALKING_PAUSED);
                else if (game.getGameMode() == WALKING_PAUSED)
                    game.setGameMode(WALKING);
                break;
            case 'q':
                gameLoopIsRunning = false;
                break;
            case 'h':
            case 68:
                if (game.getGameMode() == WALKING_PAUSED)
                    game.setGameMode(WALKING);
                if (game.getGameMode() != GAME_OVER)
                {
                    game.updateGameMovingLeft();
                    needToUpdateDisplay = true;
                }
                break;
            case 'j':
            case 66:
                if (game.getGameMode() == WALKING_PAUSED)
                    game.setGameMode(WALKING);
                if (game.getGameMode() != GAME_OVER)
                {
                    game.updateGameMovingDown();
                    needToUpdateDisplay = true;
                }
                break;
            case 'k':
            case 65:
                if (game.getGameMode() == WALKING_PAUSED)
                    game.setGameMode(WALKING);
                if (game.getGameMode() != GAME_OVER)
                {
                    game.updateGameMovingUp();
                    needToUpdateDisplay = true;
                }
                break;
            case 'l':
            case 67:
                if (game.getGameMode() == WALKING_PAUSED)
                    game.setGameMode(WALKING);
                if (game.getGameMode() != GAME_OVER)
                {
                    game.updateGameMovingRight();
                    needToUpdateDisplay = true;
                }
                break;
            case 'n':
                // if (game.getGameMode() == GAME_OVER || game.getGameAchievement() == LOGGED_IN_CONSOLE)
                // {
                    displayer.emptyFullScreen();
                    displayer.displayGameSection(game.getStringForSetting());
                    displayer.displaySide1Section(game.getStringForSide1Section());
                    displayer.displaySide2Section(game.getStringForSide2Section());
                    game = Game();
                // }
                break;
            case 'd':
                if (game.getGameMode() == DESK)
                {
                    game.decreaseCryptoParameter();
                    needToUpdateDisplay = true;
                }
                break;
            case 'i':
                if (game.getGameMode() == DESK)
                {
                    game.increaseCryptoParameter();
                    needToUpdateDisplay = true;
                }
                break;
        }

        // measure stop time for frame and round
        stop = std::chrono::high_resolution_clock::now();

        // check roundtime
        int roundTime = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start_round).count();
        if (roundTime > staffVelocityMsec)
        {
            if (game.getGameMode() == WALKING)
                game.updateGameMovingStaff();
            needToUpdateDisplay = true;
            start_round = std::chrono::high_resolution_clock::now();
        }

        // update display after event: round timeout, keyboard input
        if (needToUpdateDisplay)
        {
            displayer.displaySide1Section(game.getStringForSide1Section());
            displayer.displaySide2Section(game.getStringForSide2Section());

            for(Position pos : game.getPositionsToRemove())
                displayer.removeCharFromGameSectionAt(pos.y, pos.x);
            for(PositionedCharacter posChar : game.getPositionedCharactersToAdd())
                displayer.addCharToGameSectionAt(posChar.y, posChar.x, posChar.c);

            if(game.getGameMode() == GAME_OVER)
            {
                if(game.getGameAchievement() == LOGGED_IN_CONSOLE)
                    displayer.displayFullScreen(game.getStringForFullScreen());
                else
                    displayer.displayFullScreen(game.getStringForFullScreen());
            }
        }

        // check frame time and fill up to minimal frame time to get smooth motion
        int elapsedMsec = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start_frame).count();
        if (elapsedMsec < minimalDelayMsec)
        {
            std::this_thread::sleep_for(stop - start_frame);
        }
    }

    return 0;
}
