#ifndef PLAYER_H
#define PLAYER_H

#include "card.h"
#include <string>
#include <vector>

class TrapCard;

using namespace std;

class Player {
private:
    int index;
    vector<Card *> deck;
    vector<Card *> hand;
    vector<Card *> field;
    int hp;
    int skipBattlePhaseCount;

public:
    vector<int> canTrap;

    Player(int i);

    void drawCard();
    void Summon(int handIndex);

    vector<bool> attackedThisTurn;
    void resetAttackFlags();
    bool hasAttacked(int fieldIndex) const;
    void setAttacked(int fieldIndex, bool value);

    void revealMonster(int fieldIndex);
    void switchPosition(int fieldIndex);

    void takeDamage(int amount);
    int getHp() const;
    void setHp(int hp);

    void loadDeckDarkMagician();
    void loadDeckBlueEyes();
    void shuffleDeck();

    int getIndex() const;
    int getSkipBattlePhaseCount() const;
    vector<Card *> getDeck() const;
    vector<Card *> getField() const;
    vector<Card *> getHand() const;
    vector<Card *> &getDeckRef();
    vector<Card *> &getFieldRef();

    vector<Card *> &getHandRef();

    void setDeck(vector<Card *> newDeck);
    void setField(vector<Card *> newField);
    void setHand(vector<Card *> newHand);
    void setSkipBattlePhaseCount(int count);
    ~Player();
};
#endif
