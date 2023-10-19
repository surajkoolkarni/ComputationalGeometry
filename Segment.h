#pragma once

#include "Point.h"

struct Segment
{
    Point p;
    Point q;

    int id;

    Segment(const Point& p_, const Point& q_) :
        id(-1)
    {
        if (p_ < q_)
        {
            p = p_;
            q = q_;
        }
        else
        {
            p = q_;
            q = p_;
        }
    }

    Segment(const Segment& segment) :
        Segment(segment.p, segment.q)
    {
        id = segment.id;
    }

    bool operator< (const Segment& other) const
    {
        if (q.x < other.p.x && q.x < other.q.x)
        {
            return true;
        }
        else if (p.x < other.p.x && p.x < other.q.x)
        {
            return false;
        }
        else if (p.x > other.p.x && p.x < other.q.x)
        {
            return p.y > other.intercept(p.x);
        }
        else if (q.x > other.p.x && p.x < other.q.x)
        {
            return q.y > other.intercept(q.x);
        }

        return false;
    }

    bool operator> (const Segment& other) const
    {
        if (q.x < other.p.x && q.x < other.q.x)
        {
            return false;
        }
        else if (p.x < other.p.x && p.x < other.q.x)
        {
            return true;
        }
        else if (p.x > other.p.x && p.x < other.q.x)
        {
            return p.y <= other.intercept(p.x);
        }
        else if (q.x > other.p.x && p.x < other.q.x)
        {
            return q.y <= other.intercept(q.x);
        }

        return false;
    }

    bool operator== (const Segment& other) const
    {
        return p == other.p && q == other.q;
    }

    double slope() const
    {
        return (q.y - p.y) / (q.x - p.x);
    }

private:
    double intercept(double x_) const
    {
        return slope() * (x_ - p.x) + p.y;
    }
};
