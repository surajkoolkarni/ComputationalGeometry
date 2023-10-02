#include "BSTree.h"

#include <iostream>

using namespace std;

void heapSort(int s[], int n, const std::function<int (const int&, const int&)>& cmp)
{
    BSTree<int> t(cmp);
    for (int i = 0; i < n; i++)
        t.insert(s[i]);
    
    t.inorder([](int val){
        cout << val << endl;
    });

    for (int i = 0; i < n; i++)
        s[i] = t.removeMin();
}

int main()
{
    int arr[] = { 3, 0, 1, 2, 4, 6, 5 };
    heapSort(arr, 7, [](const int& x, const int& y){
        if (x < y)
            return -1;
        else if (x > y)
            return 1;
        else
            return 0;
    });
}