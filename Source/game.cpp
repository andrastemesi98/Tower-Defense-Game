#include "../Include/game.h"

#include <iostream>

namespace model
{
// must be extended later so that the "_players" vector is filled as well
game::game(uint xin, uint yin)
{
    if(xin < 1) xin = 1;
    if(yin < 1) yin = 1;
    for(uint i = 0; i < xin; ++i)
    {
        std::vector<field> column;
        for(uint j = 0; j < yin; ++j)
            column.emplace_back(i, j, this);
        _fields.push_back(column);
    }
    player* pl = new player(0, "player1", 1000, getField(_fields.size()/2, 0), this);
    _players.push_back(pl);
    player* pl2 = new player(1, "player2", 1000, getField(_fields.size()/2, _fields[0].size()-1), this);
    _players.push_back(pl2);
    //_players.emplace_back(0, "player1", 0, getField(5, 5));
    for( auto& pl : _players)
        std::cout << pl->ID() << std::endl;
    for(uint j = 0; j < _fields[0].size(); ++j) {
        for(uint i = 0; i < _fields.size(); ++i) {
            std::cout << _fields[i][j].canBeEntered() << " ";
        }
        std::cout << std::endl << std::flush;
    }
    //std::cerr << _players[0]->_base->owner() << " vs " << &_players[0] << std::endl;

    // creating the timer :
    _timer = new QTimer(this);
    connect(_timer, SIGNAL(timeout()), this, SLOT(update()));
    _timer->start(200);
}

void game::update()
{
    std::cerr << "update function\n";
    for(auto p : _players) p->update();
    for(auto p : _players) {
        if(!p->getBase()->check()){
            _timer->stop();
            gameOver(p->getEnemyPlayer()->ID());
        }
    }
}

/*
  the meaning of "num" (where F is the current field):
  123
  4F5
  678
 */
field* getNeighbourToPlusY(field* f) {
    for(uint i = 8; i >= 1; --i)
        if ( f->neighbour(i) != nullptr && f->neighbour(i)->canBeEntered()) return f->neighbour(i);
    return nullptr;
}

field* getNeighbourToMinusY(field* f) {
    for(uint i = 1; i <= 8; ++i)
        if ( f->neighbour(i) != nullptr && f->neighbour(i)->canBeEntered()) return f->neighbour(i);
    return nullptr;
}

void game::placeCreature(uint player_index) {
    field * start = nullptr;
    if( player_index % 2 == 0)
        start = getNeighbourToPlusY(getPlayer(player_index).getBase()->location());
    else start = getNeighbourToMinusY(getPlayer(player_index).getBase()->location());
    if(start == nullptr) throw "Can not place creature!! ( no room) ";
    getPlayer(player_index).placeCreature(0, start);
}

}
