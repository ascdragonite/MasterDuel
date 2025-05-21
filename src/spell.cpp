#include "spell.h"
#include "card.h"
#include "monstercard.h"
#include "player.h"
#include "serialize.h"
#include <ctime>
#include <fstream>
#include <iostream>
#include <vector>
#include "log_utilis.h"
#include <algorithm>
#include <random>
using namespace std;


bool Spell::ActivateEffect(Player &self, Player &opponent, int attackerIndex) {
    cout << "ts is not tricksters" << endl;
    return true; // Indicate success
}


bool OshamaScramble::ActivateEffect(Player &self,Player &opponent) { // tráo bài đối thủ
    opponent.shuffleDeck();
    cout << "[Oshama Scramble] has successfully shuffled opponent deck" << endl;
    writeLog("Opponent has used [Oshama Scramble] to shuffled your deck\n");

    // logic card

    return true; // Indicate success
}




bool ReEndOfADream::ActivateEffect(Player &self, Player &opponent) { // thêm lượt nếu hp<1000
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


bool RageofTheBlueEyes::ActivateEffect(Player& self, Player& opponent) {
    vector<Card*> newfield = self.getField();
    vector<Card*> newfieldopp;
    int countm;
    bool Ccanbeused = false;
    for(auto card : newfield){
        MonsterCard *card1 = dynamic_cast<MonsterCard *>(card);
        if(card1->getName() == "Blue-Eyes White Dragon" && card1->isFacedown() == false){
            Ccanbeused = true;
        }
    }
    if(Ccanbeused == false){
        cout << "[Rage of The Blue Eyes] Activation Failed : You do not control a face up Blue-Eyes White Dragon!" << endl;
        return false;
    }
    for(int i = 0; i<newfield.size(); i++ ){
        //MonsterCard *card1 = dynamic_cast<MonsterCard *>(newfield[i]);
        if(newfield[i]->getName() != "Blue-Eyes White Dragon" && self.hasAttacked(i) == true){
            Ccanbeused = false;
            break;
        }
        if(newfield[i]->getName() == "Blue-Eyes White Dragon" && self.hasAttacked(i) == true){
            Ccanbeused = true;
        }
    }
    if(Ccanbeused == false){
        cout << "[Rage of The Blue Eyes] Activation Failed : You can not activate this card when you already using another monster to attack!" << endl;
        return false;
    }
    if(Ccanbeused == true){
        for(int i = 0; i<newfield.size(); i++ ){
        //MonsterCard *card1 = dynamic_cast<MonsterCard *>(newfield[i]);
        if(newfield[i]->getName() == "Blue-Eyes White Dragon"){
            self.setAttacked(i, false);
        }
    }
    self.setHp(self.getHp() * 2 / 3);
    cout << "[Rage of The Blue Eyes] Successfully sacrifice 2/3 Hp to gain your Blue-Eyes White Dragon 1 more turn. Attack the enemy!" << endl;
    }
    return true;
}


bool DragonUnited::ActivateEffect(Player &self, Player &opponent) {// hiện tại đang sai logic, sửa sau
        vector<Card *> newfield = self.getField();
    int countC = 0;
    int count = 0;
    int in;

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
        for (auto card1 : newfield) {
        MonsterCard *card2 = dynamic_cast<MonsterCard *>(card1);
            if (card2 != nullptr && card2->getType() == "Monster" && card2->isFacedown() == false ) {
                count++;
            }
        }
        do{
            cout << "[Dragon United] : Choose the monster you want to buff" << endl;
            cin >> in;
            if(in < 0 || in > newfield.size()){
                cout << " Invalid Index. Please try again! " << endl;
                continue;
            }
            if (newfield[in]->getType() != "Monster") {
                cout << "Invalid Index! You need to choose a monster card" << endl;
            }
        }while(in < 0 || in > newfield.size() || newfield[in]->getType() != "Monster");

        cout << "You have " << count << " face up monster cards. Your " << newfield[in]->getName() << " gained " << 100 * count << " atk!" << endl;

        MonsterCard *card3 = dynamic_cast<MonsterCard *>(newfield[in]);

        card3->setAtk(card3->getAtk() + (100 * count));

    }
       
    return true; // Indicate success
 }

bool Destr0yer::ActivateEffect(Player& self, Player& opponent) { //xử 1 lá ở defense, bug siêu nặng đang fix???
 vector<Card *> newfield = opponent.getField();
    int countC = 0;
    for (auto card2 : newfield) {
        MonsterCard *card3 = dynamic_cast<MonsterCard *>(card2);
        if (card3 && card3->isInDefense()==true) {
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
                continue; 
            }

            card1 = dynamic_cast<MonsterCard*>(newfield[in]);

            if (!card1 || card1->isInDefense()==false) {
                cout << "[Destr0yer] Activation Failed : You need to choose a defense monster card" << endl;
            }
        }while (!card1 ||card1->isInDefense()==false);

            cout << "[Destr0yer] Successfully destroy 1 defense card : " << card1->getName() << endl;
            newfield.erase(newfield.begin() + in);
            opponent.setField(newfield);
    }
    return true; // Indicate success
}


bool DisortedFate::ActivateEffect(Player &self,Player &opponent) { // look at full deck and place a card at the top
    vector<Card *> newdeck = self.getDeck();
    vector<Card *> newdeckself;
    int c1 = 0;
    int c2 = 0;
    cout << "List of card in your deck : " << endl;
    for(auto card : newdeck){
        cout << c1 << ": " << card->getName() << endl;
        c1++;
    }
    int in;
        do {
            cout << "[Disorted Fate] : Choose the card you want to add to the top of your deck" << endl;
            cin >> in;
            if (in < 0 || in >= newdeck.size()) {
                cout << "[Disorted Fate] Invalid index!" << endl;
            }
        }while(in < 0 || in >= newdeck.size());

        for(auto card1 : newdeck){
            if(card1 != newdeck[in]){
                newdeckself.push_back(card1);
            }
    }

    newdeckself.push_back(newdeck[in]);

    cout << "[Disorted Fate] : Successfully move " << newdeck[in]->getName() << " to the top of your deck" << endl;
    cout << "List of card in your deck : " << endl;
    for(auto card2 : newdeckself){
        cout << c2 << ": " << card2->getName() << endl;
        c2++;
    }

    self.setDeck(newdeckself);

    return true; // Indicate success
}

bool DarkMagic::ActivateEffect(Player& self, Player& opponent) {
    int countm = 0;
    vector<Card*> newfield1 = self.getField();
    vector<Card*> newfield2 = opponent.getField();
    vector<Card*> newfieldopp;
    for(auto card : newfield1){
        MonsterCard *card1 = dynamic_cast<MonsterCard *>(card);
        if(card1->getName() == "Dark Magician" && card1->isFacedown() == false){
            countm++;
    }
    }
    if(countm == 0){
        cout << "[Dark Magic] Activation failed: You do not control a face up Dark Magician" << endl;
        return false;
    }
    if(countm > 0){
        for(auto card2 : newfield2){
            if(card2->getType() != "Monster"){
                newfieldopp.push_back(card2);
            }
            if(card2->getType()=="Monster"){
                cout << "[Dark Magic] Destroyed: " << card2->getName() << endl;
            } 
        }
        cout << "[Dark Magic] Activate successfully! All opponent's monster cards are destroyed. You can not attack this turn!" << endl;
        self.setSkipBattlePhaseCount(2);
        opponent.setField(newfieldopp);
    }

return true; // Indicate success

}


bool DarkBurningMagic::ActivateEffect(Player& self, Player& opponent) {
    int countdm = 0;
    int countdmg = 0;
    vector<Card*> newfield1 = self.getField();
    vector<Card*> newfield2 = opponent.getField();
    vector<Card*> newfieldopp;
    for(auto card : newfield1){
        MonsterCard *card1 = dynamic_cast<MonsterCard *>(card);
        if(card1->getName() == "Dark Magician"  && card1->isFacedown() == false){
            countdm++;
        }
        if(card1->getName() == "Dark Magician Girl" && card1->isFacedown() == false){
            countdmg++;           
        }
    }
    if(countdm == 0 || countdmg == 0){
        cout << "[Dark Magic] Activation failed: You do not control both face up Dark Magician and Dark Magician Girl" << endl;
        return false;     
    }
    if(countdm > 0 && countdmg > 0){
        for(auto card2 : newfield2){
            if(card2->getType() != "Monster"){
                newfieldopp.push_back(card2);
            }
            if(card2->getType()=="Monster"){
                cout << "[Dark Burning Magic] Destroyed: " << card2->getName() << endl;
            } 
        }
        cout << "[Dark Burning Magic] Activate successfully! All opponent's monster cards are destroyed. You can not attack in 2 turn!" << endl;
        self.setSkipBattlePhaseCount(2);
        opponent.setField(newfieldopp);
    }

return true; // Indicate success



}
bool BurstStreamofDestruction::ActivateEffect(Player& self, Player& opponent) {
    int countm = 0;
    vector<Card*> newfield1 = self.getField();
    vector<Card*> newfield2 = opponent.getField();
    vector<Card*> newfieldopp;
    for(auto card : newfield1){
        MonsterCard *card1 = dynamic_cast<MonsterCard *>(card);
        if(card1->getName() == "Blue-Eyes White Dragon"   && card1->isFacedown() == false){
            countm++;
    }
    }
    if(countm == 0){
        cout << "[Burst Stream of Destruction] Activation failed: You do not control a face up Blue-Eyes White Dragon" << endl;
        return false;
    }
    if(countm > 0){
        for(auto card2 : newfield2){
            if(card2->getType() != "Monster"){
                newfieldopp.push_back(card2);
            }
            if(card2->getType()=="Monster"){
                cout << "[Burst Stream of Destruction] Destroyed: " << card2->getName() << endl;
            } 
        }
        cout << "[Burst Stream of Destruction] Activate successfully! All opponent's monster cards are destroyed. You can not attack in 2 turn!" << endl;
        self.setSkipBattlePhaseCount(2);
        opponent.setField(newfieldopp);
    }

return true; // Indicate success

}

bool BondBetweenTheTeacherandStudent::ActivateEffect(Player& self, Player& opponent) {
    int countm = 0;
    vector<Card*> newfield = self.getField();
    vector<Card*> newdeck = self.getDeck();
    vector<Card*> newdeckself;
    bool hasDMG = false;
    for(auto card : newfield){
        MonsterCard *card1 = dynamic_cast<MonsterCard *>(card);
        if(card1->getName() == "Dark Magician" && card1->isFacedown() == false){
            countm++;
    }
    }
    if(countm == 0){
        cout << "[Bond Between The Teacher and Student] Activation failed: You do not control a face up Dark Magician" << endl;
        return false;
    }
    if(countm > 0){
        for(int i = 0; i < newdeck.size();i++){     
            if(newdeck[i]->getName() == "Dark Magician Girl"){
                MonsterCard *darkmagiciangirl = dynamic_cast<MonsterCard *>(newdeck[i]);
                //Card* darkmagiciangirl = newdeck[i];
                newdeck.erase(newdeck.begin()+i);
                darkmagiciangirl->setDefenseMode(true);
                darkmagiciangirl->setFacedown(false);
                darkmagiciangirl->setJustSummoned(true);
                newfield.push_back(darkmagiciangirl);
                hasDMG = true;  
                break;
        }
    }
        if(hasDMG == false){
            cout << "[Bond Between The Teacher and Student] Activation failed : You do not have any Dark Magician Girl in your deck" << endl;
            return false;
            } 
        
        cout << "[Bond Between The Teacher and Student] : Special Summon Dark Magician Girl successfully in defense position" << endl;
        self.setDeck(newdeck);
        self.setField(newfield);
        self.setSkipBattlePhaseCount(1);
    }
    return true; // Indicate success
}



bool ThePowerofFriendship::ActivateEffect(Player& self, Player& opponent) { //sm tình bạn, 1 hit là nằm
    int countm = 0;
    int countmo = 0;
    int newcard = 0;
    int newhp1 = 0;
    int newhp2 = 0;
    vector<Card*> newfield1 = self.getField();
    vector<Card*> newfield2 = opponent.getField();
    vector<Card*> newfieldself;
    for(auto card1 : newfield1){
        if(card1->getType() == "Monster"){
            countm++;
        }
    }
    if(self.getSkipBattlePhaseCount() > 0){
        cout << "[The Power of Friendship] Activation Failed : You can not use this card when you are not allow to attack!" << endl;
        return false;
    }
    if(countm == 0){
        cout << "[The Power of Friendship] Activation Failed : You do not have any Monster card on field" << endl;
        return false;
    }
    if(countm > 0){
        for(auto card2 : newfield1){
            MonsterCard *card3 = dynamic_cast<MonsterCard *>(card2);
            if(card3 != nullptr && card3->getType() == "Monster" && card3->isFacedown() == false){
                newcard += card3->getAtk();
            }
    }
    cout << "[The Power of Friendship] Activation Successful : Your friendship gained you " << newcard << " Attack this turn! Time to fight!" << endl;

    for(auto card4 : newfield2){
        if(card4->getType() == "Monster"){
            countmo++;
        }
    }
    
    if(countmo == 0){
        cout << "[The Power of Friendship] Opponent do not have any monster card on field. Attack directly their LP!" << endl;
        newhp1 = opponent.getHp() - newcard;
        if(newhp1 < 0){
            newhp1 = 0;
        }
        opponent.setHp(newhp1);
    }
    
    if(countmo > 0){
        int in;
        int newatk = 0;
        int newdef = 0;
        do{
        cout << "[The Power of Friendship] Choose the index of the card you want to attack : " << endl;
        cin >> in;
        if (in < 0 || in >= newfield2.size()) {
        cout << "Invalid index. Please choose again.\n";
        continue;  
        } 
        if(newfield2[in]->getType() != "Monster"){
            cout << "You need to choose a monster card" << endl;
        }
        }while(newfield2[in]->getType() != "Monster");
        MonsterCard *card5 = dynamic_cast<MonsterCard *>(newfield2[in]);
        if(card5 != nullptr && card5->getType() == "Monster"){
            if(card5->isInDefense() == true){

                cout << "[The Power of Friendship] Attack " << card5->getName() << " in defense position " << "( " << card5->getDef() << " def )" << endl;

                if(newcard > card5->getDef()){
                    delete card5;
                    cout << "[The Power of Friendship] Destroy " << card5->getName() << " with " << newcard << " atk " << endl;
                    newfield2.erase(newfield2.begin()+in);
                    opponent.setField(newfield2);
                }
                if(newcard < card5->getDef()){
                    cout << "[The Power of Friendship] Fail to destroy " << card5->getName() << " with " << newcard << " atk " << endl;
                    cout << "You lose " << card5->getDef() - newcard << " Hp instead" << endl;
                    newhp2 = self.getHp() - (card5->getDef() - newcard);
                    self.setHp(newhp2);
                }
                if(newcard == card5->getDef()){
                    cout << "[The Power of Friendship] Your power are equal! Nothing happen!" << endl;
                }
            }
            if(card5->isInDefense() == false){

                cout << "[The Power of Friendship] Attack " << card5->getName() << " in attack position " << "( " << card5->getAtk() << " atk )" << endl;

                if(newcard > card5->getAtk()){

                    cout << "[The Power of Friendship] Destroy " << card5->getName() << " with " << newcard << " atk " << endl;
                    cout << "Opponent lose " << newcard - card5->getAtk() << " Hp " << endl;
                    newfield2.erase(newfield2.begin()+in);
                    opponent.setField(newfield2);
                    newhp2 = opponent.getHp() - (newcard - card5->getAtk());
                    opponent.setHp(newhp2);
                }
                if(newcard < card5->getAtk()){
                    cout << "[The Power of Friendship] Fail to destroy " << card5->getName() << " with " << newcard << " atk " << endl;
                    cout << "All of your monster used in [The Power of Friendship] will be destroy!" << endl;
                    cout << "[The Power of Friendship] Destroy : " << endl;
                    for(auto card6 : newfield1){
                        MonsterCard *card7 = dynamic_cast<MonsterCard *>(card6);
                        if(card7 == nullptr || card7->getType() != "Monster" || card7->isFacedown() == true){
                            newfieldself.push_back(card6);
                        }
                        if(card7->getType() == "Monster" && card7->isFacedown() == false){
                            cout << card7->getName() << endl;
                        }
                    }
                    cout << "You lose " << card5->getAtk() - newcard << " Hp " << endl;
                    newhp2 = self.getHp() - (card5->getAtk() - newcard);
                    self.setHp(newhp2);
                    self.setField(newfieldself);
                }
                if(newcard == card5->getAtk()){
                    cout << "[The Power of Friendship] Power comes at an equal cost. All of the monster in this battle will be destroy!" << endl; 
                
                    cout << "[The Power of Friendship] Self Destruct : " << endl;
                    for(auto card6 : newfield1){
                        MonsterCard *card7 = dynamic_cast<MonsterCard *>(card6);
                        if(card7 == nullptr || card7->getType() != "Monster" || card7->isFacedown() == true){
                            newfieldself.push_back(card6);
                        }
                        if(card7->getType() == "Monster" && card7->isFacedown() == false){
                            cout << card7->getName() << endl;
                        }
                    }
                    self.setField(newfieldself);
                    
                    cout << "[The Power of Friendship] Opponent Destruct : " << endl;
                    cout << card5->getName() << endl;        
                    newfield2.erase(newfield2.begin()+in);
                    opponent.setField(newfield2);        
                
                }
            }

                }
            }
        }
        self.setSkipBattlePhaseCount(1);
        return true;
    }

bool RoarofTheBlueEyedDragons :: ActivateEffect(Player& self, Player& opponent) {
    vector<Card*> newfield = self.getField();
    vector<Card*> newdeck = self.getDeck();
    vector<Card*> newdeckself;
    bool hasBEWD = false;
        for(int i = 0; i < newdeck.size();i++){     
            if(newdeck[i]->getName() == "Blue-Eyes White Dragon"){
                MonsterCard *BEWD = dynamic_cast<MonsterCard *>(newdeck[i]);

                newdeck.erase(newdeck.begin()+i);
                BEWD->setDefenseMode(true);
                BEWD->setFacedown(false);
                BEWD->setJustSummoned(true);
                newfield.push_back(BEWD);
                hasBEWD = true;  
                break;
        }
    }
        if(hasBEWD == false){
            cout << "[Roar of the Blue-Eyed Dragons] Activation failed : You do not have any Blue-Eyes White Dragon in your deck" << endl;
            return false;
            } 
        
        cout << "[Roar of the Blue-Eyed Dragons] : Special Summon Blue-Eyes White Dragon successfully in defense position" << endl;
        self.setDeck(newdeck);
        self.setField(newfield);
        return true;
    }


bool MajestyofTheWhiteDragons :: ActivateEffect(Player& self, Player& opponent) {
    vector<Card*> newfield1 = self.getField();
    vector<Card*> newfield2 = opponent.getField();
    vector<Card*> newfieldopp;
    int count = 0;
    for(auto card1 : newfield1){    
        MonsterCard *card2= dynamic_cast<MonsterCard *>(card1);
        if(card2->getName() == "Blue-Eyes White Dragon" && card2->isFacedown() == false){
            count++;
        }
    }   
    if(count == 0){
        cout << "[Majesty of the White Dragons] Activation failed: You need to control at least 1 face up Blue-Eyes White Dragon" << endl;
        return false;
    }
    if(count > 0){
        if (count >= newfield2.size()){
            cout << "[Majesty of the White Dragons] You have " << count << " Blue-Eyes White Dragon. Destroy " << count << " enemy's card!" << endl;
            cout << "[Majesty of the White Dragons] Succesfully destroy all opponent's field!" << endl;
            opponent.setField(newfieldopp);
        }
        if (count < newfield2.size()){
        shuffle(newfield2.begin(), newfield2.end(), default_random_engine(time(0)));    
    
    cout << "[Majesty of the White Dragons] You have " << count << " Blue-Eyes White Dragon. Destroy " << count << " enemy's card!" << endl;
    for (int i = 0; i < count; ++i){
        cout << "[Majesty of the White Dragons] Destroy : " << newfield2[i]->getName() << endl;
    }
    newfield2.erase(newfield2.begin(), newfield2.begin() + count);
    opponent.setField(newfield2);
}
}
return true;
}


bool DarkMagicVeil :: ActivateEffect(Player& self, Player& opponent) {
    vector<Card*> newfield = self.getField();
    vector<Card*> newdeck = self.getDeck();
    vector<Card*> newdeckself;
    bool hasDM = false;
        for(int i = 0; i < newdeck.size();i++){     
            if(newdeck[i]->getName() == "Dark Magician"){
                MonsterCard *DM = dynamic_cast<MonsterCard *>(newdeck[i]);

                newdeck.erase(newdeck.begin()+i);
                DM->setDefenseMode(true);
                DM->setFacedown(false);
                DM->setJustSummoned(true);
                newfield.push_back(DM);
                hasDM = true;  
                break;
        }
    }
        if(hasDM == false){
            cout << "[Dark Magic Veil] Activation failed : You do not have any Dark Magician in your deck" << endl;
            return false;
            } 
        
        cout << "[Dark Magic Veil] : Special Summon Dark Magician successfully in defense position" << endl;
        self.setDeck(newdeck);
        self.setField(newfield);
        return true;
    }

bool ThousandKnifes::ActivateEffect(Player& self, Player& opponent) {
    int countm = 0;
    vector<Card*> newfield1 = self.getField();
    vector<Card*> newfield2 = opponent.getField();

    int in;
    for(auto card1 : newfield1){
        MonsterCard *card2 = dynamic_cast<MonsterCard *>(card1);
        if(card2->getName() == "Dark Magician" && card2->isFacedown() == false ){
            countm++;
    }
    }
    if(countm == 0){
        cout << "[Thousand Knifes] Activation failed: You do not control a face up Dark Magician" << endl;
        return false;
    }
    if(countm > 0){
        do{
            cout << "[Thousand Knifes] Choose the card you want to destroy : " << endl;
            cin >> in;

            if (in < 0 || in >= newfield2.size()) {
                cout << "[Thousand Knifes] Invalid index!" << endl; 
            }
   
        }while (in < 0 || in >= newfield2.size());

            cout << "[Thousand Knifes] Successfully destroy 1 card : " << newfield2[in]->getName() << endl;
            newfield2.erase(newfield2.begin() + in);
            opponent.setField(newfield2);
    }
return true; // Indicate success

}

bool CruelPact::ActivateEffect(Player& self, Player& opponent) {
    vector<Card*> newfield = self.getField();
    vector<Card*> newhand = self.getHand();
    vector<Card*> newdeck = self.getDeck();
    vector<Card*> newdeckself;
    bool hasDM = false;
    bool hasM = false;
    for(auto card1 : newfield){
        if(card1->getType() == "Monster"){
            hasM = true;
            break;
        }
    }
    if(hasM == false){
        cout << "[Cruel Pact] Activation Failed : You do not control any monster card!" << endl;
        return false;
    }
    for(int i = 0; i < newdeck.size();i++){     
        if(newdeck[i]->getName() == "Dark Magician"){
            MonsterCard *DM = dynamic_cast<MonsterCard *>(newdeck[i]);
            newdeck.erase(newdeck.begin()+i);
            DM->setAtk(DM->getAtk() + 200);
            newhand.push_back(DM);
            hasDM = true;  
            break;
        }
    }
    if(hasM == true && hasDM == true){
        int in;
        do{
            cout << "[Cruel Pact] Choose a monster card to tribute : " << endl;
            cin >> in;
            if(in<0 || in >=newfield.size()){
                cout << "Invalid Index. Please choose again!" << endl;
                continue;
            }
            if(newfield[in]->getType() != "Monster"){
                cout << "You need to choose a monster card. Try again!" << endl;
            }
        }while(newfield[in]->getType() != "Monster");
        
    newfield.erase(newfield.begin() + in);
    self.setHp(self.getHp() - 1000);
    self.setDeck(newdeck);
    self.setField(newfield);
    self.setHand(newhand);
    cout << "[Cruel Pact] Successfully sacrifice a monster and 1000 Hp to add a Dark Magician from your deck to your hand!" << endl;
    return true;


}
}

bool CallofTheSky::ActivateEffect(Player& self, Player& opponent) {
    vector<Card*> newfield = self.getField();
    vector<Card*> newhand = self.getHand();
    vector<Card*> newdeck = self.getDeck();
    vector<Card*> newdeckself;
    bool hasBEWD = false;
    bool hasMoTWD = false;
    int count = 0;
    for(auto card1 : newfield){
        if(card1 ->getType() == "Monster"){
            count++;
        }
    }
    if(count < 2){
        cout << "[Call of The Sky] Activation Failed : You do not control enough monster card!" << endl;
        return false;
    }
    if(count >=2){
        for(int i = 0; i < newdeck.size();i++){     
            if(newdeck[i]->getName() == "Blue-Eyes White Dragon"){
                Card* BEWD = newdeck[i];
                newdeck.erase(newdeck.begin()+i);
                newhand.push_back(BEWD);
                hasBEWD = true;  
                break;
            }
        }
        for(int i = 0; i < newdeck.size();i++){     
            if(newdeck[i]->getName() == "Majesty of The White Dragons"){
                Card* MoTWD = newdeck[i];
                newdeck.erase(newdeck.begin()+i);
                newhand.push_back(MoTWD);
                hasMoTWD = true;  
                break;
            }
        }
        if(hasMoTWD == false || hasBEWD == false){
            cout << "[Call of The Sky] Activation Failed : You do not have both Blue-Eyes White Dragon and Majesty of The White Dragons in your deck! " << endl;
            return false;
        }



        int in1;
        int in2;
        do{
            cout << "[Call of The Sky] Choose 2 monster cards to tribute :" << endl;
            cin >> in1;
            cin >> in2;

            if(in1<0 || in1 >=newfield.size() || in2 < 0 || in2 >= newfield.size()){
            cout << "Invalid Index. Please choose again!" << endl;
            continue;
            }
            if (in1 == in2) {
                cout << "You must choose two different cards. Try again!" << endl;
                continue;
            }
            if(newfield[in1]->getType() != "Monster" || newfield[in2]->getType() != "Monster"){
                cout << "You need to choose 2 monster cards. Try again!" << endl;
            }
            
        }while(newfield[in1]->getType() != "Monster" || newfield[in2]->getType() != "Monster");

        
        if(hasMoTWD == true && hasBEWD == true){
            string cardname1 = newfield[in1]->getName();
            string cardname2 = newfield[in2]->getName();
            cout << "[Call of The Sky] The Night Sky has heard your call and granted you Blue-Eyes White Dragon and Majesty of The White Dragons for the price of " << cardname1 << " and " << cardname2 << " !" << endl;
            if (in1 > in2) swap(in1, in2);
                newfield.erase(newfield.begin() + in2);
                newfield.erase(newfield.begin() + in1);
            
            self.setDeck(newdeck);
            self.setField(newfield);
            self.setHand(newhand);
            return true;    
        }
        }
        
    }
    

    bool AshAgain::ActivateEffect(Player& self, Player& opponent){
        int countm = 0;
        vector<Card*> newfield1 = self.getField();
        vector<Card*> newfield2 = opponent.getField();
        vector<Card*> newfieldopp;
        for(auto card : newfield1){
            MonsterCard *card1 = dynamic_cast<MonsterCard *>(card);
            if(card1->getName() == "Blue-Eyes White Dragon"   && card1->isFacedown() == false){
                countm++;
            }
        }
        if(countm == 0){
            cout << "[Ash Again] Activation failed: You do not control a face up Blue-Eyes White Dragon" << endl;
            return false;
        }
        if(countm > 0){
            for(auto card2 : newfield2){
                if(card2->getType() == "Monster"){
                    newfieldopp.push_back(card2);
                }
                if(card2->getType()!="Monster"){
                    cout << "[Ash Again] Destroyed: " << card2->getName() << endl;
                } 
            }
            opponent.setField(newfieldopp);
            cout << "[Ash Again] Activate successfully! All opponent's trap cards have returned to ashes. Destroy the enemy now!" << endl;
        }
    return true;
}














//trap
bool MirrorForce::ActivateEffect(Player& self, Player& opponent) {
    vector<Card*> newfield = opponent.getField();
    vector<Card*> newfieldopp;
    int countm;
    bool candestroy = false;

    for(auto card1 : newfield){
        MonsterCard *card2 = dynamic_cast<MonsterCard *>(card1);
        if(card2 != nullptr && card2->getType() == "Monster" && card2->isInDefense() == false){
            cout << "[Mirror Force] Destroyed : " << card2->getName() << endl;
            candestroy = true;
        }
        else{
            newfieldopp.push_back(card1);
        }
    }
    if(candestroy == false){
        cout << "[Mirror Force] Opponent do not have any monster in attack position" << endl;
    }
    
    opponent.setField(newfieldopp);
    return true; // Indicate success
}

bool Tsunagite::ActivateEffect(Player& self, Player& opponent) {
    cout << "[Mirror Force] End opponent battle phase!" << endl;
    opponent.setSkipBattlePhaseCount(1);
    return true; // Indicate success}
}



bool Trrricksters::ActivateEffect(Player& self, Player& opponent)
{
    cout << "wrong one bro" << endl;
    return false;
}

bool Trrricksters :: ActivateEffect(Player& self, Player& opponent, int attackerIndex){

    vector<Card*> newfield = opponent.getField();
    MonsterCard *card = dynamic_cast<MonsterCard *>(newfield[attackerIndex]);
    cout << "[Trrricksters!!] Targeting " << card->getName() << endl;
    cout << "[Trrricksters!!] Counterfire " << card->getAtk() << " atk directly to opponent's Hp!" << endl;
    opponent.setHp(opponent.getHp() - card->getAtk());
    cout << "Opponent now have " << opponent.getHp() << " Hp" << endl;
    return true;// Indicate success
}


//bool AshAgain::ActivateEffect(Player& self, Player& opponent)

