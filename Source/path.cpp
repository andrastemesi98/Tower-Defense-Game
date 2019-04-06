#include "../Include/path.h"

#include <set>
#include <limits>
#include <algorithm>

namespace model
{


  // returnd with "true", if there is a path between "start" and "goal"
  // "fields" is filled so that fields[0] = start and fields.back() = goal 
  bool dijkstra(std::vector<field* >& fields, field* start, field* goal)
  {
    game* g = start->getGame();
    if(goal->getGame() != g)
      return false;
    
    std::set<field*> Q;
    
    start->prev = nullptr;
    start->dist = 0;
    Q.insert(start);

    field *minf = nullptr;
    double mindist = std::numeric_limits<double>::infinity();
    double curdist;
    bool all_looked_at = false;
    while(minf != goal && !all_looked_at)
      {
	bool look_new = false;
	for(auto f : Q)
	  for(unsigned int i = 1; i <= 8; ++i)
	    {
	      field* nei = f->neighbour(i);
	      if(! nei->canBeEntered())
		continue;
	      if(Q.find(nei) != Q.end()) // if the it is already in Q
		  continue;
	      look_new = true;
	      curdist = f->dist + nei->weight;
	      if(curdist < mindist)
		{
		  minf = nei;
		  mindist = curdist;
		  minf -> prev = f;
		}
	    }
	if(look_new == false)
	  all_looked_at = true;
      }
    if(all_looked_at)
      return false;
    fields.resize(0);
    // at this point, we know that minf == goal
    for(field* f = goal; f != start; f = f->prev)
      fields.push_back(f);
    fields.push_back(start);
    std::reverse(myvector.begin(),myvector.end());
    return true;
  }

  
  path::path(field* startin, field* goalin) : _goal(goalin), current(currentin)
  {
    uint current = 0;
    dijkstra(_fields, startin, goalin);
  }

  bool path::check() const
  {
    // !!! does not check the last element of "_fields", as the "goal filed" 
    // usually is occupied by the enemy base
    for(uint i = _current+1; i < _fields.size() -1; ++i)
      if(! _fields[i]->canBeEntered())
	return false;
    return true;    
  }
  
  void path::recheck()
  {
    if(! check() )
      recalculate();
  }

  bool path::operator++()
  {
    // if the current field is a neighbour of the enemy base:
    if(_current == _fields.size() -2)
      return false;
    ++current;
    return true;
  }

  void path::recalculate()
  {
    field* f = _fields[_current];
    dijkstra(_fields, f, _goal);
  }
