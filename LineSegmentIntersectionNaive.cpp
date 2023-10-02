#include "Point.h"
#include "Segment.h"
#include "Util.h"

#include <vector>
#include <utility>

#include <gtest/gtest.h>


using namespace std;

void lineSegmentIntersectionNaive(const vector<Segment>& segments, vector<pair<int, int>>& intersectingSegmentIds, vector<Point>& intersectionPoints)
{
    Point p;

    for (int i = 0; i < segments.size(); ++i)
    {
        vector<bool> visited(false, segments.size());
        
        for (int j = i + 1; j < segments.size(); ++j)
        {
            if (intersection(segments[i], segments[j], p))
            {
                intersectingSegmentIds.push_back(make_pair(i, j));
                intersectionPoints.push_back(p);
            }
        }
    }
}


TEST(lineSegmentIntersectionNaive, simple)
{
    Point iHat{1, 0};
    Point jHat{0, 1};
    
    EXPECT_DOUBLE_EQ(iHat ^ jHat, 1.);

    Point i;
    Point e{0.5, 0.5};

    EXPECT_TRUE(intersection({{0., 0.}, {1., 1.}}, {{1., 0.}, {0., 1.}}, i));
    EXPECT_EQ(i, e);

    vector<Segment> segments = { { {0, 0}, {1, 1} }, { {1, 0}, {0, 1} } };
    vector<pair<int, int>> intersectingSegmentIdsOut;
    vector<Point> intersectionsOut;

    lineSegmentIntersectionNaive(segments, intersectingSegmentIdsOut, intersectionsOut);

    vector<pair<int, int>> intersectingSegmentIdsExpected = {{0, 1}};
    vector<Point> intersectionsExpected = { {0.5, 0.5} };

    EXPECT_TRUE(intersectingSegmentIdsExpected == intersectingSegmentIdsOut);
    EXPECT_TRUE(intersectionsExpected == intersectionsOut);

    segments = { {{1, 5}, {4, 5}}, {{2, 5}, {10, 1}},{{3, 2}, {10, 3}},{{6, 4}, {9, 4}},{{7, 1}, {8, 1}} };

    intersectionsOut.clear();
    intersectingSegmentIdsOut.clear();
    lineSegmentIntersectionNaive(segments, intersectingSegmentIdsOut, intersectionsOut);

    intersectionsExpected = {{2, 5}, {6.8889, 2.5556}};
    intersectingSegmentIdsExpected = {{0, 1}, {1, 2}};

    EXPECT_TRUE(intersectingSegmentIdsExpected == intersectingSegmentIdsOut);
    EXPECT_TRUE(intersectionsExpected == intersectionsOut);
}
