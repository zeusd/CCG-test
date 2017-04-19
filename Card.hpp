#ifndef CARD_HPP
#define CARD_HPP

#include <SFML/Graphics.hpp>

class Card
{
public:
    Card(int cardID = 0);
    virtual ~Card();

    int getID() const;
    void setID(int);
    virtual void kill() = 0;
    virtual void revive() = 0;

private:
    int _cardID;
};

#endif // CARD_HPP
