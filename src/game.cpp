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
    // cout << "\033[2J\033[H\033[3J";
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
void GameState::playerTurn(Player &self, Player &opponent, bool isFirstTurn) {

    ofstream clearLog("log.txt");
    clearLog.close();

    for (Card *card : self.getField()) {
        MonsterCard *mc = dynamic_cast<MonsterCard *>(card);
        if (mc)
            mc->clearSummonFlag();
    }

    cout << "\n[Start of Turn Status]\n";
    cout << "Player 1 HP: " << player1->getHp()
        << " | Deck: " << player1->getDeck().size() << endl;
    cout << "Player 2 HP: " << player2->getHp()
        << " | Deck: " << player2->getDeck().size() << endl;

    bool hasSummoned = false;

    self.resetAttackFlags();
    if (!isFirstTurn) {
        self.drawCard();
        cout << " draws a card.\n";
    }

    while (true) {
        json j = readFromFile();
        from_json(j["Player1"], *player1);
        from_json(j["Player2"], *player2);
        if (!opponent.canTrap.empty()) {
            this_thread::sleep_for(std::chrono::milliseconds(1000));
            cout << opponent.canTrap[0] << endl;
            continue;
        }
        ConsoleClear();
        cout << "Hand: ";
        int i = 0;
        for (int i = 0; i < self.getHand().size(); ++i) {
            cout << "Index " << i << ":\n";
            self.getHand()[i]->showInfo();
        }
        cout << "\n Enemy Field: ";
        for (int i = 0; i < opponent.getField().size(); ++i) {
            cout << i << ". ";

            MonsterCard *mc = dynamic_cast<MonsterCard *>(opponent.getField()[i]);
            if (mc) {
                mc->showInfoHidden();
            } else {
                opponent.getField()[i]->showInfo(); // Cho spell/trap hoặc card khác
            }

            cout << " | ";
        }
        cout << "\n Your Field: ";
        for (int i = 0; i < self.getField().size(); ++i) {
            cout << i << ". ";
            self.getField()[i]->showInfo();
            cout << " | ";
        }

        cout << "\nChoose an action:\n";
        cout << "0: End Turn\n";
        cout << "1: Play Card\n";
        cout << "2: Switch Monster Position\n";
        cout << "3: Attack\n";
        cout << "4: Flip summon\n";
        cout << "Enter your choice [0/1/2/3/4]: ";

        int code, index = -1;
        cin >> code;

        if (code != 0) {
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
                        writeLog("Opponent activated a " + type +
                                 " card: " + card->getName());
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
                    mc->showInfo();   // in đầy đủ thông tin

                    writeLog("Opponent flip summon their monster: " + mc->getName());

                } else {
                    cout << "Invalid index.\n";
                }
                break;

            default:
                cout << "Invalid action. Try again.\n";
                break;
        }

        if (checkVictory(*player1, *player2)) {
            if (player1->getHp() <= 0 || player1->getDeck().empty()) {
                cout << "\nPlayer 2 wins!\n";
            } else if (player2->getHp() <= 0 || player2->getDeck().empty()) {
                cout << "\nPlayer 1 wins!\n";
            } else {
                cout << "\nIt's a draw!\n";
            }
            cout << "Game Over.\n";
            exit(0);
        };

        j = readFromFile();
        j["Player1"] = json(*player1);
        j["Player2"] = json(*player2);
        writeToFile(j);
        this_thread::sleep_for(std::chrono::milliseconds(3000));
    }
}

void GameState::battlePhase(Player& self, Player& opponent, int index) {
    vector<Card*> atkField = self.getField();

    // 🔒 Check: sân mình không có quái vật thì return
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

    // 🔁 Nhập lại attacker index nếu không hợp lệ
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

    // ✅ Kiểm tra sân đối thủ có quái vật không
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
        // ✅ Không có quái vật → hỏi có muốn tấn công trực tiếp không
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
            json j = readFromFile();
            string turn = j.at("turn");
            j[string("Player") + to_string(self.getIndex())] = self;
            j[string("Player") + to_string(opponent.getIndex())] = opponent;
            writeToFile(j);
            if(opponent.canTrap.empty()) {
                int damage = atkCard->getAtk();
                opponent.takeDamage(damage);
                cout << atkCard->getName() << " attacks directly! Opponent loses "
                     << damage << " HP!\n";
                writeLog("Opponent attacked directly with " + atkCard->getName() +
                         " for " + to_string(damage) + " damage.");
                return;
            }

        }
        return;
    }

    // 🔁 Nhập chỉ số quái vật đối thủ để tấn công
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
        defCard->showInfo();
    }

    // Ghi lại trap
    self.setAttacked(index, true);
    json j = readFromFile();
    string turn = j.at("turn");
    j[string("Player") + to_string(self.getIndex())] = self;
    j[string("Player") + to_string(opponent.getIndex())] = opponent;
    writeToFile(j);

    if (trapCardIndexes.empty()) {

        *atkCard += *defCard; //operator overload

        this_thread::sleep_for(chrono::milliseconds(1000));
    }

}

bool GameState::checkVictory(const Player &p1, const Player &p2) {
    if (p1.getHp() <= 0 || p1.getDeck().empty())
        return true;
    if (p2.getHp() <= 0 || p2.getDeck().empty())
        return true;
    return false;
}
