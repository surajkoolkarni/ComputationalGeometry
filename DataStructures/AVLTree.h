#pragma once

#include <functional>

template <typename T>
class AVLTree
{
public:
    struct AVLNode
    {
        int balanceFactor;

        int height;

        T key;

        AVLNode* leftChild;
        AVLNode* rightChild;

        AVLNode() = default;

        AVLNode(const T& val) :
            balanceFactor(0),
            height(0),
            key(val),
            leftChild(nullptr),
            rightChild(nullptr)
        {
        }
    };

public:
    AVLTree();

    ~AVLTree();

    bool isEmpty() const;

    AVLNode* min() const;
    AVLNode* max() const;

    bool find(const T& val) const;

    void inorder(std::function<void (T)> traverser) const;
    
    void insert(const T& key);

    void remove(const T& key);

    int height() const;

    AVLNode* removeMin();

    AVLNode* predecessor(const T& val) const;

    AVLNode* successor(const T& val) const;
     
private:
    void update(AVLNode* node);

    AVLNode* balance(AVLNode* node);

    void destroy(AVLNode* node);
 
    bool _find(const T& val, AVLNode* node) const;
    
    AVLNode* _insert(const T& val, AVLNode* node);

    AVLNode* _findMin(AVLNode* node) const;
    AVLNode* _findMax(AVLNode* node) const;
    
    AVLNode* _remove(const T& val, AVLNode* node);
    
    void _inorder(AVLNode* node, std::function<void (T)> traverser) const;

    int _height(AVLNode* node) const;

    void _predecessor(const T& val, AVLNode* node, AVLNode** predecessorNode) const;

    void _successor(const T& val, AVLNode* node, AVLNode** successorNode) const;

private:
    AVLNode* leftLeftCase(AVLNode* node);
    AVLNode* leftRightCase(AVLNode* node);

    AVLNode* rightLeftCase(AVLNode* node);
    AVLNode* rightRightCase(AVLNode* node);

    AVLNode* leftRotation(AVLNode* node);
    AVLNode* rightRotation(AVLNode* node);

private:
    AVLNode* m_root;
    int m_nNodes = 0;
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
inline typename AVLTree<T>::AVLNode* AVLTree<T>::min() const
{
    return _findMin(m_root);
}

template <typename T>
inline typename AVLTree<T>::AVLNode *AVLTree<T>::max() const
{
    return _findMax(m_root);
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
void AVLTree<T>::insert(const T &key)
{
    m_root = _insert(key, m_root);
    ++m_nNodes;
}

template <typename T>
void AVLTree<T>::remove(const T &key)
{
    m_root = _remove(key, m_root);
}

template <typename T>
inline int AVLTree<T>::height() const
{
    return _height(m_root);
}

template <typename T>
typename AVLTree<T>::AVLNode* AVLTree<T>::removeMin()
{
    auto* minNode = _findMin(m_root);

    if (minNode)
        remove(minNode->key);
    
    return minNode;
}

template <typename T>
inline typename AVLTree<T>::AVLNode* AVLTree<T>::predecessor(const T &val) const
{
    AVLNode* predecessorNode = nullptr;
    _predecessor(val, m_root, &predecessorNode);
    return predecessorNode;
}

template <typename T>
inline typename AVLTree<T>::AVLNode* AVLTree<T>::successor(const T &val) const
{
    AVLNode* successorNode = nullptr;
    _successor(val, m_root, &successorNode);
    return successorNode;
}

template <typename T>
void AVLTree<T>::update(AVLNode* node)
{
    if (!node)
        return;

    int leftNodeHeight = (!node->leftChild) ? -1 : node->leftChild->height;
    int rightNodeHeight = (!node->rightChild) ? -1 : node->rightChild->height;

    node->height =  1 + std::max(leftNodeHeight, rightNodeHeight);

    node->balanceFactor = rightNodeHeight - leftNodeHeight;
}

template <typename T>
typename AVLTree<T>::AVLNode* AVLTree<T>::balance(AVLNode *node)
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
void AVLTree<T>::destroy(AVLNode *node)
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

    if (val < node->key)
        return _find(val, node->leftChild);
    else if (val > node->key)
        return _find(val, node->rightChild);
    else
        return true;
}

template <typename T>
typename AVLTree<T>::AVLNode* AVLTree<T>::_insert(const T &val, AVLNode *node)
{
    if (!node)
        return new AVLNode(val);

    if (val < node->key)
        node->leftChild = _insert(val, node->leftChild);
    else if (val > node->key)
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

template<typename T>
typename AVLTree<T>::AVLNode* AVLTree<T>::_findMax(AVLNode *node) const
{
    while (node && node->rightChild)
        node = node->rightChild;
    
    return node;
}

template <typename T>
typename AVLTree<T>::AVLNode* AVLTree<T>::_remove(const T &val, AVLNode *node)
{
    if (!node)
        return nullptr;

    if (val < node->key)
        node->leftChild = _remove(val, node->leftChild);
    else if (val > node->key)
        node->rightChild = _remove(val, node->rightChild);
    else
    {
        if (!node->leftChild)
        {
            return node->rightChild;
        }
        else if (!node->rightChild)
        {
            return node->leftChild;
        }
        else
        {
            if (node->leftChild->height > node->rightChild->height)
            {
                AVLNode *temp = _findMax(node->leftChild);
                node->key = temp->key;
                node->leftChild = _remove(temp->key, node->leftChild);
            }
            else
            {
                AVLNode* temp = _findMin(node->rightChild);
                node->key = temp->key;
                node->rightChild = _remove(temp->key, node->rightChild);
            }
        }
    }

    update(node);

    return balance(node);
}

template <typename T>
void AVLTree<T>::_inorder(AVLNode *node, std::function<void(T)> traverser) const
{
    if (!node)
        return;

    _inorder(node->leftChild, traverser);
    traverser(node->key);
    _inorder(node->rightChild, traverser);
}

template <typename T>
int AVLTree<T>::_height(AVLNode *node) const
{
    if (!node)
        return -1;

    return 1 + std::max(_height(node->leftChild), _height(node->rightChild));
}

template <typename T>
void AVLTree<T>::_predecessor(const T &val, AVLNode *node, AVLNode** predecessorNode) const
{
    if (!node)
        return;

    if (node->key == val)
    {
        if (node->leftChild)
        {
            AVLNode* n = node->leftChild;
            while (n->rightChild)
                n = n->rightChild;

            *predecessorNode = n;
        }
    }
    else if (node->key > val)
    {
        _predecessor(val, node->leftChild, predecessorNode);
    }
    else
    {
        *predecessorNode = node;
        _predecessor(val, node->rightChild, predecessorNode);
    }
}

template <typename T>
void AVLTree<T>::_successor(const T &val, AVLNode *node, AVLNode** successorNode) const
{
    if (!node)
        return;
    
    if (node->key == val)
    {
        if (node->rightChild)
        {
            AVLNode* n = node->rightChild;
            while (n->leftChild)
                n = n->leftChild;

            *successorNode = n;
        }
    }
    else if (node->key > val)
    {
        *successorNode = node;
        _successor(val, node->leftChild, successorNode);
    }
    else
    {
        _successor(val, node->rightChild, successorNode);
    }
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
