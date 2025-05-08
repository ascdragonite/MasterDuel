#include "monstercard.h"
#include "player.h"
#include <iostream>
#include "game.h"
using namespace std;

MonsterCard :: MonsterCard(string name, int atk, int def, string description, int owner) : Card(name, "Monster", description){
    this -> atk = atk;
    this -> def = def;
    this -> isSet = false;
    this -> defenseMode = false;
    this -> owner = owner;
}

MonsterCard :: MonsterCard(string name, int atk, int def, string description, int owner, bool defenseMode, bool isSet) : Card(name, "Monster", description){
    this -> atk = atk;
    this -> def = def;
    this -> isSet = isSet;
    this -> defenseMode = defenseMode;
    this -> owner = owner;
}

int MonsterCard::getOwner() const{
    return owner;
}

void MonsterCard::setOwner(int owner){
    this->owner = owner;
}

Player* OwnerFromIndex(int index){
    GameState* gameState = GameState::getInstance();
    if (index == 1) {
        return gameState->getPlayer(1);
    } else if (index == 2) {
        return gameState->getPlayer(2);
    } else {
        return nullptr; // Invalid player ID
    }
}

MonsterCard& MonsterCard::operator+=(MonsterCard& other) {
    cout << "operator activated!!!" << endl;
    int defValue = other.isInDefense() ? other.getDef() : other.getAtk();


    Player* self = OwnerFromIndex(this->getOwner());
    Player* target = OwnerFromIndex(other.getOwner());
    
    const auto& atkField = self->getField();
    const auto& defField = target->getField();

    int attackIndex = find(atkField.begin(), atkField.end(), this) - atkField.begin();
    int defendIndex = find(defField.begin(), defField.end(), &other) - defField.begin();
    
    
    if(other.isInDefense()){
        if(atk > defValue){
            auto field = defField;
            field.erase(field.begin() + defendIndex);
            target -> setField(field);
            other.setOwner(-1);
            return *this;
        }
        else if(atk < defValue){
            self -> takeDamage(defValue - atk);
            return *this;
        }
        else{
            //atk = def
            return *this;
        }
    }

    else{

        if(atk > defValue){
            target->takeDamage(atk - defValue);
            auto field = defField;
            field.erase(field.begin() + defendIndex);
            target->setField(field);
            other.setOwner(-1);
            return *this;
        }
        else if(atk < defValue){
            self->takeDamage(defValue - atk);
            auto field = atkField;
            field.erase(field.begin() + attackIndex);
            self->setField(field);
            return *this;
        }
        else{
            //atk = atk
            auto field1 = atkField;
            auto field2 = defField;
            field1.erase(field1.begin() + attackIndex);
            field2.erase(field2.begin() + defendIndex);
            self->setField(field1);
            target->setField(field2);
            return *this;
        }
    }
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

void MonsterCard::PlayCard(vector<Card*>& field)
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
    j["owner"] = owner; // Assuming Player has a method getIndex()
    j["defenseMode"] = defenseMode;
    j["description"] = description;
    return j;
}

