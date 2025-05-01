#include "spellcard.h"
#include <iostream>
using namespace std;
SpellCard :: SpellCard(string name, string description) : Card(name, description, "Spell"){}

void SpellCard :: activateEffect(){
    cout << "Activating spell: " << getName() << endl;
}

void SpellCard :: showInfo() const{
    cout << "Name: " << getName() << endl;
    cout << "Type: " << getType() << endl;
    cout << "Description: " << getDescription() << endl;
}


json SpellCard::toJson() const 
{
    json j;
    j["type"] = type;
    j["name"] = name;
    j["description"] = description;
    return j;
}
