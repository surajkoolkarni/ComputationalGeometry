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
    Point p;
    int id1;
    int id2;

    double value;

    Event() = default;

    Event(EventType type_, const Point& p_, int id1_, int id2_) :
        type(type_), p(p_), id1(id1_), id2(id2_), value(p.x)
    {
    }

    bool operator< (const Event& e) const
    {
        return value < e.value;
    }

    bool operator> (const Event& e) const
    {
        return value > e.value;
    }

    bool operator== (const Event& e) const
    {
        return value == e.value;
    }
};

void checkForIntersection(const Segment& s1, const Segment& s2, Point& i, AVLTree<Event>& eventQueue)
{
    if (intersection(s1, s2, i))
        eventQueue.insert(Event{CROSS, i, s1.id, s2.id});
}

void removeFuture(const Segment& s1, const Segment& s2, AVLTree<Event>& eventQueue)
{
    auto* it = eventQueue.min();

    while (it)
    {
        const auto& event = it->key;

        if (event.type == CROSS)
        {
            if ((s1.id == event.id1 && s2.id == event.id2) || (s1.id == event.id2 && s2.id == event.id1))
            {
                eventQueue.remove(event);
                return;
            }
        }
    
        it = eventQueue.successor(event);
    }
}

void recalculate(double l, AVLTree<Segment>& segmentTree)
{
    auto* it = segmentTree.min();
    while (it)
    {
        auto& seg = it->key;
        seg.calculateValue(l);
        it = segmentTree.successor(seg);
    }
}

void swapSegments(Segment segment1, Segment segment2, AVLTree<Segment>& segmentTree)
{
    segmentTree.remove(segment1);
    segmentTree.remove(segment2);

    double val = segment1.value;
    segment1.value = segment2.value;
    segment2.value = val;

    segmentTree.insert(segment1);
    segmentTree.insert(segment2);
}

void lineSegmentIntersectionSweepLine(vector<Segment>& segments, vector<pair<int, int>>& intersectingSegmentIds, vector<Point>& intersectionPoints)
{
    for (int i = 0; i < segments.size(); ++i)
        segments[i].id = i;

    AVLTree<Event> eventQueue;
    AVLTree<Segment> segmentTree;

    for (int i = 0; i < segments.size(); ++i)
    {
        eventQueue.insert(Event{START, segments[i].first(), segments[i].id, segments[i].id});
        eventQueue.insert(Event{END, segments[i].second(), segments[i].id, segments[i].id});
    }

    while(!eventQueue.isEmpty())
    {
        auto* eventNode = eventQueue.removeMin();
        Point intersectionPoint;

        auto event = eventNode->key;
        double l = event.value;

        if (event.type == START)
        {
            const auto& segment = segments[event.id1];
            const auto* successor = segmentTree.successor(segment);
            const auto* predecessor = segmentTree.predecessor(segment);

            recalculate(l, segmentTree);
            
            segmentTree.insert(segment);

            if (successor)
                checkForIntersection(segment, successor->key, intersectionPoint, eventQueue);

            if (predecessor)
                checkForIntersection(predecessor->key, segment, intersectionPoint, eventQueue);

            if (predecessor && successor)
                removeFuture(predecessor->key, successor->key, eventQueue);
        }
        else if (event.type == END)
        {
            const auto& segment = segments[event.id1];
            const auto* successor = segmentTree.successor(segment);
            const auto* predecessor = segmentTree.predecessor(segment);
            
            if (predecessor && successor)
                checkForIntersection(predecessor->key, successor->key, intersectionPoint, eventQueue);

            segmentTree.remove(segment);
        }
        else if (event.type == CROSS)
        {
            auto& segment1 = segments[event.id1];
            auto& segment2 = segments[event.id2];

            intersection(segment1, segment2, intersectionPoint);
            intersectionPoints.push_back(intersectionPoint);
            intersectingSegmentIds.push_back({segment1.id, segment2.id});

            swapSegments(segment1, segment2, segmentTree);

            const auto* segment1Successor = segmentTree.successor(segment1);
            const auto* segment1Predecessor = segmentTree.predecessor(segment1);

            const auto* segment2Successor = segmentTree.successor(segment2);
            const auto* segment2Predecessor = segmentTree.predecessor(segment2);
            
            if (segment1.value < segment2.value)
            {
                if (segment1Successor)
                {
                    checkForIntersection(segment1, segment1Successor->key, intersectionPoint, eventQueue);
                    removeFuture(segment1Successor->key, segment2, eventQueue);
                }

                if (segment2Predecessor)
                {
                    checkForIntersection(segment2Predecessor->key, segment2, intersectionPoint, eventQueue);
                    removeFuture(segment2Predecessor->key, segment1, eventQueue);
                }
            }
            else
            {
                if (segment1Predecessor)
                {
                    checkForIntersection(segment1Predecessor->key, segment1, intersectionPoint, eventQueue);
                    removeFuture(segment1Predecessor->key, segment2, eventQueue);
                }

                if (segment2Successor)
                {
                    checkForIntersection(segment2, segment2Successor->key, intersectionPoint, eventQueue);
                    removeFuture(segment2Successor->key, segment1, eventQueue);
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

    segments = { {{1, 5}, {4, 5}}, {{2, 5}, {10, 1}}, {{3, 2}, {10, 3}} , {{6, 4}, {9, 4}},{{7, 1}, {8, 1}} };

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
    eventTree.insert(Event{START, {1., 2}, 0, 1});
    eventTree.insert(Event{END, {1., 2}, 1, 1});
    eventTree.insert(Event{START, {1., -1}, 2, 2});
    eventTree.insert(Event{END, {1., -1}, 3, 2});
    eventTree.insert(Event{START, {-1., 2}, 4, 3});

    auto event = eventTree.removeMin();
}
