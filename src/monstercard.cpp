#include "monstercard.h"
#include <iostream>
using namespace std;

Monstercard :: Monstercard(string name, int atk, int def, string description) : Card(name, description, "Monster"){
    this -> atk = atk;
    this -> def = def;
}
int Monstercard::getAtk() const{
    return atk;
}
int Monstercard::getDef() const{
    return def;
}
void Monstercard::showInfo() const{
    cout << "Name: " << getName() << endl;
    cout << "Type: " << getType() << endl;
    cout << "Atk: " << atk << "Def: " << def << endl;
    cout << "Description: " << getDescription() << endl;
}