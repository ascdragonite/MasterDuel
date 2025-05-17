#include "spell.h"
#include "card.h"
#include "monstercard.h"
#include "player.h"
#include "serialize.h"
#include <ctime>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

bool OshamaScramble::ActivateEffect(Player &self,Player &opponent) { // tráo bài đối thủ
    opponent.shuffleDeck();
    cout << "[Oshama Scramble] has successfully shuffled opponent deck" << endl;

    // logic card

    return true; // Indicate success
}


bool DarkMagic::ActivateEffect(Player& self, Player& opponent) {
    int countm = 0;
    vector<Card*> newfield1 = self.getField();
    vector<Card*> newfield2 = opponent.getField();
    vector<Card*> newfieldopp;
    for(auto card1 : newfield1){
        if(card1->getName() == "Dark Magician"){
            countm++;
    }
    }
    if(countm == 0){
        cout << "[Dark Magic] Activation failed: You do not control a Dark Magician" << endl;
        return false;
    }
    if(countm > 0){
        for(auto card2 : newfield2){
            if(card2->getType()!="Monster"){
                newfieldopp.push_back(card2);
            }
            if(card2->getType()=="Monster"){
                cout << "[Dark Magic] Destroyed: " << card2->getName() << endl;
            } 
        }
        cout << "[Dark Magic] Activate successfully! All opponent's monster cards are destroyed" << endl;
        opponent.setField(newfieldopp);
    }

return true; // Indicate success
}


bool ReEndOfADream::ActivateEffect(Player &self,
                                   Player &opponent) { // thêm lượt nếu hp<1000
    if (self.getHp() > 1000)
    {
        cout << "[Re:End of a Dream] HP is too high"; //Disabled for testing purposes, uncomment line below to activate
        //return false;
    }
    json j = readFromFile();
    if (j["ExtraTurn"])
    {
        cout << "[Re:End of a Dream] Already used Re:End of a Dream";
        return false;
    }
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
        cout << "[World Vanquisher] Activation Failed : You need at least 1 monster card"
            << endl;
        return false;
    }
    if (countC > 0) {
        int in;
        do {
            cout << "[World Vanquisher] : Choose the card you want to buff" << endl;
            cin >> in;
            if (newfield[in]->getType() != "Monster") {
                cout << "[World Vanquisher] Activation Failed : You need to choose a monster card" << endl;
            }
        } while (newfield[in]->getType() != "Monster");
        MonsterCard *card = dynamic_cast<MonsterCard *>(newfield[in]);
        card->setAtk(card->getAtk() + 200);
        cout << "[World Vanquisher] successfully gained " << card->getName() << " 200 atk" << endl;
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
            cout << "[Flower Snow Drum n Bass] Activation Failed : There is not enough card in your deck"<< endl;
            return false;
        }
        self.drawCard();
        self.drawCard();
        cout << "[Flower Snow Drum n Bass] successfully drew you 2 cards" << endl;
    }
    if (r == 2) {
        if (copp < 2) {
            cout << "[Flower Snow Drum n Bass] Activation Failed : There is not enough card in opponent's deck"
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
bool DragonUnited::ActivateEffect(Player &self, Player &opponent) {// hiện tại đang sai logic, sửa sau
        vector<Card *> newfield = self.getField();
    int countC = 0;
    for (auto card : newfield) {

        if (card->getType() == "Monster") {
            countC++;
        }
    }
    if (countC == 0) {
        cout << "[Dragon United] Activation Failed : You need at least 1 monster card" << endl;
        return false;
    }
    if (countC > 0) {
        for(auto card : newfield){
            if (card->getType() == "Monster"){
                MonsterCard *card1 = dynamic_cast<MonsterCard *>(card);
                if(card1 != nullptr &&card1->isFacedown()==false){
                card1->setAtk(card1->getAtk() + 100);
            cout << "[Dragon United] successfully gained " << card1->getName() << " 100 atk" << endl;
                }
    }

            }
        }
    return true; // Indicate success
} 

bool Destr0yer::ActivateEffect(Player& self, Player& opponent) {
 vector<Card *> newfield = opponent.getField();
 vector<Card*> newfieldopp;
    int countC = 0;
    for (auto card2 : newfield) {
        MonsterCard *card3 = dynamic_cast<MonsterCard *>(card2);
        if (card3->getType() == "Monster" && card3->isInDefense()==true) {
            countC++;
        }
    }
    if (countC == 0) {
        cout << "[Destr0yer] Activation Failed : Opponent need to have at least 1 monster card on defense mode"
            << endl;
        return false;
    }
    if (countC > 0) {
        int in;
        MonsterCard* card1 = nullptr;
        do {
            cout << "[Destr0yer] : Choose the card you want to destroy" << endl;
            cin >> in;

            
            if (in < 0 || in >= newfield.size()) {
                cout << "[Destr0yer] Invalid index!" << endl;
                card1 = nullptr;
                continue;  // ⬅ Tại sao?
            }

            card1 = dynamic_cast<MonsterCard*>(newfield[in]);

            if (!card1 || card1->getType() != "Monster"||card1->isInDefense()==false) {
                cout << "[Destr0yer] Activation Failed : You need to choose a defense monster card" << endl;
            }
        } while (!card1 || card1->getType() != "Monster"||card1->isInDefense()==false);
        for(auto card0 : newfield){
            if(card0 != newfield[in]){
                newfieldopp.push_back(card0);
            }
            if(card0 == newfield[in]){
                cout << "[Destr0yer] successfully destroy 1 defense card : " << card0->getName() << endl;
            }
        }
    }
    return true; // Indicate success
}
