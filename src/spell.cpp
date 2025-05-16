#include "spell.h"
#include "card.h"
#include "monstercard.h"
#include "player.h"
#include "serialize.h"
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

bool OshamaScramble::ActivateEffect(Player &self,
                                    Player &opponent) { // tráo bài đối thủ
    opponent.shuffleDeck();
    cout << "[Oshama Scramble] has successfully shuffled opponent deck" << endl;

    // logic card

    return true; // Indicate success
}
/*

bool DarkMagic::ActivateEffect(Player& self, Player& opponent) {

    //logic card
    //Step 2: Implement the logic of the card here


    if (name == "Dark Magic") {
        // B1: Kiểm tra xem player có điều khiển "Dark Magician" không
        bool hasDarkMagician = false;
        for (Card* c : self.getField()) {
            if (MonsterCard*m = dynamic_cast<MonsterCard*>(c)) {
                if (m->getName() == "Dark Magician") {
                    hasDarkMagician = true;
                    break;
                }
            }
        }

        if (!hasDarkMagician) {
            cout << "[Dark Magic] Activation failed: You do not control a Dark
Magician.\n"; return false;
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
    }

    return true; // Indicate success
} */

bool ReEndOfADream::ActivateEffect(Player &self,
                                   Player &opponent) { // thêm lượt nếu hp<1000
    json j = readFromFile();
    j["ExtraTurn"] = true;

    ofstream out("game_state.json");
    if (!out) {
        cerr << "Error opening file for writing.\n";
        return false;
    }
    out << j.dump(4) << std::endl; // Pretty print with indent
    out.close();
    // logic card

    return true; // Indicate success
}
/*
bool Destroyer::ActivateEffect(Player& self, Player& opponent) { // xử 1 lá ở
thế thủ (k quan tâm stat)

}
*//*

bool RageOfTheBlueEyes ::ActivateEffect(Player& self, Player& opponent) { // hi sinh 2/3 hp khiến rồng xanh tấn công 2 lần/turn

}
*/
bool WorldVanquisher::ActivateEffect(Player &self,
                                     Player &opponent) { // buff 200 atk
    vector<Card *> newfield = self.getField();
    int countC = 0;
    for (auto card : newfield) {

        if (card->getType() == "Monster") {
            countC++;
        }
    }
    if (countC == 0) {
        cout << "[World Vanquisher] Activation Failed : You need at least 1 "
            "monster card"
            << endl;
        return false;
    }
    if (countC > 0) {
        int in;
        do {
            cout << "[World Vanquisher] : Choose the card you want to buff" << endl;
            cin >> in;
            if (newfield[in]->getType() != "Monster") {
                cout << "[World Vanquisher] Activation Failed : You need to choose a "
                    "monster card"
                    << endl;
            }
        } while (newfield[in]->getType() != "Monster");
        MonsterCard *card = dynamic_cast<MonsterCard *>(newfield[in]);
        card->setAtk(card->getAtk() + 200);
        cout << "[World Vanquisher] successfully gained " << card->getName()
            << " 200 atk" << endl;
    }
    return true; // Indicate success
}

bool FlowerSnowDrumNBass::ActivateEffect(Player &self, Player &opponent) {
    int cself = 0;
    int copp = 0;
    int r;
    vector<Card *> newdeck1 = self.getDeck();
    for (auto card1 : newdeck1) {
        cself++;
    }

    vector<Card *> newdeck2 = opponent.getDeck();
    for (auto card2 : newdeck2) {
        copp++;
    }
    cout << "[Flower Snow Drum n Bass] is chosing a player" << endl;
    srand(time(0));
    r = rand() % 2 + 1;
    if (r == 1) {
        if (cself < 2) {
            cout << "[Flower Snow Drum n Bass] Activation Failed : There is not "
                "enough card in your deck"
                << endl;
            return false;
        }
        self.drawCard();
        self.drawCard();
        cout << "[Flower Snow Drum n Bass] successfully drew you 2 cards" << endl;
    }
    if (r == 2) {
        if (copp < 2) {
            cout << "[Flower Snow Drum n Bass] Activation Failed : There is not "
                "enough card in opponent's deck"
                << endl;
            return false;
        }
        opponent.drawCard();
        opponent.drawCard();
        cout << "[Flower Snow Drum n Bass] successfully drew your opponent 2 cards"
            << endl;
    }
    return true; // Indicate success
}
