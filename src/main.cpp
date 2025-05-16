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

void ActivateTrapCards(vector<int> indexes, Player &self, Player &opponent) {
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
        return;
    } else if (choice > 0 && choice <= indexes.size()) {
        int trapIndex = indexes[choice - 1];
        TrapCard *trapCard = dynamic_cast<TrapCard *>(field[trapIndex]);
        trapCard->activateEffect(self, opponent);
        return;
    }
}

int main() {
    string player;
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
        writeToFile(j);
    }

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
                gameState->playerTurn(*player1, *player2, false);
            } else {
                gameState->playerTurn(*player2, *player1, false);
            }

            // Check for victory
            if (gameState->checkVictory(*player1, *player2)) {
                cout << "Game Over! ";
                if (player1->getHp() <= 0 || player1->getDeck().empty()) {
                    cout << "Player 2 wins!\n";
                } else {
                    cout << "Player 1 wins!\n";
                }
                break;
            }
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
            
            if (!self->canTrap.empty()) {
                
                ActivateTrapCards(self->canTrap, *self, *opponent);
                self->canTrap.clear();

                json j = readFromFile();
                j["Player1"] = *player1;
                j["Player2"] = *player2;
                writeToFile(j);
            } else {
                static int linesSeen = 0;
                gameState->ConsoleClear();
                cout << "Waiting for your turn...\n";

                monitorLog(linesSeen);
                this_thread::sleep_for(std::chrono::milliseconds(1000));
            }
        }
    }
    delete player1;
    delete player2;
    return 0;
}
