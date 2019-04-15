#pragma once
#ifndef GAME_H
#define GAME_H

#include "field.h"
#include "player.h"
#include <iostream>
#include <QTimer>

namespace model
{
class field;
class player;

// collection of data used in the model
class game : public QObject
{
    Q_OBJECT
public:
    // must be extended later so that the "_players" vector is filled
    game(uint xin, uint yin);
    inline virtual ~game() {}

    inline field* getField(uint x, uint y);
    inline const field* getField(uint x, uint y) const;
    inline uint sizex() const {return _fields.size();}
    inline uint sizey() const {return _fields[0].size();}

    inline const std::vector<player*>& players() const {return _players;}
    inline player& getPlayer(uint index) {return *(_players[index]);}

    void placeCreature(uint player_index);
public slots:
    void update();
private:
    std::vector< std::vector <field > > _fields;
    std::vector<player *> _players;

    // timer:
    QTimer *_timer;
};



field* game::getField(uint x, uint y)
{
    if(x >= _fields.size() || // x < 0 || y < 0
            y >= _fields[0].size() )
        return nullptr;
    return &_fields[x][y];
}

const field* game::getField(uint x, uint y) const
{
    if(x >= static_cast<uint>(_fields.size()) ||
            y >= static_cast<uint>(_fields[0].size()) )
        return nullptr;
    return &_fields[x][y];
}


}

#endif // GAME_H
