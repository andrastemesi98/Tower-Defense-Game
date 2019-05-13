#include "../Include/field.h"
#include <algorithm>

namespace model
{
    /** @brief Places tower on the field, if allowed.
    *
    *  @param t The pointer to the tower.
    *  @return Bool. True-tower successfully placed False-already tower/base there
    */
    bool field::addTower(tower *t) {
        if ( ! canBeEntered()) return false;
        _tower = t;
        return true;
    }
    /** @brief Places base on the field, if allowed.
     *
     *  @param t The pointer to the base.
     *  @return Bool. True-tower successfully placed False-already base there
     */
    bool field::addBase(base *t) {
        if ( ! canBeEntered()) return false;
        _base = t;
        return true;
    }
    /** @brief Return if field is empty.
     *
     *  @return Bool. True-No tower or base False-already base/tower there
     */
  bool field::canBeEntered() const
  {
    if(_tower == nullptr && _base == nullptr)
      return true;
    return false;
  }
  /** @brief Places unit on the field, if allowed.
   *
   *  @param u The pointer to the unit.
   *  @return Bool.
   */
    bool field::addUnit(unit* u)
    {
        if(!canBeEntered())
        {
            return false;
        }
        _units.push_back(u);
        return true;
    }
    /** @brief Removes unit the field.
     *
     *  @param u The pointer to the unit.
     *  @return Void.
     */
    void field::remUnit(unit* u)
    {
        _units.erase(std::remove(_units.begin(), _units.end(), u), _units.end());
    }
    /** @brief Return pointer to the neighbour field
     *
     *  @param index Integer between 1 and 8. From top-left corner clockwise.
     *  @return Field*.
     */
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
