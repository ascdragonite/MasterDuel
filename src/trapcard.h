#ifndef TRAPCARD_H
#define TRAPCARD_H
#include "card.h"
#include "json.hpp"
using namespace std;
using json = nlohmann::json;

class TrapCard : public Card{
    public:

    TrapCard(string name, string description);

    virtual void activateEffect();

    void showInfo() const override;

    json toJson() const override;
};
#endif