CXX=g++ -std=c++17 -g

all: ConvexHullNaive ConvexHullGrahamScan ConvexHullJarvisMarch

ConvexHullNaive: ConvexHullNaive.o
	$(CXX) ConvexHullNaive.o -o ConvexHullNaive -lgtest_main -lgtest; ./ConvexHullNaive

ConvexHullGrahamScan: ConvexHullGrahamScan.o
	$(CXX) ConvexHullGrahamScan.o -o ConvexHullGrahamScan -lgtest_main -lgtest; ./ConvexHullGrahamScan

ConvexHullJarvisMarch: ConvexHullJarvisMarch.o
	$(CXX) ConvexHullJarvisMarch.o -o ConvexHullJarvisMarch -lgtest_main -lgtest; ./ConvexHullJarvisMarch

LineSegmentIntersectionNaive: LineSegmentIntersectionNaive.o
	$(CXX) LineSegmentIntersectionNaive.o -o LineSegmentIntersectionNaive -lgtest_main -lgtest; ./LineSegmentIntersectionNaive

LineSegmentIntersectionSweepLine: LineSegmentIntersectionSweepLine.o
	$(CXX) LineSegmentIntersectionSweepLine.o -o LineSegmentIntersectionSweepLine -lgtest_main -lgtest; ./LineSegmentIntersectionSweepLine

clean:
	rm -f ConvexHullNaive ConvexHullGrahamScan ConvexHullJarvisMarch LineSegmentIntersectionNaive LineSegmentIntersectionSweepLine *.o
