#pragma once

#include "Point.h"

struct Segment
{
    Point p;
    Point q;

    int id;

    double value;

    Segment(const Point& p_, const Point& q_) :
        id(-1),
        p(p_),
        q(q_)
    {
        calculateValue(first().x);
    }

    const Point& first() const
    {
        if (p.x <= q.x)
            return p;
        else
            return q;
    }

    const Point& second() const
    {
        if (p.x <= q.x)
            return q;
        else
            return p;
    }

    bool operator< (const Segment& other) const
    {
        return value > other.value;
    }

    bool operator> (const Segment& other) const
    {
        return value < other.value;
    }

    bool operator== (const Segment& other) const
    {
        return value == other.value;
    }

    void calculateValue(double x_)
    {
        value = ((second().y - first().y) / (second().x - first().x)) * (x_ - first().x) + first().y;
    }
};
