#include "AVLTree.h"

#include <iostream>

using namespace std;

int main()
{
    AVLTree<int> tree;

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

    cout << tree.m_root->value << endl;
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

    return 0;
}
