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
    path(model::field* start, field* goal);

    bool check();
    void recheck(); // checks and recalculates if neccessary
    void recalculate(); // called by "recheck()" by default

private:
    std::vector<field *> _fields;
    field *goal;
    uint current; // index of the current field;
};
}

#endif // PATH_H
