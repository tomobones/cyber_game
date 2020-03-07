#include "GameCrypto.hpp"


/**************************************************/
/*             constructor, destructor            */
/**************************************************/


GameCrypto::GameCrypto()
{
    m_randomEngine = std::default_random_engine(
        std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> distribution(1, 25);

    m_parameterDecrypt = 0;
    m_parameterEncrypt = distribution(m_randomEngine);
    m_alphabet = "abcdefghijklmnopqrstuvwxyz";

    m_textPlain = getRandomText();
    //m_textPlain = "one two three four";
    m_textCypher = tranformStringWithParamter(m_textPlain, m_parameterEncrypt);
}

GameCrypto::~GameCrypto()
{

}


/**************************************************/
/*                 public methods                 */
/**************************************************/


std::string GameCrypto::getCypherText()
{
    return m_textCypher;
}

std::string GameCrypto::getCypherTextMapped()
{
    return tranformStringWithParamter(m_textCypher, m_parameterDecrypt);
}

std::string GameCrypto::getAlphabet()
{
    return m_alphabet;
}

std::string GameCrypto::getAlphabetMapped()
{
    return tranformStringWithParamter(m_alphabet, m_parameterDecrypt);
}

void GameCrypto::updateParameter(CryptoParameterUpdate update)
{
    if (update == INCREASE)
        m_parameterDecrypt++;
    else if (update == DECREASE)
        m_parameterDecrypt--;
    m_parameterDecrypt = moduloCalc(m_parameterDecrypt, 26);
}

bool GameCrypto::isSolved()
{
    return (m_parameterDecrypt == 26 - m_parameterEncrypt);
}


/**************************************************/
/*                 private methods                */
/**************************************************/


std::string GameCrypto::tranformStringWithParamter(std::string oldString, int parameter)
{
    std::string newString = oldString;
    int indexInAlphabet;

    for (int i = 0; i < newString.length(); ++i)
    {
        indexInAlphabet = m_alphabet.find(oldString[i]);
        if (indexInAlphabet != -1)
            newString[i] = m_alphabet[moduloCalc(indexInAlphabet + parameter, 26)];
    }
    return newString;
}

int GameCrypto::moduloCalc(int number1, int number2)
{
    return ((number1 % number2)+number2)% number2;
}

std::string GameCrypto::getRandomText()
{
    std::string stringComplete = "";
    std::vector<std::string> digitStrings(10);
    digitStrings[0] = "zero";
    digitStrings[1] = "one";
    digitStrings[2] = "two";
    digitStrings[3] = "three";
    digitStrings[4] = "four";
    digitStrings[5] = "five";
    digitStrings[6] = "six";
    digitStrings[7] = "seven";
    digitStrings[8] = "eight";
    digitStrings[9] = "nine";
    std::uniform_int_distribution<int> distribution(0, 9);
    for (int i = 0; i < 4; ++i)
    {
        int digit = distribution(m_randomEngine);
        stringComplete += digitStrings[digit];
        if (i != 3)
            stringComplete += " ";
    }
    return stringComplete;
}
