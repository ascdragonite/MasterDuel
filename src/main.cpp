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

using namespace std;

using json = nlohmann::json;




void writeToFile(const json& j) {
    ofstream out("game_state.json");
    if (!out) {
        cerr << "Error opening file for writing.\n";
        return;
    }
    out << j.dump(4) << std::endl; // Pretty print with indent
    out.close();
}

json readFromFile() {
    ifstream in("game_state.json");
    if (!in) {
        return json(); // empty json
    }
    json j;
    in >> j;
    in.close();
    return j;
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


    Player* player1 = new Player();
    Player* player2 = new Player();
    GameState gameState = GameState(*player1, *player2);

    if (player == "1") {

        json j;
        j["Player1"] = *player1;
        j["Player2"] = *player2;
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
            gameState.ConsoleClear();
            gameState.startGame();
            cout << "It's your turn!\n";

            if (player == "1") {
                gameState.playerTurn(*player1, *player2, false);
            } else {
                gameState.playerTurn(*player2, *player1, false);
            }

            // Check for victory
            if (gameState.checkVictory(*player1, *player2)) {
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
            state["turn"] = "PLAYER" + string((player == "1") ? "2" : "1");
            writeToFile(state);
        } else {
            gameState.ConsoleClear();
            cout << "Waiting for your turn...\n";
            this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }

    delete player1;
    delete player2;
    return 0;
}

