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

    void takeDamage(int amount);
    int getHp() const;

    void loadDeckDarkMagician();
    void loadDeckBlueEyes();
    void shuffleDeck();

    vector<Card*>& getDeck();
    vector<Card*>& getField();
    vector<Card*>& getHand();

    vector<string> getHandInfo() const;  
    vector<string> getFieldInfo() const;
};
#endif
