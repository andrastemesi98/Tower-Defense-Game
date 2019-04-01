#ifndef PLAYER_H
#define PLAYER_H

#include "basics.h"
#include "unit.h"
#include "tower.h"
#include "field.h"

namespace model {

class unit;
class tower;
class field;

class player
{

public:
    player(const std::string& nin, int def_gold);

    // modifying functions:
    void update();

    void removeUnit(unit*);
    void removeTower(tower*);
    void addGold(int amount);
    bool placeCreature(int ID, field* loc);
    // placeCreature(..) actually creates the creature from ID
    bool placeTower(int ID, field* loc);

    // getters:
    inline const std::string& name() const {return _name;}
    inline int gold() const {return _gold;}
    inline std::vector<unit*> const& units() const {return _units;}
    inline std::vector<field*> const& fields() const {return _fields;}


private:
    std::vector<unit *> _units;
    std::vector<field *> _fields;
    int _gold; // negatives are not excluded
    std::string _name;
};

}

#endif // PLAYER_H
