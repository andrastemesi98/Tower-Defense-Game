#ifndef BASE_H
#define BASE_H

#include "player.h"
#include "field.h"

namespace model
{

  class player;
  class field;
  class tower;

class base
{

public:
    base(player* pl, field* f, uint HPin);

    // modifying functions:
    bool check();

    // getters:
    inline player* owner() {return _owner;}
    inline field* location() {return _loc;}
    inline uint HP() const {return _HP;}

private:
    player * const _owner; // the owner cannot change
    field *_loc;
    uint _HP;
};
}


#endif // BASE_H
