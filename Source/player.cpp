#include "../Include/player.h"
#include <algorithm>
namespace model{

player::player(int IDin, const std::string& nin, int def_gold, field* baseloc, game* _g)
    :  _base(nullptr), _gold(def_gold), _name(nin), _ID(IDin), _game(_g)
{
    base *u= new base(this, baseloc, 20);
    _base = u;
    baseloc->addBase(u);
    //std::cerr << _base->owner() << " vs " << this << std::endl;
    //if( ! placeBase(0, baseloc) )
    //    throw "HIBA";
    //std::cerr << _base->owner() << " vs " << this << std::endl;
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
    tower *t = new tower(this, loc, 1, 1);
    _towers.push_back(t);
    return loc->addTower(t);
}

bool player:: placeCreature(int ID, field* loc){
    player* enemy =nullptr;
    for(uint i = 0; i < _game->players().size(); ++i)
        if(_game->players()[i] != this) {
            enemy = _game->players()[i];
            break;
        }
    if( enemy == nullptr)
        throw "Csak egy jatekos";
    std::cerr << "Enemy base location = " << enemy->_base->location()->x <<";" <<  enemy->_base->location()->y << std::endl;
    unit *u= new unit(this, loc, 1, 1, 1, enemy->_base->location());
    std::cerr << "ldsfhdfhk\n";
    _units.push_back(u);
    return loc->addUnit(u);
}

void player::update(){
    for(auto u : _units) {
        u->move();
    }

}

}
