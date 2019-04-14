#pragma once
#ifndef TOWER_H
#define TOWER_H

#include "player.h"
#include "field.h"

namespace model
{

class player;
class field;

class tower
{

public:
    inline tower(player* pl, field* f, uint dmg, uint rng) : _owner(pl), _loc(f), _range(rng),
_dmg(dmg)
    {
        //if(!f->addTower(this)
        //{
            //Hibakezeles
        //}
    }


    // modifying functions:
    // void shoot(); // is not implemented yet

    // getters:
    inline player* owner() {return _owner;}
    inline field* location() {return _loc;}
    inline uint damage() const {return _dmg;}
    inline uint range() const {return _range;}

private:
    inline void remove(){}
    // blank function by default. Can be used in a later version.

    player* const _owner; // cannot change
    field* const _loc; // cannot change
    uint _range;
    uint _dmg;
};

}

#endif // TOWER_H
