#pragma once

#include "TreeNode.h"

#include <functional>
#include <iostream>

template<typename T>
class BSTree
{
public:
    BSTree(const std::function<bool (const T&, const T&)>& comparator);

    ~BSTree();

    bool isEmpty() const;

    T find(const T& val) const;
    T findMin() const;

    void inorder(std::function<void (T)> traverser) const;
    void insert(const T& value);

    void remove(const T& value);
    T removeMin();

private:
    void _insert(const T& val, TreeNode<T>* node);

    TreeNode<T>* _findMin(TreeNode<T>* node) const;
    
    void _remove(const T& val, TreeNode<T>* & node);
    
    void _inorder(TreeNode<T>* node, std::function<void (T)> traverser) const;

private:
    TreeNode<T>* m_root;

    /// @brief return < 0 if smaller, > 0 if larger, == 0 if equal
    std::function<int (const T&, const T&)> m_comparator;
};

template <typename T>
inline BSTree<T>::BSTree(const std::function<bool(const T &, const T &)>& comparator) :
    m_comparator(comparator), m_root(nullptr)
{
}

template <typename T>
inline BSTree<T>::~BSTree()
{
    if (m_root)
        delete m_root;
}

template <typename T>
inline bool BSTree<T>::isEmpty() const
{
    return !m_root;
}

template <typename T>
inline T BSTree<T>::find(const T &val) const
{
    TreeNode<T>* root = m_root;

    while (root)
    {
        int res = m_comparator(val, m_root->value);

        if (res < 0)
            root = root->leftChild;
        else if (res > 0)
            root = root->rightChild;
        else
            return root->value;
    }

    return T();
}

template <typename T>
T BSTree<T>::findMin() const
{
    auto node = _findMin(m_root);
    return node ? node->value : T();
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
    // if (!m_root)
    // {
    //     m_root = new TreeNode<T>(value);
    // }
    // else
    // {
    //     int result;
    //     TreeNode<T>* node = m_root, *p;
    //     while (node)
    //     {
    //         p = node;
    //         result = m_comparator(value, p->value);

    //         if (result > 0)
    //             node = p->rightChild;
    //         else if (result < 0)
    //             node = p->leftChild;
    //         else
    //             return;
    //     }

    //     if (result > 0)
    //         p->rightChild = new TreeNode<T>(value);
    //     else
    //         p->leftChild = new TreeNode<T>(value);
    // }
}

template <typename T>
void BSTree<T>::remove(const T &value)
{
    auto* root = m_root;
    _remove(value, root);
}

template <typename T>
T BSTree<T>::removeMin()
{
    auto val = findMin();
    remove(val);
    return val;
}

template <typename T>
void BSTree<T>::_insert(const T &val, TreeNode<T> *node)
{
    if (!m_root)
    {
        m_root = new TreeNode<T>(val);
    }
    else if (m_comparator(val, node->value) < 0)
    {
        if (node->leftChild)
        {
            _insert(val, node->leftChild);
        }
        else
        {
            node->leftChild = new TreeNode<T>(val);
        }
    }
    else if (m_comparator(val, node->value) > 0)
    {
        if (node->rightChild)
        {
            _insert(val, node->rightChild);
        }
        else
        {
            node->rightChild = new TreeNode<T>(val);
        }
    }
    else
    {
        return;
    }
}

template <typename T>
TreeNode<T> *BSTree<T>::_findMin(TreeNode<T> *node) const
{
    if (!node)
        return nullptr;

    while (node->leftChild)
        node = node->leftChild;    

    return node;
}

template <typename T>
void BSTree<T>::_remove(const T &val, TreeNode<T> *&node)
{
    using std::cout; using std::endl;
    cout << "Removing value " << val << endl;

    if (!node)
        return;

    int result = m_comparator(val, node->value);

    if (result < 0)
        _remove(val, node->leftChild);
    else if (result > 0)
        _remove(val, node->rightChild);
    else
    {
        if (!node->leftChild)
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
void BSTree<T>::_inorder(TreeNode<T> *node, std::function<void (T)> traverser) const
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
