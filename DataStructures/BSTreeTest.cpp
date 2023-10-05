#include "BSTree.h"

#include <iostream>

using namespace std;

void heapSort(int s[], int n)
{
    BSTree<int> t;
    for (int i = 0; i < n; i++)
        t.insert(s[i]);
    
    t.inorder([](int val){
        cout << val << endl;
    });

    cout << t.height() << endl;

    for (int i = 0; i < n; i++)
        s[i] = t.removeMin();
}

int main()
{
    int arr[] = { 3, 0, 1, 2, 4, 6, 5 };
    heapSort(arr, 7);
}