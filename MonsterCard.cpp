#include <SFML/Graphics.hpp>
#include "MonsterCard.hpp"

MonsterCard::MonsterCard(int cardID): Card(cardID), _atk(0), _hp(0), _mana(0), _alive(true)
{
    setID(cardID);
}

MonsterCard::MonsterCard(const MonsterCard& other): Card(other.getID()), _atk(0), _hp(0), _mana(0), _alive(other._alive)
{
    setID(other.getID());
}

MonsterCard& MonsterCard::operator =(const MonsterCard& other)
{
    setID(other.getID());
    _alive = other._alive;

    return *this;
}

void MonsterCard::setStats(int atk, int hp, int mana)
{
    _atk = atk;
    _hp = hp;
    _mana = mana;
}

int MonsterCard::getID() const
{
    return Card::getID();
}

int MonsterCard::getAtk() const
{
    return _atk;
}

int MonsterCard::getHp() const
{
    return _hp;
}

int MonsterCard::getMana() const
{
    return _mana;
}

bool MonsterCard::isAlive() const
{
    return _alive;
}

MonsterCard::Class MonsterCard::getClass() const
{
    return _RPGclass;
}

void MonsterCard::setID(int cardID)
{
    cardID %= 100;

    Card::setID(cardID);

    switch(cardID/10)
    {
    case (1):
        {
            _RPGclass = Warrior;

            switch(cardID%10)
            {
            case (1):
                {
                    MonsterCard::setStats(1, 3, 1);
                }
            break;

            case (2):
                {
                    MonsterCard::setStats(2, 4, 2);
                }
            break;

            case (3):
                {
                    MonsterCard::setStats(2, 6, 3);
                }
            break;

            case (4):
                {
                    MonsterCard::setStats(6, 3, 4);
                }
            break;

            case (5):
                {
                    MonsterCard::setStats(4, 8, 5);
                }
            break;

            case (6):
                {
                    MonsterCard::setStats(5, 10, 7);
                }
            break;

            case (7):
                {
                    MonsterCard::setStats(7, 14, 8);
                }
            break;

            case (8):
                {
                    MonsterCard::setStats(15, 1, 9);
                }
            break;

            case (9):
                {
                    MonsterCard::setStats(12, 20, 10);
                }
            break;

            default:
                {
                    setStats(0, 0, 0);
                    kill();
                }
            }
        }
    break;

    case (2):
        {
            _RPGclass = Rogue;
        }
    break;

    case (3):
        {
            _RPGclass = Archer;
        }
    break;

    case (4):
        {
            _RPGclass = Mage;

            switch(cardID%10)
            {
            case (1):
                {
                    MonsterCard::setStats(1, 1, 1);
                }
            break;

            case (2):
                {
                    MonsterCard::setStats(2, 1, 2);
                }
            break;

            case (3):
                {
                    MonsterCard::setStats(3, 1, 3);
                }
            break;

            case (4):
                {
                    MonsterCard::setStats(4, 2, 4);
                }
            break;

            case (5):
                {
                    MonsterCard::setStats(6, 2, 5);
                }
            break;

            case (6):
                {
                    MonsterCard::setStats(7, 3, 7);
                }
            break;

            case (7):
                {
                    MonsterCard::setStats(12, 4, 8);
                }
            break;

            case (8):
                {
                    MonsterCard::setStats(15, 5, 9);
                }
            break;

            case (9):
                {
                    MonsterCard::setStats(18, 5, 10);
                }
            break;

            default:
                {
                    setStats(0, 0, 0);
                    kill();
                }
            }
        }
    break;

    default:
        {
            _RPGclass = Rogue;
            setStats(0, 0, 0);
            kill();
        }
    }
}

void MonsterCard::damage(int dmg)
{
    _hp-=dmg;

    if(_hp<1)
    {
        kill();
    }
}

void MonsterCard::attack(MonsterCard& other)
{
    other.damage(_atk);
}

void MonsterCard::kill()
{
    _alive = false;
}

void MonsterCard::revive()
{
    _alive = true;

    MonsterCard::setID(getID());
}
