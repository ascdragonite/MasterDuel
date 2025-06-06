#ifndef TRAPCARD_H
#define TRAPCARD_H
#include "card.h"
#include "spell.h"
#include "player.h"
#include "json.hpp"
using namespace std;
using json = nlohmann::json;

class TrapCard : public Card{
    private:
    shared_ptr<Spell> trap;
    public:

    TrapCard(string name, string description);

    bool activateEffect(Player& self, Player& opponent) override;

    bool activateEffect(Player& self, Player& opponent, int attackerIndex) override;

    void showInfo(bool hidden) const override;



    void PlayCard(vector<Card*>& field) override;

    json toJson() const override;
    
    static shared_ptr<Spell> getEffectInstance(const string& type);
};
#endif
