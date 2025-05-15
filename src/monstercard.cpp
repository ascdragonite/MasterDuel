#include "monstercard.h"
#include "player.h"
#include <iostream>
#include "game.h"
#include "log_utilis.h"
using namespace std;

MonsterCard :: MonsterCard(string name, int atk, int def, string description, int owner) : Card(name, "Monster", description){
    this -> atk = atk;
    this -> def = def;
    this -> isSet = false;
    this -> defenseMode = false;
    this -> owner = owner;
    this -> justSummoned = false;
    
}

MonsterCard :: MonsterCard(string name, int atk, int def, string description, int owner, bool defenseMode, bool isSet) : Card(name, "Monster", description){
    this -> atk = atk;
    this -> def = def;
    this -> isSet = isSet;
    this -> defenseMode = defenseMode;
    this -> owner = owner;
    this -> justSummoned = false;
}

void MonsterCard::showInfo() const {
    cout << "Name: " << getName() << endl;
    cout << "Type: " << getType() << endl;
    cout << "Atk: " << atk << " Def: " << def << endl;

    cout << "Position: ";
    if (defenseMode) {
        cout << "Defense";
    } else {
        cout << "Attack";
    }

    if (isSet) {
        cout << " (Facedown)";
    }

    cout << endl;
}

void MonsterCard::showInfoHidden() const {
    if (isSet) {
        cout << "Facedown Defense Position Monster";
    } else {
        showInfo(); 
    }
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
    
    if (defField.empty()) {
        cout << this->getName() << " attacks directly!" << endl;
        target->takeDamage(this->getAtk());
        cout << "Attacker HP: " << self->getHp() << endl;
        cout << "Defender HP: " << target->getHp() << endl;
        return *this;
    }

    if (other.isInDefense()) {
    if (other.isFacedown()) {
        other.reveal();
    }

    if (atk > defValue) {
        writeLog(this->getName() + " destroyed a defending monster: " + other.getName());
        auto field = defField;
        field.erase(field.begin() + defendIndex);
        target->setField(field);
        other.setOwner(-1);
        return *this;
    }
    else if (atk < defValue) {
        int loss = defValue - atk;
        self->takeDamage(loss);
        writeLog(this->getName() + " failed to destroy " + other.getName() + " in defense. Took " + to_string(loss) + " damage.");
        cout << "Attacker HP: " << self->getHp() << endl;
        cout << "Attacker HP: " << target->getHp() << endl;
        return *this;
    }
    else {
        writeLog(this->getName() + " attacked " + other.getName() + " in defense. Equal ATK and DEF, no card was destroyed.");
        return *this;
    }
}

    else{

        if(atk > defValue){
            int loss = atk - defValue;
            target->takeDamage(loss);
            writeLog(this->getName() + " destroyed " + other.getName() + " and dealt " + to_string(loss) + " damage.");
            cout << "Attacker HP: " << self -> getHp() << endl;
            cout << "Attacker HP: " << target -> getHp() << endl;
            auto field = defField;
            field.erase(field.begin() + defendIndex);
            target->setField(field);
            other.setOwner(-1);
            return *this;
        }
        else if(atk < defValue){
            int loss = defValue - atk;
            self->takeDamage(loss);
            writeLog(this->getName() + " was destroyed by " + other.getName() + " and took " + to_string(loss) + " damage.");
            auto field = atkField;
            field.erase(field.begin() + attackIndex);
            self->setField(field);
            return *this;
        }
        else{
            //atk = atk
            writeLog(this->getName() + " and " + other.getName() + " destroyed each other in battle.");
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
void MonsterCard::setAtk(int atk){
    this -> atk = atk;
}
int MonsterCard::getDef() const{
    return def;
}
void MonsterCard::setDef(int def){
    this -> def = def;
}

void MonsterCard :: setDefenseMode(bool mode){
    defenseMode = mode;
}

bool MonsterCard :: isInDefense() const{
    return defenseMode;
}

void MonsterCard :: reveal(){
    isSet = false;
    justSummoned = true;
}

void MonsterCard::flipSummon() {
    if (isSet && defenseMode) {
        isSet = false;
        defenseMode = false; 
        justSummoned = true;
    }
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
            isSet = true;
        } else if (answer == 'n' || answer == 'N') {
            defenseMode = false;
        }
        field.push_back(this);
        this->justSummoned = true;
        cout << "Monster card played: " << getName() << endl;
    }
    else
    {
        cout << "Cannot play monster card. Field is full." << endl;
    }
}

bool MonsterCard::isJustSummoned() const {
    return justSummoned;
}

void MonsterCard::clearSummonFlag() {
    justSummoned = false;
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

bool MonsterCard::activateEffect(Player& self, Player& opponent) {
    cout << "[Monstercard] No effect defined.\n";
    return false;
}
