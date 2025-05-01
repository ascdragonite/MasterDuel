#ifndef SPELLCARD_H
#define SPELLCARD_H
#include "card.h"
#include "json.hpp"
using namespace std;
using json = nlohmann::json;

class SpellCard : public Card{
    public:

    SpellCard(string name, string description);

    virtual void activateEffect();

    void showInfo() const override;
    
    json toJson() const override;

};
#endif