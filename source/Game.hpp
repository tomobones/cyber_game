#pragma once

#include <iostream>
#include <ncurses.h>
#include <vector>
#include <fstream>
#include <random>
#include <chrono>

#include "GameObject.hpp"
#include "GameCrypto.hpp"

struct PositionedChar
{
    char c;
    int x;
    int y;
};

enum GameObjectMove
{
    UP = 0,
    RIGHT = 1,
    DOWN = 2,
    LEFT = 3,
};

// game can be in severyl modes
enum GameMode
{
    WALKING = 0,
    WALKING_PAUSED = 1,
    DESK = 2,
    SAFE = 3,
    TERMINAL = 4,
    GAME_OVER = 5,
};

// game story is linear - has several "levels"
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

    void updateGameTogglePause();
    void updateGameMovingTo(GameObjectMove move);
    void updateGameMovingStaff();

    GameMode getGameMode();
    GameAchievement getGameAchievement();

    std::string getStringForSetting();
    std::string getStringForSide1Section();
    std::string getStringForSide2Section();
    std::string getStringForFullScreen();
    std::vector<PositionedChar> getPositionedCharsToAdd();
    std::vector<Position> getPositionsToRemove();

    void updateCryptoParameter(CryptoParameterUpdate update);

private:

    std::string m_gameSetting;

    GameObject m_mainCharacter;
    GameCrypto m_gameCrypto;
    std::vector<GameObject> m_staffMembers;

    std::vector<Position> m_positionsToRemove;
    std::vector<PositionedChar> m_positionedCharsToAdd;
    std::default_random_engine m_randomEngine;

    GameMode m_gameMode;
    GameAchievement m_gameAchievement;



    bool isColliding(Position position);
    bool isCollidingWithStaff(Position position);
    bool isCollidingWithMainCharacter(Position position);
    bool isGameOver();
    bool haveContact(Position pos1, Position pos2);
    std::string getStringFromFile(const std::string fileName);
    GameMode gameModeAccordingPosition(Position position);
};
