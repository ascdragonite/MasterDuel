#include "card.h"
#include "player.h"
#include <iostream>
using namespace std;

Card :: Card(string name, string type, string description){
    this -> name = name;
    this -> type = type;
    this -> description = description;
}

string Card::getName() const{
    return name;
}
string Card::getDescription() const{
    return description;
}
string Card::getType() const{
    return type;
}

void Card::SetName(string n){
    name = n;
}

void Card::SetDescription(string d)
{
    description = d;
}
void Card::SetType(string t)
{
    type = t;
}

Card::~Card()
{
} 