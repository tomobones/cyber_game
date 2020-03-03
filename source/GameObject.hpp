#pragma once

struct Position
{
    int y;
    int x;
};

class GameObject
{
public:
    GameObject();
    GameObject(int y, int x, char c);
    ~GameObject();
    Position getPosition();
    Position getFormerPosition();
    void setPosition(Position actualPosition);
    void setFormerPosition(Position formerPosition);
    char getCharacter();

private:
    Position m_position;
    Position m_formerPosition;
    char m_character;
};
