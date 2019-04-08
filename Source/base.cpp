#include "../Include/base.h"

namespace model
{
	base::base(player* pl, field* f, uint HPin): _owner(pl), _loc(f), _HP(HPin){}

	bool base::check()
	{
		//still alive?
		return _HP>0;
	}

}
