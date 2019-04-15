#pragma once
#ifndef FIELD_H
#define FIELD_H

#include "basics.h"
#include "base.h"
#include "player.h"
#include "game.h"
#include <iostream>

namespace model
{

class player;
class base;
class game;
 class unit;
 class tower;

class field
{
public:
  inline field(uint xin, uint yin, game* gin, double wein = 1.0)  :
    x(xin), y(yin), prev(nullptr), dist(0), weight(wein), _tower(nullptr),
    _base(nullptr), _game(gin)
  { }

    // getters:
    bool canBeEntered() const;
    const uint x;
    const uint y;
    game * getGame() const {return _game;}
    inline base* getBase() {return _base;}
    inline tower* getTower() {return _tower;}
    inline unit* getUnit() {if(_units.size() == 0) return nullptr; else return _units[0];}
    inline uint unitNum() const {return _units.size();}

    // setters:
    void remUnit(unit* u);
    bool addUnit(unit* u);

    // dijkstra helper values / functions:
    field* prev;
    double dist;
    const double weight; // int the first version of the game, each weight is 1.0
    field* neighbour(unsigned int num);
    /*
      the meaning of "num" (where F is the current field):
      123
      4F5
      678
     */

private:
    std::vector<unit*> _units;
    tower* _tower;
    base* _base;
    game * const _game;
    bool addTower(tower* t);
    bool addBase(base* t);
    friend class player;
};

}

#endif // FIELD_H
