#include "trapcard.h"
#include <iostream>
using namespace std;
TrapCard :: TrapCard(string name, string description) : Card(name, description, "Trap"){}

void TrapCard :: activateEffect(){
    cout << "Activating spell: " << getName() << endl;
}

void TrapCard :: showInfo() const{
    cout << "Name: " << getName() << endl;
    cout << "Type: " << getType() << endl;
    cout << "Description: " << getDescription() << endl;
}

