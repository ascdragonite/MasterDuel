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
    deck.push_back(new SpellCard("The True Power of Chaos Dual", "If you control both Black Luster Soldier and Dark Magician of Chaos:Destroy random cards on the opponent field up to the number of monsters that mention “Dark Magician” you control.Then, boost the ATK of those monsters by 100 for each card destroyed but skip your battle phase this turn."));
    deck.push_back(new SpellCard("The World Destroyer", "Reveal 1 card in your hand that mentions Dark Magician to activate this card then pay half of your Hp : If you control Gandora-X the Dragon of Demolition: Destroy all cards on the field exepct Gandora and for each destroyed monster, inflict 500 damage to your opponent Hp then raise it atk 500 but skip your battle phase this turn."));
    deck.push_back(new SpellCard("Apprentice Helper", "Special Summon 1 Magicians Apprentice from your deck in faceup defend position.Then, draw equal to number of monster that mentions Dark Magician you controll max 2."));
    deck.push_back(new SpellCard("Apprentice Helper", "Special Summon 1 Magicians Apprentice from your deck in faceup defend position.Then, draw equal to number of monster that mentions Dark Magician you controll max 2."));
    deck.push_back(new SpellCard("Apprentice Helper", "Special Summon 1 Magicians Apprentice from your deck in faceup defend position.Then, draw equal to number of monster that mentions Dark Magician you controll max 2."));
    deck.push_back(new SpellCard("Chaos Magic", "tribute 2 monster that mentions Dark Magician: special summon 1 Dark Magician of Chaos and 1 Black Luster Soldier from your deck or your hand to your field in face up defend position and add The True Power of Chaos duel if your deck have them."));
    deck.push_back(new SpellCard("Illusion Magic", "Tribute 1 monster that mention Dark Magician: Add 2 monster cards from your deck to your hand, each of which mentions Dark Magician in their description."));
    deck.push_back(new SpellCard("Illusion Magic", "Tribute 1 monster that mention Dark Magician: Add 2 monster cards from your deck to your hand, each of which mentions Dark Magician in their description."));
    deck.push_back(new SpellCard("Enternal Soul", "Reveal 1 card in your hand that mention Dark Magician special Summon up to 2 monsters from your deck in face-up Defense Position, each of which mentions “Dark Magician” in their description.If you summon both Magicians Rod” and “Magicians Soul this way, draw 2 cards."));
    deck.push_back(new SpellCard("Enternal Soul", "Reveal 1 card in your hand that mention Dark Magician special Summon up to 2 monsters from your deck in face-up Defense Position, each of which mentions “Dark Magician” in their description.If you summon both Magicians Rod” and “Magicians Soul this way, draw 2 cards."));
    deck.push_back(new SpellCard("Enternal Soul", "Reveal 1 card in your hand that mention Dark Magician special Summon up to 2 monsters from your deck in face-up Defense Position, each of which mentions “Dark Magician” in their description.If you summon both Magicians Rod” and “Magicians Soul this way, draw 2 cards."));
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
    deck.push_back(new MonsterCard("Blue-Eyes White Dragon", 3000, 2500, "A legendary dragon whose power is unmatched. With piercing eyes and roaring might, the Blue Eyes White Dragon obliterates all who stand in its path. Its name strikes fear in every heart.", index));
    deck.push_back(new MonsterCard("Blue-Eyes White Dragon", 3000, 2500, "A legendary dragon whose power is unmatched. With piercing eyes and roaring might, the Blue Eyes White Dragon obliterates all who stand in its path. Its name strikes fear in every heart.", index));
    deck.push_back(new MonsterCard("Blue-Eyes White Dragon", 3000, 2500, "A legendary dragon whose power is unmatched. With piercing eyes and roaring might, the Blue Eyes White Dragon obliterates all who stand in its path. Its name strikes fear in every heart.", index));
    deck.push_back(new MonsterCard("Sage with Eyes of Blue", 0, 1500, "A wise sorcerer who communes with the Blue Eyes White Dragon. His incantations call forth the dragon’s power and unlock its hidden might..", index));
    deck.push_back(new MonsterCard("Sage with Eyes of Blue", 0, 1500, "A wise sorcerer who communes with the Blue Eyes White Dragon. His incantations call forth the dragon’s power and unlock its hidden might..", index));
    deck.push_back(new MonsterCard("Maiden of white", 0, 0, "A mysterious girl linked by fate to the Blue Eyes White Dragon. Her silent resolve conceals a force that will awaken when she’s threatened, summoning Blue Eyes to her side in an instant.", index));
    deck.push_back(new MonsterCard("Maiden of white", 0, 0, "A mysterious girl linked by fate to the Blue Eyes White Dragon. Her silent resolve conceals a force that will awaken when she’s threatened, summoning Blue Eyes to her side in an instant.", index));
    deck.push_back(new MonsterCard("Blue-Eyes Twin Burst Dragon", 2500, 2100, "Twin Burst embodies the relentless fury of Blue Eyes evolved.", index));
    deck.push_back(new MonsterCard("Blue-Eyes Twin Burst Dragon", 2500, 2100, "Twin Burst embodies the relentless fury of Blue Eyes evolved.", index));
    deck.push_back(new MonsterCard("Neo Blue-Eyes Ultimate Dragon", 4500, 2100, "The final fusion of three Blue Eyes White Dragons. It stands as the pinnacle of Blue-Eyes power, capable of wiping out foes with consecutive assaults beyond mortal comprehension.", index));
    deck.push_back(new MonsterCard("BLue-Eyes Alternative White Dragon", 3000, 2500, "A different form of the Blue Eyes White Dragon, reborn through light and fury. Its presence on the field is a signal of unstoppable force. It may not be summoned normally, but its power exceeds all expectations.", index));
    deck.push_back(new MonsterCard("Deep-Eyes White Dragon", 3500, 2500, "This legendary dragon is a powerful engine of destruction. Virtually invincible, very few have faced this awesome creature and lived to tell the tale.", index));
    deck.push_back(new MonsterCard("Blue-Eyes Chaos MAX Dragon", 4000, 0, "An incarnation of chaos forged from the spirit of Blue Eyes. With piercing power and absolute defense, the Chaos MAX Dragon crushes foes in a single overwhelming blow.", index));
    deck.push_back(new SpellCard("Re:End of a Dream", "If and only if your lifepoint is lower or equal to 1000 you can activate this card. When this card is activated, you can conduct anorther turn"));
    //deck.push_back(new SpellCard("Disorted Fate", "Look at your full deck and place a card you like at the top of the deck."));
    deck.push_back(new SpellCard("Blessing from the Roaring Sky", "Search any card that mention Blue Eyes in it's description."));
    deck.push_back(new SpellCard("Blessing from the Roaring Sky", "Search any card that mention Blue Eyes in it's description."));
    deck.push_back(new SpellCard("Blessing from the Roaring Sky", "Search any card that mention Blue Eyes in it's description."));
    deck.push_back(new SpellCard("Flower Snow Drum n Bass", "Flip coin, winner gets draw 2 cards"));
    deck.push_back(new SpellCard("Flower Snow Drum n Bass", "Flip coin, winner gets draw 2 cards"));
    //deck.push_back(new SpellCard("Flower Snow Drum n Bass", "Flip coin, winner gets draw 2 cards"));
    //deck.push_back(new SpellCard("World Vanquisher", "Buff a monster attack by 1000 when this card is activated."));
    deck.push_back(new SpellCard("Call of the Sky", "Tribute 2 monster on the field to search 1 BLue Eyes White Dragon and Majesty of The White Dragons from your deck to your hand."));
    deck.push_back(new SpellCard("Dragon United", "Buff a monster attack and defense by 500 for each faceup monster you control when this card is activated."));
    deck.push_back(new SpellCard("Burst Stream of Destruction", "If you controll a Blue Eyes White Dragon destroy all card your opponent controll but skip your next 2 battle phase."));
    deck.push_back(new SpellCard("Rage of The Blue Eyes", "Sacrifice 2/3 of your lifepoint, “Blue Eyes White Dragon” you controll can now attack twice in a turn ( can not active “Re:End of a dream” the turn you activate this card).")); 
    deck.push_back(new SpellCard("Destr0yer",("Can destroy a monster card in defense mode")));
    deck.push_back(new SpellCard("Roar of The Blue-Eyed Dragons", "Special summon a Blue Eyes White Dragon in the deck in defend position."));
    deck.push_back(new SpellCard("Roar of The Blue-Eyed Dragons", "Special summon a Blue Eyes White Dragon in the deck in defend position."));
    deck.push_back(new SpellCard("Majesty of The White Dragons","Destroy random card in your opponent field up to the number of faceup “Blue Eyes White Dragon” you control"));
    deck.push_back(new SpellCard("Majesty of The White Dragons","Destroy random card in your opponent field up to the number of faceup “Blue Eyes White Dragon” you control"));
    deck.push_back(new SpellCard("Ash Again","If you controll a faceup Blue Eyes White Dragon destroy all trap cards your opponent control")); 
    deck.push_back(new SpellCard("Wishes for Eyes of Blue","Reveal 1 card in your hand that mention Blue Eyes. Special Summon up to 2 monsters from your deck in face-up Defense Position, each of which mentions “Blue Eyes” in their description."));
    deck.push_back(new SpellCard("Wishes for Eyes of Blue","Reveal 1 card in your hand that mention Blue Eyes. Special Summon up to 2 monsters from your deck in face-up Defense Position, each of which mentions “Blue Eyes” in their description."));
    deck.push_back(new SpellCard("Wishes for Eyes of Blue","Reveal 1 card in your hand that mention Blue Eyes. Special Summon up to 2 monsters from your deck in face-up Defense Position, each of which mentions “Blue Eyes” in their description."));
    deck.push_back(new SpellCard("Overdose","You can exchange 1 point of your HP to increase the ATK of a monster card that mentions 'Blue Eyes' in its description by 1. The total increase cannot exceed half of your current HP."));
    deck.push_back(new SpellCard("Overdose","You can exchange 1 point of your HP to increase the ATK of a monster card that mentions 'Blue Eyes' in its description by 1. The total increase cannot exceed half of your current HP."));
    deck.push_back(new SpellCard("True Light", "Reveal 1 card in your hand that mention Blue Eyes, special Summon up to 2 monsters from your deck in face-up Defense Position, each of which mentions Blue Eyes in their description.If you summon both Sage with Eyes of Blue and Maiden of white this way add 1 card that mention Blue Eyes form your deck to your hand,"));
    deck.push_back(new SpellCard("True Light", "Reveal 1 card in your hand that mention Blue Eyes, special Summon up to 2 monsters from your deck in face-up Defense Position, each of which mentions Blue Eyes in their description.If you summon both Sage with Eyes of Blue and Maiden of white this way add 1 card that mention Blue Eyes form your deck to your hand,"));
    deck.push_back(new SpellCard("True Light", "Reveal 1 card in your hand that mention Blue Eyes, special Summon up to 2 monsters from your deck in face-up Defense Position, each of which mentions Blue Eyes in their description.If you summon both Sage with Eyes of Blue and Maiden of white this way add 1 card that mention Blue Eyes form your deck to your hand,"));
    deck.push_back(new SpellCard("The Melody of the Awakening Dragon", "If you control a monster that mentions 'Blue-Eyes' in its description, add 1 monster that mentions 'Blue Eyes' from your deck to your hand; if you also control both 'Sage with Eyes of Blue' and 'Maiden of White', you can instead add any 1 card that mentions 'Blue Eyes' in its description."));
    deck.push_back(new SpellCard("The Melody of the Awakening Dragon", "If you control a monster that mentions 'Blue-Eyes' in its description, add 1 monster that mentions 'Blue Eyes' from your deck to your hand; if you also control both 'Sage with Eyes of Blue' and 'Maiden of White', you can instead add any 1 card that mentions 'Blue Eyes' in its description."));
    deck.push_back(new SpellCard("The Melody of the Awakening Dragon", "If you control a monster that mentions 'Blue-Eyes' in its description, add 1 monster that mentions 'Blue Eyes' from your deck to your hand; if you also control both 'Sage with Eyes of Blue' and 'Maiden of White', you can instead add any 1 card that mentions 'Blue Eyes' in its description."));
}

void Player::shuffleDeck(){
    auto rng = default_random_engine(time(0));
    shuffle(deck.begin(), deck.end(), rng);
}





