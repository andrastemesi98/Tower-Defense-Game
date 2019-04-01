#ifndef GAME_H
#define GAME_H

#include "field.h"
#include "player.h"

namespace model
{
class field;
class player;

// collection of data used in the model
struct game
{
public:
    inline game() {}
    inline ~game() {}

    // NOTE: will be inlined later
    field* getField(uint x, uint y);
    field const* getField(uint x, uint y) const;
    field* operator()(uint x, uint y);
    field const* operator()(uint x, uint y) const;

    inline const std::vector<player>& players() const {return _players;}
    inline player& getPlayer(uint index) {return _players[index];}

private:
    std::vector< std::vector <field > > _fields;
    std::vector<player> _players;
};
}

#endif // GAME_H
