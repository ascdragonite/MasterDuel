#include "spellcard.h"
#include "player.h"
#include "monstercard.h"

#include <iostream>
using namespace std;
SpellCard :: SpellCard(string name, string description) : Card(name, "Spell", description){}

void SpellCard :: showInfo() const{
    cout << "Name: " << getName() << endl;
    cout << "Type: " << getType() << endl;
    cout << "Description: " << getDescription() << endl;
}

void SpellCard::PlayCard(vector<Card*>& field)
{
    int count = 0;
    for (Card* card : field) {
        string type = card->getType();
        if (type == "Spell" || type == "Trap") count++;
    }
    if (count < 5)
    {
        field.push_back(this);
        cout << "Spell card played: " << getName() << endl;
    }
    else
    {
        cout << "Cannot play spell card. Field is full." << endl;
    }
}


json SpellCard::toJson() const 
{
    json j;
    j["type"] = type;
    j["name"] = name;
    j["description"] = description;
    return j;
}

bool SpellCard::activateEffect(Player& self, Player& opponent) {
    if (name == "Dark Magic") {
        // B1: Kiểm tra xem player có điều khiển "Dark Magician" không
        bool hasDarkMagician = false;
        for (Card* c : self.getField()) {
            if (MonsterCard* m = dynamic_cast<MonsterCard*>(c)) {
                if (m->getName() == "Dark Magician") {
                    hasDarkMagician = true;
                    break;
                }
            }
        }

        if (!hasDarkMagician) {
            cout << "[Dark Magic] Activation failed: You do not control a Dark Magician.\n";
            return false;
        }

        // B2: Tiêu diệt toàn bộ monster bên opponent
        vector<Card*> newField;
        for (Card* c : opponent.getField()) {
            if (c->getType() != "Monster") {
                newField.push_back(c); // Giữ lại các lá khác
            } else {
                if (c != nullptr) {
                    cout << "[Dark Magic] Destroyed: " << c->getName() << endl;
                    delete c;
                }
            }
        }

        opponent.setField(newField); // Cập nhật lại field
        return true;
    }
}