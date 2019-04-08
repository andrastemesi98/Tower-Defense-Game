#include "../Include/unit.h"

namespace model
{
unit::unit(player *p, field *l, uint HPin, uint dmg, uint sp): _owner(p), _loc(l), _HP(HPin), _dmg(dmg),
_speed(sp) {}

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

void unit::remove(){
	//TODO
}



}
