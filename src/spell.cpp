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




bool WorldVanquisher::ActivateEffect(Player &self, Player &opponent) { // buff 200 atk
    vector<Card *> newfield = self.getField();
    int countC = 0;
    for (auto card : newfield) {

        if (card->getType() == "Monster") {
            countC++;
        }
    }
    if (countC == 0) {
        cout << "[World Vanquisher] Activation Failed : You need at least 1 monster card" << endl;
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
        card->setAtk(card->getAtk() + 1000);
        cout << "[World Vanquisher] Successfully gained " << card->getName() << " 1000 atk" << endl;
        writeLog("Opponent used [World Vanquisher] and increase " + card->getName() + " 1000 atk! \n");
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
        writeLog("Opponent used [Flower Snow Drum n Bass] and drew 2 cards. Pay attention! \n" );
    }
    if (r == 2) {
        if (copp < 2) {
            cout << "[Flower Snow Drum n Bass] Activation Failed : There is not enough card in opponent's deck" << endl;
            return false;
        }
        opponent.drawCard();
        opponent.drawCard();
        cout << "[Flower Snow Drum n Bass] Successfully drew your opponent 2 cards" << endl;
        writeLog("Opponent used [Flower Snow Drum n Bass] and drew you 2 cards. Thank you, I guess? \n");
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
    self.setHp(self.getHp() * 1 / 3);
    cout << "[Rage of The Blue Eyes] Successfully sacrifice 2/3 Hp to gain your Blue-Eyes White Dragon 1 more turn. Attack the enemy!" << endl;
    writeLog("Opponent used [Rage of The Blue Eyes] to sacrifice 2/3 Hp for Blue-Eyes White Dragon's 1 more extra turn. Becareful! \n");
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

        cout << "You have " << count << " face up monster cards. Your " << newfield[in]->getName() << " gained " << 500 * count << " atk and def!" << endl;

        MonsterCard *card3 = dynamic_cast<MonsterCard *>(newfield[in]);

        card3->setAtk(card3->getAtk() + (500 * count));
        card3->setDef(card3->getDef() + (500 * count));

        writeLog("Opponent used [Dragon United] to gain " + to_string(100*count) + " atk and def for " + newfield[in]->getName() + ". Stay focus! \n");
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
        cout << "[Destr0yer] Activation Failed : Opponent need to have at least 1 monster card on defense mode" << endl;
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
            writeLog("Opponent used [Destr0yer] to destroy your " + card1->getName() + ". We have to revenge! \n");
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
        cout << "[" << c1 << "] " << card->getName() << " : " << card->getDescription() << endl;
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

    cout << "[Disorted Fate] : Successfully move " << newdeck[in]->getName() << " to the top of your deck." << endl;
    cout << "List of card in your deck : " << endl;
    for(auto card2 : newdeckself){
        cout << c2 << ": " << card2->getName() << endl;
        c2++;
    }
    writeLog("Opponent used [Disorted Fate] to move " + newdeck[in]->getName() + " to the top of there deck. What are they trying to do? \n");
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
        cout << "[Dark Magic] Activate successfully! All opponent's monster cards are destroyed. You can not attack for 2 turns!" << endl;

        writeLog("Opponent used [Dark Magic] to destroy all of your monster cards in the cost of losing 2 battle phases. Attack them now!");
        self.setSkipBattlePhaseCount(1);
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
        cout << "[Dark Burning Magic] Activate successfully! All opponent's monster cards are destroyed. You can not attack in 1 turn!" << endl;
        
        writeLog("Opponent used [Dark Burning Magic] to destroy all of your monster cards at the cost of losing 1 battle phases. Attack them! \n");
        
        self.setSkipBattlePhaseCount(1);
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
        cout << "[Burst Stream of Destruction] Activate successfully! All opponent's monster cards are destroyed. You can not attack in 1 turn!" << endl;
        self.setSkipBattlePhaseCount(1);
        writeLog("Opponent used [Burst Stream of Destruction] to destroy all of your monster cards at the cost of losing 1 battle phases. Try your best! \n");
        opponent.setField(newfieldopp);
    }

return true; // Indicate success

}

bool BondBetweenTheTeacherandStudent::ActivateEffect(Player& self, Player& opponent) {
    int countm = 0;
    vector<Card*> newfield = self.getField();
    vector<Card*> newdeck = self.getDeck();
    vector<Card*> newhand = self.getHand();
    bool hasDMG = false;
    bool hasDBM = false;
    int indexDBM = -1;

    // Kiểm tra có Dark Magician ngửa không
    for (auto card : newfield) {
        MonsterCard* card1 = dynamic_cast<MonsterCard*>(card);
        if (card1 && card1->getName() == "Dark Magician" && !card1->isFacedown()) {
            countm++;
        }
    }

    if (countm == 0) {
        cout << "[Bond Between The Teacher and Student] Activation failed: You do not control a face-up Dark Magician" << endl;
        return false;
    }

    // Tìm Dark Magician Girl và special summon
    for (int i = 0; i < static_cast<int>(newdeck.size()); i++) {
        if (newdeck[i]->getName() == "Dark Magician Girl") {
            MonsterCard* dmg = dynamic_cast<MonsterCard*>(newdeck[i]);
            if (dmg) {
                newdeck.erase(newdeck.begin() + i);
                dmg->setDefenseMode(true);
                dmg->setFacedown(false);
                dmg->setJustSummoned(true);
                newfield.push_back(dmg);
                hasDMG = true;
                break;
            }
        }
    }

    if (!hasDMG) {
        cout << "[Bond Between The Teacher and Student] Activation failed: No Dark Magician Girl in deck" << endl;
        return false;
    }

    cout << "[Bond Between The Teacher and Student] Special Summoned Dark Magician Girl in defense position!" << endl;
    writeLog("Opponent used [Bond Between The Teacher and Student] to Special Summon Dark Magician Girl in defense position. One more target!\n");

    // Kiểm tra trong deck có Dark Burning Magic không
    for (int i = 0; i < static_cast<int>(newdeck.size()); i++) {
        if (newdeck[i]->getName() == "Dark Burning Magic") {
            hasDBM = true;
            indexDBM = i;
            break;
        }
    }

    if (hasDBM) {
        char choice;
        cout << "[Bond Between The Teacher and Student] Do you want to add [Dark Burning Magic] from your deck to your hand? (y/n): ";
        cin >> choice;

        if (choice == 'y' || choice == 'Y') {
            Card* dbm = newdeck[indexDBM];
            newhand.push_back(dbm);
            newdeck.erase(newdeck.begin() + indexDBM);
            cout << "[Bond Between The Teacher and Student] Dark Burning Magic added to your hand!" << endl;
            writeLog("Opponent chose to add [Dark Burning Magic] to their hand using [Bond Between The Teacher and Student].\n");
        } else {
            cout << "[Bond Between The Teacher and Student] You chose not to add Dark Burning Magic." << endl;
        }
    } else {
        cout << "[Bond Between The Teacher and Student] You do not have Dark Burning Magic in your deck." << endl;
    }

    // Cập nhật lại game state
    self.setDeck(newdeck);
    self.setField(newfield);
    self.setHand(newhand);
    self.setSkipBattlePhaseCount(0);

    return true;
}


bool ThePowerofFriendship::ActivateEffect(Player& self, Player& opponent) { //sm tình bạn, 1 hit là nằm
    //đang dở writeLog, dài quá
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
    writeLog("Opponent used [The Power of Friendship] and gained " + to_string(newcard) + " atk in one single attack. Focus on defense! \n");
    for(auto card4 : newfield2){
        if(card4->getType() == "Monster"){
            countmo++;
        }
    }
    
    if(countmo == 0){
        cout << "[The Power of Friendship] Opponent do not have any monster card on field. Attack directly their Hp!" << endl;
        newhp1 = opponent.getHp() - newcard;
        if(newhp1 < 0){
            newhp1 = 0;
        }
        writeLog("You do not have any monster card on field. [The Power of Friendship] attacked directly to your Hp and cost you " + to_string(newcard) + " Hp \n You have " + to_string(newhp1) + " Hp left \n");
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
                    writeLog("The Power of Friendship] Destroy " + card5->getName() + " with " + to_string(newcard) + " atk. We tried our best... \n ");
                    newfield2.erase(newfield2.begin()+in);
                    opponent.setField(newfield2);
                }
                if(newcard < card5->getDef()){
                    cout << "[The Power of Friendship] Fail to destroy " << card5->getName() << " with " << newcard << " atk " << endl;
                    cout << "You lose " << card5->getDef() - newcard << " Hp instead" << endl;
                    writeLog("Opponent failed to attack you with " + to_string(newcard) + " atk using [The Power of Friendship]. That costs them " + to_string(card5->getDef() - newcard) + " Hp. How careless are they? \n" );
                    newhp2 = self.getHp() - (card5->getDef() - newcard);
                    self.setHp(newhp2);
                }
                if(newcard == card5->getDef()){
                    cout << "[The Power of Friendship] Your power are equal! Nothing happen!" << endl;
                    writeLog("[The Power of Friendship] Your power are equal! Nothing happen!");
                }
            }
            if(card5->isInDefense() == false){

                cout << "[The Power of Friendship] Attack " << card5->getName() << " in attack position " << "( " << card5->getAtk() << " atk )" << endl;

                if(newcard > card5->getAtk()){

                    cout << "[The Power of Friendship] Destroy " << card5->getName() << " with " << newcard << " atk " << endl;
                    cout << "Opponent lose " << newcard - card5->getAtk() << " Hp " << endl;
                    writeLog("Opponent destroyed " + card5->getName() + " using " + to_string(newcard) + " atk from [The Power of Friendship]. \n You lose " + to_string(newcard - card5->getAtk()) + " Hp. That's dangerous! \n" );
            
                    newfield2.erase(newfield2.begin()+in);
                    opponent.setField(newfield2);
                    newhp2 = opponent.getHp() - (newcard - card5->getAtk());
                    opponent.setHp(newhp2);
                }
                if(newcard < card5->getAtk()){
                    cout << "[The Power of Friendship] Fail to destroy " << card5->getName() << " with " << newcard << " atk " << endl;
                    cout << "All of your monster used in [The Power of Friendship] will be destroy!" << endl;
                    cout << "[The Power of Friendship] Destroy : " << endl;
                    writeLog("Even [The Power of Friendship] can not stop us! Counterattack and destroy : ");
                    for(auto card6 : newfield1){
                        MonsterCard *card7 = dynamic_cast<MonsterCard *>(card6);
                        if(card7 == nullptr || card7->getType() != "Monster" || card7->isFacedown() == true){
                            newfieldself.push_back(card6);
                        }
                        if(card7->getType() == "Monster" && card7->isFacedown() == false){
                            cout << card7->getName() << endl;
                            writeLog(card7->getName());
                        }
                    }
                    cout << "You lose " << card5->getAtk() - newcard << " Hp " << endl;
                    writeLog("They lose " + to_string(card5->getAtk() - newcard) + " Hp. How pathethic!");
                    newhp2 = self.getHp() - (card5->getAtk() - newcard);
                    self.setHp(newhp2);
                    self.setField(newfieldself);
                }
                if(newcard == card5->getAtk()){
                    cout << "[The Power of Friendship] Power comes at an equal cost. All of the monster in this battle will be destroy!" << endl; 
                
                    cout << "[The Power of Friendship] Self Destruct : " << endl;

                    writeLog("Every battle has its own consequence. Both of you and opponent lose everything in the battlefield! [The Power of Friendship] costs them : ");
                    for(auto card6 : newfield1){
                        MonsterCard *card7 = dynamic_cast<MonsterCard *>(card6);
                        if(card7 == nullptr || card7->getType() != "Monster" || card7->isFacedown() == true){
                            newfieldself.push_back(card6);
                        }
                        if(card7->getType() == "Monster" && card7->isFacedown() == false){
                            cout << card7->getName() << endl;
                            writeLog(card7->getName());
                        }
                    }
                    self.setField(newfieldself);
                    
                    cout << "[The Power of Friendship] Opponent Destruct : " << endl;
                    writeLog("We also lost : " + card5->getName());
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
        writeLog("Opponent used [Roar of the Blue-Eyed Dragons] to Special Summon Blue-Eyes White Dragon in defense position. One more target to go! \n");
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
            writeLog("Opponent used [Majesty of the White Dragons] when having " + to_string(count) + "Blue-Eyes White Dragon. They destroyed all of your card!");
            opponent.setField(newfieldopp);
        }
        if (count < newfield2.size()){
        shuffle(newfield2.begin(), newfield2.end(), default_random_engine(time(0)));    
    
    cout << "[Majesty of the White Dragons] You have " << count << " Blue-Eyes White Dragon. Destroy " << count << " enemy's card!" << endl;

    writeLog("Opponent used [Majesty of the White Dragons] when having " + to_string(count) + "Blue-Eyes White Dragon. They destroyed : \n " );

    for (int i = 0; i < count; ++i){
        cout << "[Majesty of the White Dragons] Destroy : " << newfield2[i]->getName() << endl;
        writeLog(newfield2[i]->getName() + "\n");
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
        writeLog("Opponent used [Dark Magic Veil] to Special Summon Dark Magician in defense position. Be careful! \n");
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
            writeLog("Opponent used [Thousand Knifes] to detroy " + newfield2[in]->getName() + ". Don't give up! \n");
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
            DM->setAtk(DM->getAtk() + 600);
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
        }while(newfield[in]->getType() != "Monster" || in<0 || in >=newfield.size());

        string namecard = newfield[in]->getName();
        
    newfield.erase(newfield.begin() + in);
    self.setHp(self.getHp() - 500);
    self.setDeck(newdeck);
    self.setField(newfield);
    self.setHand(newhand);
    cout << "[Cruel Pact] Successfully sacrifice " << namecard << " and 500 Hp to add a Dark Magician with 600 bonus atk from your deck to hand!" << endl;
    writeLog("Opponent signed [Cruel Pact] and sacrificed "  + namecard + " and 500 Hp to add a Dark Magician with 600 bonus atk from their deck to hand! They really go that far? \n");
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
            cout << "[Call of The Sky] The Blue Sky has heard your call and granted you Blue-Eyes White Dragon and Majesty of The White Dragons for the price of " << cardname1 << " and " << cardname2 << " !" << endl;
            writeLog("Opponent used [Call of The Sky]. The Blue Sky choose to help them with Blue-Eyes White Dragon and Majesty of The White Dragons for the price of " + cardname1 + " and " + cardname2);
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
    

    bool AshAgain::ActivateEffect(Player& self, Player& opponent){ //chưa thêm writeLog do chưa nghĩ ra tên
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
            writeLog("Opponent used [Ash Again] to disable all trap cards in our field. We can not fool them.");
        }
    return true;
}
/*
bool TheAncientKnowledge::ActivateEffect(Player& self, Player& opponent) {
    vector<Card*> newDeck = self.getDeck();
    vector<Card*> newHand = self.getHand();

    vector<pair<int, Card*>> validCards;

    for (int i = 0; i < static_cast<int>(newDeck.size()); ++i) {
        string desc = newDeck[i]->getDescription();
        if (desc.find("Dark Magician") != string::npos) {
            validCards.push_back({i, newDeck[i]});
        }
    }

    if (validCards.empty()) {
        cout << "[The Ancient Knowledge] Activation Failed: No card mentions 'Dark Magician' in your deck!" << endl;
        return false;
    }

    cout << "[The Ancient Knowledge] Cards that mention 'Dark Magician':" << endl;
    for (int i = 0; i < static_cast<int>(validCards.size()); ++i) {
        cout << "  [" << i << "] " << validCards[i].second->getName()
             << " - " << validCards[i].second->getDescription() << endl;
    }

    int choice = -1;
    do {
        cout << "Choose one card to add to your hand: ";
        cin >> choice;

        if (choice < 0 || choice >= static_cast<int>(validCards.size())) {
            cout << "Invalid choice. Please try again." << endl;
            choice = -1;
        }
    } while (choice == -1);

    int deckIndex = validCards[choice].first;
    Card* selectedCard = newDeck[deckIndex];
    newHand.push_back(selectedCard);
    newDeck.erase(newDeck.begin() + deckIndex);

    self.setDeck(newDeck);
    self.setHand(newHand);

    self.shuffleDeck();

    cout << "[The Ancient Knowledge] You have added " << selectedCard->getName()
         << " to your hand! The deck has been shuffled." << endl;

    writeLog("Opponent activated [The Ancient Knowledge], added " +
             selectedCard->getName() + " to their hand, then shuffled the deck.");

    return true;
}

bool SoulServant::ActivateEffect(Player& self, Player& opponent) {
    vector<Card*> deck = self.getDeck();
    vector<Card*> field = self.getField();
    vector<Card*> newDeck;

    if (deck.empty()) {
        cout << "[Soul Servant] Your deck is empty. Activation failed." << endl;
        return false;
    }

    int selectedIndex = -1;

    while (true) {
        cout << "[Soul Servant] Choose a card that mentions 'Dark Magician':" << endl;
        for (int i = 0; i < deck.size(); ++i) {
            cout << "[" << i << "] " << deck[i]->getName() << " - " << deck[i]->getDescription() << endl;
        }

        cout << "Index: ";
        cin >> selectedIndex;

        if (selectedIndex < 0 || selectedIndex >= deck.size()) {
            cout << "[Soul Servant] Invalid index. Try again." << endl;
            continue;
        }

        if (deck[selectedIndex]->getDescription().find("Dark Magician") == string::npos) {
            cout << "[Soul Servant] That card does NOT mention 'Dark Magician'. Try again." << endl;
            continue;
        }

        break;
    }

    Card* selectedCard = deck[selectedIndex];

    for (int i = 0; i < deck.size(); ++i) {
        if (i != selectedIndex) newDeck.push_back(deck[i]);
    }
    newDeck.push_back(selectedCard); 

    self.setDeck(newDeck);

    cout << "[Soul Servant] Placed " << selectedCard->getName() << " on top of your deck." << endl;
    writeLog("Opponent used [Soul Servant] to place " + selectedCard->getName() + " to the top of their deck.");

    int count = 0;
    for (Card* c : field) {
        if (c->getType() == "Monster" &&
            c->getDescription().find("Dark Magician") != string::npos) {
            count++;
        }
    }

    int drawAmount = min(count, 2);
    for (int i = 0; i < drawAmount; ++i) {
        self.drawCard();
    }

    cout << "[Soul Servant] You control " << count << " monster(s) that mention 'Dark Magician'. You draw " << drawAmount << " card(s)." << endl;
    writeLog("Opponent drew " + to_string(drawAmount) + " card(s) using [Soul Servant].");

    return true;
}

bool EnternalSoul::ActivateEffect(Player& self, Player& opponent) {
    vector<Card*> hand = self.getHand();
    vector<Card*> deck = self.getDeck();
    vector<Card*> field = self.getField();

    // Tìm các bài trên tay có mention "Dark Magician"
    vector<int> revealableIndexes;
    for (int i = 0; i < hand.size(); ++i) {
        if (hand[i]->getDescription().find("Dark Magician") != string::npos) {
            revealableIndexes.push_back(i);
        }
    }

    if (revealableIndexes.empty()) {
        cout << "[Enternal Soul] Activation failed: You must reveal a card in your hand that mentions 'Dark Magician'." << endl;
        return false;
    }

    cout << "[Enternal Soul] Reveal a card from your hand that mentions 'Dark Magician':" << endl;
    for (int i = 0; i < revealableIndexes.size(); ++i) {
        cout << "[" << i << "] " << hand[revealableIndexes[i]]->getName() << endl;
    }

    int revealChoice = -1;
    do {
        cout << "Choose index to reveal: ";
        cin >> revealChoice;
    } while (revealChoice < 0 || revealChoice >= revealableIndexes.size());

    int handIndex = revealableIndexes[revealChoice];
    Card* revealedCard = hand[handIndex];
    cout << "[Enternal Soul] You revealed: " << revealedCard->getName() << endl;

    // Xây dựng danh sách summon từ hand + deck
    vector<pair<Card*, string>> summonables;

    for (Card* c : deck) {
        if (c->getType() == "Monster" && c->getDescription().find("Dark Magician") != string::npos) {
            summonables.emplace_back(c, "deck");
        }
    }
    for (Card* c : hand) {
        if (c->getType() == "Monster" && c->getDescription().find("Dark Magician") != string::npos) {
            summonables.emplace_back(c, "hand");
        }
    }

    if (summonables.empty()) {
        cout << "[Enternal Soul] No valid monsters in your hand or deck to summon." << endl;
        return false;
    }

    cout << "[Enternal Soul] Choose up to 2 monsters to special summon (from deck or hand):" << endl;
    for (int i = 0; i < summonables.size(); ++i) {
        cout << "[" << i << "] " << summonables[i].first->getName()
             << " (" << summonables[i].second << ")" << endl;
    }

    vector<Card*> summoned;
    int choice1 = -1, choice2 = -1;

    cout << "First index: ";
    cin >> choice1;
    if (choice1 < 0 || choice1 >= summonables.size()) return false;

    Card* c1 = summonables[choice1].first;
    string src1 = summonables[choice1].second;
    MonsterCard* m1 = dynamic_cast<MonsterCard*>(c1);
    m1->setDefenseMode(true);
    m1->setFacedown(false);
    m1->setJustSummoned(true);
    summoned.push_back(m1);

    if (src1 == "deck") {
        deck.erase(remove(deck.begin(), deck.end(), c1), deck.end());
    } else {
        hand.erase(remove(hand.begin(), hand.end(), c1), hand.end());
    }

    // Second summon
    if (summonables.size() >= 2) {
        cout << "Second index (optional, -1 to skip): ";
        cin >> choice2;
        if (choice2 != -1 && choice2 >= 0 && choice2 < summonables.size() && choice2 != choice1) {
            Card* c2 = summonables[choice2].first;
            string src2 = summonables[choice2].second;
            MonsterCard* m2 = dynamic_cast<MonsterCard*>(c2);
            m2->setDefenseMode(true);
            m2->setFacedown(false);
            m2->setJustSummoned(true);
            summoned.push_back(m2);

            if (src2 == "deck") {
                deck.erase(remove(deck.begin(), deck.end(), c2), deck.end());
            } else {
                hand.erase(remove(hand.begin(), hand.end(), c2), hand.end());
            }
        }
    }

    // Add lên field
    for (Card* c : summoned) {
        field.push_back(c);
    }

    // Cập nhật lại bộ bài, tay, sân
    self.setDeck(deck);
    self.setHand(hand);
    self.setField(field);

    if (!summoned.empty()) {
        cout << "[Enternal Soul] You summoned: ";
        for (int i = 0; i < summoned.size(); ++i) {
            cout << summoned[i]->getName();
            if (i != summoned.size() - 1) cout << " and ";
        }
        cout << "." << endl;
    }

    // Check phần thưởng nếu summon được cả Rod + Soul
    bool hasRod = false, hasSoul = false;
    for (Card* c : summoned) {
        if (c->getName() == "Magicians Rod") hasRod = true;
        if (c->getName() == "Magicians Soul") hasSoul = true;
    }

    if (hasRod && hasSoul) {
        for (int i = 0; i < 2; ++i) {
            self.drawCard();
        }
        cout << "[Enternal Soul] You summoned both Rod and Soul, draw 2 cards successfully!" << endl;
    }

    // Ghi log
    string logMsg = "[Enternal Soul] opens the gate — ";
    for (int i = 0; i < summoned.size(); ++i) {
        logMsg += summoned[i]->getName();
        if (i != summoned.size() - 1) logMsg += " and ";
    }
    logMsg += " emerged through eternal magic.";
    writeLog(logMsg);

    return true;
}

bool IllusionMagic::ActivateEffect(Player& self, Player& opponent) {
    vector<Card*> field = self.getField();
    vector<Card*> hand = self.getHand();
    vector<Card*> deck = self.getDeck();

    vector<int> tributeCandidates;
    for (int i = 0; i < field.size(); ++i) {
        if (field[i]->getType() == "Monster" &&
            field[i]->getDescription().find("Dark Magician") != string::npos) {
            tributeCandidates.push_back(i);
        }
    }

    if (tributeCandidates.empty()) {
        cout << "[Illusion Magic] Activation Failed: You have no monster on the field that mentions 'Dark Magician'." << endl;
        return false;
    }

    cout << "[Illusion Magic] Choose 1 monster that mentions 'Dark Magician' to tribute:" << endl;
    for (int i = 0; i < tributeCandidates.size(); ++i) {
        int idx = tributeCandidates[i];
        cout << "[" << i << "] " << field[idx]->getName() << " - " << field[idx]->getDescription() << endl;
    }

    int tributeChoice = -1;
    do {
        cout << "Index: ";
        cin >> tributeChoice;
    } while (tributeChoice < 0 || tributeChoice >= tributeCandidates.size());

    int fieldIndex = tributeCandidates[tributeChoice];
    string tributedName = field[fieldIndex]->getName();
    field.erase(field.begin() + fieldIndex);


    vector<int> validIndexes;
    for (int i = 0; i < deck.size(); ++i) {
        if (deck[i]->getType() == "Monster" &&
            deck[i]->getDescription().find("Dark Magician") != string::npos) {
            validIndexes.push_back(i);
        }
    }

    if (validIndexes.size() < 2) {
        cout << "[Illusion Magic] Failed: Not enough valid monsters in deck that mention 'Dark Magician'." << endl;
        return false;
    }

    cout << "[Illusion Magic] Choose 2 monster cards from your deck that mention 'Dark Magician':" << endl;
    for (int i = 0; i < validIndexes.size(); ++i) {
        cout << "[" << i << "] " << deck[validIndexes[i]]->getName()
             << " - " << deck[validIndexes[i]]->getDescription() << endl;
    }

    int index1 = -1, index2 = -1;
    do {
        cout << "First index: ";
        cin >> index1;
        cout << "Second index: ";
        cin >> index2;

        if (index1 == index2) {
            cout << "You must choose two different cards." << endl;
        }

    } while (
        index1 < 0 || index1 >= validIndexes.size() ||
        index2 < 0 || index2 >= validIndexes.size() ||
        index1 == index2
    );

    int deckIndex1 = validIndexes[index1];
    int deckIndex2 = validIndexes[index2];

    Card* card1 = deck[deckIndex1];
    Card* card2 = deck[deckIndex2];

    hand.push_back(card1);
    hand.push_back(card2);

    if (deckIndex1 > deckIndex2) {
        deck.erase(deck.begin() + deckIndex1);
        deck.erase(deck.begin() + deckIndex2);
    } else {
        deck.erase(deck.begin() + deckIndex2);
        deck.erase(deck.begin() + deckIndex1);
    }

    cout << "[Illusion Magic] You tributed " << tributedName
         << " and added " << card1->getName() << " and " << card2->getName() << " to your hand!" << endl;

    writeLog("Opponent activated [Illusion Magic]. Tributed " + tributedName +
             ", added " + card1->getName() + " and " + card2->getName() + " to their hand.");

    self.setField(field);
    self.setDeck(deck);
    self.setHand(hand);
    return true;
}

bool ApprenticeHelper::ActivateEffect(Player& self, Player& opponent) {
    vector<Card*> newfield = self.getField();
    vector<Card*> newdeck = self.getDeck();
    bool hasApprentice = false;

    for (int i = 0; i < newdeck.size(); i++) {
        if (newdeck[i]->getName() == "Magicians Apprentice") {
            MonsterCard* apprentice = dynamic_cast<MonsterCard*>(newdeck[i]);
            newdeck.erase(newdeck.begin() + i);
            apprentice->setDefenseMode(true);
            apprentice->setFacedown(false);
            apprentice->setJustSummoned(true);
            newfield.push_back(apprentice);
            hasApprentice = true;
            break;
        }
    }

    if (!hasApprentice) {
        cout << "[Apprentice Helper] Activation failed: No 'Magicians Apprentice' in your deck." << endl;
        return false;
    }

    cout << "[Apprentice Helper] Special Summon 'Magicians Apprentice' successfully in defense position." << endl;

    // Tính số lá rút
    int drawCount = 0;
    for (Card* c : newfield) {
        if (c->getType() == "Monster" &&
            c->getDescription().find("Dark Magician") != string::npos) {
            drawCount++;
        }
    }

    if (drawCount > 2) drawCount = 2;

    for (int i = 0; i < drawCount; ++i) {
        self.drawCard();
    }

    if (drawCount > 0) {
        cout << "[Apprentice Helper] You control monster(s) that mention 'Dark Magician', so you draw " << drawCount << " card(s)." << endl;
    } else {
        cout << "[Apprentice Helper] No monster you control mentions 'Dark Magician', you draw nothing." << endl;
    }

    writeLog("A call was made through [Apprentice Helper] — 'Magicians Apprentice' appeared, and " + to_string(drawCount) + " scroll(s) of wisdom were drawn.");

    self.setDeck(newdeck);
    self.setField(newfield);
    return true;
}

bool WishesforEyesofBlue::ActivateEffect(Player& self, Player& opponent) {
    vector<Card*> hand = self.getHand();
    vector<Card*> deck = self.getDeck();
    vector<Card*> field = self.getField();

    vector<int> revealableIndexes;
    for (int i = 0; i < hand.size(); ++i) {
        if (hand[i]->getDescription().find("Blue Eyes") != string::npos) {
            revealableIndexes.push_back(i);
        }
    }

    if (revealableIndexes.empty()) {
        cout << "[Wishes for Eyes of Blue] Activation failed: You must reveal a card in your hand that mentions 'Blue Eyes'." << endl;
        return false;
    }

    cout << "[Wishes for Eyes of Blue] Reveal a card from your hand that mentions 'Blue Eyes':" << endl;
    for (int i = 0; i < revealableIndexes.size(); ++i) {
        cout << "[" << i << "] " << hand[revealableIndexes[i]]->getName() << endl;
    }

    int revealChoice = -1;
    do {
        cout << "Choose index to reveal: ";
        cin >> revealChoice;
    } while (revealChoice < 0 || revealChoice >= revealableIndexes.size());

    Card* revealedCard = hand[revealableIndexes[revealChoice]];
    cout << "[Wishes for Eyes of Blue] You revealed: " << revealedCard->getName() << endl;

    // Tìm các quái vật có mô tả chứa "Blue Eyes"
    vector<int> validIndexes;
    for (int i = 0; i < deck.size(); ++i) {
        if (deck[i]->getType() == "Monster" &&
            deck[i]->getDescription().find("Blue Eyes") != string::npos) {
            validIndexes.push_back(i);
        }
    }

    if (validIndexes.empty()) {
        cout << "[Wishes for Eyes of Blue] No valid monsters in your deck to summon." << endl;
        return false;
    }

    cout << "[Wishes for Eyes of Blue] Choose up to 2 monsters to summon:" << endl;
    for (int i = 0; i < validIndexes.size(); ++i) {
        cout << "[" << i << "] " << deck[validIndexes[i]]->getName() << endl;
    }

    vector<Card*> summoned;
    int choice1 = -1, choice2 = -1;

    cout << "First index: ";
    cin >> choice1;
    if (choice1 < 0 || choice1 >= validIndexes.size()) return false;

    int index1 = validIndexes[choice1];
    MonsterCard* m1 = dynamic_cast<MonsterCard*>(deck[index1]);
    m1->setDefenseMode(true);
    m1->setFacedown(false);
    m1->setJustSummoned(true);
    summoned.push_back(m1);
    deck.erase(deck.begin() + index1);

    validIndexes.clear();
    for (int i = 0; i < deck.size(); ++i) {
        if (deck[i]->getType() == "Monster" &&
            deck[i]->getDescription().find("Blue Eyes") != string::npos) {
            validIndexes.push_back(i);
        }
    }

    if (validIndexes.size() >= 1) {
        cout << "Second index (optional, -1 to skip): ";
        cin >> choice2;
        if (choice2 != -1 && choice2 >= 0 && choice2 < validIndexes.size()) {
            int index2 = validIndexes[choice2];
            MonsterCard* m2 = dynamic_cast<MonsterCard*>(deck[index2]);
            m2->setDefenseMode(true);
            m2->setFacedown(false);
            m2->setJustSummoned(true);
            summoned.push_back(m2);
            deck.erase(deck.begin() + index2);
        }
    }

    for (Card* c : summoned) {
        field.push_back(c);
    }

    self.setDeck(deck);
    self.setField(field);
    self.setHand(hand);  

    if (!summoned.empty()) {
        cout << "[Wishes for Eyes of Blue] You summoned: ";
        string logMsg = "A silent wish echoed through the skies... [Wishes for Eyes of Blue] was activated — ";
        for (int i = 0; i < summoned.size(); ++i) {
        logMsg += summoned[i]->getName();
        if (i != summoned.size() - 1) logMsg += " and ";
    }
    logMsg += " descended from the deck in defense position.";
    writeLog(logMsg);
    }

    return true;
}


bool Overdose::ActivateEffect(Player& self, Player& opponent) {
    vector<Card*> newfield = self.getField();
    int countBE = 0;
        for (auto card1 : newfield) {
        if (card1->getType() == "Monster" && card1->getDescription().find("Blue Eyes") != string::npos) {
            countBE++;
        }
    }
    if(countBE == 0){
        cout << "[Overdose] Activation Failed : You do not control any Blue Eyes Monster!" << endl;
        return false;
    }
    if(countBE > 0){
        int minushp;
        int in;
        int newatk;
        int hp = self.getHp();
        do{
            cout << "[Overdose] Choose the monster you want to buff : " << endl;
            cin >> in;
            if(in<0 || in >= newfield.size()){
                cout << "Invalid Index! Choose again" << endl;
                continue;
            }
            if(newfield[in]->getType() != "Monster" || newfield[in]->getDescription().find("Blue Eyes") == string::npos){
                cout << "Invalid Index! You have to choose a monster card which has mentioned 'Blue Eyes'" << endl;
            }
        }while(newfield[in]->getType() != "Monster" || newfield[in]->getDescription().find("Blue Eyes") == string::npos);
        do{
            cout << "[Overdose] Choose the amount of Hp you want to exchange : " << endl;
            cin >> minushp;
            if(minushp < 1 || minushp > (hp / 2)){
                cout << "Invalid amount! You can not exchange more than half of your current Hp ( " << hp << " ) or less than 1 hp " << endl;
            }
        }while(minushp < 1 || minushp > (hp / 2));
        MonsterCard* card2 = dynamic_cast<MonsterCard*>(newfield[in]);
        newatk = card2->getAtk() + minushp;
        cout << "[Overdose] Succesfully exchanged " << minushp << " Hp for the same amount of atk" << endl;
        cout << "[Overdose] The monster gained the effect was : " << newfield[in]->getName() << endl;
        writeLog("Opponent activated [Overdose], sacrificing " + to_string(minushp) + " LP to empower '" + newfield[in]->getName() + "' to " + to_string(newatk) + " ATK — at the price of their own life force.");
        card2->setAtk(newatk);
        card2->setFacedown(false);
        self.setHp(hp - minushp);
    }
    return true;
}


bool ChaosMagic::ActivateEffect(Player& self, Player& opponent) {
    vector<Card*> newfield = self.getField();
    vector<Card*> newdeck = self.getDeck();
    vector<Card*> newhand = self.getHand();
    bool hasDMOC = false;
    bool hasBLS = false;
    vector<int> tributeIndexes;

    for (int i = 0; i < newfield.size(); ++i) {
        if (newfield[i]->getType() == "Monster" &&
            newfield[i]->getDescription().find("Dark Magician") != string::npos) {
            tributeIndexes.push_back(i);
        }
    }

    if (tributeIndexes.size() < 2) {
        cout << "[Chaos Magic] Activation failed: You do not control at least 2 monsters that mention 'Dark Magician'." << endl;
        return false;
    }

    int in1, in2;
    do {
        cout << "[Chaos Magic] Choose 2 monsters that mention 'Dark Magician' to tribute:" << endl;
        for (int i = 0; i < tributeIndexes.size(); ++i) {
            cout << "[" << i << "] " << newfield[tributeIndexes[i]]->getName() << endl;
        }
        cin >> in1 >> in2;

        if (in1 == in2 || in1 < 0 || in1 >= tributeIndexes.size() || in2 < 0 || in2 >= tributeIndexes.size()) {
            cout << "Invalid input. Try again." << endl;
        }
    } while (in1 == in2 || in1 < 0 || in1 >= tributeIndexes.size() || in2 < 0 || in2 >= tributeIndexes.size());

    int idx1 = tributeIndexes[in1];
    int idx2 = tributeIndexes[in2];

    if (idx1 > idx2) {
        newfield.erase(newfield.begin() + idx1);
        newfield.erase(newfield.begin() + idx2);
    } else {
        newfield.erase(newfield.begin() + idx2);
        newfield.erase(newfield.begin() + idx1);
    }

    // Tìm và triệu hồi DMOC và BLS từ deck
    for (int i = 0; i < newdeck.size(); ++i) {
        if (!hasDMOC && newdeck[i]->getName() == "Dark Magician of Chaos") {
            MonsterCard* dmoc = dynamic_cast<MonsterCard*>(newdeck[i]);
            if (dmoc) {
                newdeck.erase(newdeck.begin() + i);
                dmoc->setDefenseMode(true);
                dmoc->setFacedown(false);
                dmoc->setJustSummoned(true);
                newfield.push_back(dmoc);
                hasDMOC = true;
            }
            break;
        }
    }

    for (int i = 0; i < newdeck.size(); ++i) {
        if (!hasBLS && newdeck[i]->getName() == "Black Luster Soldier") {
            MonsterCard* bls = dynamic_cast<MonsterCard*>(newdeck[i]);
            if (bls) {
                newdeck.erase(newdeck.begin() + i);
                bls->setDefenseMode(true);
                bls->setFacedown(false);
                bls->setJustSummoned(true);
                newfield.push_back(bls);
                hasBLS = true;
            }
            break;
        }
    }

    // Tìm trong tay nếu thiếu
    if (!hasDMOC) {
        for (int i = 0; i < newhand.size(); ++i) {
            if (newhand[i]->getName() == "Dark Magician of Chaos") {
                MonsterCard* dmoc = dynamic_cast<MonsterCard*>(newhand[i]);
                if (dmoc) {
                    newhand.erase(newhand.begin() + i);
                    dmoc->setDefenseMode(true);
                    dmoc->setFacedown(false);
                    dmoc->setJustSummoned(true);
                    newfield.push_back(dmoc);
                    hasDMOC = true;
                }
                break;
            }
        }
    }

    if (!hasBLS) {
        for (int i = 0; i < newhand.size(); ++i) {
            if (newhand[i]->getName() == "Black Luster Soldier") {
                MonsterCard* bls = dynamic_cast<MonsterCard*>(newhand[i]);
                if (bls) {
                    newhand.erase(newhand.begin() + i);
                    bls->setDefenseMode(true);
                    bls->setFacedown(false);
                    bls->setJustSummoned(true);
                    newfield.push_back(bls);
                    hasBLS = true;
                }
                break;
            }
        }
    }

    if (!hasDMOC || !hasBLS) {
        cout << "[Chaos Magic] Activation failed: Missing Dark Magician of Chaos or Black Luster Soldier in your deck or hand." << endl;
        return false;
    }

    // Tìm spell bonus
    bool addedChaosPower = false;
    for (int i = 0; i < newdeck.size(); ++i) {
        if (newdeck[i]->getName() == "The True Power of Chaos Dual") {
            Card* spell = newdeck[i];
            newdeck.erase(newdeck.begin() + i);
            newhand.push_back(spell);
            addedChaosPower = true;
            cout << "[Chaos Magic] You also added 'The True Power of Chaos Dual' to your hand!" << endl;
            writeLog("Two legendary forces rise. As [Chaos Magic] concludes, 'The True Power of Chaos Dual' is drawn. The age of destruction begins.");
            break;
        }
    }

    cout << "[Chaos Magic] You tributed 2 monsters to Special Summon 'Dark Magician of Chaos' and 'Black Luster Soldier' in defense position!" << endl;
    writeLog("The battlefield shook as [Chaos Magic] was activated. DMOC and BLS returned from chaos — summoned in defense, yet brimming with power.");

    if (!addedChaosPower) {
        cout << "[Chaos Magic] Summon succeeded, but 'The True Power of Chaos Dual' was not found in your deck." << endl;
    }

    self.setDeck(newdeck);
    self.setHand(newhand);
    self.setField(newfield);
    return true;
}



bool TheWorldDestroyer::ActivateEffect(Player& self, Player& opponent) {
    vector<Card*> hand = self.getHand();
    vector<Card*> fieldSelf = self.getField();
    vector<Card*> fieldOpp = opponent.getField();

    vector<int> revealableIndexes;
    for (int i = 0; i < hand.size(); ++i) {
        if (hand[i]->getDescription().find("Dark Magician") != string::npos) {
            revealableIndexes.push_back(i);
        }
    }

    if (revealableIndexes.empty()) {
        cout << "[The World Destroyer] Activation failed: You must reveal a card that mentions 'Dark Magician'." << endl;
        return false;
    }

    cout << "[The World Destroyer] Reveal a card that mentions 'Dark Magician':" << endl;
    for (int i = 0; i < revealableIndexes.size(); ++i) {
        cout << "[" << i << "] " << hand[revealableIndexes[i]]->getName() << endl;
    }

    int choice = -1;
    do {
        cout << "Choose index to reveal: ";
        cin >> choice;
    } while (choice < 0 || choice >= revealableIndexes.size());

    Card* revealedCard = hand[revealableIndexes[choice]];
    cout << "[The World Destroyer] You revealed: " << revealedCard->getName() << endl;

    MonsterCard* gandora = nullptr;
    for (Card* c : fieldSelf) {
        if (c->getName() == "Gandora-X the Dragon of Demolition") {
            MonsterCard* mc = dynamic_cast<MonsterCard*>(c);
            if (mc && !mc->isFacedown()) {
                gandora = mc;
                break;
            }
        }
    }

    if (!gandora) {
        cout << "[The World Destroyer] Activation failed: You do not control a face-up 'Gandora-X the Dragon of Demolition'." << endl;
        return false;
    }

    int halfHp = self.getHp() / 2;
    self.takeDamage(halfHp);
    cout << "[The World Destroyer] You paid half your LP (" << halfHp << " LP) to activate the effect." << endl;

    int destroyedCount = 0;
    vector<Card*> updatedFieldSelf;

    for (Card* c : fieldSelf) {
        if (c == gandora) {
            updatedFieldSelf.push_back(c);
            continue;
        }
        cout << "[The World Destroyer] Destroyed (your side): " << c->getName() << endl;
        destroyedCount++;
    }

    for (Card* c : fieldOpp) {
        cout << "[The World Destroyer] Destroyed (opponent's side): " << c->getName() << endl;
        destroyedCount++;
    }

    self.setField(updatedFieldSelf);
    opponent.setField(vector<Card*>());

    int atkGain = destroyedCount * 500;
    int burnDmg = destroyedCount * 500;

    gandora->setAtk(gandora->getAtk() + atkGain);
    opponent.takeDamage(burnDmg);

    cout << "[The World Destroyer] Gandora gained " << atkGain << " ATK!" << endl;
    cout << "[The World Destroyer] Opponent took " << burnDmg << " damage from Gandora's wrath!" << endl;

    self.setSkipBattlePhaseCount(1);
    cout << "[The World Destroyer] You must skip your Battle Phase this turn." << endl;

    writeLog("Opponent activated [The World Destroyer].");
    writeLog("They revealed '" + revealedCard->getName());
    writeLog("Paid half their LP...");
    writeLog("Every card burned to dust. Gandora alone remains.");
    writeLog("Gandora grew stronger — +" + to_string(atkGain) + " ATK — feeding on everyone losses.");
    writeLog("You took " + to_string(burnDmg) + " damage.");
    writeLog("They skipped their Battle Phase.");
    writeLog("Get up. You're still in the game. For now.");

    return true;
}

bool TheTruePowerOfChaosDual::ActivateEffect(Player& self, Player& opponent) {
    vector<Card*> fieldSelf = self.getField();
    vector<Card*> fieldOpp = opponent.getField();

    int mentionCount = 0;
    for (Card* c : fieldSelf) {
        if (c->getType() == "Monster" &&
            c->getDescription().find("Dark Magician") != string::npos) {
            mentionCount++;
        }
    }

    if (mentionCount == 0) {
        cout << "[The True Power of Chaos Dual] Activation failed: You control no monsters that mention 'Dark Magician'." << endl;
        return false;
    }

    if (fieldOpp.empty()) {
        cout << "[The True Power of Chaos Dual] Activation failed: Opponent has no cards on the field." << endl;
        return false;
    }

    int destroyCount = min(mentionCount, static_cast<int>(fieldOpp.size()));

    shuffle(fieldOpp.begin(), fieldOpp.end(), default_random_engine(time(0)));

    cout << "[The True Power of Chaos Dual] You control " << mentionCount
         << " monster(s) that mention 'Dark Magician'. Destroying " << destroyCount << " opponent's card(s) at random!" << endl;

    writeLog("[The True Power of Chaos Dual] was unleashed — obliteration sequence initiated. Destroyed:");
     for (int i = 0; i < destroyCount; ++i) {
    writeLog("  - " + fieldOpp[i]->getName());
    }


    fieldOpp.erase(fieldOpp.begin(), fieldOpp.begin() + destroyCount);

 
    for (Card* c : fieldSelf) {
    if (c->getType() == "Monster") {
        string name = c->getName();
        if (name == "Black Luster Soldier" || name == "Dark Magician of Chaos") {
            MonsterCard* mc = dynamic_cast<MonsterCard*>(c);
            if (mc) {
                mc->setAtk(mc->getAtk() + 100 * destroyCount);
                cout << "[ATK Boost] " << mc->getName() << " gains +" << (100 * destroyCount) << " ATK!" << endl;
            }
        }
    }
}

    cout << "[The True Power of Chaos Dual] You must skip your Battle Phase this turn." << endl;
    writeLog("Status: [The Chaos Dual] empowered by destruction. Gained +" + to_string(100 * destroyCount) + " ATK.");
    writeLog("Notice: Opponent's Battle Phase forcibly skipped.");

    self.setField(fieldSelf);
    opponent.setField(fieldOpp);
    self.setSkipBattlePhaseCount(1);
    return true;
}

bool TrueLight::ActivateEffect(Player& self, Player& opponent) {
    vector<Card*> hand = self.getHand();
    vector<Card*> deck = self.getDeck();
    vector<Card*> field = self.getField();

    vector<int> revealableIndexes;
    for (int i = 0; i < hand.size(); ++i) {
        if (hand[i]->getDescription().find("Blue Eyes") != string::npos) {
            revealableIndexes.push_back(i);
        }
    }

    if (revealableIndexes.empty()) {
        cout << "[True Light] Activation failed: You must reveal a card in your hand that mentions 'Blue-Eyes'." << endl;
        return false;
    }

    cout << "[True Light] Reveal a card from your hand that mentions 'Blue Eyes':" << endl;
    for (int i = 0; i < revealableIndexes.size(); ++i) {
        cout << "[" << i << "] " << hand[revealableIndexes[i]]->getName() << endl;
    }

    int revealChoice = -1;
    do {
        cout << "Choose index to reveal: ";
        cin >> revealChoice;
    } while (revealChoice < 0 || revealChoice >= revealableIndexes.size());

    cout << "[True Light] You revealed: " << hand[revealableIndexes[revealChoice]]->getName() << endl;

    vector<pair<Card*, string>> summonables; 

    for (Card* c : deck) {
        if (c->getType() == "Monster" && c->getDescription().find("Blue Eyes") != string::npos) {
            summonables.emplace_back(c, "deck");
        }
    }

    for (Card* c : hand) {
        if (c->getType() == "Monster" && c->getDescription().find("Blue Eyes") != string::npos) {
            summonables.emplace_back(c, "hand");
        }
    }

    if (summonables.empty()) {
        cout << "[True Light] No valid monsters in your hand or deck to summon." << endl;
        return false;
    }

    cout << "[True Light] Choose up to 2 monsters to special summon (from deck or hand):" << endl;
    for (int i = 0; i < summonables.size(); ++i) {
        cout << "[" << i << "] " << summonables[i].first->getName()
             << " (" << summonables[i].second << ")" << endl;
    }

    vector<Card*> summoned;
    int choice1 = -1, choice2 = -1;

    cout << "First index: ";
    cin >> choice1;
    if (choice1 < 0 || choice1 >= summonables.size()) return false;

    Card* c1 = summonables[choice1].first;
    string src1 = summonables[choice1].second;

    MonsterCard* m1 = dynamic_cast<MonsterCard*>(c1);
    m1->setDefenseMode(true);
    m1->setFacedown(false);
    m1->setJustSummoned(true);
    summoned.push_back(m1);

    if (src1 == "deck") {
        deck.erase(remove(deck.begin(), deck.end(), c1), deck.end());
    } else {
        hand.erase(remove(hand.begin(), hand.end(), c1), hand.end());
    }

    if (summonables.size() >= 2) {
        cout << "Second index (optional, -1 to skip): ";
        cin >> choice2;
        if (choice2 != -1 && choice2 >= 0 && choice2 < summonables.size() && choice2 != choice1) {
            Card* c2 = summonables[choice2].first;
            string src2 = summonables[choice2].second;

            MonsterCard* m2 = dynamic_cast<MonsterCard*>(c2);
            m2->setDefenseMode(true);
            m2->setFacedown(false);
            m2->setJustSummoned(true);
            summoned.push_back(m2);

            if (src2 == "deck") {
                deck.erase(remove(deck.begin(), deck.end(), c2), deck.end());
            } else {
                hand.erase(remove(hand.begin(), hand.end(), c2), hand.end());
            }
        }
    }

    for (Card* c : summoned) {
        field.push_back(c);
    }

    self.setDeck(deck);
    self.setHand(hand);
    self.setField(field);

    if (!summoned.empty()) {
        cout << "[True Light] You summoned: ";
        for (int i = 0; i < summoned.size(); ++i) {
            cout << summoned[i]->getName();
            if (i != summoned.size() - 1) cout << " and ";
        }
        cout << "." << endl;
    }

    bool hasSage = false, hasMaiden = false;
    for (Card* c : summoned) {
        if (c->getName() == "Sage with Eyes of Blue") hasSage = true;
        if (c->getName() == "Maiden of white") hasMaiden = true;
    }

    if (hasSage && hasMaiden) {
        vector<int> searchables;
        for (int i = 0; i < deck.size(); ++i) {
            if (deck[i]->getDescription().find("Blue Eyes") != string::npos) {
                searchables.push_back(i);
            }
        }

        if (!searchables.empty()) {
            cout << "[True Light] You summoned both Sage and Maiden!" << endl;
            cout << "Choose 1 card that mentions 'Blue Eyes' to add to your hand:" << endl;
            for (int i = 0; i < searchables.size(); ++i) {
                cout << "[" << i << "] " << deck[searchables[i]]->getName() << endl;
            }

            int pick = -1;
            do {
                cout << "Enter index: ";
                cin >> pick;
            } while (pick < 0 || pick >= searchables.size());

            int deckIndex = searchables[pick];
            Card* added = deck[deckIndex];
            deck.erase(deck.begin() + deckIndex);
            hand.push_back(added);
            self.setDeck(deck);
            self.setHand(hand);
            cout << "[True Light] You added '" << added->getName() << "' to your hand!" << endl;
        }
    }

string logMsg = "[True Light] has broken through the darkness — ";
for (int i = 0; i < summoned.size(); ++i) {
    logMsg += summoned[i]->getName();
    if (i != summoned.size() - 1) logMsg += " and ";
}
logMsg += " emerged from beyond. Prepare for impact.";
writeLog(logMsg);

    return true;
}

bool TheMelodyOfTheAwakeningDragon::ActivateEffect(Player& self, Player& opponent) {
    vector<Card*> field = self.getField();
    vector<Card*> deck = self.getDeck();
    vector<Card*> hand = self.getHand();

    bool controlMentionBE = false;
    bool hasSage = false;
    bool hasMaiden = false;

    for (Card* c : field) {
        if (c->getDescription().find("Blue Eyes") != string::npos) {
            controlMentionBE = true;
        }
        if (c->getName() == "Sage with Eyes of Blue") hasSage = true;
        if (c->getName() == "Maiden of white") hasMaiden = true;
    }

    if (!controlMentionBE) {
        cout << "[The Melody of the Awakening Dragon] Activation failed: You do not control any monster that mentions 'Blue Eyes'." << endl;
        return false;
    }

    bool fullPower = (hasSage && hasMaiden);
    vector<int> searchableIndexes;

    cout << "[The Melody of the Awakening Dragon] You can search the following cards:" << endl;

    for (int i = 0; i < deck.size(); ++i) {
        if (deck[i]->getDescription().find("Blue Eyes") != string::npos) {
            if (fullPower || deck[i]->getType() == "Monster") {
                searchableIndexes.push_back(i);
                cout << "[" << searchableIndexes.size() - 1 << "] " << deck[i]->getName() << endl;
            }
        }
    }

    if (searchableIndexes.empty()) {
        cout << "[The Melody of the Awakening Dragon] No card found in your deck that can be searched." << endl;
        return false;
    }

    int choice = -1;
    do {
        cout << "Choose index to add to your hand: ";
        cin >> choice;
    } while (choice < 0 || choice >= searchableIndexes.size());

    int selectedIdx = searchableIndexes[choice];
    Card* selected = deck[selectedIdx];
    deck.erase(deck.begin() + selectedIdx);
    hand.push_back(selected);

    self.setDeck(deck);
    self.setHand(hand);

    cout << "[The Melody of the Awakening Dragon] You added '" << selected->getName() << "' to your hand." << endl;

    writeLog("[The Melody of the Awakening Dragon] was activated — '" + selected->getName() + "' has been added to the hand. A storm may soon follow...");

    return true;
}

bool BlessingfromtheRoaringSky::ActivateEffect(Player& self, Player& opponent) {
    vector<Card*> newDeck = self.getDeck();
    vector<Card*> newHand = self.getHand();

    vector<pair<int, Card*>> validCards;

    for (int i = 0; i < static_cast<int>(newDeck.size()); ++i) {
        string desc = newDeck[i]->getDescription();
        if (desc.find("Blue Eyes") != string::npos) {
            validCards.push_back({i, newDeck[i]});
        }
    }

    if (validCards.empty()) {
        cout << "[Blessing from the Roaring Sky] Activation Failed: No card mentions 'Blue Eyes' in your deck!" << endl;
        return false;
    }

    cout << "[Blessing from the Roaring Sky] Cards that mention 'Blue Eyes':" << endl;
    for (int i = 0; i < static_cast<int>(validCards.size()); ++i) {
        cout << "  [" << i << "] " << validCards[i].second->getName()
             << " - " << validCards[i].second->getDescription() << endl;
    }

    int choice = -1;
    do {
        cout << "Choose one card to add to your hand: ";
        cin >> choice;

        if (choice < 0 || choice >= static_cast<int>(validCards.size())) {
            cout << "Invalid choice. Please try again." << endl;
            choice = -1;
        }
    } while (choice == -1);

    int deckIndex = validCards[choice].first;
    Card* selectedCard = newDeck[deckIndex];
    newHand.push_back(selectedCard);
    newDeck.erase(newDeck.begin() + deckIndex);

    self.setDeck(newDeck);
    self.setHand(newHand);

    self.shuffleDeck();

    cout << "[Blessing from the Roaring Sky] You have added " << selectedCard->getName()
         << " to your hand! The deck has been shuffled." << endl;

    writeLog("[Blessing from the Roaring Sky] called upon the will of the Blue-Eyes — " +
         selectedCard->getName() + " ascended from the deck as fate was rewritten.");

    return true;
}

bool DarkMagicalCircle::ActivateEffect(Player& self, Player& opponent) {
    vector<Card*> deck = self.getDeck();
    vector<Card*> field = self.getField();
    vector<Card*> hand = self.getHand();

    if (deck.size() < 3) {
        cout << "[Dark Magical Circle] You must have at least 3 cards in your deck to activate this effect." << endl;
        return false;
    }

    vector<Card*> top3(deck.end() - 3, deck.end());
    cout << "[Dark Magical Circle] Revealing top 3 cards:" << endl;
    for (int i = 0; i < 3; ++i) {
        cout << "[" << i << "] " << top3[i]->getName() << " - " << top3[i]->getDescription() << endl;
    }

    int choice = -1;
    do {
        cout << "Choose a card to add to your hand (index), or -1 to skip: ";
        cin >> choice;

        if (choice == -1) break;

        if (choice < 0 || choice >= 3) {
            cout << "[Dark Magical Circle] Invalid choice." << endl;
            choice = -2;
            continue;
        }

        if (top3[choice]->getDescription().find("Dark Magician") == string::npos) {
            cout << "[Dark Magical Circle] That card does not mention 'Dark Magician'. Try again." << endl;
            choice = -2;
        }
    } while (choice == -2);

    if (choice != -1) {
        Card* chosen = top3[choice];
        hand.push_back(chosen);
        top3.erase(top3.begin() + choice);
        cout << "[Dark Magical Circle] Added " << chosen->getName() << " to hand." << endl;
        writeLog("Opponent used [Dark Magical Circle] to add " + chosen->getName() + " to their hand.");
    }

    vector<Card*> newDeck(deck.begin(), deck.end() - 3);
    for (Card* c : top3) newDeck.push_back(c);
    self.setDeck(newDeck);
    self.setHand(hand);

    bool hasDM = false;
    for (Card* c : field) {
        if (c->getDescription().find("Dark Magician") != string::npos && c->getType() == "Monster") {
            hasDM = true;
            break;
        }
    }

    if (hasDM) {
        int option = -1;
        cout << "[Dark Magical Circle] You control a monster that mentions 'Dark Magician'. Choose an effect:" << endl;
        cout << "  [1] Destroy 1 card your opponent controls" << endl;
        cout << "  [2] Draw 1 card" << endl;
        do {
            cout << "Choice: ";
            cin >> option;
        } while (option != 1 && option != 2);

        if (option == 1) {
            vector<Card*> oppField = opponent.getField();
            if (oppField.empty()) {
                cout << "[Dark Magical Circle] No cards to destroy." << endl;
            } else {
                int destroyIndex = -1;
                do {
                    cout << "[Dark Magical Circle] Choose card to destroy:" << endl;
                    for (int i = 0; i < oppField.size(); ++i) {
                        cout << "  [" << i << "] " << oppField[i]->getName() << endl;
                    }
                    cout << "Index: ";
                    cin >> destroyIndex;
                } while (destroyIndex < 0 || destroyIndex >= oppField.size());

                cout << "[Dark Magical Circle] Destroyed " << oppField[destroyIndex]->getName() << "." << endl;
                writeLog("Opponent used [Dark Magical Circle] to destroy " + oppField[destroyIndex]->getName() + ".");
                oppField.erase(oppField.begin() + destroyIndex);
                opponent.setField(oppField);
            }
        } else {
            self.drawCard();
            cout << "[Dark Magical Circle] Drew 1 card." << endl;
            writeLog("Opponent used [Dark Magical Circle] to draw 1 card.");
        }
    }

    return true;
}

bool TheMagicCurtain::ActivateEffect(Player& self, Player& opponent) {
    vector<Card*> hand = self.getHand();
    vector<Card*> deck = self.getDeck();

    vector<int> revealableIndexes;
     for (int i = 0; i < hand.size(); ++i) {
        if (hand[i]->getType() == "Spell" && hand[i]->getDescription().find("Dark Magician") != string::npos) {
            revealableIndexes.push_back(i);
        }
    }

    if (revealableIndexes.empty()) {
        cout << "[The Magic Curtain] Activation failed: You must reveal a Spell card in your hand that mentions 'Dark Magician'." << endl;
        return false;
    }

    cout << "[The Magic Curtain] Reveal a Spell card in your hand that mentions 'Dark Magician':" << endl;
    for (int i = 0; i < revealableIndexes.size(); ++i) {
        cout << "[" << i << "] " << hand[revealableIndexes[i]]->getName() << endl;
    }

    int revealChoice = -1;
    do {
        cout << "Choose index to reveal: ";
        cin >> revealChoice;
    } while (revealChoice < 0 || revealChoice >= revealableIndexes.size());

    string revealedName = hand[revealableIndexes[revealChoice]]->getName();
    cout << "[The Magic Curtain] You revealed: " << revealedName << endl;

    vector<pair<int, Card*>> validSpells;
    for (int i = 0; i < deck.size(); ++i) {
        if (deck[i]->getType() == "Spell" &&
            deck[i]->getName() != revealedName &&
            deck[i]->getDescription().find("Dark Magician") != string::npos) {
            validSpells.push_back({i, deck[i]});
        }
    }

    if (validSpells.empty()) {
        cout << "[The Magic Curtain] No valid target found in your deck." << endl;
        return false;
    }

    cout << "[The Magic Curtain] Spells that mention 'Dark Magician':" << endl;
    for (int i = 0; i < validSpells.size(); ++i) {
        cout << "[" << i << "] " << validSpells[i].second->getName() << endl;
    }

    int choice = -1;
    do {
        cout << "Choose a card to add to your hand: ";
        cin >> choice;

        if (choice < 0 || choice >= validSpells.size()) {
            cout << "Invalid choice. Try again." << endl;
            choice = -1;
        }
    } while (choice == -1);

    int deckIndex = validSpells[choice].first;
    Card* selected = deck[deckIndex];
    deck.erase(deck.begin() + deckIndex);
    hand.push_back(selected);

    self.setDeck(deck);
    self.setHand(hand);

    self.shuffleDeck();

    cout << "[The Magic Curtain] You added " << selected->getName()
         << " to your hand! The deck has been shuffled." << endl;

    writeLog("Opponent activated [The Magic Curtain], revealed '" + revealedName +
             "' and drew forth '" + selected->getName() +
             "' from the shadows of magic.");

    return true;
}
*/

//trap
bool MirrorForce::ActivateEffect(Player& self, Player& opponent) { //cần check kĩ writeLog
    vector<Card*> newfield = opponent.getField();
    vector<Card*> newfieldopp;
    bool candestroy = false;

    writeLog("[Mirror Force] Activate Effect : Your ");
    for(auto card1 : newfield){
        MonsterCard *card2 = dynamic_cast<MonsterCard *>(card1);
        if(card2 != nullptr && card2->getType() == "Monster" && card2->isInDefense() == false){
            cout << "[Mirror Force] Destroyed : " << card2->getName() << endl;
            writeLog(card2->getName() + "\n");
            
            candestroy = true;
        }
        else{
            newfieldopp.push_back(card1);
        }
    }
    if(candestroy == false){
        cout << "[Mirror Force] Opponent do not have any monster in attack position" << endl;
    }
    writeLog(" has been destroyed! Be careful nextime. \n");
    opponent.setField(newfieldopp);
    return true; // Indicate success
}



bool Tsunagite::ActivateEffect(Player& self, Player& opponent) {
    cout << "[Tsunagite] End opponent battle phase!" << endl;
    writeLog("[Tsunagite] Activate Effect : End your battle phase! You can not attack anymore. \n");
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
    writeLog("[Trrricksters!!] Activate Effect : Your " + card->getName() + " atk now attack your Hp directly! Your lose " + to_string(card->getAtk()) + " Hp\n You have " + to_string(opponent.getHp()) + " Hp left \n Maybe try to think next time?" );
    return true;// Indicate success
}

