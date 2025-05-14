#ifndef MONSTERCARD_H
#define MONSTERCARD_H

#include "card.h"
#include "json.hpp"
using namespace std;
using json = nlohmann::json;

class MonsterCard : public Card{
    private:
        int atk;
        int def;
        bool defenseMode;
        bool isSet;
        int owner;
        bool justSummoned;
        int extraAttackThisTurn = 0;
        int attacksThisTurn = 0;
        int extraAttacksRemaining = 0;
    public:
        MonsterCard(string name, int atk, int def, string description, int owner);
        MonsterCard(string name, int atk, int def, string description, int owner, bool defenseMode, bool isSet);
        int getAtk() const;
        int getDef() const;
        int getOwner() const;
        void setOwner(int owner);

        MonsterCard& operator+=(MonsterCard& other);
        void showInfo() const override;
        void showInfoHidden() const;
        void setDefenseMode(bool mode);
        bool isInDefense() const;
        void reveal();
        void flipSummon();
        bool isFacedown() const;
        void PlayCard(vector<Card*>& field) override;
        bool isJustSummoned() const;     
        void clearSummonFlag();  
        
        bool activateEffect(Player& self, Player& opponent) override;
        
        json toJson() const override;
};
#endif


