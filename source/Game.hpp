#pragma once

#include <iostream>
#include <ncurses.h>
#include <vector>
#include <fstream>
#include <random>
#include <chrono>

#include "GameObject.hpp"
#include "GameCrypto.hpp"

struct PositionedCharacter
{
    char c;
    int x;
    int y;
};

enum GameMode
{
    WALKING = 0,
    WALKING_PAUSED = 1,
    DESK = 2,
    SAFE = 3,
    TERMINAL = 4,
    GAME_OVER = 5,
};

enum GameAchievement
{
    NO_ACHIEVEMENT = 0,
    KNOWS_CODE = 1,
    KNOWS_PASSWD = 2,
    LOGGED_IN_CONSOLE = 3,
};

class Game
{
public:

    Game();
    ~Game();

    void updateGameMovingLeft();
    void updateGameMovingRight();
    void updateGameMovingUp();
    void updateGameMovingDown();
    void updateGameMovingStaff();

    GameMode getGameMode();
    void setGameMode(GameMode gameMode);
    GameAchievement getGameAchievement();
    void setGameAchievement(GameAchievement gameAchievement);

    std::string getStringForSetting();
    std::string getStringForSide1Section();
    std::string getStringForSide2Section();
    std::string getStringForFullScreen();
    std::vector<PositionedCharacter> getPositionedCharactersToAdd();
    std::vector<Position> getPositionsToRemove();

    void increaseCryptoParameter();
    void decreaseCryptoParameter();

private:

    std::string m_gameSetting;
    GameObject m_mainCharacter;
    std::vector<GameObject> m_staffMembers;
    std::vector<Position> m_positionsToRemove;
    std::vector<PositionedCharacter> m_positionedCharactersToAdd;
    std::default_random_engine m_randomEngine;
    GameMode m_gameMode;
    GameAchievement m_gameAchievement;
    GameCrypto m_gameCrypto;

    bool isColliding(Position position);
    bool isCollidingWithStaff(Position position);
    bool isCollidingWithMainCharacter(Position position);
    bool isGameOver();
    bool haveContact(Position pos1, Position pos2);
    std::string getStringFromFile(const std::string fileName);
    GameMode gameModeAccordingPosition(Position position);
};
