#ifndef TRAPCARD_H
#define TRAPCARD_H
#include "card.h"
#include "json.hpp"
using namespace std;
using json = nlohmann::json;

class TrapCard : public Card{
    public:

    TrapCard(string name, string description);

    void showInfo() const override;

    void PlayCard(vector<Card*>& field) override;

    bool activateEffect(Player& self, Player& opponent) override;

    json toJson() const override;
};
#endif