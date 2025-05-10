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

GameState* GameState::instance = nullptr;

void GameState::ConsoleClear()
{
    //cout << "\033[2J\033[H\033[3J";
}

GameState* GameState::getInstance() {
    if (!instance) {
        Player* player1 = new Player(1);
        Player* player2 = new Player(2);
        instance = new GameState(*player1, *player2);
        cout << "GameState instance created.\n";
    }
    return instance;
}

Player* GameState::getPlayer(int id) {
    if (id == 1) {
        return player1;
    } else if (id == 2) {
        return player2;
    } else {
        return nullptr; // Invalid player ID
    }
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
    self.setCannotUseReEndThisTurn(false);
    for (Card* card : self.getField()) {
        MonsterCard* mc = dynamic_cast<MonsterCard*>(card);
        if (mc) mc->clearSummonFlag();
    }

    cout << "\n[Start of Turn Status]\n";
    cout << "Player 1 HP: " << player1->getHp() << " | Deck: " << player1->getDeck().size() << endl;
    cout << "Player 2 HP: " << player2->getHp() << " | Deck: " << player2->getDeck().size() << endl;

    bool hasSummoned = false;
    bool hasBattled = false; 

    self.resetAttackFlags();
    if (!isFirstTurn) {
        self.drawCard();
        cout << "Draws a card.\n";
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
        cout << "\n Enemy Field: ";
        for (int i = 0; i < opponent.getField().size(); ++i) {
            cout << i << ". ";

        MonsterCard* mc = dynamic_cast<MonsterCard*>(opponent.getField()[i]);
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
            for (Card* c : self.getField()) {
             MonsterCard* mc = dynamic_cast<MonsterCard*>(c);
              if (mc && mc->originalAtk != -1) {
               mc->setAtk(mc->originalAtk);
               mc->originalAtk = -1;
               cout << "[Effect Ended] " << mc->getName() << " returns to original ATK.\n";
               }
            }

            for (Card* c : self.getField()) {
            MonsterCard* mc = dynamic_cast<MonsterCard*>(c);
            if (mc) {
            mc->setAttacksThisTurn(0);
            mc->setExtraAttackThisTurn(false);
            }
        }   

            return; // End turn

            case 1:
            if (index >= 0 && index < self.getHand().size()) {
              Card* card = self.getHand()[index];
              string type = card->getType();

              if (type == "Monster") {
                if (!hasSummoned) { 
                self.Summon(index);
                hasSummoned = true;
              } else {
                cout << "You cannot summon more than once per turn.\n";
              }
            } 
            else if (type == "Spell" || type == "Trap") {
                 bool success = card->activateEffect(self, opponent);
                 if (success) {
                    delete card;
                    self.getHand().erase(self.getHand().begin() + index);
                 } else {
                     cout << "[Spell/Trap] Effect was not activated. Card remains in hand.\n";
                 }
            } 
            else {
              cout << "Unsupported card type.\n";
            }
            } else {
              cout << "Invalid index.\n";
            } 
            break;

            case 2:
                self.switchPosition(index);
                break;

            case 3:
            if (self.skipBattlePhaseCount > 0) {
                cout << "You must skip this battle phase due to a card effect.\n";
                self.skipBattlePhaseCount--;
                break;
            }
        
            if (index < 0 || index >= self.getField().size()) {
                cout << "Invalid index.\n";
                break;
            }

            MonsterCard* atkCard = dynamic_cast<MonsterCard*>(self.getField()[index]);
            if (!atkCard) {
               cout << "This is not a monster card.\n";
               break;
            }      

            if (!atkCard->canAttackThisTurn()) {
               cout << atkCard->getName() << " cannot attack anymore this turn.\n";
               break;
            }

            if (opponent.getField().empty()) {
               cout << "Opponent has no monsters.\n";
               cout << "Do you want to attack directly with " << atkCard->getName() << "? (y/n): ";
               char choice;
               cin >> choice;

            if (choice == 'y' || choice == 'Y') {
               int damage = atkCard->getAtk();
               opponent.takeDamage(damage);
               cout << atkCard->getName() << " attacks directly! Opponent loses " << damage << " HP!\n";
               atkCard->setAttacksThisTurn(atkCard->getAttacksThisTurn() + 1);
               hasBattled = true;
            }
            } else {
                 int defendIndex;
                 cout << "Enter the index of the opponent's card to attack: ";
                 cin >> defendIndex;
                 battlePhase(self, opponent, index, defendIndex);
                 atkCard->setAttacksThisTurn(atkCard->getAttacksThisTurn() + 1);
                  hasBattled = true;
            }
            break;

            case 4:
            if (index >= 0 && index < self.getField().size()) {
            MonsterCard* mc = dynamic_cast<MonsterCard*>(self.getField()[index]);
            if (!mc) {
             cout << "This is not a monster card.\n";
             break;
            }

            if (!mc->isFacedown()) {
             cout << mc->getName() << " is already face-up.\n";
             break;
            }

            if (mc->isJustSummoned()) {
             cout << "You cannot reveal a monster that was summoned this turn.\n";
             break;
            }

            mc->reveal();        // lật ngửa + chuyển sang attack
            mc->showInfo();      // in đầy đủ thông tin

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

        this_thread::sleep_for(std::chrono::milliseconds(3000));
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

    *atkCard += *defCard;

    this_thread::sleep_for(chrono::milliseconds(800));
}

bool GameState::checkVictory(const Player& p1, const Player& p2) {
    if (p1.getHp() <= 0 || p1.getDeck().empty()) return true;
    if (p2.getHp() <= 0 || p2.getDeck().empty()) return true;
    return false;
}
