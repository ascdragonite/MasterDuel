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

bool SpellCard :: activateEffect(Player& self, Player& opponent) {
    if (name == "Dark Magic") {
        bool hasDM = false;
        for (Card* c : self.getField()) {
            if (c->getName() == "Dark Magician") {
                hasDM = true;
                break;
            }
        }

        if (hasDM) {
            vector<Card*> newField;
            for (Card* c : opponent.getField()) {
                if (c->getType() == "Monster") {
                    newField.push_back(c); // giữ quái vật
                } else {
                    cout << "[Dark Magic] destroyed: " << c->getName() << endl;
                    delete c;
                }
            }
            opponent.setField(newField);
            cout << "[Effect Activated] Dark Magic: destroyed all opponent's spell/trap!\n";
            return true;
        } else {
            cout << "You don't control Dark Magician.\n";
            return false;
        }
    }

    if (name == "Dark Burning Magic") {
    bool hasDM = false;
    bool hasDMG = false;

    for (Card* c : self.getField()) {
        string cardName = c->getName();
        if (cardName == "Dark Magician") hasDM = true;
        if (cardName == "Dark Magician Girl") hasDMG = true;
    }

    if (hasDM && hasDMG) {
        vector<Card*> newField;
        for (Card* c : opponent.getField()) {
            cout << "[Dark Burning Magic] destroyed: " << c->getName() << endl;
            delete c;
        }
        opponent.setField(newField);
        cout << "[Effect Activated] Dark Burning Magic: destroyed ALL opponent cards!\n";
        self.skipBattlePhaseCount = 2;
        return true;
    } else {
        cout << "You don't control both Dark Magician and Dark Magician Girl.\n";
        return false;
    }
    }

    if (name == "Bond Between the Teacher and Student") {
    bool hasDM = false;

    for (Card* c : self.getField()) {
        if (c->getName() == "Dark Magician") {
            hasDM = true;
            break;
        }
    }

    if (hasDM) {
        vector<Card*>& deck = self.getDeckRef();
        bool foundDMG = false;

        for (auto it = deck.begin(); it != deck.end(); ++it) {
            if ((*it)->getName() == "Dark Magician Girl") {
                self.getField().push_back(*it);  // triệu hồi
                deck.erase(it);                 // xoá khỏi deck
                foundDMG = true;
                cout << "[Effect Activated] Special Summoned Dark Magician Girl from the deck!\n";
                break;
                return true;
            }
        }

        if (!foundDMG) {
            cout << "Dark Magician Girl is not in the deck.\n";
            return false;
        }

        self.skipBattlePhaseCount = 1;  
    } else {
        cout << "You don't control Dark Magician.\n";
        return false;
    }
    }

    if (name == "The Power of Friendship") {
    vector<Card*>& field = self.getFieldRef();
    vector<MonsterCard*> faceUpMonsters;

    // Lọc ra các quái vật ngửa mặt
    for (Card* c : field) {
        MonsterCard* mc = dynamic_cast<MonsterCard*>(c);
        if (mc && !mc->isFacedown()) {
            faceUpMonsters.push_back(mc);
        }
    }

    if (faceUpMonsters.size() < 2) {
        cout << "You need at least 2 face-up monsters to use this card.\n";
        return false;
    }

    // Chọn quái đầu tiên làm quái được buff
    MonsterCard* boosted = faceUpMonsters[0];
    int totalATK = 0;

    for (size_t i = 1; i < faceUpMonsters.size(); ++i) {
        totalATK += faceUpMonsters[i]->getAtk();
        faceUpMonsters[i]->setAtk(0);  // optional: reset atk về 0 để tránh cộng dồn lặp
    }

    boosted->setAtk(boosted->getAtk() + totalATK);

    cout << "[Effect Activated] " << boosted->getName() << " gains " << totalATK << " ATK from friendship!\n";
    return true;
   }

   if (name == "Oshama Scramble") {
    if (opponent.getDeck().empty()) {
        cout << "[Oshama Scramble] Opponent's deck is empty. Nothing to shuffle.\n";
        return false;
    } else {
        opponent.shuffleDeck();
        cout << "[Effect Activated] Oshama Scramble: Opponent's deck has been shuffled!\n";
        return true;
    }
    }

    if (name == "Disorted Fate") {
    vector<Card*>& deck = self.getDeckRef();  

    if (deck.empty()) {
        cout << "[Disorted Fate] Your deck is empty. Cannot activate.\n";
        return false;
    }

    cout << "[Disorted Fate] Your current deck:\n";
    for (size_t i = 0; i < deck.size(); ++i) {
        cout << i << ". ";
        deck[i]->showInfo();
        cout << "-----------------------------\n";
    }

    int choice = -1;
    cout << "Enter the index of the card you want to move to the top: ";
    cin >> choice;

    if (choice < 0 || choice >= deck.size()) {
        cout << "Invalid choice. Effect failed.\n";
        return false;
    }

    // Lấy lá chọn, xóa ở vị trí cũ và đưa lên đầu
    Card* chosen = deck[choice];
    deck.erase(deck.begin() + choice);
    deck.insert(deck.begin(), chosen);

    cout << "[Effect Activated] Disorted Fate: \"" << chosen->getName() << "\" is now on top of your deck!\n";
    return true;
    }

    if (name == "Re:End of a Dream") {
    if (self.getCannotUseReEndThisTurn()) {
        cout << "[Re:End of a Dream] You cannot activate this card this turn due to a restriction from another effect.\n";
        return false;  
    }

    if (self.getHp() < 500) {
        self.setExtraTurn(true);
        cout << "[Effect Activated] Re:End of a Dream: You will get an extra turn!\n";
        return true;  
    } else {
        cout << "Your HP is not low enough to activate this effect (must be < 500).\n";
        return false; 
    }
    }

    if (name == "Destroyer") {
    vector<Card*>& enemyField = opponent.getFieldRef();

    // Tìm quái vật thủ
    vector<int> validTargets;
    for (int i = 0; i < enemyField.size(); ++i) {
        MonsterCard* mc = dynamic_cast<MonsterCard*>(enemyField[i]);
        if (mc && mc->isInDefense()) {
            validTargets.push_back(i);
        }
    }

    if (validTargets.empty()) {
        cout << "[Destroyer] No enemy monsters in defense mode to destroy.\n";
        return false;
    }

    cout << "[Destroyer] Select a monster in defense mode to destroy:\n";
    for (int idx : validTargets) {
        cout << idx << ". ";
        enemyField[idx]->showInfo();
        cout << "--------------------------\n";
    }

    int choice = -1;
    cout << "Enter the index of the monster to destroy: ";
    cin >> choice;

    // Kiểm tra hợp lệ
    if (find(validTargets.begin(), validTargets.end(), choice) == validTargets.end()) {
        cout << "Invalid choice or monster not in defense mode.\n";
        return false;
    }

    cout << "[Effect Activated] Destroyer: Destroyed " << enemyField[choice]->getName() << "!\n";
    delete enemyField[choice];
    enemyField.erase(enemyField.begin() + choice);
    return true;
   }

   if (name == "Flower Snow Drum’n’Bass") {
    int p1Flip = rand() % 2; // 0 hoặc 1
    int p2Flip = rand() % 2;

    cout << "[Flower Snow Drum’n’Bass] Coin flips...\n";
    cout << "You: " << (p1Flip == 0 ? "Heads" : "Tails") << endl;
    cout << "Opponent: " << (p2Flip == 0 ? "Heads" : "Tails") << endl;

    if (p1Flip == p2Flip) {
        cout << "It's a tie! No one draws.\n";
        return true;
    }

    Player* winner = (p1Flip > p2Flip) ? &self : &opponent;
    int drawCount = 2;
    int available = winner->getDeck().size();

    if (available == 0) {
        cout << (winner == &self ? "You" : "Opponent") << " have no cards left to draw!\n";
        return true;
    }

    drawCount = min(drawCount, available);
    cout << (winner == &self ? "You" : "Opponent") 
         << " win(s) the coin toss and draw(s) " << drawCount << " card(s)!\n";

    for (int i = 0; i < drawCount; ++i) {
        winner->drawCard();
    }

    return true;
    }

    if (name == "WorldVanquisher") {
    vector<Card*>& field = self.getFieldRef();
    vector<MonsterCard*> faceUpMonsters;

    for (Card* c : field) {
        MonsterCard* mc = dynamic_cast<MonsterCard*>(c);
        if (mc && !mc->isFacedown()) {
            faceUpMonsters.push_back(mc);
        }
    }

    if (faceUpMonsters.empty()) {
        cout << "[WorldVanquisher] You have no face-up monsters.\n";
        return false;
    }

    cout << "[WorldVanquisher] Choose a monster to buff:\n";
    for (size_t i = 0; i < faceUpMonsters.size(); ++i) {
        cout << i << ". " << faceUpMonsters[i]->getName()
             << " | ATK: " << faceUpMonsters[i]->getAtk() << endl;
    }

    int choice = -1;
    cout << "Enter your choice: ";
    cin >> choice;

    if (choice < 0 || choice >= faceUpMonsters.size()) {
        cout << "Invalid choice.\n";
        return false;
    }

    MonsterCard* selected = faceUpMonsters[choice];
    if (selected->originalAtk == -1)
        selected->originalAtk = selected->getAtk();  // lưu atk gốc

    selected->setAtk(selected->getAtk() + 200);

    cout << "[Effect Activated] " << selected->getName()
         << " gains +200 ATK until end of turn!\n";

    return true;
    }

    if (name == "Dragon United") {
    vector<Card*>& field = self.getFieldRef();
    vector<MonsterCard*> faceUpMonsters;

    for (Card* c : field) {
        MonsterCard* mc = dynamic_cast<MonsterCard*>(c);
        if (mc && !mc->isFacedown()) {
            faceUpMonsters.push_back(mc);
        }
    }

    int count = faceUpMonsters.size();
    if (count == 0) {
        cout << "[Dragon United] You have no face-up monsters.\n";
        return false;
    }

    int buffAmount = count * 300;

    for (MonsterCard* mc : faceUpMonsters) {
        if (mc->originalAtk == -1)
            mc->originalAtk = mc->getAtk();
        if (mc->originalDef == -1)
            mc->originalDef = mc->getDef();

        mc->setAtk(mc->getAtk() + buffAmount);
        mc->setDef(mc->getDef() + buffAmount);
    }

    cout << "[Effect Activated] Dragon United: All your face-up monsters gain "
         << buffAmount << " ATK and DEF until end of turn!\n";
    return true;
    }

    if (name == "Burst Stream of Destruction") {
    bool hasBlueEyes = false;
    for (Card* c : self.getField()) {
        if (c->getName() == "Blue-Eyes White Dragon") {
            hasBlueEyes = true;
            break;
        }
    }

    if (hasBlueEyes) {
        vector<Card*> newField;
        for (Card* c : opponent.getField()) {
            cout << "[Burst Stream] destroyed: " << c->getName() << endl;
            delete c;
        }
        opponent.setField(newField);

        cout << "[Effect Activated] Burst Stream of Destruction: Destroyed ALL opponent's cards!\n";
        self.skipBattlePhaseCount = 2;
        return true;
    } else {
        cout << "You don't control Blue-Eyes White Dragon.\n";
        return false;
    }
    }

    if (name == "Rage of the Blue Eyes") {
    vector<Card*>& field = self.getFieldRef();
    bool hasBlueEyes = false;
    for (Card* c : field) {
        if (c->getName() == "Blue-Eyes White Dragon") {
            hasBlueEyes = true;
            break;
        }
    }

    if (!hasBlueEyes) {
        cout << "You don't control Blue-Eyes White Dragon.\n";
        return false;
    }

    int newHp = self.getHp() / 3;
    self.setHp(newHp);
    cout << "[Effect Activated] Sacrificed 2/3 of HP. New HP: " << newHp << endl;

    for (Card* c : field) {
        MonsterCard* mc = dynamic_cast<MonsterCard*>(c);
        if (!mc) continue;
        if (mc->getName() == "Blue-Eyes White Dragon") {
            mc->setExtraAttackThisTurn(true);
            cout << mc->getName() << " can attack twice this turn!\n";
        } else {
            mc->setCanAttackThisTurn(false);
        }
    }

    self.setCannotUseReEndThisTurn(true);  // Flag mới
    return true;
}





}

    