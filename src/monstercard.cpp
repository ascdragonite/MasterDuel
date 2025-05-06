#include "monstercard.h"
#include <iostream>
using namespace std;

MonsterCard :: MonsterCard(string name, int atk, int def, string description) : Card(name, "Monster", description){
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

void MonsterCard::PlayCard(vector<Card*> field)
{
    int monsterCount = 0;
    for (Card* card : field) {
        if (card->getType() == "Monster") {
            monsterCount++;
        }
    }

    if (monsterCount < 5)
    {
        char answer;
        cout << "Defense mode? [y/n]";
        cin >> answer;
        if (answer == 'y' || answer == 'Y') {
            defenseMode = true;
        } else if (answer == 'n' || answer == 'N') {
            defenseMode = false;
        }
        field.push_back(this);
        cout << "Monster card played: " << getName() << endl;
    }
    else
    {
        cout << "Cannot play monster card. Field is full." << endl;
    }
}

json MonsterCard::toJson() const 
{
    json j;
    j["type"] = type;
    j["name"] = name;
    j["atk"] = atk;
    j["def"] = def;
    j["isSet"] = isSet;
    j["defenseMode"] = defenseMode;
    j["description"] = description;
    return j;
}

