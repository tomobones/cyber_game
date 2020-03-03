#include "GameObject.hpp"


/**************************************************/
/*             constructor, destructor            */
/**************************************************/


GameObject::GameObject()
{
    m_position.y = 3;
    m_position.x = 0;

    m_formerPosition.y = 3;
    m_formerPosition.x = 0;

    m_character = 'O';
}

GameObject::GameObject(int y, int x, char c)
{
    m_position.y = y;
    m_position.x = x;

    m_formerPosition.y = y;
    m_formerPosition.x = x;

    m_character = c;
}

GameObject::~GameObject()
{

}


/**************************************************/
/*                  public methods                */
/**************************************************/


Position GameObject::getPosition()
{
    return m_position;
}

void GameObject::setPosition(Position actualPosition)
{
    m_position.x = actualPosition.x;
    m_position.y = actualPosition.y;
}

Position GameObject::getFormerPosition()
{
    return m_formerPosition;
}

void GameObject::setFormerPosition(Position formerPosition)
{
    m_formerPosition.x = formerPosition.x;
    m_formerPosition.y = formerPosition.y;
}

char GameObject::getCharacter()
{
    return m_character;
}
