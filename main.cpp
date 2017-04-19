#include <string>
#include <sstream>
#include <SFML/Graphics.hpp>
#include "MonsterCard.hpp"
#include "DrawableCard.hpp"

void turn(DrawableCard field[4][5], bool full[4][5])
{
    bool bTemp = false;
    DrawableCard cTemp;
    cTemp.null();

    for (int row=0; row<2; row++)
    {
        for (int col=0; col<5; col++)
        {
            cTemp = field[row][col];
            bTemp = full[row][col];
            field[row][col] = field[3-row][col];
            full[row][col] = full[3-row][col];
            field[3-row][col] = cTemp;
            full[3-row][col] = bTemp;
        }
    }
}

void clean(bool grid[4][5])
{
    for (int row=0; row<4; row++)
    {
        for (int col=0; col<5; col++)
        {
            grid[row][col] = false;
        }
    }
}

bool barren(bool grid[4][5])
{
    for (int row=0; row<4; row++)
    {
        for (int col=0; col<5; col++)
        {
            if (grid[row][col])
            {
                return false;
            }
        }
    }

    return true;
}

struct Slot
{
    int row;
    int col;

    void operator = (int ID)
    {
        if (ID>0)
        {
            ID -= 1;
        }

        else if (ID==0)
        {
            ID = -3;
        }

        row = ID/5;
        col = ID%5;
    }

    bool operator == (int ID) const
    {
        if (ID>0)
        {
            ID -= 1;
        }

        else if (ID==0)
        {
            ID = -3;
        }

        return ((row*5 + col) == ID);
    }

    void is(int _row, int _col)
    {
        row = _row;
        col = _col;
    }

    bool valid() const
    {
        if (row>=0 && row<4 && col>=0 && col<5)
        {
            return true;
        }

        return false;
    }
};

int main()
{
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "FIGHT", sf::Style::Fullscreen);

    sf::Font font;
    font.loadFromFile("Augusta.ttf");

    sf::RectangleShape endTurnButton(sf::Vector2f(150, 50));
    endTurnButton.setPosition(200, 515);

    sf::Text endTurnText;
    endTurnText.setString("End Turn");
    endTurnText.setFont(font);
    endTurnText.setCharacterSize(30);
    endTurnText.setOutlineThickness(2);
    endTurnText.setOutlineColor(sf::Color::Black);
    endTurnText.setPosition(218, 518);

    sf::Text manaText;
    manaText.setString("0");
    manaText.setFont(font);
    manaText.setCharacterSize(60);
    manaText.setFillColor(sf::Color::White);
    manaText.setOutlineThickness(4);
    manaText.setOutlineColor(sf::Color::Black);

    sf::RectangleShape nest[4][5];

    for (int row=0; row<4; row++)
    {
        for (int col=0; col<5; col++)
        {
            nest[row][col].setSize(sf::Vector2f(180, 240));
            nest[row][col].setFillColor(sf::Color(115, 77, 38));
            nest[row][col].setPosition(400 + 184*col, 54 + 244*row);
        }
    }

    sf::RectangleShape highlight(sf::Vector2f(180, 240));
    highlight.setFillColor(sf::Color::Transparent);
    highlight.setOutlineThickness(4);
    highlight.setOutlineColor(sf::Color::Blue);

    sf::CircleShape hero1(60, 6);
    hero1.setFillColor(sf::Color(110, 110, 110));
    hero1.setOutlineThickness(4);
    hero1.setOutlineColor(sf::Color(150, 100, 50));
    hero1.setPosition(800, 980);

    sf::CircleShape hero2(60, 6);
    hero2.setFillColor(sf::Color(110, 110, 110));
    hero2.setOutlineThickness(4);
    hero2.setOutlineColor(sf::Color(150, 102, 50));
    hero2.setPosition(800, -20);

    sf::CircleShape portrait1(130);
    portrait1.setFillColor(sf::Color(110, 110, 110));
    portrait1.setOutlineThickness(10);
    portrait1.setOutlineColor(sf::Color(255, 215, 0, 255));
    portrait1.setPosition(40, 654);

    sf::CircleShape portrait2(130);
    portrait2.setFillColor(sf::Color(110, 110, 110));
    portrait2.setOutlineThickness(10);
    portrait2.setOutlineColor(sf::Color::Transparent);
    portrait2.setPosition(40, 166);

    Slot touchID;

    const int handSize = 8;

    Card* deck[handSize];
    deck[0] = new MonsterCard(13);
    deck[1] = new MonsterCard(15);
    deck[2] = new MonsterCard(17);
    deck[3] = new MonsterCard(19);
    deck[4] = new MonsterCard(43);
    deck[5] = new MonsterCard(45);
    deck[6] = new MonsterCard(47);
    deck[7] = new MonsterCard(49);


    DrawableCard hand[handSize];

    for (int i=0; i<handSize; i++)
    {
        hand[i] = (MonsterCard*)deck[i];
        hand[i].setPosition(1510 + (i/4)*200, 30 + (i%4)*260);
    }

    DrawableCard field[4][5];

    DrawableCard* dragCard = nullptr;
    sf::Vector2f posOrig(0, 0);

    ///Field tracking
    bool full[4][5] = {0};
    bool choosable[4][5] = {0};

    int mana = 26;
    int round = 1;
    ///

    ///States
    bool Lclick = false;
    bool Rclick = false;

    bool dragging = false;
    bool attack = false;
    bool hit = false;

    bool endTurn = false;
    ///

    std::stringstream manaString;
    manaString << mana;
    manaText.setString(manaString.str());
    manaText.setOrigin(manaText.getGlobalBounds().width/2, manaText.getGlobalBounds().height);
    manaText.setPosition(865, 1040);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        /// /// //// Start of Game Logic

        Lclick = sf::Mouse::isButtonPressed(sf::Mouse::Left);
        Rclick = sf::Mouse::isButtonPressed(sf::Mouse::Right);

        ///Highlighter module
        touchID = -3;

        if (hero1.getGlobalBounds().contains((sf::Vector2f)sf::Mouse::getPosition()))
        {
            touchID = -1;
        }

        else if (hero2.getGlobalBounds().contains((sf::Vector2f)sf::Mouse::getPosition()))
        {
            touchID = -2;
        }

        else
        {
            for (int row=0; row<4; row++)
            {
                for (int col=0; col<5; col++)
                {
                    if (nest[row][col].getGlobalBounds().contains((sf::Vector2f)sf::Mouse::getPosition())
                        && !(hero1.getGlobalBounds().contains((sf::Vector2f)sf::Mouse::getPosition()) || hero2.getGlobalBounds().contains((sf::Vector2f)sf::Mouse::getPosition())))
                    {
                        highlight.setPosition(400 + 184*col, 54 + 244*row);
                        touchID.is(row, col);
                    }
                }
            }
        }
        ///

        ///Drag&Drop module
        for(int i=0; i<handSize; i++)
        {
            if(!touchID.valid() && !dragging && !attack && !hand[i].isEmpty() && hand[i].contains((sf::Vector2f)sf::Mouse::getPosition()) && Lclick)
            {
                dragCard = &hand[i];
                posOrig = hand[i].getPosition();
                dragging = true;

                if (mana - dragCard->getMana() >= 0)
                {
                    mana -= dragCard->getMana();
                    manaText.setFillColor(sf::Color::Blue);
                }

                else
                {
                    manaText.setFillColor(sf::Color::Red);
                }

                manaString.str("");
                manaString << mana;
                manaText.setString(manaString.str());
                manaText.setOrigin(manaText.getGlobalBounds().width/2, manaText.getGlobalBounds().height);
                manaText.setPosition(865, 1040);
            }
        }

        if(dragging && Lclick)
        {
            dragCard->setPosition(sf::Mouse::getPosition().x-90, sf::Mouse::getPosition().y-120);
        }

        else if(dragging && !Lclick)
        {
            if(touchID.valid() && !full[touchID.row][touchID.col] &&
               (((touchID.row == 2) && (dragCard->getClass() == MonsterCard::Class::Warrior || dragCard->getClass() == MonsterCard::Class::Rogue)) ||
                ((touchID.row == 3) && (dragCard->getClass() == MonsterCard::Class::Archer || dragCard->getClass() == MonsterCard::Class::Mage))) &&
               manaText.getFillColor()!=sf::Color::Red)
            {
                field[touchID.row][touchID.col] = *dragCard;
                dragCard->null();
                full[touchID.row][touchID.col] = true;
                posOrig = sf::Vector2f(0, 0);
                dragging = false;
                dragCard = nullptr;

                manaText.setFillColor(sf::Color::White);
            }

            else
            {
                if (manaText.getFillColor() != sf::Color::Red)
                {
                    mana += dragCard->getMana();
                }

                manaString.str("");
                manaString << mana;
                manaText.setString(manaString.str());
                manaText.setOrigin(manaText.getGlobalBounds().width/2, manaText.getGlobalBounds().height);
                manaText.setPosition(865, 1040);
                manaText.setFillColor(sf::Color::White);

                dragCard->setPosition(posOrig);
                posOrig = sf::Vector2f(0, 0);
                dragging = false;
                dragCard = nullptr;
            }
        }

        else
        {
            dragging = false;
        }
        ///

        ///Attack module
        if (touchID.valid() && full[touchID.row][touchID.col] && !field[touchID.row][touchID.col].isEmpty() && round>2)
        {
            if (!dragging && !attack && Lclick)
            {
                attack = false;
                hit = true;
            }

            if (!attack && hit)
            {
                dragCard = &field[touchID.row][touchID.col];
                dragCard->attack(choosable, full);

                if (barren(choosable))
                {
                    dragCard = nullptr;
                    attack = false;
                    hit = false;
                }

                else
                {
                    attack = true;
                    hit = false;
                }
            }
        }

        else
        {
            hit = false;
        }

        if (attack && !hit && touchID.valid() && choosable[touchID.row][touchID.col])
        {
            if (Lclick)
            {
                hit = true;
            }
        }

        else if (attack && hit && !(touchID.valid() && choosable[touchID.row][touchID.col]))
        {
            hit = false;
        }

        if (attack && hit && touchID.valid() && choosable[touchID.row][touchID.col] && !field[touchID.row][touchID.col].isEmpty() && !Lclick)
        {
            dragCard->attack(field[touchID.row][touchID.col]);
            clean(choosable);
            dragCard = nullptr;
            attack = false;
            hit = false;

            if (!field[touchID.row][touchID.col].isAlive())
            {
                field[touchID.row][touchID.col].kill();
                field[touchID.row][touchID.col].null();
                full[touchID.row][touchID.col] = false;
            }
        }
        ///

        ///End Turn module
        if (!dragging && endTurnButton.getGlobalBounds().contains((sf::Vector2f)sf::Mouse::getPosition()))
        {
            endTurnButton.setFillColor(sf::Color(255, 255, 0, 255));
            endTurnText.setFillColor(sf::Color(255, 255, 255, 255));
            endTurnText.setOutlineColor(sf::Color(0, 0, 0, 255));

            if (Lclick)
            {
                endTurn = 1;
                endTurnButton.setFillColor(sf::Color(255, 185, 0, 255));
            }

            if (endTurn && !Lclick)
            {
                clean(choosable);
                dragging = false;
                attack = false;
                hit = false;
                dragCard = nullptr;

                turn(field, full);
                endTurn = 0;
                round++;
                mana = 26;
                manaString.str("");
                manaString << mana;
                manaText.setString(manaString.str());
                manaText.setOrigin(manaText.getGlobalBounds().width/2, manaText.getGlobalBounds().height);
                manaText.setPosition(865, 1040);
            }
        }

        else
        {
            endTurn = 0;
            endTurnButton.setFillColor(sf::Color(255, 215, 0, 255));
        }
        ///

        /// /// /// End of Game Logic

        ///Draw
        window.clear(sf::Color(150, 100, 50));

        for (int row=0; row<4; row++)
        {
            for (int col=0; col<5; col++)
            {
                if (attack && choosable[row][col])
                {
                    nest[row][col].setOutlineThickness(4);
                    nest[row][col].setOutlineColor(sf::Color::Red);
                    nest[row][col].setFillColor(sf::Color(115, 77, 38));
                }

                else if (dragging &&
                        (((row == 2) && (dragCard->getClass() == MonsterCard::Class::Warrior || dragCard->getClass() == MonsterCard::Class::Rogue)) ||
                        ((row == 3) && (dragCard->getClass() == MonsterCard::Class::Archer || dragCard->getClass() == MonsterCard::Class::Mage))) &&
                         manaText.getFillColor() != sf::Color::Red)
                {
                    nest[row][col].setOutlineThickness(0);
                    nest[row][col].setOutlineColor(sf::Color::Transparent);
                    nest[row][col].setFillColor(sf::Color(0, 80, 255, 255));
                }

                else
                {
                    nest[row][col].setOutlineThickness(0);
                    nest[row][col].setOutlineColor(sf::Color::Transparent);
                    nest[row][col].setFillColor(sf::Color(115, 77, 38));
                }

                window.draw(nest[row][col]);
            }
        }

        window.draw(endTurnButton);
        window.draw(endTurnText);

        window.draw(portrait1);
        window.draw(portrait2);

        for (int i=0; i<handSize; i++)
        {
            if (!hand[i].isEmpty())
            {
                window.draw(hand[i]);
            }
        }

        for (int row=0; row<4; row++)
        {
            for (int col=0; col<5; col++)
            {
                if (!field[row][col].isEmpty())
                {
                    field[row][col].setPosition(400 + 184*col, 54 + 244*row);
                    window.draw(field[row][col]);
                }
            }
        }

        if (dragging && touchID.valid() && !full[touchID.row][touchID.col] &&
            (((touchID.row == 2) && (dragCard->getClass() == MonsterCard::Class::Warrior || dragCard->getClass() == MonsterCard::Class::Rogue)) ||
            ((touchID.row == 3) && (dragCard->getClass() == MonsterCard::Class::Archer || dragCard->getClass() == MonsterCard::Class::Mage))) &&
            manaText.getFillColor() != sf::Color::Red)
        {
            highlight.setFillColor(sf::Color::Transparent);
            highlight.setOutlineColor(sf::Color(0, 120, 255, 255));
            window.draw(highlight);
        }

        else if (attack && touchID.valid() && choosable[touchID.row][touchID.col])
        {
            highlight.setFillColor(sf::Color(255, 0, 0, 150 - (int)hit*30));
            highlight.setOutlineColor(sf::Color::Transparent);
            window.draw(highlight);
        }

        else if (!dragging && !attack && touchID.valid() && full[touchID.row][touchID.col])
        {
            highlight.setFillColor(sf::Color::Transparent);
            highlight.setOutlineColor(sf::Color(0, 80 + (int)!Lclick*40, 255, 255));
            window.draw(highlight);
        }

        if (dragCard != nullptr && !dragCard->isEmpty())
        {
            window.draw(*dragCard);
        }

        window.draw(hero1);
        window.draw(hero2);

        if (round<3)
        {
            window.draw(manaText);
        }

        else
        {
            mana = 0;
        }

        window.display();
    }

    ///Cleanup
    for (int i=0; i<handSize; i++)
    {
        delete deck[i];
    }
    ///

    return 0;
}
