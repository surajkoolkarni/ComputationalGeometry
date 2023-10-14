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

    cout << tree.height() << endl;

    return 0;
}
