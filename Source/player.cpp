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

player::~player() {
    for(auto u : _removed_units) delete u;
    for(auto t : _removed_towers) delete t;
}

void player::addGold(int amount){
    _gold+=amount;
    _game->goldChanged("1. játékos aranya: " + QString::number(_game->getPlayer(0)._gold) + ", 2. játékos aranya: " + QString::number(_game->getPlayer(1)._gold));

}

void player::removeUnit(unit* myUnit) {
    _removed_units.push_back(myUnit);
    //_units.erase(std::remove(_units.begin(), _units.end(), myUnit), _units.end());
}

void player::removeTower(tower* myTower){
    _removed_towers.push_back(myTower);
    _towers.erase(std::remove(_towers.begin(), _towers.end(), myTower), _towers.end());
}

player* player::getEnemyPlayer() {
    player* enemy =nullptr;
    for(uint i = 0; i < _game->players().size(); ++i)
        if(_game->players()[i] != this) {
            enemy = _game->players()[i];
            break;
        }
    if( enemy == nullptr )
        throw "Csak egy jatekos; nincs enemy player.";
    return enemy;
}

std::vector<field*> buf;
bool player:: placeTower(int ID, field* loc){
    tower *t = new tower(this, loc, 1, 1);
    _towers.push_back(t);
    bool b = loc->addTower(t);
    if( ! b ) return false;
    // remove tower if neccessary:
    if ( ! dijkstra(buf, _base->location(), getEnemyPlayer()->_base->location()) ) {
        _towers.resize( _towers.size() -1 );
        loc->_tower = nullptr;
        delete t;
        return false;
    }
    return true;
}

bool player:: placeCreature(int ID, field* loc){
    player* enemy = getEnemyPlayer();
    //std::cerr << "Enemy base location = " << enemy->_base->location()->x <<";" <<  enemy->_base->location()->y << std::endl;
    unit *u= new unit(this, loc, 1, 1, 1, enemy->_base->location());
    //std::cerr << "ldsfhdfhk\n";
    _units.push_back(u);
    return loc->addUnit(u);
}

void player::update(){
    for ( uint i = 0; i < _units.size(); ++i) {
        std::cerr << "position of unit " << i << " of player " << this->ID() << ": " <<
                     _units[i]->location()->x << ";" << _units[i]->location()->y << std::endl;
    }
    for(auto u : _units) {
        u->move();
    }
    for( auto u : _removed_units) {
        _units.erase(std::remove(_units.begin(), _units.end(), u), _units.end());
    }
}

}
