#include "../Include/field.h"

namespace model
{

  bool field::canBeEntered() const
  {
    if(_tower == nullptr && _base == nullptr)
      return true;
    return false;    
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
