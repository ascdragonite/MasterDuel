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
    void playMonster(int handIndex);
    void activateSpell(int handIndex);
    void setTrap(int handIndex);

    void showHand() const;
    void showField() const;

    void takeDamage(int amount);
    int getHp() const;

    void loadDeckDarkMagician();
    void loadDeckBlueEyes();

    vector<Card*>& getDeck();
    vector<Card*>& getField();
};
#endif
