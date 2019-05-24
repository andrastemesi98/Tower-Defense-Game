#include "../Include/player.h"
#include <algorithm>
namespace model{

/**
 * Player class constructor
 *
 * Creates player object
 */
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
/**
 * Player class destructor
 *
 * Destroys player object
 */
player::~player() {
    for(auto u : _removed_units) delete u;
    for(auto t : _removed_towers) delete t;
}
/** @brief Increments a player's gold count
 *
 *  @param amount The integer that is added to the player's _gold.
 *  @return Void.
 */
void player::addGold(int amount){
    _gold+=amount;
    _game->goldChanged("1. játékos aranya: " + QString::number(_game->getPlayer(0)._gold) + ", 2. játékos aranya: " + QString::number(_game->getPlayer(1)._gold));

}
/** @brief Decrements a player's gold count
 *
 *  @param amount The integer that is subtracted from the player's _gold.
 *  @return Void.
 */
void player::spendGold(int amount){
    _gold-=amount;
    _game->goldChanged("1. játékos aranya: " + QString::number(_game->getPlayer(0)._gold) + ", 2. játékos aranya: " + QString::number(_game->getPlayer(1)._gold));
}
/** @brief Deletes the input unit.
 *
 *   Called when a unit dies. Function adds the specific unit to the vector of units which will be removed.
 *
 *  @param myUnit The pointer to the unit that needs to be deleted.
 *  @return Void.
 */
void player::removeUnit(unit* myUnit) {
    _removed_units.push_back(myUnit);
    addGold(20);
    //_units.erase(std::remove(_units.begin(), _units.end(), myUnit), _units.end());
}
/** @brief Deletes the input tower.
 *
 *   Called when a tower dies. Function adds the specific tower to the vector of towers which will be removed.
 *   Designed for future versions.
 *
 *  @param myTower The pointer to the unit that needs to be deleted.
 *  @return Void.
 */
void player::removeTower(tower* myTower){
    _removed_towers.push_back(myTower);
    _towers.erase(std::remove(_towers.begin(), _towers.end(), myTower), _towers.end());
}
/** @brief Returns an enemy player pointer.
 *
 *  @return player*.
 */
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
/** @brief Places a tower on the given field with given tower type.
 *
 *  @param ID Tower type
 *  @param loc Pointer to the field.
 *  @return bool. returns success
 */
bool player:: placeTower(int ID, field* loc){
    if(_gold>=100){
        tower *t = new tower(this, loc, 1, 1);
        bool b = loc->addTower(t);
        if( ! b ) return false;
        _towers.push_back(t);
        // remove tower if neccessary:
        if ( ! dijkstra(buf, _base->location(), getEnemyPlayer()->_base->location()) ) {
            _towers.resize( _towers.size() -1 );
            loc->_tower = nullptr;
            delete t;
            return false;
        }
        spendGold(100);
        return true;
    } else {
        return false;
    }
}

/** @brief Places a Creature(unit) on the given field with given type.
 *
 *  @param ID Creature type
 *  @param loc Pointer to the field.
 *  @return bool. returns success
 */
bool player:: placeCreature(int ID, field* loc){
    if(_gold>=100){
        player* enemy = getEnemyPlayer();
        //std::cerr << "Enemy base location = " << enemy->_base->location()->x <<";" <<  enemy->_base->location()->y << std::endl;
        unit *u= new unit(this, loc, 1, 1, 1, enemy->_base->location());
        //std::cerr << "ldsfhdfhk\n";
        _units.push_back(u);
        spendGold(100);
        return loc->addUnit(u);
    } else{
        return false;
    }
}
/** @brief Updates: moving with living units, deleting lost units from vector, shooting with towers.
 *
 *  @return void.
 */
void player::update(){
    for ( uint i = 0; i < _units.size(); ++i) {
        std::cerr << "position of unit " << i << " of player " << this->ID() << ": " <<
                     _units[i]->location()->x << ";" << _units[i]->location()->y << std::endl;
    }
    for( auto u : _removed_units) {
        _units.erase(std::remove(_units.begin(), _units.end(), u), _units.end());
    }
        for(auto u : _units) {
        u->move();
    }
    for( auto t: _towers) {
        t->shoot();
    }
    addGold(1);
}

}
