#include "../Include/unit.h"

namespace model
{
/**
 * Unit class constructor
 *
 * Creates unit object
 */
unit::unit(player *p, field *l, uint HPin, uint dmg, uint sp, field* en_base_loc)
    : _owner(p), _loc(l), _HP(HPin), _dmg(dmg),
_speed(sp) {
    _path = new path(l, en_base_loc);
}

//?delete from field?
void unit::take_damage(uint amount){
    if(amount>=_HP){
        _HP=0;
        _owner->removeUnit(this);
        remove();
    }
    else{
        _HP-=amount;
    }
}
bool unit::alive() const{
    return _HP>0;
}

void unit::remove()
{
    _loc->remUnit(this);
    _owner->removeUnit(this);
}

void unit::move()
{
    if ( ! _path->recheck() ) { // if there is no path left to the goal -> the unit is destroyed
        remove();
        return;
    }
    if(++(*_path)) {
    _path->current()->addUnit(this);
    _loc->remUnit(this);
    _loc = _path->current();
    }
    else {
        remove();
        if( _path->goal()->getBase() != nullptr) {
                _path->goal()->getBase()->take_damage(_dmg);
        }
    }
}

}
