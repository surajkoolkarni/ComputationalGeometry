#pragma once

#include "BSTree.h"

template <typename T>
class AVLTree
{
public:
    struct AVLNode
    {
        int balanceFactor;

        int height;

        T value;

        AVLNode* leftChild;
        AVLNode* rightChild;

        AVLNode(const T& val) :
            balanceFactor(),
            height(),
            value(val)
        {
        }
    };

public:
    AVLTree();

    ~AVLTree();

    bool isEmpty() const;

    T findMin() const;

    bool find(const T& val) const;

    void inorder(std::function<void (T)> traverser) const;
    
    void insert(const T& value);

    void remove(const T& value);

    int height() const;

    T removeMin();
     
private:
    void update(AVLNode*& node);

    AVLNode* balance(AVLNode*& node);

    void destroy(AVLNode*& node);
 
    bool _find(const T& val, AVLNode* node) const;
    
    AVLNode* _insert(const T& val, AVLNode*& node);

    AVLNode* _findMin(AVLNode* node) const;
    
    void _remove(const T& val, AVLNode* & node);
    
    void _inorder(AVLNode* node, std::function<void (T)> traverser) const;

    int _height(AVLNode* node) const;

private:
    AVLNode* leftLeftCase(AVLNode* node);
    AVLNode* leftRightCase(AVLNode* node);

    AVLNode* rightLeftCase(AVLNode* node);
    AVLNode* rightRightCase(AVLNode* node);

    AVLNode* leftRotation(AVLNode* node);
    AVLNode* rightRotation(AVLNode* node);

private:
    AVLNode* m_root;
};

template <typename T>
AVLTree<T>::AVLTree() :
    m_root(nullptr)
{
}

template <typename T>
AVLTree<T>::~AVLTree()
{
    destroy(m_root);
}

template <typename T>
inline bool AVLTree<T>::isEmpty() const
{
    return !m_root;
}

template <typename T>
inline T AVLTree<T>::findMin() const
{
    auto* node = _findMin(m_root);
    return node ? node->value : T();
}

template <typename T>
bool AVLTree<T>::find(const T &val) const
{
    return _find(val, m_root);
}

template <typename T>
void AVLTree<T>::inorder(std::function<void(T)> traverser) const
{
    _inorder(m_root, traverser);
}

template <typename T>
void AVLTree<T>::insert(const T &value)
{
    m_root = _insert(value, m_root);
}

template <typename T>
void AVLTree<T>::remove(const T &value)
{
    _remove(value, m_root);
}

template <typename T>
inline int AVLTree<T>::height() const
{
    return _height(m_root);
}

template <typename T>
T AVLTree<T>::removeMin()
{
    auto val = findMin();
    remove(val);
    return val;
}

template <typename T>
void AVLTree<T>::update(AVLNode *&node)
{
    int leftNodeHeight = (!node->leftChild) ? -1 : node->leftChild->height;
    int rightNodeHeight = (!node->rightChild) ? -1 : node->rightChild->height;

    node->height = 1 + std::max(leftNodeHeight, rightNodeHeight);

    node->balanceFactor = rightNodeHeight - leftNodeHeight;
}

template <typename T>
typename AVLTree<T>::AVLNode* AVLTree<T>::balance(AVLNode *&node)
{
    if (node->balanceFactor == -2)
    {
        if (node->leftChild->balanceFactor <= 0)
            return leftLeftCase(node);
        else
            return leftRightCase(node);
    }
    else if (node->balanceFactor == 2)
    {
        if (node->rightChild->balanceFactor >= 0)
            return rightRightCase(node);
        else
            return rightLeftCase(node);
    }

    return node;
}

template <typename T>
void AVLTree<T>::destroy(AVLNode *&node)
{
    if (!node)
        return;
    
    destroy(node->leftChild);
    destroy(node->rightChild);
    delete node;
}

template <typename T>
bool AVLTree<T>::_find(const T &val, AVLNode *node) const
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
typename AVLTree<T>::AVLNode* AVLTree<T>::_insert(const T &val, AVLNode *&node)
{
    if (!node)
        return new AVLNode(val);

    if (val < node->value)
        node->leftChild = _insert(val, node->leftChild);
    else if (val > node->value)
        node->rightChild =_insert(val, node->rightChild);

    update(node);

    return balance(node);
}

template <typename T>
typename AVLTree<T>::AVLNode *AVLTree<T>::_findMin(AVLNode *node) const
{
    while (node && node->leftChild)
        node = node->leftChild;
    
    return node;
}

template <typename T>
void AVLTree<T>::_remove(const T &val, AVLNode *&node)
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

    update(node);

    balance(node);
}

template <typename T>
void AVLTree<T>::_inorder(AVLNode *node, std::function<void(T)> traverser) const
{
    if (!node)
        return;

    _inorder(node->leftChild, traverser);
    traverser(node->value);
    _inorder(node->rightChild, traverser);
}

template <typename T>
int AVLTree<T>::_height(AVLNode *node) const
{
    if (!node)
        return -1;
    
    return std::max(_height(node->leftChild), _height(node->rightChild)) + 1;
}

template <typename T>
typename AVLTree<T>::AVLNode* AVLTree<T>::leftLeftCase(AVLNode *node)
{
    return rightRotation(node);
}

template <typename T>
typename AVLTree<T>::AVLNode* AVLTree<T>::leftRightCase(AVLNode *node)
{
    node->leftChild = leftRotation(node->leftChild);
    return leftLeftCase(node);
}

template <typename T>
typename AVLTree<T>::AVLNode* AVLTree<T>::rightLeftCase(AVLNode *node)
{
    node->rightChild = rightRotation(node->rightChild);
    return rightRightCase(node);
}

template <typename T>
typename AVLTree<T>::AVLNode* AVLTree<T>::rightRightCase(AVLNode *node)
{
    return leftRotation(node);
}

template <typename T>
typename AVLTree<T>::AVLNode* AVLTree<T>::leftRotation(AVLNode *node)
{
    auto* newParent = node->rightChild;
    node->rightChild = newParent->leftChild;
    newParent->leftChild = node;

    update(node);
    update(newParent);

    return newParent;
}

template <typename T>
typename AVLTree<T>::AVLNode* AVLTree<T>::rightRotation(AVLNode *node)
{
    auto* newParent = node->leftChild;
    node->leftChild = newParent->rightChild;
    newParent->rightChild = node;

    update(node);
    update(newParent);

    return newParent;
}
