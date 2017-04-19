#ifndef DRAWABLECARD_HPP
#define DRAWABLECARD_HPP

#include <string>
#include <sstream>
#include "MonsterCard.hpp"
#include <SFML/Graphics.hpp>

class DrawableCard : public sf::Transformable, public sf::Drawable
{
public:
    DrawableCard();
    DrawableCard(MonsterCard*);
    DrawableCard(const DrawableCard&);
    DrawableCard& operator =(const DrawableCard&);
    DrawableCard& operator =(MonsterCard*);

    void setPosition(float, float);
    void setPosition(sf::Vector2f);
    sf::Vector2f getPosition() const;
    bool contains(sf::Vector2f) const;
    bool isEmpty() const;
    void update();
    void null();

    MonsterCard::Class getClass() const;
    void attack(bool choosable[4][5], bool full[4][5]) const;
    void attack(DrawableCard&) const;
    int getMana() const;
    bool isAlive() const;
    void kill();
    void revive();

private:
    MonsterCard* _card;

    sf::Sprite _sprite;
    sf::Texture _image;
    sf::Font _font;
    sf::Text _atkNum;
    sf::Text _hpNum;
    sf::Text _manaNum;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        if (_card->isAlive())
        {
            states.transform *= getTransform();

            target.draw(_sprite, states);
            target.draw(_atkNum, states);
            target.draw(_hpNum, states);
            target.draw(_manaNum, states);
        }
    }

    void init();
    void resolve();
};

#endif // DRAWABLECARD_HPP
