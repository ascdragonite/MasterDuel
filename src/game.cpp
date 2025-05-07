#include "game.h"
#include "player.h"
#include "monstercard.h"    
#include "spellcard.h"       
#include "trapcard.h"  
#include <algorithm>
#include <random>
#include <ctime>
#include <iostream>
#include <thread>
#include <chrono>

void GameState::ConsoleClear()
{
    //cout << "\033[2J\033[H\033[3J";
}

GameState::GameState(Player& p1, Player& p2) 
{
    player1 = &p1;
    player2 = &p2;
    player1 -> loadDeckDarkMagician();
    player2 -> loadDeckBlueEyes();
}

void GameState::startGame() {
    player1 -> shuffleDeck();
    player2 -> shuffleDeck();
    cout << "Game started!\n";

    for (int i = 0; i < 4; i++) {
        player1 -> drawCard();
        player2 -> drawCard();
    }
}

void GameState::playerTurn(Player& self, Player& opponent, bool isFirstTurn) {
    bool hasSummoned = false;
    bool hasBattled = false; 

    self.resetAttackFlags();
    if (!isFirstTurn) {
        self.drawCard();
    }

    while (true) {
        ConsoleClear();
        cout << "Hand: ";
        int i = 0;
        for (int i = 0; i < self.getHand().size(); ++i) {
            cout << "Index " << i << ":\n";
            self.getHand()[i]->showInfo();
            cout << "-----------------------------\n";
        }
        cout << "\nField: ";
        for (int i = 0; i < self.getField().size(); ++i) {
            cout << i << ". " << self.getField()[i]->getName() << " | ";
        }

        cout << "\nChoose an action:\n";
        cout << "0: End Turn\n";
        cout << "1: Play Card\n";
        cout << "2: Switch Monster Position\n";
        cout << "3: Attack\n";
        cout << "4: Reveal Monster\n";
        cout << "Enter your choice: ";


        int code, index = -1;
        cin >> code;

        if (code != 0) {
            cout << "Enter the index of the card: ";
            cin >> index;
        }

        switch (code) {
            case 0:
                return; // End turn

            case 1:
                if (!hasSummoned) {
                    self.Summon(index);
                    hasSummoned = true;
                } else {
                    cout << "You cannot summon more than once per turn.\n";
                }
                break;
            case 2:
                self.switchPosition(index);
                break;

            case 3:
                if (!self.hasAttacked(index)) {
                    int defendIndex;
                    cout << "Enter the index of the opponent's card to attack: ";
                    cin >> defendIndex;
                    battlePhase(self, opponent, index, defendIndex);
                    hasBattled = true;
                } else {
                    cout << "This monster already battle.\n";
                }
            
                break;

            case 4:
                self.revealMonster(index);
                break;

            default:
                cout << "Invalid action. Try again.\n";
                break;
        }

        // Pause to let the player see the result of their action
        this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

void GameState :: battlePhase(Player& attacker, Player& defender, int attackIndex, int defendIndex){
    if (attacker.hasAttacked(attackIndex)){
        cout << "This monster already battle" << endl;
        return;
    }

    vector<Card*> atkField = attacker.getField();
    vector<Card*> defField = defender.getField();

    if(attackIndex >= atkField.size() || defendIndex >= defField.size()){
        cout << "Invalid indexes" << endl;
        return;
    }

    MonsterCard* atkCard = dynamic_cast<MonsterCard*>(atkField[attackIndex]);
    MonsterCard* defCard = dynamic_cast<MonsterCard*>(defField[defendIndex]);

    if(!atkCard || !defCard){
        cout << "Invalid cards for battle" << endl;
        return;
    }

    cout << atkCard->getName() << " attacks " << defCard->getName() << endl;

    if(defCard -> isFacedown()){
        defCard -> reveal();
        cout << "The defending card was face-down. It is now revealed: ";
        defCard -> showInfo();
    }

    int atkValue = atkCard -> getAtk();
    int defValue = defCard -> isInDefense() ? defCard -> getDef() : defCard -> getAtk();

    if(defCard -> isInDefense()){
        if(atkValue > defValue){
            auto field = defender.getField();
            delete field[defendIndex];
            field.erase(field.begin() + defendIndex);
            defender.setField(field);
        }
        else if(atkValue < defValue){
            attacker.takeDamage(defValue - atkValue);
        }
        else{
            //atk = def
            return;
        }
    }

    else{
        if(atkValue > defValue){
            defender.takeDamage(atkValue - defValue);
            auto field = defender.getField();
            delete field[defendIndex];
            field.erase(field.begin() + defendIndex);
            defender.setField(field);
        }
        if (defender.getField().empty()) {
            cout << "Opponent has no more monsters on the field!\n";
        }
        else if(atkValue < defValue){
            attacker.takeDamage(defValue - atkValue);
            auto field = attacker.getField();
            delete field[attackIndex];
            field.erase(field.begin() + attackIndex);
            attacker.setField(field);
        }
        else{
            //atk = atk
            auto field1 = attacker.getField();
            auto field2 = defender.getField();
            delete field1[attackIndex];
            delete field2[defendIndex];
            field1.erase(field1.begin() + attackIndex);
            field2.erase(field2.begin() + defendIndex);
            attacker.setField(field1);
            defender.setField(field2);
        }
    }

    attacker.setAttacked(attackIndex);
    this_thread::sleep_for(chrono::milliseconds(800));
}

bool GameState :: checkVictory(const Player& p1, const Player& p2){
    if(p1.getHp() <= 0 || p1.getDeck().empty()) return true;
    if(p2.getHp() <= 0 || p2.getDeck().empty()) return true;
    return false;
}
