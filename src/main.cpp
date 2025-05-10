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
    cout << "writeing to file\n" << j.dump(4) << std::endl; // Pretty print with indent
    out << j.dump(4) << std::endl; // Pretty print with indent
    out.close();
}


//json readFromFile() {
    //ifstream in("game_state.json");
    //if (!in) {
        //return json(); // empty json
    //}
    //json j;
    //in >> j;
    //in.close();
    //return j;
//}

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
    } catch (json::parse_error& e) {
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
            state["turn"] = "PLAYER" + string((player == "1") ? "2" : "1");
            writeToFile(state);
        } else {
            gameState -> ConsoleClear();
            cout << "Waiting for your turn...\n";
            this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }

    delete player1;
    delete player2;
    return 0;
}

