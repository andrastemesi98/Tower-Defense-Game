#include "../Include/game.h"

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
  }

  
  



}
