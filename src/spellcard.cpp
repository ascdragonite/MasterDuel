#include "spellcard.h"
#include "player.h"
#include "monstercard.h"

#include <iostream>
using namespace std;
SpellCard :: SpellCard(string name, string description) : Card(name, "Spell", description)
{
  this -> spell = getEffectInstance(name);
}

void SpellCard :: showInfo(bool hidden) const{
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
    else if (type == "Cruel Pact") effect = std::make_shared<CruelPact>();
    else if (type == "Call of the Sky") effect = std::make_shared<CallofTheSky>();
    else if (type == "Ash Again") effect = std::make_shared<AshAgain>();
    /*
    else if (type == "The Ancient knowledge") effect = std::make_shared<TheAncientKnowledge>();
    else if (type == "Soul Servant") effect = std::make_shared<SoulServant>();
    else if (type == "Enternal Soul") effect = std::make_shared<EnternalSoul>();
    else if (type == "Illusion Magic") effect = std::make_shared<IllusionMagic>();
    else if (type == "Apprentice Helper") effect = std::make_shared<ApprenticeHelper>();
    else if (type == "Wishes for Eyes of Blue") effect = std::make_shared<WishesforEyesofBlue>();
    else if (type == "Chaos Magic") effect = std::make_shared<ChaosMagic>();
    else if (type == "Overdose") effect = std::make_shared<Overdose>();
    else if (type == "The World Destroyer") effect = std::make_shared<TheWorldDestroyer>();
    else if (type == "The True Power of Chaos Dual") effect = std::make_shared<TheTruePowerOfChaosDual>();
    else if (type == "True Light") effect = std::make_shared<TrueLight>();
    else if (type == "The Melody of the Awakening Dragon") effect = std::make_shared<TheMelodyOfTheAwakeningDragon>();
    else if (type == "Blessing from the Roaring Sky") effect = std::make_shared<BlessingfromtheRoaringSky>();
    else if (type == "Dark Magical Circle") effect = std::make_shared<DarkMagicalCircle>();
    else if (type == "The Magic Curtain") effect = std::make_shared<TheMagicCurtain>();
    */
  //Step 3: add spell effect names here
    else 
    {
        return nullptr;
    };

    effectCache[type] = effect; 
    return effect;
}
