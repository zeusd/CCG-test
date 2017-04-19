#ifndef MONSTERCARD_HPP
#define MONSTERCARD_HPP

#include "Card.hpp"

class MonsterCard : public Card
{
public:

    enum Class
    {
        Warrior,
        Rogue,
        Archer,
        Mage
    };

    MonsterCard(int cardID = 0);
    MonsterCard(const MonsterCard&);
    MonsterCard& operator = (const MonsterCard&);

    int getID() const;
    int getAtk() const;
    int getHp() const;
    int getMana() const;
    bool isAlive() const;
    Class getClass() const;

    void setID(int);

    void damage(int);
    void attack(MonsterCard&);
    void kill();
    void revive();


private:

    int _atk;
    int _hp;
    int _mana;
    bool _alive;

    Class _RPGclass;

    void setStats(int, int, int);
};

#endif //MONSTERCARD_HPP
