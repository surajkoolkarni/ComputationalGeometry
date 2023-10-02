#pragma once

#include <iostream>

template<typename T>
struct TreeNode
{
    TreeNode* leftChild;
    TreeNode* rightChild;

    T value;

    TreeNode(const T& val) :
        value(val)
    {
    }

    ~TreeNode()
    {
        if (leftChild)
            delete leftChild;

        if (rightChild)
            delete rightChild;
    }
};
