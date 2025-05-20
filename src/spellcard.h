#ifndef SPELLCARD_H
#define SPELLCARD_H
#include "spell.h"
#include "card.h"
#include "player.h"
#include "json.hpp"
using namespace std;
using json = nlohmann::json;

class SpellCard : public Card{
    private:
    shared_ptr<Spell> spell; // Use shared_ptr for better memory management
    public:

    SpellCard(string name, string description);

    bool activateEffect(Player& self, Player& opponent) override;
    bool activateEffect(Player& self, Player& opponent, int attackerIndex) override;

    void showInfo() const override;

    void PlayCard(vector<Card*>& field) override;
    
    json toJson() const override;
    static shared_ptr<Spell> getEffectInstance(const string& type);

};
#endif
