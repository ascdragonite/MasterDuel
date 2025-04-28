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
        else{
            cout << "This is not a monster card" << endl;
        }
    }
}

void Player :: activateSpell(int handIndex){
    if(handIndex >= 0 && handIndex < hand.size()){
        if(dynamic_cast<SpellCard*>(hand[handIndex])){
            field.push_back(hand[handIndex]);
            hand.erase(hand.begin() + handIndex);
        }
        else{
            cout << "This is not a spell card" << endl;
        }
    }
}

void Player :: setTrap(int handIndex){
    if(handIndex >= 0 && handIndex < hand.size()){
        if(dynamic_cast<TrapCard*>(hand[handIndex])){
            field.push_back(hand[handIndex]);
            hand.erase(hand.begin() + handIndex);
        }
        else{
            cout << "This is not a trap card" << endl;
        }
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
    cout << "Field: " << endl;
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
    deck.push_back(new MonsterCard("Dark Magician", 2500, 2100, "The ultimate wizard in term of attack and defend"));
    deck.push_back(new MonsterCard("Dark Magician", 2500, 2100, "The ultimate wizard in term of attack and defend"));
    deck.push_back(new MonsterCard("Dark Magician Girl", 2000, 1700, "Dark Magician favourite student"));
    deck.push_back(new MonsterCard("Kuriboh", 0, 0, "Little fluffy ball that like to help others"));
    deck.push_back(new SpellCard("Re:End of a dream", "If and only if your lifepoint is lower or equal to 500 you can activate this card. When this card is activated, you can conduct anorther turn"));
    deck.push_back(new SpellCard("Disorted Fate", "Look at your full deck and place a card you like at the top of the deck"));
    deck.push_back(new SpellCard("Oshama Scramble", "Shuffle your opponent's deck when this card is activated"));
    deck.push_back(new SpellCard("Oshama Scramble", "Shuffle your opponent's deck when activated"));
}




