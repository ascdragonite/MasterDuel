#ifndef SPELLCARD_H
#define SPELLCARD_H
#include "card.h"
#include "player.h"
#include "json.hpp"
using namespace std;
using json = nlohmann::json;

class SpellCard : public Card{
    public:

    SpellCard(string name, string description);

    bool activateEffect(Player& self, Player& opponent) override;

    void showInfo() const override;

    void PlayCard(vector<Card*>& field) override;
    
    json toJson() const override;

};
#endif