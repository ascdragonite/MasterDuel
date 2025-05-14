#include "spell.h"
#include "player.h"
#include <iostream>
using namespace std;

bool OshamaScramble::ActivateEffect(Player& self, Player& opponent) {

    //logic card

    return true; // Indicate success
}

bool DarkMagic::ActivateEffect(Player& self, Player& opponent) {

    //logic card

    return true; // Indicate success
}

bool ReEndOfADream::ActivateEffect(Player& self, Player& opponent) {
    
    //logic card

    return true; // Indicate success
}

//Step 2: Implement the logic of the card here


    /* if (name == "Dark Magic") {
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
    } */
