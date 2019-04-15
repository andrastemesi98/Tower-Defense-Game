#include "../Include/base.h"

namespace model
{
base::base(player* pl, field* f, uint HPin): _owner(pl), _loc(f), _HP(HPin)
{ }

bool base::check()
{
    return _HP>0;
}

void base::take_damage(uint amount)
{
    if(_HP<=amount){
        _HP=0;
    }
    else{
        _HP-=amount;
    }
}
}
