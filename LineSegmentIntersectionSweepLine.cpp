#include "DataStructures/AVLTree.h"
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
    int id1;
    int id2;

    Event() = default;

    Event(EventType type_, double x_, int id1_, int id2_) :
        type(type_), x(x_), id1(id1_), id2(id2_)
    {
    }

    Event(const Event& e) :
        Event(e.type, e.x, e.id1, e.id2)
    {
    }

    bool operator< (const Event& e) const
    {
        return x < e.x;
    }

    bool operator> (const Event& e) const
    {
        return x > e.x;
    }

    bool operator== (const Event& e) const
    {
        return x == e.x;
    }
};

void lineSegmentIntersectionSweepLine(vector<Segment>& segments, vector<pair<int, int>>& intersectingSegmentIds, vector<Point>& intersectionPoints)
{
    for (int i = 0; i < segments.size(); ++i)
        segments[i].id = i;

    AVLTree<Event> eventQueue;
    AVLTree<Segment> segmentTree;

    for (int i = 0; i < segments.size(); ++i)
    {
        if (segments[i].p < segments[i].q)
        {
            eventQueue.insert(Event{START, segments[i].p.x, segments[i].id, segments[i].id});
            eventQueue.insert(Event{END, segments[i].q.x, segments[i].id, segments[i].id});
        }
        else if (segments[i].p > segments[i].q)
        {
            eventQueue.insert(Event{END, segments[i].p.x, segments[i].id, segments[i].id});
            eventQueue.insert(Event{START, segments[i].q.x, segments[i].id, segments[i].id});
        }
    }

    while(!eventQueue.isEmpty())
    {
        auto event = eventQueue.removeMin();
        Point intersectionPoint;

        if (event.type == START)
        {
            const auto& segment = segments[event.id1];
            const auto* successor = segmentTree.successor(segment);
            const auto* predecessor = segmentTree.predecessor(segment);
            
            segmentTree.insert(segments[segment.id]);

            if (successor)
                if (intersection(segment, successor->value, intersectionPoint))
                    eventQueue.insert(Event{CROSS, intersectionPoint.x, segment.id, successor->value.id});

            if (predecessor)
                if (intersection(segment, predecessor->value, intersectionPoint))
                    eventQueue.insert(Event{CROSS, intersectionPoint.x, predecessor->value.id, segment.id});
        }
        else if (event.type == END)
        {
            const auto& segment = segments[event.id1];
            const auto* successor = segmentTree.successor(segment);
            const auto* predecessor = segmentTree.predecessor(segment);
            
            if (predecessor && successor)
                if (intersection(predecessor->value, successor->value, intersectionPoint))
                    eventQueue.insert(Event{CROSS, intersectionPoint.x, predecessor->value.id, successor->value.id});

            segmentTree.remove(segment);
        }
        else if (event.type == CROSS)
        {
            const auto& segment1 = segments[event.id1];
            const auto& segment2 = segments[event.id2];

            intersection(segment1, segment2, intersectionPoint);
            intersectionPoints.push_back(intersectionPoint);
            intersectingSegmentIds.push_back({segment1.id, segment2.id});

            segmentTree.remove(segment1);
            segmentTree.remove(segment2);

            segmentTree.insert(segment2);
            segmentTree.insert(segment1);

            const auto* segment1Successor = segmentTree.successor(segment1);
            const auto* segment1Predecessor = segmentTree.predecessor(segment1);

            if (segment1Predecessor)
                if (intersection(segment1Predecessor->value, segment1, intersectionPoint))
                    eventQueue.insert(Event{CROSS, intersectionPoint.x, segment1Predecessor->value.id, segment1.id});

            if (segment1Successor)
                if (intersection(segment1, segment1Successor->value, intersectionPoint))
                    eventQueue.insert(Event{CROSS, intersectionPoint.x, segment1.id, segment1Successor->value.id});

            const auto* segment2Successor = segmentTree.successor(segment2);
            const auto* segment2Predecessor = segmentTree.predecessor(segment2);

            if (segment2Predecessor)
                if (intersection(segment2Predecessor->value, segment2, intersectionPoint))
                    eventQueue.insert(Event{CROSS, intersectionPoint.x, segment2Predecessor->value.id, segment2.id});

            if (segment2Successor)
                if (intersection(segment2, segment2Successor->value, intersectionPoint))
                    eventQueue.insert(Event{CROSS, intersectionPoint.x, segment2.id, segment2Successor->value.id});
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
    vector<pair<int, int>> intersectingSegmentIdsOut;

    lineSegmentIntersectionSweepLine(segments, intersectingSegmentIdsOut, intersectionsOut);

    cout << "Intersecting segment ids\n";
    for (auto sId : intersectingSegmentIdsOut)
        cout << sId.first << " " << sId.second << endl;
    cout << endl;

    cout << "Intersections\n";
    for (auto i : intersectionsOut)
        cout << i.x << " " << i.y << endl;

    vector<pair<int, int>> intersectingSegmentIdsExpected = {{0, 1}};
    vector<Point> intersectionsExpected = { {0.5, 0.5} };

    EXPECT_TRUE(intersectionsExpected == intersectionsOut);

    segments = { {{1, 5}, {4, 5}}, {{2, 5}, {10, 1}},{{3, 2}, {10, 3}},{{6, 4}, {9, 4}},{{7, 1}, {8, 1}} };

    intersectionsOut.clear();
    intersectingSegmentIdsOut.clear();
    lineSegmentIntersectionSweepLine(segments, intersectingSegmentIdsOut, intersectionsOut);

    cout << "Intersecting segment ids\n";
    for (auto sId : intersectingSegmentIdsOut)
        cout << sId.first << " " << sId.second << endl;
    cout << endl;

    cout << "Intersections\n";
    for (auto i : intersectionsOut)
        cout << i.x << " " << i.y << endl;

    intersectionsExpected = {{2, 5}, {6.8889, 2.5556}};
    intersectingSegmentIdsExpected = {{0, 1}, {1, 2}};

    EXPECT_TRUE(intersectionsExpected == intersectionsOut);

    AVLTree<Event> eventTree;
    eventTree.insert(Event{START, 1.2, 0, 1});
    eventTree.insert(Event{END, -1.2, 0, 1});
    eventTree.insert(Event{START, 1.2, 1, 2});
    eventTree.insert(Event{END, -1.2, 1, 2});
    eventTree.insert(Event{START, 1.2, 2, 3});

    auto event = eventTree.removeMin();
}
