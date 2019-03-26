#ifndef GAME_H
#define GAME_H

#include "field.h"
#include "player.h"

namespace model
{

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

    inline const vector<player>& players() const {return _players;}
    inline player& getPlayer(uint index) {return players[index];}

private:
    vector< vector <field > > _fields;
    vector<player> _players;
};
}

#endif // GAME_H
