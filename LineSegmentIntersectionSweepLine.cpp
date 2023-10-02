#include "Point.h"
#include "Segment.h"
#include "Util.h"

#include <vector>
#include <queue>
#include <utility>
#include <set>

#include <gtest/gtest.h>


using namespace std;

enum EventType
{
    START,
    END,
    CROSS
};

struct Event
{
    EventType type;
    double x;
    int id;

    bool operator< (const Event& e) const
    {
        return x < e.x;
    }
};

void lineSegmentIntersectionSweepLine(vector<Segment>& segments, vector<Point>& intersectionPoints)
{
    multiset<Event> eventQueue;

	for (int k = 0; k < segments.size(); ++k)
	{
		if (segments[k].p.x > segments[k].q.x)
			swap(segments[k].p, segments[k].q);

		eventQueue.insert(Event{START,segments[k].p.x, k});
		eventQueue.insert(Event{END,segments[k].q.x, k});
	}

    auto segmentCompare = [](const Segment& s1, const Segment& s2) {
        s1.p.y < s2.p.y;
    };

    multiset<Segment> activeSegments;
    Point intersectionPoint;
    while (!eventQueue.empty())
    {
        cout << "Running event queue\n"; 
        auto eventPoint = *eventQueue.begin();
        eventQueue.erase(eventQueue.cbegin());

        if (eventPoint.type == START)
        {
            auto ins = activeSegments.insert(segments[eventPoint.id]);
            for (int dir = 0; dir <= 1; ++dir)
            {
                auto it = ins;

                while ((dir ? ++it : it--) != (dir ? activeSegments.cbegin() : activeSegments.cend()))
                {
                    if (intersection(segments[eventPoint.id], *it, intersectionPoint))
                    {
                        intersectionPoints.push_back(intersectionPoint);
                    }
                }
            }
        }
    }
}

TEST(lineSegmentIntersectionSweepLine, simple)
{
    Point iHat{1, 0};
    Point jHat{0, 1};
    
    EXPECT_DOUBLE_EQ(iHat ^ jHat, 1.);

    Point i;
    Point e{0.5, 0.5};

    EXPECT_TRUE(intersection({{0., 0.}, {1., 1.}}, {{1., 0.}, {0., 1.}}, i));
    EXPECT_EQ(i, e);

    vector<Segment> segments = { { {0, 0}, {1, 1} }, { {1, 0}, {0, 1} } };
    vector<Point> intersectionsOut;

    lineSegmentIntersectionSweepLine(segments, intersectionsOut);

    vector<Point> intersectionsExpected = { {0.5, 0.5} };

    EXPECT_TRUE(intersectionsExpected == intersectionsOut);

    segments = { {{1, 5}, {4, 5}}, {{2, 5}, {10, 1}},{{3, 2}, {10, 3}},{{6, 4}, {9, 4}},{{7, 1}, {8, 1}} };

    intersectionsOut.clear();
    lineSegmentIntersectionSweepLine(segments, intersectionsOut);

    intersectionsExpected = {{2, 5}, {6.8889, 2.5556}};

    EXPECT_TRUE(intersectionsExpected == intersectionsOut);
}
