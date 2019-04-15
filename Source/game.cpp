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
    player* pl = new player(0, "player1", 0, getField(_fields.size()/2, 0), this);
    _players.push_back(pl);
    player* pl2 = new player(1, "player2", 0, getField(_fields.size()/2, _fields[0].size()-1), this);
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
}

void game::update()
{
    for(auto p : _players) p->update();
}

}
