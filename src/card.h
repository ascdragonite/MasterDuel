#ifndef CARD_H
#define CARD_H
#include <string>
#include "json.hpp"
class Player;
using namespace std;
using json = nlohmann::json;
class Card{
    protected:
    string name;
    string description;
    string type;

    public:
    Card(string name, string type, string description);

    string getName() const;
    string getDescription() const;
    string getType() const;

    void SetName(string n);
    void SetDescription(string d);
    void SetType(string t);

    virtual void showInfo() const = 0;
    virtual json toJson() const = 0;
    virtual void PlayCard(vector<Card*>& field) = 0;
    virtual bool activateEffect(Player& self, Player& opponent) = 0;

    virtual ~Card();
};
#endif