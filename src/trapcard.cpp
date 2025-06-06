#include "trapcard.h"
#include "player.h"
#include <iostream>
using namespace std;
TrapCard :: TrapCard(string name, string description) : Card(name, "Trap", description)
{
    this -> trap = getEffectInstance(name);
}

void TrapCard :: showInfo(bool hidden) const{

    if (hidden) cout << " | _____<@>_____ |  (Trap)" << endl;
    else cout << " | " << getName() << " | " << endl;
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

bool TrapCard::activateEffect(Player& self, Player& opponent, int attackerIndex) {
    if (trap)
    {
        cout << "Trrricksters!! activated: " << getName() << endl;
        return trap->ActivateEffect(self, opponent, attackerIndex);
    } else {
        cout << "epic Trrricksters!! fail" << endl;
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
    else if (type == "Trrricksters!!") effect = std::make_shared<Trrricksters>();
    else if (type == "Mirror Force") effect = std::make_shared<MirrorForce>();
    else
    {
        cout << "cannot find " << type << endl;
        return nullptr;
    };

    TrapEffectCache[type] = effect; 
    return effect;
}
