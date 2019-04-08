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
        _towers.erase(std::remove(_towers.begin(), _towers.end(), myTower), _towers.end());
}

bool player:: placeTower(int ID, field* loc){
	//TODO
	return true;
}

bool player:: placeCreature(int ID, field* loc){
	//TODO
	return true;
}

void player::update(){

}

}
