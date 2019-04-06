#ifndef GAME_H
#define GAME_H

#include "field.h"
#include "player.h"
#include <iostream>
namespace model
{
class field;
class player;

// collection of data used in the model
struct game
{
public:
  // must be extended later so that the "_players" vector is filled
  game(uint xin, uint yin);
  inline ~game() {}

  inline field* getField(int x, int y);
  inline const field* getField(int x, int y) const;
  inline uint sizex() const {return _fields.size();}
  inline uint sizey() const {return _fields[0].size();}
 
  inline const std::vector<player>& players() const {return _players;}
  inline player& getPlayer(uint index) {return _players[index];}
  
private:
    std::vector< std::vector <field > > _fields;
    std::vector<player> _players;
};



 field* game::getField(int x, int y)
 {
   if(x < 0 || y < 0 || x >= static_cast<int>(_fields.size()) || 
      y >= static_cast<int>(_fields[0].size()) )
     return nullptr;
   return &_fields[x][y];
 }
 
 const field* game::getField(int x, int y) const
 {
   if(x < 0 || y < 0 || x >= static_cast<int>(_fields.size()) || 
      y >= static_cast<int>(_fields[0].size()) )
     return nullptr;
   return &_fields[x][y];
 }

 
}

#endif // GAME_H
