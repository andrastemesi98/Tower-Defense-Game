#ifndef PATH_H
#define PATH_H

#include "field.h"
#include "basics.h"

namespace model
{

class field;

class path
{

public:
    path(model::field* startin, field* goalin);

    // getters:
    inline field* goal() {return _goal;}
    inline const field* goal() const { return _goal;}
    inline field* current() {return _fields[_current];}
    
    // modifiers:    
    bool recheck(); // checks and recalculates if neccessary
    bool operator++();

    
private:
    std::vector<field *> _fields;
    field *_goal;
    uint _current; // index of the current field;

    bool check() const;
    bool recalculate(); // called by "recheck()" by default
};

 bool dijkstra(std::vector<field* >& fields, field* start, field* goal);
}

#endif // PATH_H
