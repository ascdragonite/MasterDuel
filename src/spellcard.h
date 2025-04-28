#ifndef SPELLCARD_H
#define SPELLCARD_H
#include "card.h"
class SpellCard : public Card{
    public:

    SpellCard(string name, string description);

    virtual void activateEffect();

    void showInfo() const override;

};
#endif