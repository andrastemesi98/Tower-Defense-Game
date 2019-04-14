#pragma once
#ifndef UNIT_H
#define UNIT_H

#include "player.h"
#include "field.h"
#include "path.h"

namespace model
{

class player;
class field;
class path;

class unit
{
public:
    unit(player *p, field *l, uint HPin, uint dmg, uint sp);
    inline virtual ~unit() {}

    // modifying functions:
    void move();
    void dijkstra();
    void take_damage(uint amount);

    // getters:
    bool alive() const;
    inline player* owner() {return _owner;}
    inline field* location() {return _loc;}
    inline uint HP() const {return _HP;}
    inline uint damage() const {return _dmg;}
    inline uint speed() const {return _speed;}

private:
    void remove(); // called by take_damage()

    player * const _owner; // the owner cannot change
    field *_loc;
    uint _HP;
    uint _dmg;
    uint _speed;
    //path _path; // valamiért fordítási hibát ad
};

}

#endif // UNIT_H
