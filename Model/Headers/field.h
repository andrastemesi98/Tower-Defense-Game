#ifndef FIELD_H
#define FIELD_H

#include "basics.h"
#include "base.h"
#include "player.h"
#include "game.h"

namespace model
{

class field
{
public:
    field(uint xin, uint yin);

    // getters:
    bool canBeEntered() const;
    inline uint x() const {return locx;}
    inline uint y() const {return locy;}
    inline vector<field*> const& neighbours() const {return _neighbours;}
private:
    const uint locx;
    const uint locy;
    vector<unit*> _units;
    tower* _tower;
    base* _base;
    game* _game;
    vector<field*> _neighbours;
};

}

#endif // FIELD_H
