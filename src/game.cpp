#include "game.h"
#include "player.h"
#include "monstercard.h"    
#include "spellcard.h"       
#include "trapcard.h"  
#include <algorithm>
#include <random>
#include <ctime>
#include <iostream>

void GameState::startGame(Player& p1, Player& p2) {
    p1.shuffleDeck();
    p2.shuffleDeck();

    for (int i = 0; i < 5; ++i) {
        p1.drawCard();
        p2.drawCard();
    }
}

void GameState :: playerTurn(Player& self, Player& opponent, bool isFirstTurn, const vector<pair<int, int>>& actionPlan){
    bool hasSummoned = false;
    bool hasBattled = false;

    self.resetAttackFlags();
    if (!isFirstTurn) {
        self.drawCard();
    } //turn 1 ko draw card

    for(const auto& action : actionPlan){
        int code = action.first;
        int index = action.second;

        switch(code){
            case 0:
            return; //endturn

            case 1:
            if(!hasSummoned && !hasBattled){
                self.playMonster(index, false);
                hasSummoned = true;
            }
            break;

            case 2:
            self.activateSpell(index);
            break;

            case 3:
            self.setTrap(index);
            break;

            case 4:
            self.switchPosition(index);
            break;

            case 5:
            if (!hasBattled) {
                battlePhase(self, opponent, index, index);
                hasBattled = true;
            }
            break;

            case 6:
            self.revealMonster(index);
            break;

            default:
            break;
        }
    }
}

void GameState :: battlePhase(Player& attacker, Player& defender, int attackIndex, int defendIndex){
    if (attacker.hasAttacked(attackIndex)) return;

    vector<Card*> atkField = attacker.getField();
    vector<Card*> defField = defender.getField();

    if(attackIndex >= atkField.size() || defendIndex >= defField.size()) return;

    MonsterCard* atkCard = dynamic_cast<MonsterCard*>(atkField[attackIndex]);
    MonsterCard* defCard = dynamic_cast<MonsterCard*>(defField[defendIndex]);

    if(!atkCard || !defCard) return;

    if(defCard -> isFacedown()){
        defCard -> reveal();
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
}

bool GameState :: checkVictory(const Player& p1, const Player& p2){
    if(p1.getHp() <= 0 || p1.getDeck().empty()) return true;
    if(p2.getHp() <= 0 || p2.getDeck().empty()) return true;
    return false;
}
