#ifndef FIELD_H
#define FIELD_H

#include "basics.h"
#include "base.h"
#include "player.h"
#include "game.h"

namespace model
{

class player;
class base;
class game;

class field
{
public:
    field(uint xin, uint yin, game* gin);

    // getters:
    bool canBeEntered() const;
    const uint x;
    const uint y;
    game * getGame() const {return _game;}

    // dijkstra helper values / functions:
    field* prev;
    double dist;
    const double weight = 1.0; // int the first version of the game, each weight is 0
    field* neighbour(unsinged int num); 
    /*
      the meaning of "num" (where F is the current field):
      123
      4F5
      678
     */
    
private:
    const uint locx;
    const uint locy;
    std::vector<unit*> _units;
    tower* _tower;
    base* _base;
    game * const _game;
};

}

#endif // FIELD_H
