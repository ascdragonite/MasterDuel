#include <iostream>
#include "card.h"
#include "game.h"
#include "monstercard.h"
#include "spellcard.h"
#include "trapcard.h"
#include "player.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <string>
#include "json.hpp" // <-- include nlohmann/json (one header file)
#include "serialize.h"
#include "log_utilis.h"

using namespace std;

using json = nlohmann::json;



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

    GameState* gameState = GameState::getInstance();
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
            
            gameState -> ConsoleClear();
            
            cout << "It's your turn!\n";

            from_json(state["Player1"], *player1);
            from_json(state["Player2"], *player2);

            if (player == "1") {
                gameState -> playerTurn(*player1, *player2, false);
            } else {
                gameState -> playerTurn(*player2, *player1, false);
            }

            // Check for victory
            if (gameState -> checkVictory(*player1, *player2)) {
                cout << "Game Over! ";
                if (player1->getHp() <= 0 || player1->getDeck().empty()) {
                    cout << "Player 2 wins!\n";
                } else {
                    cout << "Player 1 wins!\n";
                }
                break;
            }

            // Update JSON state
            state["Player1"] = *player1;
            state["Player2"] = *player2;
            bool hasExtra = (player == "1") ? player1->hasExtraTurn() : player2->hasExtraTurn();
            if (hasExtra) {
               if (player == "1") player1->setExtraTurn(false);
               else player2->setExtraTurn(false);

               cout << "[Extra Turn] You get to play another turn!\n";
               writeToFile(state); 
               continue;
            }
            state["turn"] = "PLAYER" + string((player == "1") ? "2" : "1");
            writeToFile(state);
        }
        else 
        {
            Player* self = (player == "1") ? player2 : player1;
            if (self->canTrap)
            {
                string input;
                cout << "Activate trap card? (y/n): ";
                cin >> input;
                if (input == "y" || input == "Y") {
                    self->canTrap = nullptr;
                    // 
                    cout << "Trap card activated!\n";
                } else {
                    cout << "Trap card not activated.\n";
                }
                self->canTrap = nullptr;
            }
            static int linesSeen = 0;
            gameState -> ConsoleClear();
            cout << "Waiting for your turn...\n";

            monitorLog(linesSeen);
            this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }
    delete player1;
    delete player2;
    return 0;
    
}

