#pragma once

#include "Point.h"

struct Segment
{
    Point p;
    Point q;

    int id;

    Segment(const Point& p_, const Point& q_):
        p(p_), q(q_), id(-1)
    {
    }

    Segment(const Segment& segment) :
        p(segment.p), q(segment.q), id(segment.id)
    {
    }

    bool operator< (const Segment& other) const
    {
        return p.x < other.p.x;
    }

    bool operator> (const Segment& other) const
    {
        return p.x > other.p.x;
    }

    bool operator== (const Segment& other) const
    {
        return p.x == other.p.x;
    }

    double slope() const
    {
        return (p.y - q.y) / (p.x - q.x);
    }

    double intercept() const
    {
        return (1. / slope()) * (p.y / p.x);
    }
};
