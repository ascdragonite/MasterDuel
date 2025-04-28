#ifndef TRAPCARD_H
#define TRAPCARD_H
#include "card.h"
class TrapCard : public Card{
    public:

    TrapCard(string name, string description);

    virtual void activateEffect();

    void showInfo() const override;

};
#endif