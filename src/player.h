#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <string>
#include "card.h"
using namespace std;

class Player{
    private:
    vector<Card*> deck;
    vector<Card*> hand;
    vector<Card*> field;
    int hp;

    public:
    Player();

    void drawCard();
    void playMonster(int handIndex, bool defenseMode);
    void activateSpell(int handIndex);
    void setTrap(int handIndex);

    void switchPosition(int fieldIndex);

    void takeDamage(int amount);
    int getHp() const;
    void setHp(int hp);

    void loadDeckDarkMagician();
    void loadDeckBlueEyes();
    void shuffleDeck();

    vector<Card*> getDeck() const;
    vector<Card*> getField() const;
    vector<Card*> getHand() const;

    void setDeck(vector<Card*> newDeck);
    void setField(vector<Card*> newField);
    void setHand(vector<Card*> newHand);

    ~Player();
};
#endif
