#include "DrawableCard.hpp"
#include "MonsterCard.hpp"
#include <SFML/Graphics.hpp>

void DrawableCard::init()
{
    _font.loadFromFile("Augusta.ttf");
    _image.loadFromFile("blackcolor.png");
    _image.setSmooth(true);
    _sprite.setTexture(_image);

    _atkNum.setFont(_font);
    _hpNum.setFont(_font);
    _manaNum.setFont(_font);

    _atkNum.setCharacterSize(32);
    _hpNum.setCharacterSize(32);
    _manaNum.setCharacterSize(32);

    _atkNum.setFillColor(sf::Color::White);
    _hpNum.setFillColor(sf::Color::Red);
    _manaNum.setFillColor(sf::Color::Blue);

    _atkNum.setString("0");
    _hpNum.setString("0");
    _manaNum.setString("0");

    _atkNum.setOrigin(_atkNum.getGlobalBounds().width/2, _atkNum.getGlobalBounds().height);
    _hpNum.setOrigin(_hpNum.getGlobalBounds().width/2, _hpNum.getGlobalBounds().height);
    _manaNum.setOrigin(_manaNum.getGlobalBounds().width/2, _manaNum.getGlobalBounds().height);

    setPosition(_sprite.getPosition());
}

void DrawableCard::resolve()
{
    std::stringstream path;
    path << "Cards/";

    switch (_card->getClass())
    {
    case (MonsterCard::Warrior):
        {
            path << "Warrior/";
        }
    break;

    case (MonsterCard::Rogue):
        {
            path << "Rogue/";
        }
    break;

    case (MonsterCard::Archer):
        {
            path << "Archer/";
        }
    break;

    case (MonsterCard::Mage):
        {
            path << "Mage/";
        }
    break;
    }

    path << _card->getID()%10 << ".jpg";

    _image.loadFromFile(path.str());
}

DrawableCard::DrawableCard() : _card(nullptr)
{
    DrawableCard::init();
}

DrawableCard::DrawableCard(MonsterCard* monCard) : _card(monCard)
{
    if (_card != nullptr)
    {
        DrawableCard::init();

        resolve();
        update();
    }
}

DrawableCard::DrawableCard(const DrawableCard& other) : _card(other._card)
{
    if (_card != nullptr)
    {
        resolve();
        update();

        _atkNum.setPosition(other._atkNum.getPosition());
        _hpNum.setPosition(other._hpNum.getPosition());
        _manaNum.setPosition(other._manaNum.getPosition());
        _sprite.setPosition(other._sprite.getPosition());
    }
}

DrawableCard& DrawableCard::operator =(const DrawableCard& other)
{
    _card = other._card;

    if(_card != nullptr)
    {
        resolve();
        update();

        _atkNum.setPosition(other._atkNum.getPosition());
        _hpNum.setPosition(other._hpNum.getPosition());
        _manaNum.setPosition(other._manaNum.getPosition());
        _sprite.setPosition(other._sprite.getPosition());
    }

    return *this;
}

DrawableCard& DrawableCard::operator =( MonsterCard* monCard)
{
    _card = monCard;

    if (_card != nullptr)
    {
        resolve();
        update();
    }

    return *this;
}

void DrawableCard::setPosition(float x, float y)
{
    _sprite.setPosition(x, y);
    _atkNum.setPosition(x + 15, y + 220);
    _hpNum.setPosition(x + 165, y + 220);
    _manaNum.setPosition(x + 15, y + 20);
}

void DrawableCard::setPosition(sf::Vector2f vec)
{
    setPosition(vec.x, vec.y);
}

sf::Vector2f DrawableCard::getPosition() const
{
    return _sprite.getPosition();
}

bool DrawableCard::contains(sf::Vector2f vec) const
{
    return _sprite.getGlobalBounds().contains(vec);
}

bool DrawableCard::isEmpty() const
{
    return (_card == nullptr);
}

void DrawableCard::update()
{
    std::stringstream atkString, hpString, manaString;
    atkString << _card->getAtk();
    hpString << _card->getHp();
    manaString << _card->getMana();

    _atkNum.setString(atkString.str());
    _hpNum.setString(hpString.str());
    _manaNum.setString(manaString.str());

    _atkNum.setOrigin(_atkNum.getGlobalBounds().width/2, _atkNum.getGlobalBounds().height);
    _hpNum.setOrigin(_hpNum.getGlobalBounds().width/2, _hpNum.getGlobalBounds().height);
    _manaNum.setOrigin(_manaNum.getGlobalBounds().width/2, _manaNum.getGlobalBounds().height);
}

void DrawableCard::null()
{
    _card = nullptr;
}

MonsterCard::Class DrawableCard::getClass() const
{
    return _card->getClass();
}

void DrawableCard::attack(bool choosable[4][5], bool full [4][5]) const
{
    int row = ((_sprite.getPosition().y - 54)/244);
    int col = ((_sprite.getPosition().x - 400)/184);

    switch (_card->getClass())
    {
    case (MonsterCard::Warrior):
        {
            if (row>=2 && row<4 && col>=0 && col<5)
            {
                if (full[row-1][col])
                {
                    choosable[row-1][col] = true;
                }

                if (col>0 && full[row-1][col-1])
                {
                    choosable[row-1][col-1] = true;
                }

                if (col<4 && full[row-1][col+1])
                {
                    choosable[row-1][col+1] = true;
                }
            }
        }
    break;

    case (MonsterCard::Rogue):
        {

        }
    break;

    case (MonsterCard::Archer):
        {

        }
    break;

    case (MonsterCard::Mage):
        {
            if (row==3 && col>=0 && col<5)
            {
                if (full[1][col])
                {
                    choosable[1][col] = true;
                }

                else
                {
                    if (full[0][col])
                    {
                        choosable[0][col] = true;
                    }

                    if (col>0 && full[0][col-1])
                    {
                        choosable[0][col-1] = true;
                    }

                    if (col<4 && full[0][col+1])
                    {
                        choosable[0][col+1] = true;
                    }
                }
            }
        }
    break;
    }
}

void DrawableCard::attack(DrawableCard& other) const
{
    _card->attack(*other._card);
    other.update();
}

int DrawableCard::getMana() const
{
    return _card->getMana();
}

bool DrawableCard::isAlive() const
{
    return _card->isAlive();
}

void DrawableCard::kill()
{
    _card->kill();
}

void DrawableCard::revive()
{
    _card->revive();
    update();
}
