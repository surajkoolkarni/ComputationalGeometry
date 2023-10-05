#pragma once

#include "TreeNode.h"

#include <functional>
#include <iostream>

template<typename T>
class BSTree
{
public:
    struct TreeNode
    {
        TreeNode* leftChild;
        TreeNode* rightChild;

        T value;

        TreeNode(const T& val) :
            value(val),
            leftChild(),
            rightChild()
        {
        }

        ~TreeNode()
        {
        }
    };

public:
    BSTree();

    ~BSTree();

    bool isEmpty() const;

    T findMin() const;

    bool find(const T& val) const;

    void inorder(std::function<void (T)> traverser) const;
    
    void insert(const T& value);

    void remove(const T& value);

    T removeMin();

    int height() const;
    
private:
    void destroy(TreeNode*& node);

    bool _find(const T& val, TreeNode* node) const;
    
    void _insert(const T& val, TreeNode*& node);

    TreeNode* _findMin(TreeNode* node) const;
    
    void _remove(const T& val, TreeNode* & node);
    
    void _inorder(TreeNode* node, std::function<void (T)> traverser) const;

    int _height(TreeNode* node) const;

private:
    TreeNode* m_root;
};

template <typename T>
inline BSTree<T>::BSTree() :
    m_root(nullptr)
{
}

template <typename T>
inline BSTree<T>::~BSTree()
{
    destroy(m_root);
}

template <typename T>
inline bool BSTree<T>::isEmpty() const
{
    return !m_root;
}

template <typename T>
void BSTree<T>::destroy(TreeNode *&node)
{
    if (node)
    {
        destroy(node->leftChild);
        destroy(node->rightChild);
        delete node;
    }
}

template <typename T>
inline bool BSTree<T>::_find(const T &val, TreeNode *node) const
{
    if (!node)
        return false;

    if (val < node->value)
        return _find(val, node->leftChild);
    else if (val > node->value)
        return _find(val, node->rightChild);
    else
        return true;
}

template <typename T>
T BSTree<T>::findMin() const
{
    auto node = _findMin(m_root);
    return node ? node->value : T();
}

template <typename T>
bool BSTree<T>::find(const T &val) const
{
    return _find(val, m_root);
}

template <typename T>
void BSTree<T>::inorder(std::function<void(T)> traverser) const
{
    _inorder(m_root, traverser);
}

template <typename T>
void BSTree<T>::insert(const T &value)
{
    _insert(value, m_root);
}

template <typename T>
void BSTree<T>::remove(const T &value)
{
    _remove(value, m_root);
}

template <typename T>
T BSTree<T>::removeMin()
{
    auto val = findMin();
    remove(val);
    return val;
}

template <typename T>
inline int BSTree<T>::height() const
{
    return _height(m_root);
}

template <typename T>
void BSTree<T>::_insert(const T &val, TreeNode*& node)
{
    if (!node)
    {
        node = new TreeNode(val);
    }
    else if (val < node->value)
    {
        _insert(val, node->leftChild);
    }
    else if (val > node->value)
    {
        _insert(val, node->rightChild);
    }
}

template <typename T>
typename BSTree<T>::TreeNode *BSTree<T>::_findMin(TreeNode *node) const
{
    while (node && node->leftChild)
        node = node->leftChild;    

    return node;
}

template <typename T>
void BSTree<T>::_remove(const T &val, TreeNode *&node)
{
    if (!node)
        return;

    if (val < node->value)
        _remove(val, node->leftChild);
    else if (val > node->value)
        _remove(val, node->rightChild);
    else
    {
        if (!node->leftChild && !node->rightChild)
        {
            delete node;
            node = nullptr;
        }
        else if (!node->leftChild)
        {
            auto* old = node;
            node = old->rightChild;
            delete old;
        }
        else if (!node->rightChild)
        {
            auto* old = node;
            node = old->leftChild;
            delete old;
        }
        else
        {
            auto* minNodeInRightSubTree = _findMin(node->rightChild);
            node->value = minNodeInRightSubTree->value;
            _remove(minNodeInRightSubTree->value, node->rightChild);
        }
    }
}

template <typename T>
void BSTree<T>::_inorder(TreeNode *node, std::function<void (T)> traverser) const
{
    if (!node)
    {
        return;
    }

    if (node->leftChild)
        _inorder(node->leftChild, traverser);

    traverser(node->value);
    
    if (node->rightChild)
        _inorder(node->rightChild, traverser);
}

template <typename T>
int BSTree<T>::_height(TreeNode *node) const
{
    if (!node)
        return -1;
    
    return std::max(_height(node->leftChild), _height(node->rightChild)) + 1;
}
