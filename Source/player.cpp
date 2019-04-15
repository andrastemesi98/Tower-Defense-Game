#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "basics.h"
#include "unit.h"
#include "tower.h"
#include "field.h"
#include "game.h"

namespace model {

class unit;
class tower;
class field;
class base;
class game;

class player
{

public:
    player(int IDin, const std::string& nin, int def_gold, field* baseloc, game* gin);
    virtual ~player();

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
    inline std::vector<tower*> const& towers() const {return _towers;}
    inline int ID() const {return _ID;}
    inline base* getBase() {return _base;}

private:
    std::vector<unit *> _units;
    std::vector<unit *> _removed_units;
    std::vector<tower *> _towers;
    std::vector<tower *> _removed_towers;
    base* _base;
    int _gold = 20; // negatives are not excluded
    std::string _name;
    const int _ID;
    game* const _game;
    friend class game;
    player* getEnemyPlayer();
};

}

#endif // PLAYER_H
