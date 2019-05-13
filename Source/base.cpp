#include "../Include/base.h"

namespace model
{
/**
 * Base class constructor
 *
 * Creates base object
 */
base::base(player* pl, field* f, uint HPin): _owner(pl), _loc(f), _HP(HPin)
{ }
/** @brief Checks if base is alive
 *
 *  @return bool. True-alive, false-dead
 */
bool base::check()
{
    return _HP>0;
}
/** @brief Decrements a base's health score
 *
 *  @param amount The integer that is subtracted from _HP. New _HP minimum is zero.
 *  @return Void.
 */
void base::take_damage(uint amount)
{
    if(_HP<=amount){
        _HP=0;
    }
    else{
        _HP-=amount;
    }
}
}
