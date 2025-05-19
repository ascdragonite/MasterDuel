#include "trapcard.h"
#include "player.h"
#include <iostream>
using namespace std;
TrapCard :: TrapCard(string name, string description) : Card(name, "Trap", description){}

void TrapCard :: showInfo() const{
    cout << "Name: " << getName() << endl;
    cout << "Type: " << getType() << endl;
    cout << "Description: " << getDescription() << endl;
}

void TrapCard::PlayCard(vector<Card*>& field)
{
    int count = 0;
    for (Card* card : field) {
        string type = card->getType();
        if (type == "Spell" || type == "Trap") count++;
    }
    if (count < 5)
    {
        field.push_back(this);
        cout << "Trap card played: " << getName() << endl;
    }
    else
    {
        cout << "Cannot play trap card. Field is full." << endl;
    }
}

json TrapCard::toJson() const 
{
    json j;
    j["type"] = type;
    j["name"] = name;
    j["description"] = description;
    return j;
}
bool TrapCard::activateEffect(Player& self, Player& opponent) {
    if (trap)
    {
        cout << "Trap card activated: " << getName() << endl;
        return trap->ActivateEffect(self, opponent);
    } else {
        cout << "epic trap fail" << endl;
        return false;
    }
}
shared_ptr<Spell> TrapCard::getEffectInstance(const std::string& type) {
    static unordered_map<string, shared_ptr<Spell>> TrapEffectCache;

    auto it = TrapEffectCache.find(type);
    if (it != TrapEffectCache.end()) {
        return it->second;
    }

    std::shared_ptr<Spell> effect;

    if (type == "Tsunagite") effect = std::make_shared<Tsunagite>();
    else if (type == "Mirror Force") effect = std::make_shared<MirrorForce>();
    {
        return nullptr;
    };

    TrapEffectCache[type] = effect; 
    return effect;
}
