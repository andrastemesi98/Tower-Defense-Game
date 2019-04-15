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
    if(start == nullptr || goal == nullptr)
        return false;

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
        minf = nullptr;
        mindist = std::numeric_limits<double>::infinity();
        bool look_new = false;
        for(auto f : Q)
            for(unsigned int i = 1; i <= 8; ++i)
            {
                field* nei = f->neighbour(i);
                if(nei == nullptr)
                    continue;
                if(nei != goal && ! nei->canBeEntered())
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
        if(look_new == false) {
            all_looked_at = true;
            break;
        }
        Q.insert(minf);
        minf->dist = mindist;
    }
    if(all_looked_at) {
        return false;
    }
    fields.resize(0);
    // at this point, we know that minf == goal
    for(field* f = goal; f != start; f = f->prev)
        fields.push_back(f);
    fields.push_back(start);
    std::reverse(fields.begin(), fields.end()); // fields[0] should be equal to start
    return true;
}


path::path(field* startin, field* goalin) : _goal(goalin), _current(0)
{
    if( ! dijkstra(_fields, startin, goalin) )
        throw "NINCS UT";
}

bool path::check() const
{
    //std::cerr << "current0 = " << _fields[_current]->x << ";" << _fields[_current]->y << std::endl;0
    for(uint i = _current+1; i < _fields.size() -1; ++i)
        if(! _fields[i]->canBeEntered())
            return false;
    return true;
}

bool path::recheck()
{
    if(! check() )
        if( ! recalculate() )
            return false;
    return true;
}

bool path::operator++()
{
    // if the current field is a neighbour of the enemy base:
    if(_current == _fields.size() -2)
        return false;
    ++_current;
    return true;
}

bool path::recalculate()
{
    field* f = _fields[_current];
    _fields.resize(0);
    _current = 0;
    bool b = dijkstra(_fields, f, _goal);
    //std::cerr << "needs recheck ... current= " << _fields[_current]->x << ";" <<  _fields[_current]->y << std::endl;
    return b;
}

}
