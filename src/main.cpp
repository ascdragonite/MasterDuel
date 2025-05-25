#include "card.h"
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
#include <vector>

using namespace std;

using json = nlohmann::json;

int ActivateTrapCards(vector<int> indexes, Player &self, Player &opponent, int attackerCardIndex) {
    int i = 1;
    auto field = self.getField();
    for (int index : indexes) {
        cout << i << ". " << field[index]->getName() << endl;
        i++;
    }
    i = 1;
    cout << "Select trap card to activate (0 to cancel) [0/";
    for (int index : indexes) {
        cout << i << "/";
        i++;
    }
    cout << "]: " << endl;
    int choice;
    cin >> choice;
    if (choice == 0) {
        cout << "Trap card activation canceled.\n";
        return -1;
    } else if (choice > 0 && choice <= indexes.size()) {
        int trapIndex = indexes[choice - 1];
        TrapCard *trapCard = dynamic_cast<TrapCard *>(field[trapIndex]);
        if(trapCard->getName() != "Trrricksters!!") trapCard->activateEffect(self, opponent);
        else trapCard->activateEffect(self, opponent, attackerCardIndex);
        vector<Card*> newField = self.getField(); // lấy bản gốc
        delete newField[trapIndex];               // giải phóng bộ nhớ
        newField.erase(newField.begin() + trapIndex); // xóa khỏi field
        self.setField(newField);                  // cập nhật lại field
        return indexes[choice - 1]; 
    }
    return -1;
}

int main() {
    string player;
    cout << "==================== GAME RULE ====================" << endl;
    cout << "Each player starts with 8000 HP." << endl;
    cout << "Each player has a deck of 26 cards." << endl;
    cout << "Each player draws 5 cards at the start of the duel." << endl;
    cout << "A player loses when their HP reaches 0 or if they cannot draw a card." << endl;
    cout << "Starting from turn 2, each player draws 1 card at the beginning of their turn." << endl;
    cout << "There are 3 types of cards in the game: Monster, Spell, and Trap cards." << endl;
    cout << "Each type plays a different role during the duel." << endl;
    cout << "Each turn, you can Normal Summon 1 monster in face-up attack or face-down defense position." << endl;
    cout << "You can flip summon a monster from face-down defense to face-up attack position once per turn," << endl;
    cout << "as long as it was already on the field since a previous turn." << endl;
    cout << "as long as it was not summoned this turn." << endl;
    cout << "NOTE: Face-down monsters can only change position by flip summoning." << endl;
    cout << "NOTE: Monster summoned this turn cannot switch position aplied to all kind of summon (normal summon, special summon)." << endl;
    cout << "You can activate multiple Spell Cards per turn." << endl;
    cout << "If a Spell resolves successfully, it is removed from your hand." << endl;
    cout << "If it fails, it remains in your hand." << endl;
    cout << "Trap Cards must first be set on the field and cannot be activated during the same turn they are set." << endl;

    cout << "=================== BATTLE RULES ===================" << endl;
    cout << "Each monster can attack once per turn, unless stated otherwise." << endl;
    cout << "You cannot attack during the first turn of the duel." << endl;
    cout << "If your opponent controls no monsters, you can attack their HP directly." << endl;
    cout << "If your monster attacks a face-down monster, it is flipped face-up before damage calculation." << endl;
    cout << "- ATK vs ATK: The weaker monster is destroyed." << endl;
    cout << "              Its controller takes damage equal to the difference." << endl;
    cout << "              If both monster have equal ATK, both are destroyed." << endl;
    cout << "- ATK vs DEF: If ATK > DEF, the defending monster is destroyed." << endl;
    cout << "             If ATK < DEF, the attacker takes damage = DEF - ATK." << endl;
    cout << "             If ATK == DEF, no monsters are destroyed and no damage is taken." << endl;
    cout << "====================================================" << endl;

    cout << "==================== TURN PHASES ===================" << endl;
    cout << "1. Draw Phase    : Draw 1 card from your deck." << endl;
    cout << "2. Main Phase    : Summon monsters, activate Spells/Traps, and change battle positions." << endl;
    cout << "3. Battle Phase  : Declare attacks using your monsters." << endl;
    cout << "====================================================" << endl;

    cout << "====================================================" << endl;
    cout << "Are you Player 1 or Player 2? (Enter 1 or 2): ";
    cin >> player;
    cin.ignore();

    if (player != "1" && player != "2") {
        cout << "Invalid player number. Exiting.\n";
        return 1;
    }

    std::cout << "You are Player " << player << ".\n";

    GameState *gameState = GameState::getInstance();
    auto player1 = gameState->getPlayer(1);
    auto player2 = gameState->getPlayer(2);
    if (player == "1") {
        gameState->startGame();
        json j;
        j["Player1"] = json(*player1);
        j["Player2"] = json(*player2);
        j["turn"] = "PLAYER1";
        j["ExtraTurn"] = false;
        j["hasBattled"] = false;
        writeToFile(j);
    }

    auto self = (player == "1") ? player1 : player2;

    vector<bool> oppAttacked;

    bool isFirstTurn = (player == "1");

    vector<string> log;

    while (true) {
        json state = readFromFile();

        if (state.is_null() || state.empty()) {
            this_thread::sleep_for(std::chrono::milliseconds(500));
            continue;
        }

        bool myTurn = (state["turn"] == "PLAYER" + player);
        if (myTurn) {

            gameState->ConsoleClear();

            cout << "It's your turn!\n";

            from_json(state["Player1"], *player1);
            from_json(state["Player2"], *player2);

            if (player == "1") {
                gameState->playerTurn(*player1, *player2, isFirstTurn);
            } else {
                gameState->playerTurn(*player2, *player1, isFirstTurn);
            }

            isFirstTurn = false;


            self->setSkipBattlePhaseCount(self->getSkipBattlePhaseCount() - 1);
            state = readFromFile();
            // Update JSON state
            state["Player1"] = *player1;
            state["Player2"] = *player2;

            if (state["ExtraTurn"])
            {
                cout << "Player " << player << " gets an Extra turn!!!" << endl;
                state["ExtraTurn"] = false;
            } else {
                state["turn"] = "PLAYER" + string((player == "1") ? "2" : "1");
            }
            writeToFile(state);
        } else {
            from_json(state["Player1"], *player1);
            from_json(state["Player2"], *player2);
            Player *self = (player == "1") ? player1 : player2;
            Player *opponent = (player == "1") ? player2 : player1;

            auto temp = opponent->attackedThisTurn;
            int attackerIndex = -1;

            if (!oppAttacked.empty() && temp != oppAttacked) {
                for (int i = 0; i < temp.size(); ++i) {
                    if (temp[i] && !oppAttacked[i]) {
                        cout << "Your opponent's monster at index " << i << " has attacked this turn.\n";
                        attackerIndex = i;
                    }
                }
            }
            oppAttacked = temp;


            if (!self->canTrap.empty() && self->canTrap[0] != -1) {
                int index = ActivateTrapCards(self->canTrap, *self, *opponent, attackerIndex) ;
                self->canTrap.clear();

                self->canTrap.push_back(-1);
                if (index != -1) self->canTrap.push_back(index);

                json j = readFromFile();
                j["Player1"] = *player1;
                j["Player2"] = *player2;
                writeToFile(j);
                self->canTrap.clear();

            } else{
                static int linesSeen = 0;
                gameState->ConsoleClear();
                gameState->printFields(*self, *opponent);

                if (gameState->checkVictory(*player1, *player2)) {
                    if (player1->getHp() <= 0 || player1->getDeck().empty()) {
                        cout << (self->getIndex() == 2 ? "================= YOU WIN =================\n" : "================ YOU LOSE =================\n");
                    } else if (player2->getHp() <= 0 || player2->getDeck().empty()) {
                        cout << (self->getIndex() == 1 ? "================= YOU WIN =================\n" : "================ YOU LOSE =================\n");
                    } else {
                        cout << "================== DRAW ====================\n";
                    }
                    break;
                } else {
                    cout << "\n-------... WAITING FOR YOUR TURN ...-------" << endl;
                    log = getLastLogLines(3);
                    for (const auto& line : log) {
                        cout << line << endl;
                    }
                    this_thread::sleep_for(std::chrono::milliseconds(1000));
                }
            }
        }
    }
    cout << "Game Over!" << endl;
    delete player1;
    delete player2;
    return 0;
}
