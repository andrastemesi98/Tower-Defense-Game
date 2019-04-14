#include "../Include/player.h"
#include <algorithm>
namespace model{

player::player(const std::string& nin, int def_gold){
	_name=nin;
	_gold=def_gold;
}

void player::addGold(int amount){
	_gold+=amount;
}

void player::removeUnit(unit* myUnit){
	_units.erase(std::remove(_units.begin(), _units.end(), myUnit), _units.end());
}

void player::removeTower(tower* myTower){
        //_towers.erase(std::remove(_towers.begin(), _towers.end(), myTower), _towers.end());
		//field* f;
		//f=myTower->location();
		//f->deleteTower();
		//~myTower();
}

bool player:: placeTower(int ID, field* loc){
	//tower* t;
	//t= new tower(this, loc, iddmg, idrng);
	//_towers.push_back(t);
	//return loc->addTower(t);
	return true;
}

bool player:: placeCreature(int ID, field* loc){
	//unit* u;
	//u= new unit(this, basefield, iddmg, idrng, idsp);
	//_units.push_back(u);
	//return basefield->addUnit(u);
	return true;
}

void player::update(){

}

}
