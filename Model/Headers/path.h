#ifndef PATH_H
#define PATH_H

#include "field.h"
#include "basics.h"

namespace model
{
class path
{

public:
    path(model::field* start, field* goal);

    bool check();
    void recheck(); // checks and recalculates if neccessary
    void recalculate(); // called by "recheck()" by default

private:
    vector<field *> _fields;
    field *goal;
    uint current; // index of the current field;
};
}

#endif // PATH_H
