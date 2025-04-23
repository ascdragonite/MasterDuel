#ifndef CARD_H
#define CARD_H

#include <string>
using namespace std;

class Card{
    protected:
    string name;
    string description;
    string type;

    public:
    Card(string name, string description, string type);

    string getName() const;
    string getDescription() const;
    string getType() const;

    virtual void showInfo() const = 0;

    virtual ~Card(){}
};
#endif