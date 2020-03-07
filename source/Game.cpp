#include "Game.hpp"


/**************************************************/
/*             constructor, destructor            */
/**************************************************/


Game::Game()
{
    const int numberOfStaff = 10;

    m_gameSetting = getStringFromFile("./source/assets/gameSetting.txt");

    m_gameMode = WALKING_PAUSED;
    m_gameAchievement = NO_ACHIEVEMENT;

    m_gameCrypto = GameCrypto();
    m_mainCharacter = GameObject(3, 0, 'O');

    // set staff
    m_randomEngine = std::default_random_engine(
        std::chrono::system_clock::now().time_since_epoch().count());
    for (int i = 0; i < numberOfStaff; ++i)
    {
        Position staffPosition;
        do {
            std::uniform_int_distribution<int> distribution(0, 39);
            staffPosition.y = (distribution(m_randomEngine) % 39);
            staffPosition.x = distribution(m_randomEngine);
        } while (isColliding(staffPosition));

        GameObject newStaffMember(staffPosition.y, staffPosition.x, '@');
        m_staffMembers.push_back(newStaffMember);
    }
}

Game::~Game()
{
}


/**************************************************/
/*       public methods: set/get game state       */
/**************************************************/


void Game::updateGameTogglePause()
{
    if (m_gameMode == WALKING)
        m_gameMode = WALKING_PAUSED;
    else if (m_gameMode == WALKING_PAUSED)
        m_gameMode = WALKING;
}

void Game::updateGameMovingTo(GameObjectMove move)
{
    Position oldPosition = m_mainCharacter.getPosition();
    Position newPosition;

    // wake up from pause mode
    if (m_gameMode == WALKING_PAUSED)
        m_gameMode = WALKING;

    if (m_gameMode != GAME_OVER)
    {
        switch(move)
        {
            case UP:
                newPosition.x = oldPosition.x;
                newPosition.y = oldPosition.y - 1;
                break;
            case RIGHT:
                newPosition.x = oldPosition.x + 1;
                newPosition.y = oldPosition.y;
                break;
            case DOWN:
                newPosition.x = oldPosition.x;
                newPosition.y = oldPosition.y + 1;
                break;
            case LEFT:
                newPosition.x = oldPosition.x - 1;
                newPosition.y = oldPosition.y;
                break;
        }

        if(!isColliding(newPosition))
        {
            m_mainCharacter.setPosition(newPosition);
            m_mainCharacter.setFormerPosition(oldPosition);
            updateGameModeAndAchievementAccPos(newPosition);
            if (isGameOver())
                m_gameMode = GAME_OVER;
        }
    }
}

void Game::updateGameMovingStaff()
{
    if (m_gameMode == WALKING)
    {
        std::uniform_int_distribution<GameObjectMove> distribution(UP, LEFT);

        for (int i = 0; i < m_staffMembers.size(); i++)
        {
            int randomNumber = distribution(m_randomEngine);
            Position oldPosition = m_staffMembers[i].getPosition();
            Position newPosition;

            switch(randomNumber)
            {
                case UP:
                    newPosition.x = oldPosition.x;
                    newPosition.y = oldPosition.y - 1;
                    break;
                case RIGHT:
                    newPosition.x = oldPosition.x + 1;
                    newPosition.y = oldPosition.y;
                    break;
                case DOWN:
                    newPosition.x = oldPosition.x;
                    newPosition.y = oldPosition.y + 1;
                    break;
                case LEFT:
                    newPosition.x = oldPosition.x - 1;
                    newPosition.y = oldPosition.y;
                    break;
            }

            if(!isColliding(newPosition))
            {
                m_staffMembers[i].setPosition(newPosition);
                m_staffMembers[i].setFormerPosition(oldPosition);
            }
        }
        if (isGameOver())
            m_gameMode = GAME_OVER;
    }
}

GameMode Game::getGameMode()
{
    return m_gameMode;
}

GameAchievement Game::getGameAchievement()
{
    return m_gameAchievement;
}


/**************************************************/
/*       public methods: crypto game              */
/**************************************************/


void Game::updateCryptoParameter(CryptoParameterUpdate update){
    if (m_gameMode == DESK && m_gameAchievement == NO_ACHIEVEMENT)
    {
        m_gameCrypto.updateParameter(update);
        if (m_gameCrypto.isSolved() && (m_gameAchievement == NO_ACHIEVEMENT))
            m_gameAchievement = KNOWS_CODE;
    }
}


/**************************************************/
/*     public methods: get info to feed display   */
/**************************************************/


std::vector<PositionedChar> Game::getPositionedCharsToAdd()
{
    std::vector<PositionedChar> positionedCharsToAdd;

    PositionedChar posChar;
    posChar.c = m_mainCharacter.getChar();
    posChar.y = m_mainCharacter.getPosition().y;
    posChar.x = m_mainCharacter.getPosition().x;

    positionedCharsToAdd.push_back(posChar);

    for (int i = 0; i < m_staffMembers.size(); i++)
    {
        PositionedChar posCharStaff;
        posCharStaff.c = m_staffMembers[i].getChar();
        posCharStaff.y = m_staffMembers[i].getPosition().y;
        posCharStaff.x = m_staffMembers[i].getPosition().x;
        positionedCharsToAdd.push_back(posCharStaff);
    }

    return positionedCharsToAdd;
}

std::vector<Position> Game::getPositionsToRemove()
{
    std::vector<Position> positionsToRemove;
    positionsToRemove.push_back(m_mainCharacter.getFormerPosition());

    for (int i = 0; i < m_staffMembers.size(); i++)
    {
        positionsToRemove.push_back(m_staffMembers[i].getFormerPosition());
    }

    return positionsToRemove;
}

std::string Game::getStringForSetting()
{
    return m_gameSetting;
}

std::string Game::getStringForSide1Section()
{
    if ((m_gameMode == WALKING) || (m_gameMode == WALKING_PAUSED))
        return getStringFromFile("./source/assets/side1_story.txt");
    else if (m_gameMode == DESK)
    {
        std::string stringComplete(30*14, ' ');
        std::string stringInfo;
        if (m_gameAchievement == NO_ACHIEVEMENT)
            stringInfo = getStringFromFile("./source/assets/side1_desk_open.txt");
        else
            stringInfo = getStringFromFile("./source/assets/side1_desk_closed.txt");
        std::string stringAlph = m_gameCrypto.getAlphabet();
        std::string stringAlphMapped = m_gameCrypto.getAlphabetMapped();
        std::string stringCypher = "CYPHR: " + m_gameCrypto.getCypherText();
        std::string stringCypherMapped = "DCRPT: " + m_gameCrypto.getCypherTextMapped();

        stringComplete.replace(0, stringInfo.length(), stringInfo);
        stringComplete.replace(10*30, stringAlph.length(), stringAlph);
        stringComplete.replace(11*30, stringAlphMapped.length(), stringAlphMapped);
        stringComplete.replace(12*30, stringCypher.length(), stringCypher);
        stringComplete.replace(13*30, stringCypherMapped.length(), stringCypherMapped);

        return stringComplete;
    }
    else if (m_gameMode == SAFE)
    {
        if (m_gameAchievement == KNOWS_CODE || m_gameAchievement == KNOWS_PASSWD)
            return getStringFromFile("./source/assets/side1_safe_closed.txt");
        else
            return getStringFromFile("./source/assets/side1_safe_open.txt");
    }
    else if (m_gameMode == TERMINAL || m_gameAchievement != KNOWS_PASSWD)
    {
        return getStringFromFile("./source/assets/side1_terminal_open.txt");
    }
    else
        return getStringFromFile("./source/assets/side1_story.txt");
}

std::string Game::getStringForSide2Section()
{
    std::string stringComplete = getStringFromFile("./source/assets/side2.txt");
    std::string stringToAdd = "";
    switch(m_gameAchievement){
        case NO_ACHIEVEMENT:
            stringToAdd = "Nothing yet.";
            break;
        case KNOWS_CODE:
            stringToAdd = "Code for the safe!";
            break;
        case KNOWS_PASSWD:
            stringToAdd = "Console password!";
            break;
        case LOGGED_IN_CONSOLE:
            stringToAdd = "You won!";
            break;
    }
    return stringComplete.replace(10, stringToAdd.length(), stringToAdd);;
}

std::string Game::getStringForFullScreen()
{
    if (m_gameMode == GAME_OVER && m_gameAchievement == LOGGED_IN_CONSOLE)
        return getStringFromFile("./source/assets/finishedScreen.txt");
    else if (m_gameMode == GAME_OVER)
        return getStringFromFile("./source/assets/gameOver.txt");
    else
        return getStringFromFile("./source/assets/startScreen.txt");
}



/**************************************************/
/*                private methods                 */
/**************************************************/


bool Game::isColliding(Position position)
{
    if (position.y < 0 || position.y > 19 || position.x < 0 || position.x > 39)
        return true;
    else if (m_gameSetting[position.y * 40 + position.x] != ' ')
        return true;
    else if (isCollidingWithStaff(position) || isCollidingWithMainCharacter(position))
        return true;
    return false;
}

bool Game::isCollidingWithStaff(Position position)
{
    for (int i = 0; i < m_staffMembers.size(); i++)
    {
        Position staffPosition = m_staffMembers[i].getPosition();
        if (staffPosition.y == position.y && staffPosition.x == position.x)
            return true;
    }
    return false;
}

bool Game::isCollidingWithMainCharacter(Position position)
{
    Position mainCharacterPos = m_mainCharacter.getPosition();
    if (mainCharacterPos.y == position.y && mainCharacterPos.x == position.x)
        return true;
    return false;
}

bool Game::isGameOver()
{
    Position staffPos;
    Position mainPos = m_mainCharacter.getPosition();

    for (int i = 0; i < m_staffMembers.size(); i++)
    {
        staffPos = m_staffMembers[i].getPosition();
        // if (haveContact(staffPos, mainPos))
        //     return true;
        if (staffPos.y == mainPos.y && (staffPos.x == mainPos.x+1 || staffPos.x == mainPos.x-1 ))
            return true;
        else if(staffPos.x == mainPos.x && (staffPos.y == mainPos.y+1 || staffPos.y == mainPos.y-1 ))
            return true;
    }
    return false;
}

bool Game::haveContact(Position pos1, Position pos2)
{
    if (pos1.y == pos2.y && (pos1.x == pos2.x+1 || pos1.x == pos2.x-1 ))
        return true;
    else if(pos1.x == pos2.x && (pos1.y == pos2.y+1 || pos1.y == pos2.y-1 ))
        return true;
    else
        return false;
}

std::string Game::getStringFromFile(const std::string fileName)
{
    std::string line;
    std::string complete;
    std::ifstream iffile;
    iffile.open(fileName);

    if (iffile.is_open())
        while (std::getline(iffile, line))
            complete += line;
    else
        std::cout << "could not open file" << std::endl;

    iffile.close();
    return complete;
}

void Game::updateGameModeAndAchievementAccPos(Position position)
{
    std::vector<char> charsAroundPosition;
    charsAroundPosition.push_back(m_gameSetting[position.y * 40 + position.x + 1]);
    charsAroundPosition.push_back(m_gameSetting[position.y * 40 + position.x - 1]);
    charsAroundPosition.push_back(m_gameSetting[(position.y - 1) * 40 + position.x]);
    charsAroundPosition.push_back(m_gameSetting[(position.y + 1) * 40 + position.x]);

    for (char character : charsAroundPosition)
    {
        if (character == 'D')
        {
            // logic to achieve KNOWS_CODE -> see method updateCryptoParameter
            m_gameMode = DESK;
            return;
        }
        else if (character == 'S')
        {
            if (m_gameAchievement == KNOWS_CODE)
                m_gameAchievement = KNOWS_PASSWD;
            m_gameMode = SAFE;
            return;
        }
        else if (character == 'T')
        {
            if (m_gameAchievement == KNOWS_PASSWD)
            {
                m_gameAchievement = LOGGED_IN_CONSOLE;
                m_gameMode = GAME_OVER;
                return;
            }
            m_gameMode = TERMINAL;
            return;
        }
    }
    m_gameMode = WALKING;
}
