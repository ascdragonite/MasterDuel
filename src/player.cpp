#include "player.h"
#include "monstercard.h"
#include "spellcard.h"
#include "trapcard.h"
#include <iostream>
using namespace std;

Player :: Player(){
    hp = 4000;
}

void Player :: drawCard(){
    if(!deck.empty()){
        hand.push_back(deck.back());
        deck.pop_back();
    }
    else{
        cout << "Unable to draw card" << endl;
    }
}

void Player :: playMonster(int handIndex){
    if(handIndex >= 0 && handIndex < hand.size()){
        if(dynamic_cast<MonsterCard*>(hand[handIndex])){
            field.push_back(hand[handIndex]);
            hand.erase(hand.begin() + handIndex);
        }
    }
    else{
        cout << "This is not a monster card" << endl;
    }
}

void Player :: activateSpell(int handIndex){
    if(handIndex >= 0 && handIndex < hand.size()){
        if(dynamic_cast<SpellCard*>(hand[handIndex])){
            field.push_back(hand[handIndex]);
            hand.erase(hand.begin() + handIndex);
        }
    }
    else{
        cout << "This is not a spell card" << endl;
    }
}

void Player :: setTrap(int handIndex){
    if(handIndex >= 0 && handIndex < hand.size()){
        if(dynamic_cast<TrapCard*>(hand[handIndex])){
            field.push_back(hand[handIndex]);
            hand.erase(hand.begin() + handIndex);
        }
    }
    else{
        cout << "This is not a trap card" << endl;
    }
}

void Player :: showHand() const{
    cout << "Hand: " << endl;
    for(int i = 0 ; i < hand.size() ; i++){
        cout << i << " ";
        hand[i] -> showInfo();
        cout << endl;
    }
}

void Player :: showField() const{
    cout << "Fiend: " << endl;
    for(int i = 0 ; i < field.size() ; i++){
        cout << i << " ";
        field[i] -> showInfo();
        cout << endl;
    }
}

void Player :: takeDamage(int amount){
    hp -= amount;
    if(hp < 0) hp = 0;
}

int Player :: getHp() const{
    return hp;
}

vector<Card*>& Player :: getDeck(){
    return deck;
}

vector<Card*>& Player :: getField(){
    return field;
}

void Player :: loadDeckDarkMagician(){
    
}




