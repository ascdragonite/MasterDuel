#include "monstercard.h"
#include <iostream>
using namespace std;

MonsterCard :: MonsterCard(string name, int atk, int def, string description) : Card(name, description, "Monster"){
    this -> atk = atk;
    this -> def = def;
    this -> isSet = false;
    this -> defenseMode = false;
}
int MonsterCard::getAtk() const{
    return atk;
}
int MonsterCard::getDef() const{
    return def;
}
void MonsterCard::showInfo() const{
    if (isSet) {
        cout << "Facedown Defense Position Monster" << endl;
    } 
    else {
        cout << "Name: " << getName() << endl;
        cout << "Type: " << getType() << endl;
        cout << "Atk: " << atk << " Def: " << def << endl;
        cout << "Description: " << getDescription() << endl;
    }
}
void MonsterCard :: setDefenseMode(bool mode){
    defenseMode = mode;
    isSet = true;
}

bool MonsterCard :: isInDefense() const{
    return defenseMode;
}

void MonsterCard :: reveal(){
    isSet = false;
}

bool MonsterCard :: isFacedown() const{
    return isSet;
}

json MonsterCard::toJson() const 
{
    json j;
    j["type"] = type;
    j["name"] = name;
    j["atk"] = atk;
    j["def"] = def;
    j["description"] = description;
    return j;
}

