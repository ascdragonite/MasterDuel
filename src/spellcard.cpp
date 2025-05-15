#include "spellcard.h"
#include "player.h"
#include "monstercard.h"

#include <iostream>
using namespace std;
SpellCard :: SpellCard(string name, string description) : Card(name, "Spell", description)
{
  this -> spell = getEffectInstance(name);
}

void SpellCard :: showInfo() const{
    cout << "Name: " << getName() << endl;
    cout << "Type: " << getType() << endl;
    cout << "Description: " << getDescription() << endl;
}

void SpellCard::PlayCard(vector<Card*>& field)
{
    int count = 0;
    for (Card* card : field) {
        string type = card->getType();
        if (type == "Spell" || type == "Trap") count++;
    }
    if (count < 5)
    {
        field.push_back(this);
        cout << "Spell card played: " << getName() << endl;
    }
    else
    {
        cout << "Cannot play spell card. Field is full." << endl;
    }
}


json SpellCard::toJson() const 
{
    json j;
    j["type"] = type;
    j["name"] = name;
    j["description"] = description;
    return j;
}

bool SpellCard::activateEffect(Player& self, Player& opponent) 
{
    cout << "Spell card activated: " << getName() << endl;
    return spell->ActivateEffect(self, opponent);
}
shared_ptr<Spell> SpellCard::getEffectInstance(const std::string& type) {
    static unordered_map<string, shared_ptr<Spell>> effectCache;

    auto it = effectCache.find(type);
    if (it != effectCache.end()) {
        return it->second;
    }

    std::shared_ptr<Spell> effect;

    if (type == "Oshama Scramble") effect = std::make_shared<OshamaScramble>();
    //else if (type == "Dark Magic") effect = std::make_shared<DarkMagic>();
    /*else if (type == "Re:End Of A Dream") effect = std::make_shared<Destroyer>();
    else if (type == "Re:End Of A Dream") effect = std::make_shared<RageOfTheBlueEyes>();*/
    else if (type == "Re:End Of A Dream") effect = std::make_shared<ReEndOfADream>();
    else if (type == "World Vanquisher") effect = std::make_shared<WorldVanquisher>();
    else if (type == "Flower Snow Drum n Bass") effect = std::make_shared<FlowerSnowDrumNBass>();
  //Step 3: add spell effect names here
    else 
    {
        return nullptr;
    };

    effectCache[type] = effect; 
    return effect;
}
