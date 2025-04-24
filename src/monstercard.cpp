#include "monstercard.h"
#include <iostream>
using namespace std;

MonsterCard :: MonsterCard(string name, int atk, int def, string description) : Card(name, description, "Monster"){
    this -> atk = atk;
    this -> def = def;
}
int MonsterCard::getAtk() const{
    return atk;
}
int MonsterCard::getDef() const{
    return def;
}
void MonsterCard::showInfo() const{
    cout << "Name: " << getName() << endl;
    cout << "Type: " << getType() << endl;
    cout << "Atk: " << atk << " Def: " << def << endl;
    cout << "Description: " << getDescription() << endl;
}