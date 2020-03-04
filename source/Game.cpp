#include "Game.hpp"


#define NUMBER_STAFF 10


/**************************************************/
/*             constructor, destructor            */
/**************************************************/


Game::Game()
{
    m_gameSetting = getStringFromFile("./source/assets/gameSetting.txt");
    m_gameMode = WALKING_PAUSED;
    m_gameAchievement = NO_ACHIEVEMENT;
    m_gameCrypto = GameCrypto();
    m_mainCharacter = GameObject(3, 0, 'O');
    m_randomEngine = std::default_random_engine(
        std::chrono::system_clock::now().time_since_epoch().count());

    for (int i = 0; i < NUMBER_STAFF; ++i)
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


void Game::updateGameMovingLeft()
{
    Position oldPosition = m_mainCharacter.getPosition();
    Position newPosition;
    newPosition.x = oldPosition.x - 1;
    newPosition.y = oldPosition.y;
    if(!isColliding(newPosition))
    {
        m_mainCharacter.setPosition(newPosition);
        m_mainCharacter.setFormerPosition(oldPosition);
        m_gameMode = gameModeAccordingPosition(newPosition);
        if (isGameOver())
            m_gameMode = GAME_OVER;
    }
}

void Game::updateGameMovingRight()
{
    Position oldPosition = m_mainCharacter.getPosition();
    Position newPosition;
    newPosition.x = oldPosition.x + 1;
    newPosition.y = oldPosition.y;
    if(!isColliding(newPosition))
    {
        m_mainCharacter.setPosition(newPosition);
        m_mainCharacter.setFormerPosition(oldPosition);
        m_gameMode = gameModeAccordingPosition(newPosition);
        if (isGameOver())
            m_gameMode = GAME_OVER;
    }
}

void Game::updateGameMovingUp()
{
    Position oldPosition = m_mainCharacter.getPosition();
    Position newPosition;
    newPosition.x = oldPosition.x;
    newPosition.y = oldPosition.y - 1;
    if(!isColliding(newPosition))
    {
        m_mainCharacter.setPosition(newPosition);
        m_mainCharacter.setFormerPosition(oldPosition);
        m_gameMode = gameModeAccordingPosition(newPosition);
        if (isGameOver())
            m_gameMode = GAME_OVER;
    }
}

void Game::updateGameMovingDown()
{
    Position oldPosition = m_mainCharacter.getPosition();
    Position newPosition;
    newPosition.x = oldPosition.x;
    newPosition.y = oldPosition.y + 1;
    if(!isColliding(newPosition))
    {
        m_mainCharacter.setPosition(newPosition);
        m_mainCharacter.setFormerPosition(oldPosition);
        m_gameMode = gameModeAccordingPosition(newPosition);
        if (isGameOver())
            m_gameMode = GAME_OVER;
    }
}

void Game::updateGameMovingStaff()
{
    std::uniform_int_distribution<int> distribution(0, 3);

    for (int i = 0; i < m_staffMembers.size(); i++)
    {
        int randomNumber = distribution(m_randomEngine);
        Position oldPosition = m_staffMembers[i].getPosition();
        Position newPosition;

        switch(randomNumber)
        {
            case 0:
                newPosition.x = oldPosition.x;
                newPosition.y = oldPosition.y + 1;
                break;
            case 1:
                newPosition.x = oldPosition.x;
                newPosition.y = oldPosition.y - 1;
                break;
            case 2:
                newPosition.x = oldPosition.x + 1;
                newPosition.y = oldPosition.y;
                break;
            case 3:
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

GameMode Game::getGameMode()
{
    return m_gameMode;
}

void Game::setGameMode(GameMode gameMode)
{
    m_gameMode = gameMode;
}

GameAchievement Game::getGameAchievement()
{
    return m_gameAchievement;
}

void Game::setGameAchievement(GameAchievement gameAchievement)
{
    m_gameAchievement = gameAchievement;
}


/**************************************************/
/*     public methods: get info to feed display   */
/**************************************************/


std::vector<PositionedCharacter> Game::getPositionedCharactersToAdd()
{
    m_positionedCharactersToAdd.clear();

    PositionedCharacter posChar;
    posChar.c = m_mainCharacter.getCharacter();
    posChar.y = m_mainCharacter.getPosition().y;
    posChar.x = m_mainCharacter.getPosition().x;

    m_positionedCharactersToAdd.push_back(posChar);

    for (int i = 0; i < m_staffMembers.size(); i++)
    {
        PositionedCharacter posCharStaff;
        posCharStaff.c = m_staffMembers[i].getCharacter();
        posCharStaff.y = m_staffMembers[i].getPosition().y;
        posCharStaff.x = m_staffMembers[i].getPosition().x;
        m_positionedCharactersToAdd.push_back(posCharStaff);
    }

    return m_positionedCharactersToAdd;
}

std::vector<Position> Game::getPositionsToRemove()
{
    m_positionsToRemove.clear();
    m_positionsToRemove.push_back(m_mainCharacter.getFormerPosition());

    for (int i = 0; i < m_staffMembers.size(); i++)
    {
        m_positionsToRemove.push_back(m_staffMembers[i].getFormerPosition());
    }

    return m_positionsToRemove;
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
        std::string stringCypher = "CYPHER: " + m_gameCrypto.getCypherText();
        std::string stringCypherMapped = "DCRYPT: " + m_gameCrypto.getCypherTextMapped();

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
/*       public methods: crypto game              */
/**************************************************/


void Game::increaseCryptoParameter()
{
    m_gameCrypto.increaseParameter();
    if (m_gameCrypto.isSolved() && (m_gameAchievement == NO_ACHIEVEMENT))
        m_gameAchievement = KNOWS_CODE;
}

void Game::decreaseCryptoParameter()
{
    m_gameCrypto.decreaseParameter();
    if (m_gameCrypto.isSolved() && (m_gameAchievement == NO_ACHIEVEMENT))
        m_gameAchievement = KNOWS_CODE;
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

GameMode Game::gameModeAccordingPosition(Position position)
{
    std::vector<char> characterAround;
    characterAround.push_back(m_gameSetting[position.y * 40 + position.x + 1]);
    characterAround.push_back(m_gameSetting[position.y * 40 + position.x - 1]);
    characterAround.push_back(m_gameSetting[(position.y - 1) * 40 + position.x]);
    characterAround.push_back(m_gameSetting[(position.y + 1) * 40 + position.x]);

    for (char character : characterAround)
    {
        if (character == 'D')
            // find logic to achieve KNOWS_CODE in in/decreaseParameter
            return DESK;
        else if (character == 'S')
        {
            if (m_gameAchievement == KNOWS_CODE)
                m_gameAchievement = KNOWS_PASSWD;
            return SAFE;
        }
        else if (character == 'T')
        {
            if (m_gameAchievement == KNOWS_PASSWD)
            {
                m_gameAchievement = LOGGED_IN_CONSOLE;
                return GAME_OVER;
            }
            return TERMINAL;
        }
    }
    return WALKING;
}
