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

