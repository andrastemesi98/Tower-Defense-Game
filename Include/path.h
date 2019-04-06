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
    inline field* current() {return _fields[current];}
    
    // modifiers:    
    void recheck(); // checks and recalculates if neccessary
    bool operator++();

    
private:
    std::vector<field *> _fields;
    field *_goal;
    uint _current; // index of the current field;

    bool check() const;
    void recalculate(); // called by "recheck()" by default
};
}

#endif // PATH_H
