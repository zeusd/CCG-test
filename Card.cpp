#include <SFML/Graphics.hpp>
#include "Card.hpp"

Card::Card(int cardID): _cardID(cardID) {}

Card::~Card() {}

int Card::getID() const
{
    return _cardID;
}

void Card::setID(int cardID)
{
    _cardID = cardID;
}
