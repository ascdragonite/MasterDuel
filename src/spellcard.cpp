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
    if (spell)
    {
        cout << "Spell card activated: " << getName() << endl;
        return spell->ActivateEffect(self, opponent);
    } else {
        cout << "epic spell fail" << endl;
        return false;
    }
}

bool SpellCard::activateEffect(Player& self, Player& opponent, int attackerIndex) 
{ cout << "i forgor" << endl; }

shared_ptr<Spell> SpellCard::getEffectInstance(const std::string& type) {
    static unordered_map<string, shared_ptr<Spell>> effectCache;

    auto it = effectCache.find(type);
    if (it != effectCache.end()) {
        return it->second;
    }

    std::shared_ptr<Spell> effect;

    if (type == "Oshama Scramble") effect = std::make_shared<OshamaScramble>();
    else if (type == "Disorted Fate") effect = std::make_shared<DisortedFate>();
    else if (type == "Dragon United") effect = std::make_shared<DragonUnited>();
    else if (type == "Dark Magic") effect = std::make_shared<DarkMagic>();
    else if (type == "Destr0yer") effect = std::make_shared<Destr0yer>();
    else if (type == "Re:End of a Dream") effect = std::make_shared<ReEndOfADream>();
    else if (type == "World Vanquisher") effect = std::make_shared<WorldVanquisher>();
    else if (type == "Flower Snow Drum n Bass") effect = std::make_shared<FlowerSnowDrumNBass>();
    else if (type == "Dark Burning Magic") effect = std::make_shared<DarkBurningMagic>();
    else if (type == "Burst Stream of Destruction") effect = std::make_shared<BurstStreamofDestruction>();
    else if (type == "Bond Between The Teacher and Student") effect = std::make_shared<BondBetweenTheTeacherandStudent>();
    else if (type == "The Power of Friendship") effect = std::make_shared<ThePowerofFriendship>();
    else if (type == "Rage of The Blue Eyes") effect = std::make_shared<RageofTheBlueEyes>();
    else if (type == "Majesty of The White Dragons") effect = std::make_shared<MajestyofTheWhiteDragons>();
    else if (type == "Roar of The Blue-Eyed Dragons") effect = std::make_shared<RoarofTheBlueEyedDragons>();
    else if (type == "Dark Magic Veil") effect = std::make_shared<DarkMagicVeil>();
    else if (type == "Thousand Knifes") effect = std::make_shared<ThousandKnifes>();

  //Step 3: add spell effect names here
    else 
    {
        return nullptr;
    };

    effectCache[type] = effect; 
    return effect;
}
