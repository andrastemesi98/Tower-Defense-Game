#include "../Include/unit.h"

namespace model
{
/** @brief Shoots at a unit on a neighbour field
 *
 *  Checks from the top left corner if there is a unit.
 *  If that is an enemy unit calls take_damage of that.
 *  Only one shot.
 *
 *  @return Void.
 */
void tower::shoot(){
    //Implemented only for 1 range
    //
    bool shot=0;
    for (uint i=1; i<9; ++i){
        if(_loc->neighbour(i)!=nullptr){
            model::unit* u=_loc->neighbour(i)->getUnit();
            if(u!=nullptr && u->owner()!=_owner){
                u->take_damage(_dmg);
                shot=true;
            }
            if(shot){
                break;
            }
        }
    }
}

}
