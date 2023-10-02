#pragma once

#include <cstdint>
#include <ostream>
#include <cmath>


struct Point
{
    double x, y;

    double mag() const
    {
        return sqrt(x * x + y * y);
    }

    bool operator== (const Point& p) const
    {
        return abs(x - p.x) < 1e-6 && abs(y - p.y) < 1e-6;
    }

    bool operator!= (const Point& p) const
    {
        return !(p == *this);
    }

    friend Point operator+ (const Point& p, const Point& q)
    {
        Point r;
        r.x = q.x + p.x;
        r.y = q.y + p.y;

        return r;
    }

    friend Point operator- (const Point& p, const Point& q)
    {
        Point r;
        r.x = p.x - q.x;
        r.y = p.y - q.y;

        return r;
    }

    // cross product
    friend double operator^ (const Point& p, const Point& q)
    {
        return p.x * q.y - q.x * p.y;
    }

    double cross(const Point& p) const
    {
        return *this ^ p;
    }

    friend double operator& (const Point& p, const Point& q)
    {
        return p.x * q.x + p.y * q.y;
    }

    double dot(const Point& p) const
    {
        return *this & p;
    }

    Point operator* (double val)
    {
        Point q;

        q.x = x * val;
        q.y = y * val;

        return q;
    }

    Point operator/ (std::size_t n)
    {
        Point q;

        q.x = x / n;
        q.y = y / n;

        return q;
    }

    friend std::ostream& operator<< (std::ostream& os, const Point& p)
    {
        os << p.x << ", " << p.y;
        return os;
    }
};
