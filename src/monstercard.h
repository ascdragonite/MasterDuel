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
    public:
        MonsterCard(string name, int atk, int def, string description);

        int getAtk() const;
        int getDef() const;

        void showInfo() const override;
        void setDefenseMode(bool mode);
        bool isInDefense() const;
        void reveal();
        bool isFacedown() const;
        
        json toJson() const override;
};
#endif


