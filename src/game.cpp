#include "game.h"
#include "json.hpp" // <-- include nlohmann/json (one header file)
#include "log_utilis.h"
#include "monstercard.h"
#include "player.h"
#include "serialize.h"
#include "spellcard.h"
#include "trapcard.h"
#include <chrono>
#include <iostream>
#include <string>
#include <thread>

using namespace std;
using json = nlohmann::json;

GameState *GameState::instance = nullptr;
// commit

void GameState::ConsoleClear() {
    cout << "\033[2J\033[H\033[3J";
}

GameState *GameState::getInstance() {
    if (!instance) {
        Player *player1 = new Player(1);
        Player *player2 = new Player(2);
        instance = new GameState(*player1, *player2);
        cout << "GameState instance created.\n";
    }
    return instance;
}
void writeToFile(const json &j) {
    ofstream out("game_state.json");
    if (!out) {
        cerr << "Error opening file for writing.\n";
        return;
    }
    out << j.dump(4) << std::endl; // Pretty print with indent
    out.close();
}

Player *GameState::getPlayer(int id) {
    if (id == 1) {
        return player1;
    } else if (id == 2) {
        return player2;
    } else {
        return nullptr; // Invalid player ID
    }
}

GameState::GameState(Player &p1, Player &p2) {
    player1 = &p1;
    player2 = &p2;
    player1->loadDeckDarkMagician();
    player2->loadDeckBlueEyes();
}

void GameState::startGame() {
    player1->shuffleDeck();
    player2->shuffleDeck();
    cout << "Game started!\n";

    for (int i = 0; i < 5; i++) {
        player1->drawCard();
        player2->drawCard();
    }
}
json readFromFile() {
    ifstream in("game_state.json");
    if (!in) {
        cerr << "Error: save file not found.\n";
        return json();
    }

    if (in.peek() == ifstream::traits_type::eof()) {
        cerr << "Error: save file is empty.\n";
        return json(); // không đọc gì cả
    }

    json j;
    try {
        in >> j;
    } catch (json::parse_error &e) {
        cerr << "JSON Parse Error: " << e.what() << endl;
        return json();
    }

    in.close();
    return j;

    if (!in) {
        ofstream out("game_state.json");
        out << "{}";
        out.close();
        return json();
    }
}

void GameState::printFields(const Player& self, const Player& opponent)
{

    cout << "\n--------------- ENEMY (" << opponent.getHp() << ") --------------\n" << endl;
        vector<Card*> enemyField = opponent.getField();

        for (int i = 0; i < enemyField.size(); ++i) {
            cout << i << ": ";

            // Nếu là Monster
            enemyField[i]->showInfo(true);  // Giả sử spell không bị úp

            cout << "\n \n";
        }
        cout << "\n---------------- YOU (" << self.getHp() << ") ---------------\n " << endl;
        for (int i = 0; i < self.getField().size(); ++i) {
            cout << i << ": ";
            self.getField()[i]->showInfo();
            cout << "\n \n";
        }
}

void GameState::playerTurn(Player &self, Player &opponent, bool isFirstTurn) {

    ofstream clearLog("log.txt");
    clearLog.close();

    for (Card *card : self.getField()) {
        MonsterCard *mc = dynamic_cast<MonsterCard *>(card);
        if (mc)
            mc->clearSummonFlag();
    }


    bool hasSummoned = false;

    self.resetAttackFlags();

    self.setCanUseReEnd(true);
    
    if (!isFirstTurn) {
        self.setCanUsePowerOfFriendship(true);
        self.drawCard();
        cout << " draws a card.\n";
    }
    else{
         self.setCanUsePowerOfFriendship(false);
    }

    while (true) {
        json j = readFromFile();
        from_json(j["Player1"], *player1);
        from_json(j["Player2"], *player2);

        ConsoleClear();
        cout << "  ------------------- Hand ------------------" << endl;
        int i = 0;
        vector<Card*> hand = self.getHand();
        for (int i = 0; i < hand.size(); ++i) {
            cout << "\n";
            cout << i << ": ";

            if (hand[i]->getType() == "Monster") {
                // Cast sang MonsterCard và in thủ công, không dùng showInfo()
                MonsterCard* monster = dynamic_cast<MonsterCard*>(hand[i]);
                if (monster) {
                    cout << monster->getName();
                    cout << " (Monster)" << endl;
                    cout << "Atk: " << monster->getAtk() << " Def: " << monster->getDef() << endl;
                    // Không in position
                }
            } else {
                cout << hand[i]->getName();
                cout << " (" << hand[i]->getType() << ")" << endl;
                cout << "Description: " << hand[i]->getDescription() << endl;
                // Spell hoặc Trap thì vẫn dùng showInfo()
            }
        }
        cout << "\n";

        printFields(self, opponent);
        
        if (checkVictory(*player1, *player2)) {
            if (player1->getHp() <= 0 || player1->getDeck().empty()) {
                cout << (self.getIndex() == 2 ? "================= YOU WIN =================\n" : "================ YOU LOSE =================\n");
            } else if (player2->getHp() <= 0 || player2->getDeck().empty()) {
                cout << (self.getIndex() == 1 ? "================= YOU WIN =================\n" : "================ YOU LOSE =================\n");
            } else {
                cout << "================== DRAW ====================\n";
            }
            return;
        }

        cout << "\n================ YOUR TURN ================\n";
        cout << "0: End Turn\n";
        cout << "1: Play Card\n";
        cout << "2: Switch Monster Position\n";
        cout << "3: Attack\n";
        cout << "4: Flip summon\n";
        cout << "5: Surrender\n";
        cout << "Enter your choice [0/1/2/3/4/5]: ";

        int code, index = -1;
        cin >> code;

        if (code == 1 || code == 2 || code == 3 || code == 4) {
            cout << "Enter the index of the card: ";
            cin >> index;
        }

        switch (code) {
            case 0:
                cout << "Ending your turn..." << endl;
                this_thread::sleep_for(chrono::milliseconds(800));
                return; // End turn

            case 1:
                {
                    if (!(index >= 0 && index < self.getHand().size())) {
                        cout << "Invalid index.\n";
                        break;
                    }

                    Card *card = self.getHand()[index];
                    string type = card->getType();
                    if (type == "Monster") {
                        if (hasSummoned) {
                            cout << "You cannot summon more than once per turn.\n";
                            break;
                        }
                        self.Summon(index);
                        hasSummoned = true;

                        MonsterCard *mc = dynamic_cast<MonsterCard *>(card);
                        if (mc) {
                            writeLog(
                                "Opponent summoned a monster in " +
                                string(mc->isInDefense() ? "face-down defense" : "attack") +
                                " position.");
                        }
                    } else if (type == "Spell") {
                        bool success = card->activateEffect(self, opponent);
                        if (!success) {
                            cout << "[Spell] Effect was not activated. Card remains in hand. \n";
                            break;
                        } 
                        delete card;
                        auto &hand = self.getHandRef();
                        hand.erase(hand.begin() + index);
                    } else if (type == "Trap") {
                        cout << "Placed Trap: " << self.getHand()[index]->getName() << endl;
                        self.Summon(index);
                    } else {
                        cout << "Unsupported card type.\n";
                    }
                    break;
                }
            case 2:
                self.switchPosition(index);

                // Ghi log nếu switch hợp lệ
                if (index >= 0 && index < self.getField().size()) {
                    MonsterCard *m = dynamic_cast<MonsterCard *>(self.getField()[index]);
                    if (m && !m->isFacedown() && !self.hasAttacked(index) &&
                        !m->isJustSummoned()) {
                        string pos = m->isInDefense() ? "Defense" : "Attack";
                        writeLog("Opponent switched position of " + m->getName() + " to " +
                                 pos + " position.");
                    }
                }

                break;

            case 3:
                {
                    if (self.getSkipBattlePhaseCount() > 0 || isFirstTurn) {
                        cout << "You cannot attack this turn.\n";
                        break;
                    }

                    battlePhase(self, opponent, index);
                    break;
                }
            case 4:
                {
                    if (index >= 0 && index < self.getField().size()) {
                        MonsterCard *mc = dynamic_cast<MonsterCard *>(self.getField()[index]);
                        if (!mc) {
                            cout << "This is not a monster card.\n";
                            break;
                        }

                        if (!mc->isFacedown()) {
                            cout << mc->getName() << " is already face-up.\n";
                            break;
                        }

                        if (mc->isJustSummoned()) {
                            cout << "You cannot flip summon a monster that was summoned this "
                                "turn.\n";
                            break;
                        }

                        mc->flipSummon(); // lật ngửa + chuyển sang attack
                        self.getField()[index]->showInfo();   // in đầy đủ thông tin

                        writeLog("Opponent flip summon their monster: " + mc->getName());

                    } else {
                        cout << "Invalid index.\n";
                    }
                    break;
                }
            case 5:
                {
                    char confirm;
                    while (true) {
                        if (self.getIndex() == 1) {
                            cout << "Are you sure you want to surrender?\n";
                            cout << "Believe in your deck, a true duelist never gives up!\n";
                        } else {
                            cout << "You're thinking of surrendering?\n";
                            cout << "Pathetic. Believe in something at least...\n";
                        }

                        cout << "(y/n): ";
                        cin >> confirm;

                        if (confirm == 'y' || confirm == 'Y') {
                            cout << "You have surrendered. Opponent wins by default.\n";
                            if (self.getIndex() == 1) {
                                cout << "\nPlayer 2 wins!\n";
                                writeLog("Player 2 wins!");
                            } else {
                                cout << "\nPlayer 1 wins!\n";
                                writeLog("Player 1 wins!");
                            }
                            cout << "Game Over.\n";
                            exit(0);
                        } else if (confirm == 'n' || confirm == 'N') {
                            cout << "Surrender cancelled. Back to the game.\n";
                            break;
                        } else {
                            cout << "Invalid input. Please enter 'y' or 'n'.\n";
                        }
                    }
                    break;
                }

            default:
                cout << "Invalid action. Try again.\n";
                break;
        }


        j = readFromFile();
        j["Player1"] = json(*player1);
        j["Player2"] = json(*player2);
        writeToFile(j);


        this_thread::sleep_for(std::chrono::milliseconds(3000));
    }
}

void TrapResult(Card* trapcard){
    if(trapcard->getName() == "Tsunagite"){
        cout << "[Tsunagite] Activate Effect : End your battle phase! You can not attack anymore." << endl;
    }
    else if(trapcard->getName() == "Mirror Force") {
        cout << "[Mirror Force Activate Effect : All your monsters in attack position are destroyed!]" << endl;
    }
    else if(trapcard->getName() == "Trrricksters!!"){
        cout << "[Trrricksters!!] Activate Effect : Your attack now reflect back to you! " << endl;
    }
    else{
        cout <<"???" << endl;
    }
}


void GameState::battlePhase(Player& self, Player& opponent, int index) {
    vector<Card*> atkField = self.getField();

    bool hasAttacker = false;
    for (Card* c : atkField) {
        if (dynamic_cast<MonsterCard*>(c) != nullptr) {
            hasAttacker = true;
            break;
        }
    }

    if (!hasAttacker) {
        cout << "You have no monsters to attack with!\n";
        return;
    }


    while (index < 0 || index >= atkField.size() || 
        dynamic_cast<MonsterCard*>(atkField[index]) == nullptr) {
        cout << "Invalid attacker index. Please enter again: ";
        cin >> index;
    }

    MonsterCard* atkCard = dynamic_cast<MonsterCard*>(atkField[index]);

    if (atkCard->isInDefense()) {
        cout << atkCard->getName() << " is in Defense Position and cannot attack!\n";
        return;
    }

    if (self.hasAttacked(index)) {
        cout << "This monster already attacked this turn.\n";
        return;
    }

    vector<Card*> defField = opponent.getField();

    bool hasMonster = false;
    for (Card* c : defField) {
        if (c->getType() == "Monster") {
            hasMonster = true;
            break;
        }
    }

    vector<int> trapCardIndexes;
    for (int i = 0; i < defField.size(); ++i) {
        if (defField[i]->getType() == "Trap") {
            trapCardIndexes.push_back(i);
        }
    }
    opponent.canTrap = trapCardIndexes;



    if (!hasMonster) {

        cout << "Opponent has no monsters.\n";
        char choice;
        do {
            cout << "Do you want to attack directly with " << atkCard->getName() << "? (y/n): ";
            cin >> choice;
            if (choice != 'y' && choice != 'Y' && choice != 'n' && choice != 'N') {
                cout << "Invalid input. Please enter 'y' or 'n'.\n";
            }
        } while (choice != 'y' && choice != 'Y' && choice != 'n' && choice != 'N');

        if (choice == 'y' || choice == 'Y') {
            self.setAttacked(index, true);

            json j = readFromFile(); //write to json
            string turn = j.at("turn");
            j[string("Player") + to_string(self.getIndex())] = self;
            j[string("Player") + to_string(opponent.getIndex())] = opponent;
            writeToFile(j);

            auto temp = trapCardIndexes;
            while (temp == trapCardIndexes && !temp.empty()) {
                //cout << "index unchanged" << endl;

                json state = readFromFile();
                from_json(state["Player" + to_string(opponent.getIndex())], opponent);
                from_json(state["Player" + to_string(self.getIndex())], self);
                trapCardIndexes = opponent.canTrap;
                this_thread::sleep_for(chrono::milliseconds(1000));
            }
            if (temp.empty() || (trapCardIndexes.size() == 1 && trapCardIndexes[0] == -1)) {
                opponent.canTrap.clear();
                int damage = atkCard->getAtk();
                opponent.takeDamage(damage);
                cout << atkCard->getName() << " attacks directly! Opponent loses "
                    << damage << " HP!\n";
                writeLog("Opponent attacked directly with " + atkCard->getName() +
                         " for " + to_string(damage) + " damage.");

            } else if (trapCardIndexes.size() == 2 && trapCardIndexes[0] == -1) {
                cout << "Trap Card " << defField[trapCardIndexes[1]]->getName() << " activated!" << endl;
                TrapResult(defField[trapCardIndexes[1]]);
                opponent.canTrap.clear();
            }
            return;

        }
        return;
    }

    int defendIndex = -1;
    while (true) {
        cout << "Enter the index of the opponent's card to attack: ";
        cin >> defendIndex;

        if (defendIndex >= 0 && defendIndex < defField.size()) {
            MonsterCard* defCheck = dynamic_cast<MonsterCard*>(defField[defendIndex]);
            if (defCheck != nullptr) break;
            cout << "The selected card is not a monster. Try again.\n";
        } else {
            cout << "Invalid target index. Try again.\n";
        }
    }

    MonsterCard* defCard = dynamic_cast<MonsterCard*>(defField[defendIndex]);

    writeLog("Opponent declared an attack from " +
             atkCard->getName() +
             " targeting opponent's card at index " + to_string(defendIndex) + ".");

    // Reveal nếu là quái úp
    if (defCard->isFacedown()) {
        defCard->reveal();
        cout << "The defending card was face-down. It is now revealed:\n";
        defField[defendIndex]->showInfo();
    }

    // Ghi lại trap
    self.setAttacked(index, true);
    json j = readFromFile();
    j[string("Player") + to_string(self.getIndex())] = self;
    j[string("Player") + to_string(opponent.getIndex())] = opponent;
    writeToFile(j);

    auto temp = trapCardIndexes;

    while (temp == trapCardIndexes && !temp.empty()) 
    {
        //cout << "index unchanged" << endl;
        json state = readFromFile();
        from_json(state["Player" + to_string(opponent.getIndex())], opponent);
        from_json(state["Player" + to_string(self.getIndex())], self);
        trapCardIndexes = opponent.canTrap;
        this_thread::sleep_for(chrono::milliseconds(1000));
    }
    //cout << "index changed" << endl;
    if (temp.empty() || (trapCardIndexes.size() == 1 && trapCardIndexes[0] == -1)) {
        *atkCard += *defCard; //operator overload
        opponent.canTrap.clear();
    } else if (trapCardIndexes.size() == 2 && trapCardIndexes[0] == -1) {
        cout << "Trap Card " << defField[trapCardIndexes[1]]->getName() << " activated!" << endl;
        TrapResult(defField[trapCardIndexes[1]]);
        opponent.canTrap.clear();
    }
    this_thread::sleep_for(chrono::milliseconds(1000));

}

bool GameState::checkVictory(const Player &p1, const Player &p2) {
    if (p1.getHp() <= 0 || p1.getDeck().empty())
        return true;
    if (p2.getHp() <= 0 || p2.getDeck().empty())
        return true;
    return false;
}
