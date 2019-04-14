#include "../Include/field.h"
#include <algorithm>

namespace model
{

  bool field::canBeEntered() const
  {
    if(_tower == nullptr && _base == nullptr)
      return true;
    return false;    
  }

	bool field::addUnit(unit* u)
	{
		if(!canBeEntered())
    	{
			return false;
		}
		_units.push_back(u);
		return true;
	}

	void field::remUnit(unit* u)
	{
		_units.erase(std::remove(_units.begin(), _units.end(), u), _units.end());
	}

  field* field::neighbour(uint index)
  {
    switch(index)
      {
      case 1 : return _game->getField(x-1, y-1);
      case 2 : return _game->getField(x, y-1);
      case 3 : return _game->getField(x+1, y-1);
      case 4 : return _game->getField(x-1, y);
      case 5 : return _game->getField(x+1, y);
      case 6 : return _game->getField(x-1, y+1);
      case 7 : return _game->getField(x, y+1);
      case 8 : return _game->getField(x+1, y+1);
      default : return nullptr;
      }
  }
  
  
  
  
}
