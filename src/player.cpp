#include "player.h"
#include "card.h"
#include "monstercard.h"
#include "spellcard.h"
#include "trapcard.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <random>
#include <vector>
#include <ctime>
#include "serialize.h"


using namespace std;

int Player::getIndex() const
{
    return index;
}

void DumpInfo(Player& player)
{
    json j;
    ifstream in("game_state.json");
    if (in) {
        in >> j;
        in.close();
    }
    j["Player" + to_string(player.getIndex())] = player;
    ofstream out("game_state.json");
    if (!out) {
        cerr << "Error opening file for writing.\n";
        return;
    }
    out << j.dump(4) << std::endl; // Pretty print with indent
    out.close();
}

void Player::setSkipBattlePhaseCount(int count)
{ 
    if (count < 0) {
        skipBattlePhaseCount = 0;
        return;
    }
    skipBattlePhaseCount = count; 
}

int Player::getSkipBattlePhaseCount() const
{ return skipBattlePhaseCount; }

Player :: Player(int i){
    index = i;
    hp = 8000;
    skipBattlePhaseCount = 0;
    hasBattledThisTurn = false; 
}

Player::~Player()
{
    for(Card* card : deck)
    {
        delete card;
    }
    for(Card* card : field)
    {
        delete card;
    }
    for(Card*card : hand)
    {
        delete card;
    }
}

void Player :: drawCard(){
    if(!deck.empty()){
        hand.push_back(deck.back());
        deck.pop_back();
    }
    DumpInfo(*this);
}

void Player::Summon(int handIndex) {
    if (handIndex >= 0 && handIndex < static_cast<int>(hand.size())) {
        Card* card = hand[handIndex];
        MonsterCard* monster = dynamic_cast<MonsterCard*>(card);

        if (monster) {
            // Ngăn switch tư thế trong lượt được triệu hồi
            // Nếu đã battle trước khi triệu hồi → không được phép attack
            if (getHasBattledThisTurn()) {
                monster->setJustSummoned(true);
            } else {
                monster->setJustSummoned(false); // được phép attack
            }
        }

        card->PlayCard(field); // thêm vào sân (và hỏi attack/defense mode)
        hand.erase(hand.begin() + handIndex);
    }

    DumpInfo(*this);
}

void Player::resetAttackFlags() {
    attackedThisTurn = vector<bool>(5, false);
    hasBattledThisTurn = false;

    // Reset quyền đổi vị trí cho quái vật mỗi lượt
    for (Card* card : field) {
        MonsterCard* m = dynamic_cast<MonsterCard*>(card);
        if (m) {
            m->setCanSwitchPosition(true); 
        }
    }
}

bool Player::hasAttacked(int index) const {
    return index >= 0 && index < attackedThisTurn.size() && attackedThisTurn[index];
}

bool Player::getHasBattledThisTurn() const {
    return hasBattledThisTurn;
}

void Player::setHasBattledThisTurn(bool value) {
    hasBattledThisTurn = value;
}

void Player::setAttacked(int index, bool value) {
    if(index >= 0 && index < attackedThisTurn.size())
        attackedThisTurn[index] = value;
}



void Player::switchPosition(int fieldIndex) {
    if (fieldIndex < 0 || fieldIndex >= field.size()) {
        cout << "Invalid index.\n";
        return;
    }

    MonsterCard* m = dynamic_cast<MonsterCard*>(field[fieldIndex]);
    if (!m) {
        cout << "This is not a monster card.\n";
        return;
    }

    if (m->isFacedown()) {
        cout << "Please use function reveal monster to switch the position of a facedown monster.\n";
        return;
    }

    if (hasAttacked(fieldIndex)) {
        cout << "This monster already attacked this turn.\n";
        return;
    }

    if (m->isJustSummoned()) {
        cout << "You cannot switch the position of this monster right now.\n";
        return;
    }

    // Toggle position
    bool newPos = !m->isInDefense();
    m->setDefenseMode(newPos);

    cout << m->getName() << " switched to " << (newPos ? "Defense" : "Attack") << " position.\n";

    DumpInfo(*this);
}

void Player::revealMonster(int fieldIndex) {
    if (fieldIndex >= 0 && fieldIndex < field.size()) {
        MonsterCard* m = dynamic_cast<MonsterCard*>(field[fieldIndex]);
        if (m && m->isFacedown()) {
            m->reveal();                       
            m->setDefenseMode(false);          
            field[fieldIndex]->showInfo();
        }
    }
    DumpInfo(*this);
}




/* 
vector<string> Player :: getHandInfo() const{
    vector<string> info;
    for(auto card : hand){
        info.push_back(card -> getName());
    }
    return info;
}

vector<string> Player::getFieldInfo() const {
    vector<string> info;
    for (auto card : field) {
        info.push_back(card->getName());
    }
    return info;
} */

void Player :: takeDamage(int amount){
    hp -= amount;
    if(hp < 0) hp = 0;
}

int Player :: getHp() const{
    return hp;
}

vector<Card*> Player::getHand() const {
    return hand;
}

vector<Card*> Player :: getDeck() const {
    return deck;
}

vector<Card*> Player :: getField() const{
    return field;
}

void Player::setHand(vector<Card*> newHand)
{
    for(Card* card : hand)
    {
        //delete card;
    }
    hand.clear();

    hand = newHand;
}

void Player::setField(vector<Card*> newField)
{
    for(Card* card : field)
    {
        //delete card;
    }
    field.clear();

    field = newField;

    attackedThisTurn.resize(5, false);
}
void Player::setDeck(vector<Card*> newDeck)
{
    for(Card* card : deck)
    {
        //delete card;
    }
    deck.clear();

    deck = newDeck;
}

vector<Card*>& Player::getDeckRef() {
    return deck;
}

vector<Card*>& Player::getFieldRef() {
    return field;
}

void Player::setHp(int hp)
{
    this -> hp = hp; 
}

vector<Card*>& Player::getHandRef() {
    return hand;
}


void Player :: loadDeckDarkMagician(){
    cout << "Loading deck for player " << index << endl;
    deck.push_back(new MonsterCard("Dark Magician", 2500, 2100, "The ultimate wizard in term of attack and defense.", index));
    deck.push_back(new MonsterCard("Dark Magician", 2500, 2100, "The ultimate wizard in term of attack and defense.", index));
    deck.push_back(new MonsterCard("Magicians Apprentice", 2000, 1700, "A devoted young sorcerer who studies every incantation recorded by the legendary Dark Magician. Though her power is still blooming, her will to protect her master is unwavering.", index));
    deck.push_back(new MonsterCard("Magicians Apprentice", 2000, 1700, "A devoted young sorcerer who studies every incantation recorded by the legendary Dark Magician. Though her power is still blooming, her will to protect her master is unwavering.", index));
    deck.push_back(new MonsterCard("Dark Magician Girl", 2000, 1700, "Dark Magician favourite student.", index));
    deck.push_back(new MonsterCard("Magicians Soul", 0, 0, "The remnant spirit of the first disciple who ever trained under the Dark Magician. Though he possesses no form of his own, his essence amplifies the will of every magician he supports.", index));
    deck.push_back(new MonsterCard("Magicians Soul", 0, 0, "The remnant spirit of the first disciple who ever trained under the Dark Magician. Though he possesses no form of his own, his essence amplifies the will of every magician he supports.", index));
    deck.push_back(new MonsterCard("Black Luster Soldier", 3200, 3000, "A noble warrior summoned through ancient chaos rituals inspired by the teachings of the Dark Magician. Though his path diverges, their souls resonate through battle and destiny.", index));
    deck.push_back(new MonsterCard("Dark Magician of Chaos", 2800, 2600, "A dark reincarnation born when the powers of the Dark Magician merge with the forbidden magic of chaos. His mastery over destruction and revival surpasses mortal boundaries.", index));
    deck.push_back(new MonsterCard("Gandora-X the Dragon of Demolition", 0, 0, "A destructive force awakened by the incantations of the Dark Magician, forged to erase everything in its path. The dragon exists solely to fulfill the will of absolute magical judgment.", index));
    deck.push_back(new MonsterCard("Magicians Rod", 1600, 100, "This mystical staff has chosen its wielder and resonates only with those who follow the path of the Dark Magician. Channeling ancient magic, it guides spellcasters to the secrets long forgotten.", index));
    deck.push_back(new MonsterCard("Magicians Rod", 1600, 100, "This mystical staff has chosen its wielder and resonates only with those who follow the path of the Dark Magician. Channeling ancient magic, it guides spellcasters to the secrets long forgotten.", index));
    deck.push_back(new SpellCard("Dark Magic Veil", "Special summon a Dark Magician in the deck in defend position."));
    deck.push_back(new SpellCard("The Ancient knowledge", "Search any card that mention Dark Magician in it's description."));
    deck.push_back(new SpellCard("The Ancient knowledge", "Search any card that mention Dark Magician in it's description."));
    deck.push_back(new SpellCard("The Ancient knowledge", "Search any card that mention Dark Magician in it's description."));
    deck.push_back(new SpellCard("The True Power of Chaos Dual", "If you control both Black Luster Soldier and Dark Magician of Chaos:Destroy cards on the field up to the number of monsters that mention “Dark Magician” you control.Then, boost the ATK of those monsters by 100 for each card destroyed."));
    deck.push_back(new SpellCard("The World Destroyer", "Reveal 1 card in your hand that mentions Dark Magician to activate this card.If you control “Gandora-X the Dragon of Demolition”: Destroy all cards on the field.For each destroyed monster, inflict 500 damage to your opponent Hp."));
    deck.push_back(new SpellCard("Apprentice Helper", "Special Summon 1 Magicians Apprentice from your deck in faceup defend position.Then, draw equal to number of monster that mentions Dark Magician you controll max 2."));
    deck.push_back(new SpellCard("Apprentice Helper", "Special Summon 1 Magicians Apprentice from your deck in faceup defend position.Then, draw equal to number of monster that mentions Dark Magician you controll max 2."));
    deck.push_back(new SpellCard("Apprentice Helper", "Special Summon 1 Magicians Apprentice from your deck in faceup defend position.Then, draw equal to number of monster that mentions Dark Magician you controll max 2."));
    deck.push_back(new SpellCard("Chaos Magic", "tribute 2 monster that mentions Dark Magician: special summon 1 Dark Magician of Chaos and 1 Black Luster Soldier from your deck to your field in face up defend position."));
    deck.push_back(new SpellCard("Illusion Magic", "Tribute 1 monster that mention Dark Magician: Add 2 monster cards from your deck to your hand, each of which mentions Dark Magician in their description. You can only activate 1 Secrets of the Dark Circle per turn."));
    deck.push_back(new SpellCard("Illusion Magic", "Tribute 1 monster that mention Dark Magician: Add 2 monster cards from your deck to your hand, each of which mentions Dark Magician in their description. You can only activate 1 Secrets of the Dark Circle per turn."));
    deck.push_back(new SpellCard("Illusion Magic", "Tribute 1 monster that mention Dark Magician: Add 2 monster cards from your deck to your hand, each of which mentions Dark Magician in their description. You can only activate 1 Secrets of the Dark Circle per turn."));
    deck.push_back(new SpellCard("Enternal Soul", "Reveal 1 card in your hand that mention Dark Magician special Summon up to 2 monsters from your deck in face-up Defense Position, each of which mentions “Dark Magician” in their description.If you summon both Magicians Rod” and “Magicians Soul this way, draw 2 cards.You can only activate 1 “Enternal Soul” per turn."));
    deck.push_back(new SpellCard("Enternal Soul", "Reveal 1 card in your hand that mention Dark Magician special Summon up to 2 monsters from your deck in face-up Defense Position, each of which mentions “Dark Magician” in their description.If you summon both Magicians Rod” and “Magicians Soul this way, draw 2 cards.You can only activate 1 “Enternal Soul” per turn."));
    deck.push_back(new SpellCard("Enternal Soul", "Reveal 1 card in your hand that mention Dark Magician special Summon up to 2 monsters from your deck in face-up Defense Position, each of which mentions “Dark Magician” in their description.If you summon both Magicians Rod” and “Magicians Soul this way, draw 2 cards.You can only activate 1 “Enternal Soul” per turn."));
    deck.push_back(new SpellCard("Soul Servant", "Place a card that mention Dark Magician from your deck to your top of the deck and draw card equal to number of monster that mention Dark Magician on the field max 2 cards."));
    deck.push_back(new SpellCard("Soul Servant", "Place a card that mention Dark Magician from your deck to your top of the deck and draw card equal to number of monster that mention Dark Magician on the field max 2 cards."));
    deck.push_back(new SpellCard("Soul Servant", "Place a card that mention Dark Magician from your deck to your top of the deck and draw card equal to number of monster that mention Dark Magician on the field max 2 cards."));
    deck.push_back(new SpellCard("Cruel Pact", "Pay 500 Hp and tribute 1 monster on the field to search 1 Dark Magician from your deck and increase 600 ATK for the Dark Magician summon next turn."));
    deck.push_back(new SpellCard("Thousand Knifes", "If you control faceup Dark Magician: Target 1 monster your opponent controls; destroy that target.."));
    deck.push_back(new SpellCard("Dark Magic", " If you controll faceup a Dark Magician monster, destroy all monster your opponent controll and skip your battle phase."));
    deck.push_back(new SpellCard("Bond Between The Teacher and Student", "If you controll a Dark Magician, special summon from your deck a Dark Magician Girl in defend position "));
    deck.push_back(new SpellCard("Dark Burning Magic", "If you control a face up Dark Magician, Dark magician girl you can destroy all of your oponent card but skip your battle phase"));
    deck.push_back(new SpellCard("The Power of Friendship", "Combine all of your faceup monster atk to 1 single attack in this turn."));
    deck.push_back(new TrapCard("Mirror Force", "When an opponent's monster declares an attack: Destroy all your opponent's Attack Position monsters."));
    deck.push_back(new TrapCard("Mirror Force", "When an opponent's monster declares an attack: Destroy all your opponent's Attack Position monsters."));
    deck.push_back(new TrapCard("Tsunagite", "When an opponent's monster declares an attack: Negate the attack and end the battle phase."));
    deck.push_back(new TrapCard("Trrricksters!!", "When an opponent's monster declares an attack: Target the attacking monster, and if you do, inflict damage to your opponent equal to its atk."));
}

void Player :: loadDeckBlueEyes(){
    cout << "Loading deck for player " << index << endl;
    deck.push_back(new MonsterCard("Blue-Eyes White Dragon", 3000, 2500, "This legendary dragon is a powerful engine of destruction. Virtually invincible, very few have faced this awesome creature and lived to tell the tale.", index));
    deck.push_back(new MonsterCard("Blue-Eyes White Dragon", 3000, 2500, "This legendary dragon is a powerful engine of destruction. Virtually invincible, very few have faced this awesome creature and lived to tell the tale.", index));
    deck.push_back(new MonsterCard("Blue-Eyes White Dragon", 3000, 2500, "This legendary dragon is a powerful engine of destruction. Virtually invincible, very few have faced this awesome creature and lived to tell the tale.", index));
    deck.push_back(new MonsterCard("T+pazolite", 1750, 200, "A crystal dragon born from solidified molten lava. T+pazolite's scales shimmer like gemstones, absorbing and reflecting light to confuse enemies on the battlefield.", index));
    deck.push_back(new MonsterCard("Morimori Atsushi", 1800, 0, "A nomadic dragon god wandering through the eastern forests. With no defensive armor, it relies on immense speed and broad wings to outmaneuver and strike down any who dare approach.", index));
    deck.push_back(new MonsterCard("Feryquitous", 1500, 400, "A young flame dragon constantly yearning to prove its might. Its immature blue flames often ignite unexpected wildfires across the land.", index));
    deck.push_back(new MonsterCard("BLue-Eyes Alternative White Dragon", 3000, 2500, "This legendary dragon is a powerful engine of destruction. Virtually invincible, very few have faced this awesome creature and lived to tell the tale.", index));
    deck.push_back(new MonsterCard("Deep-Eyes White Dragon", 3500, 2500, "This legendary dragon is a powerful engine of destruction. Virtually invincible, very few have faced this awesome creature and lived to tell the tale.", index));
    deck.push_back(new MonsterCard("Kuroma", 2100, 700, "A shadow dragon, slender yet deadly. Kuroma hides within dark clouds, striking without warning and bringing silent destruction to its prey.", index));
    deck.push_back(new MonsterCard("Laur", 1450, 1000, "An ancient dragon that once guarded sacred lands. Though its strength has waned with time, Laur’s unwavering spirit remains, ready to sacrifice itself to halt any invader’s advance.", index));
    deck.push_back(new MonsterCard("Sasakure", 1900, 0, "A razorwind dragon whose translucent body leaves only trails of slashing winds. Without any defenses, it relies entirely on blinding speed and lethal precision.", index));
    deck.push_back(new MonsterCard("BlackY", 1500, 500, "A black dragon born from the ashes of fallen stars. Though not powerful in offense, BlackY possesses the stubborn will to rise from ruins and survive every battle.", index));
    deck.push_back(new MonsterCard("Camellia", 2000, 1600, "A divine flower dragon, rare and mesmerizing. Camellia commands deadly blossoms, blending beauty with fatal poison in a delicate yet deadly dance.", index));
    deck.push_back(new MonsterCard("Sakuzyo", 2300, 900, "A sonic dragon traveling across dimensions through ultrasonic waves. Sakuzyo’s roar can shatter metal, and those who hear it often lose their sense of direction completely.", index));
    deck.push_back(new SpellCard("Re:End of a Dream", "If and only if your lifepoint is lower or equal to 1000 you can activate this card. When this card is activated, you can conduct anorther turn"));
    deck.push_back(new SpellCard("Disorted Fate", "Look at your full deck and place a card you like at the top of the deck."));
    deck.push_back(new SpellCard("Oshama Scramble", "Shuffle your opponent's deck when this card is activated."));
    deck.push_back(new SpellCard("Flower Snow Drum n Bass", "Flip coin, winner gets draw 2 cards"));
    deck.push_back(new SpellCard("Flower Snow Drum n Bass", "Flip coin, winner gets draw 2 cards"));
    deck.push_back(new SpellCard("World Vanquisher", "Buff a monster attack by 200 when this card is activated."));
    deck.push_back(new SpellCard("Call of the Sky", "Tribute 2 monster on the field to search 1 BLue-Eyes WHite Dragon and Majesty of The White Dragons from your deck to your hand."));
    deck.push_back(new SpellCard("Dragon United", "Buff a monster attack and defense by 100 for each faceup monster you control when this card is activated."));
    deck.push_back(new SpellCard("Burst Stream of Destruction", "If you controll a Blue eye white dragon destroy all card your opponent controll but skip your next 2 battle phase."));
    deck.push_back(new SpellCard("Rage of The Blue Eyes", "Sacrifice 2/3 of your lifepoint, “Blue Eyes White Dragon” you controll can now attack twice in a turn ( can not active “Re:End of a dream” the turn you activate this card).")); 
    deck.push_back(new SpellCard("Destr0yer",("Can destroy a monster card in defense mode")));
    deck.push_back(new SpellCard("Roar of The Blue-Eyed Dragons", "Special summon a “Blue-Eyes White Dragon” in the deck in defend position."));
    deck.push_back(new SpellCard("Roar of The Blue-Eyed Dragons", "Special summon a “Blue-Eyes White Dragon” in the deck in defend position."));
    deck.push_back(new SpellCard("Majesty of The White Dragons","Destroy random card in your opponent field up to the number of faceup “Blue-Eyes White Dragon” you control"));
    deck.push_back(new SpellCard("Majesty of The White Dragons","Destroy random card in your opponent field up to the number of faceup “Blue-Eyes White Dragon” you control"));
    deck.push_back(new SpellCard("Ash Again","If you controll a faceup Blue-Eyes White Dragon destroy all trap cards your opponent control")); 
}

void Player::shuffleDeck(){
    auto rng = default_random_engine(time(0));
    shuffle(deck.begin(), deck.end(), rng);
}





