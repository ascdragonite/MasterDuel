#ifndef MONSTERCARD_H
#define MONSTERCARD_H

#include "card.h"

class Monstercard : public Card{
    private:
    int atk;
    int def;
    public:
    Monstercard(string name, int atk, int def, string description);

    int getAtk() const;
    int getDef() const;

    void showInfo() const override;
};
#endif


