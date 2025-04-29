#include <iostream>
#include "card.h"
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

void ConsoleClear()
{
    cout << "\033[2J\033[H";
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

    if (player == "1") {
        json j;
        j["turn"] = "PLAYER1";
        j["last_message"] = "";
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
            if (!state["last_message"].get<std::string>().empty()) {
                cout << "Opponent says: " << state["last_message"] << "\n";
            }
            cout << "It's your turn! Enter a message: ";
            string message;
            getline(std::cin, message);

            string nextPlayer = (player == "1") ? "2" : "1";

            state["last_message"] = message;
            state["turn"] = "PLAYER" + nextPlayer;
            writeToFile(state);
            
        } else {
            ConsoleClear();
            cout << "Waiting for your turn...\n";
            this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }
}

