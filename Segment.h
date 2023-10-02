#pragma once

#include "Point.h"

struct Segment
{
    Point p;
    Point q;

    bool operator< (const Segment& other) const
    {
        return p.x < other.p.x;
    }
};
