#include "trapcard.h"
#include <iostream>
using namespace std;
TrapCard :: TrapCard(string name, string description) : Card(name, "Trap", description){}

void TrapCard :: activateEffect(){
    cout << "Activating trap: " << getName() << endl;
}

void TrapCard :: showInfo() const{
    cout << "Name: " << getName() << endl;
    cout << "Type: " << getType() << endl;
    cout << "Description: " << getDescription() << endl;
}

void TrapCard::PlayCard(vector<Card*> field)
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
