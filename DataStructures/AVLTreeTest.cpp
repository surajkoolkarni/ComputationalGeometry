#include "AVLTree.h"

#include "../Point.h"
#include "../Segment.h"

#include <iostream>

using namespace std;

int main()
{
    AVLTree<int> tree;
    tree.insert(0);

    for (int i = 0; i < 8; ++i)
        tree.insert(i);

    tree.inorder([](const int& x){
        cout << x << endl;
    });
    cout << endl;

    cout << tree.height() << endl;
    cout << endl;


    cout << tree.predecessor(3) << endl;
    cout << tree.successor(3) << endl;
    cout << endl;

    // cout << tree.m_root->value << endl;
    cout << endl;

    cout << tree.predecessor(0) << endl;
    cout << tree.successor(0) << endl;
    cout << endl;

    cout << tree.predecessor(1) << endl;
    cout << tree.successor(1) << endl;
    cout << endl;

    cout << tree.predecessor(7) << endl;
    cout << tree.successor(7) << endl;
    cout << endl;

    AVLTree<Point> pointTree;
    pointTree.insert(Point{1.3, 2.4});
    pointTree.insert(Point{5.6, -9.8});
    pointTree.insert(Point{-3.2, 4.5});
    pointTree.insert(Point{0.0, 0.0});
    pointTree.insert(Point{2.3, 3.4});
    
    AVLTree<Segment> segTree;
    segTree.insert(Segment{{1.2, 3.4}, {1.3, 2.4}});
    segTree.insert(Segment{{-2.3, 4.5}, {-3.2, 4.5}});
    segTree.insert(Segment{{-2.3, 4.5}, {0.0, 0.0}});
    segTree.insert(Segment{{0.0, 0.0}, {1.3, 2.4}});
    segTree.insert(Segment{{1.2, 3.4}, {1.3, 2.4}});

    return 0;
}
