#pragma once

#include "Point.h"
#include "Segment.h"

#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <iostream>


enum eOrientation
{
    COLINEAR,
    CW,
    CCW
};

eOrientation orientation(const Point& p, const Point& q, const Point& r)
{
    double val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (val == 0.) return COLINEAR;  // Collinear
    return (val > 0.) ? CW : CCW; // Clockwise or Counterclockwise
}

void sortPolygonInClockwiseOrder(std::vector<Point>& polygon)
{
    using namespace std;

    const auto center = reduce(polygon.begin(), polygon.end(), Point(), std::plus<Point>()) / polygon.size();

    sort(polygon.begin(), polygon.end(), [&center](const Point& p1, const Point& p2) {
        return atan2(p1.y - center.y, p1.x - center.x) > atan2(p2.y - center.y, p2.x - center.x);
    });
}

void lexicographicSort(std::vector<Point>& points)
{
    using namespace std;

    sort(points.begin(), points.end(), [](const Point& p1, const Point& p2) {
        if (p1.x < p2.x)
            return true;
        else if (p1.x == p2.x)
            return p1.y < p2.y;
        
        return false;
    });
}

void sortByPolarAngle(std::vector<Point>& points)
{
    using namespace std;

    auto min = min_element(points.begin(), points.end(), [](const Point& p1, const Point& p2) {
        if (p1.x < p2.x)
            return true;
        else if (p1.x == p2.x)
            return p1.y < p2.y;
        
        return false;
    });

    cout << "min = " << min->x << ", " << min->y << endl;

    sort(points.begin(), points.end(), [&min](const Point& p1, const Point& p2) {
        return atan2(p1.y - min->y, p1.x - min->x) > atan2(p2.y - min->y, p2.x - min->x);
    });
}

// https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
// overlapping case is not handled
bool intersection(const Segment& l1, const Segment& l2, Point& i)
{
    using namespace std;

    Point s1 = l1.q - l1.p;
    Point s2 = l2.q - l2.p;

    double s, t;
    // s = s1 ^ (l1.p - l2.p) / (s1 ^ s2);
    s = (-s1.y * (l1.p.x - l2.p.x) + s1.x * (l1.p.y - l2.p.y)) / (-s2.x * s1.y + s1.x * s2.y);
    // t = s2 ^ (l1.p - l2.p) / (s1 ^ s2);
    t = ( s2.x * (l1.p.y - l2.p.y) - s2.y * (l1.p.x - l2.p.x)) / (-s2.x * s1.y + s1.x * s2.y);

    // cout << s << " " << t << endl;

    if ((s >= 0 && s <= 1) && (t >= 0 && t <= 1))
    {
        // Collision detected
        i = l1.p + s1 * t;

        return true;
    }

    return false; // No collision
}
