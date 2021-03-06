#pragma once

#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <chrono>

enum CryptoParameterUpdate
{
    INCREASE = 0,
    DECREASE = 1,
};

class GameCrypto
{

public:

    GameCrypto();
    ~GameCrypto();

    std::string getCypherText();
    std::string getCypherTextMapped();
    std::string getAlphabet();
    std::string getAlphabetMapped();
    void updateParameter(CryptoParameterUpdate update);
    bool isSolved();

private:

    std::string m_alphabet;
    std::string m_textPlain;
    std::string m_textCypher;
    int m_parameterDecrypt;
    int m_parameterEncrypt;
    std::default_random_engine m_randomEngine;

    std::string tranformStringWithParamter(std::string text, int paramter);
    int moduloCalc(int number1, int number2);
    std::string getRandomText();

};
