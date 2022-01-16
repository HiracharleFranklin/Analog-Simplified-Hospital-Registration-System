#pragma once
#include "B_Tree_Decl.h"
// #include <windows.h>
using namespace std;

template <typename T>
B_Tree<T>::B_Tree(int x)
{
    min_degree = x;
    key_num = 0;
    root = createEmptyNode();
}

template <typename T>
B_Tree<T>::~B_Tree()
{
    freeAll(root);
}

template <typename T>
bool B_Tree<T>::contain(T key) const
{
    return nullptr != search(root, key);
}

template <typename T>
T *B_Tree<T>::getHandle(T key) const
{
    return search(root, key);
}

template <typename T>
void B_Tree<T>::display() const
{

    cout << endl
         << "=====================INFO=====================" << endl
         << endl;
    // cout << "root info: " << endl;
    // printNodeInfo(root);
    // cout << "left info: " << endl;
    // printNodeInfo(root->children[0]);
    // cout << "right info: " << endl;
    // printNodeInfo(root->children[1]);

    cout << "B-Tree details (root node at left most):" << endl
         << endl;

    display(root);

    cout << endl
         << "B-Tree has " << key_num << " keys in total." << endl;

    cout << endl
         << "=====================ENDS=====================" << endl
         << endl;
}

template <typename T>
bool B_Tree<T>::insert(T key)
{
    // cout << "Now insert key: " << key << endl;
    if (contain(key))
        return false;

    if (root == nullptr)
        root = createEmptyNode();

    // while searching for the position along the tree,
    // we split every full node in time (including leaf node itself),
    // such that every time we need to split full node,
    // we can make sure that its parent node is not full.
    if (root->size == 2 * min_degree - 1)
    {
        B_Tree_Node<T> *new_root = createEmptyNode();
        new_root->is_leaf = false;
        new_root->children[0] = root;
        splitChild(new_root, 0);
        root = new_root;

        updateDepth(root);
        // cout << "point 1" << endl;
        // printNodeInfo(root);
        // cout << "point 2" << endl;
    }

    bool ret = insertNonFull_recursively(root, key);
    if (ret)
        key_num++;
    return ret;
}

template <typename T>
bool B_Tree<T>::remove(T key)
{
    if (key_num == 0)
        return false;
    bool ret = remove(root, key);
    if (ret)
        key_num--;
    return ret;
}

template <typename T>
void B_Tree<T>::printNodeInfo(B_Tree_Node<T> *p_node) const
{
    cout << endl;
    cout << "---node info---" << endl;
    cout << "is leaf: " << p_node->is_leaf << endl;
    cout << "depth is: " << p_node->depth << endl;
    cout << "key array contains: ";
    for (int i = 0; i < p_node->size; i++)
        cout << p_node->keys[i] << " ";
    cout << endl;

    for (int i = 0; i <= p_node->size; i++)
    {
        cout << i << "th child contains: ";
        for (int j = 0; j < p_node->children[i]->size; j++)
            cout << p_node->children[i]->keys[j] << " ";
        cout << endl;
    }
    cout << endl;
}

template <typename T>
B_Tree_Node<T> *B_Tree<T>::createEmptyNode()
{
    B_Tree_Node<T> *ret = new B_Tree_Node<T>;

    ret->is_leaf = true;
    ret->size = 0;
    ret->depth = 0;
    ret->keys = new T[2 * min_degree - 1];
    ret->children = new B_Tree_Node<T> *[2 * min_degree];

    return ret;
}

template <typename T>
void B_Tree<T>::freeNode(B_Tree_Node<T> *p_node)
{
    delete[] p_node->keys;
    delete[] p_node->children;
    delete p_node;
}

template <typename T>
int B_Tree<T>::findFirstNotSmaller(B_Tree_Node<T> *p_node, T a_key) const
{
    int i = 0;
    for (; i < p_node->size && p_node->keys[i] < a_key; i++)
        ;
    return i;
}

template <typename T>
T *B_Tree<T>::search(B_Tree_Node<T> *p_node, T key_to_search) const
{
    if (p_node == nullptr)
        return nullptr;

    int key_arr_size = p_node->size;
    int pos = findFirstNotSmaller(p_node, key_to_search);
    if (pos < key_arr_size && key_to_search == p_node->keys[pos])
        return &(p_node->keys[pos]);
    else
    {
        if (p_node->is_leaf)
            return nullptr;
        else
            return search(p_node->children[pos], key_to_search);
    }
}

template <typename T>
void B_Tree<T>::freeAll(B_Tree_Node<T> *root_node)
{
    if (!root_node->is_leaf)
    {
        for (int i = 0; i <= root_node->size; i++)
            freeAll(root_node->children[i]);
    }

    freeNode(root_node);
}

template <typename T>
void B_Tree<T>::display(B_Tree_Node<T> *p_node) const
{
    if (p_node == nullptr)
        return;

    if (p_node->is_leaf)
    {
        for (int i = 0; i < p_node->size; ++i)
        {
            int height = root->depth - p_node->depth;
            for (int j = 0; j < height; j++)
                cout << "------";
            cout << "|";
            cout << p_node->keys[i] << " " << endl;
        }
        return;
    }

    for (int i = 0; i <= p_node->size; ++i)
    {
        display(p_node->children[i]);
        if (i != p_node->size)
        {
            int height = root->depth - p_node->depth;
            for (int j = 0; j < height; j++)
                cout << "------";
            cout << "|";
            cout << p_node->keys[i] << " " << endl;
        }
    }

    // if (p_node->is_leaf)
    //     for (int i = 0; i < p_node->size; ++i)
    //         cout << p_node->keys[i] << " ";
    // else
    //     for (int i = 0; i <= p_node->size; ++i)
    //     {
    //         display(p_node->children[i]);
    //         if (i != p_node->size)
    //             cout << p_node->keys[i] << " ";
    //     }
}

template <typename T>
void B_Tree<T>::updateDepth(B_Tree_Node<T> *p_node)
{
    if (p_node == nullptr || p_node->is_leaf)
        return;

    int max_depth = -1;
    for (int i = 0; i <= p_node->size; i++)
    {
        if (p_node->children[i]->depth > max_depth)
            max_depth = p_node->children[i]->depth;
    }

    p_node->depth = max_depth + 1;
}

template <typename T>
T B_Tree<T>::getPred(B_Tree_Node<T> *p_node, int index) const
{
    B_Tree_Node<T> *cursor = p_node->children[index];
    while (!cursor->is_leaf)
        cursor = cursor->children[cursor->size];
    return cursor->keys[cursor->size - 1];
}

template <typename T>
T B_Tree<T>::getSucc(B_Tree_Node<T> *p_node, int index) const
{
    B_Tree_Node<T> *cursor = p_node->children[index + 1];
    while (!cursor->is_leaf)
        cursor = cursor->children[0];
    return cursor->keys[0];
}

template <typename T>
bool B_Tree<T>::insertToNode(B_Tree_Node<T> *p_node, T new_key)
{
    int pos = findFirstNotSmaller(p_node, new_key);
    if (new_key == p_node->keys[pos])
        return false; // repeated key is not allowed

    for (int i = p_node->size; i > pos; i--)
    {
        p_node->keys[i] = p_node->keys[i - 1];
        if (!p_node->is_leaf)
        {
            p_node->children[i + 1] = p_node->children[i];
        }
    }

    p_node->keys[pos] = new_key;
    p_node->size++;
    return true;
}

template <typename T>
void B_Tree<T>::splitChild(B_Tree_Node<T> *parent, int full_child_index)
{
    // cout << "split..." << endl;
    // cout << "min degree is: " << min_degree << endl;
    B_Tree_Node<T> *full_child = parent->children[full_child_index];

    // cout << "full child is: ";
    // for (int i = 0; i < 2 * min_degree - 1; i++)
    //     cout << full_child->keys[i] << " ";
    // cout << endl;

    // create a new node
    B_Tree_Node<T> *sibling = createEmptyNode();
    sibling->is_leaf = full_child->is_leaf;

    // copy node data
    for (int i = 0; i <= min_degree - 1; i++)
    {
        if (i < min_degree - 1)
        {
            sibling->keys[i] = full_child->keys[i + min_degree];
        }
        if (!full_child->is_leaf)
            sibling->children[i] = full_child->children[i + min_degree];
    }

    full_child->size = sibling->size = min_degree - 1;
    updateDepth(sibling);

    // insert middle key to parent
    insertToNode(parent, full_child->keys[min_degree - 1]);

    // connect sibling to parent
    parent->children[full_child_index + 1] = sibling;

    // cout << "full child becomes: ";
    // for (int i=0;i<full_child->size;i++)
    //     cout << full_child->keys[i] << " ";
    // cout << endl;

    // cout << "sibling becomes: ";
    // for (int i=0;i<sibling->size;i++)
    //     cout << sibling->keys[i] << " ";
    // cout << endl;

    // cout << "end split." << endl;
}

template <typename T>
void B_Tree<T>::mergeChildren(B_Tree_Node<T> *parent, int merge_index)
{
    B_Tree_Node<T> *left_child = parent->children[merge_index];
    B_Tree_Node<T> *right_child = parent->children[merge_index + 1];

    // drag down the middle key from parent
    left_child->keys[left_child->size] = parent->keys[merge_index];
    left_child->size++;

    // copy node data
    for (int i = 0; i <= right_child->size; i++)
    {
        if (i < right_child->size)
            left_child->keys[i + left_child->size] = right_child->keys[i];
        if (!left_child->is_leaf)
            left_child->children[i + left_child->size] = right_child->children[i];
    }
    left_child->size += right_child->size;

    // free right child
    freeNode(right_child);

    // update parent
    int i = merge_index;
    while (i < parent->size - 1)
    {
        parent->keys[i] = parent->keys[i + 1];
        if (!parent->is_leaf)
            parent->children[i + 1] = parent->children[i + 2];
        i++;
    }
    parent->size--;
}

template <typename T>
bool B_Tree<T>::removeFromLeaf(B_Tree_Node<T> *p_node, int remove_index)
{
    if (remove_index > p_node->size - 1)
        return false;

    int i = remove_index;
    while (i < p_node->size - 1)
    {
        p_node->keys[i] = p_node->keys[i + 1];
        i++;
    }

    p_node->size--;

    if (p_node->size == 0)
        freeNode(p_node);

    return true;
}

template <typename T>
bool B_Tree<T>::removeFromNonLeaf(B_Tree_Node<T> *&p_node, int remove_index) // reference or not???
{
    T remove_key = p_node->keys[remove_index];
    B_Tree_Node<T> *left_child = p_node->children[remove_index];
    B_Tree_Node<T> *right_child = p_node->children[remove_index + 1];

    if (left_child->size >= min_degree)
    {
        T pred = getPred(p_node, remove_index);
        p_node->keys[remove_index] = pred;
        return remove(p_node->children[remove_index], pred);
    }
    else if (right_child->size >= min_degree)
    {
        T succ = getSucc(p_node, remove_index);
        p_node->keys[remove_index] = succ;
        return remove(p_node->children[remove_index + 1], succ);
    }
    else
    {
        mergeChildren(p_node, remove_index);
        if (p_node->size == 0)
        {
            B_Tree_Node<T> *temp = p_node;
            p_node = p_node->children[0];
            freeNode(temp);
        }
        return remove(p_node, remove_key);
    }
}

template <typename T>
void B_Tree<T>::borrowFromLeft(B_Tree_Node<T> *parent, int borrow_child_index)
{
    B_Tree_Node<T> *poor_child = parent->children[borrow_child_index];
    B_Tree_Node<T> *left_sibling = parent->children[borrow_child_index - 1];

    // shift poor child
    for (int i = poor_child->size; i >= 0; i--)
    {
        if (i > 0)
            poor_child->keys[i] = poor_child->keys[i - 1];
        if (!poor_child->is_leaf)
            poor_child->children[i + 1] = poor_child->children[i];
    }

    // poor child drag key from parent
    poor_child->keys[0] = parent->keys[borrow_child_index - 1];

    // poor child drag child from sibling
    if (!poor_child->is_leaf)
        poor_child->children[0] = left_sibling->children[left_sibling->size];

    poor_child->size++;

    // parent drag from sibling
    parent->keys[borrow_child_index - 1] = left_sibling->keys[left_sibling->size - 1];
    left_sibling->size--;
}

template <typename T>
void B_Tree<T>::borrowFromRight(B_Tree_Node<T> *parent, int borrow_child_index)
{
    B_Tree_Node<T> *poor_child = parent->children[borrow_child_index];
    B_Tree_Node<T> *right_sibling = parent->children[borrow_child_index + 1];

    // poor child drag key from parent
    poor_child->keys[poor_child->size] = parent->keys[borrow_child_index];
    poor_child->size++;

    // poor child drag child from sibling
    if (!poor_child->is_leaf)
        poor_child->children[poor_child->size] = right_sibling->children[0];

    // parent drag key from sibling
    parent->keys[borrow_child_index] = right_sibling->keys[0];

    // shift sibling
    for (int i = 0; i <= right_sibling->size - 1; i++)
    {
        if (i <= right_sibling->size - 2)
            right_sibling->keys[i] = right_sibling->keys[i + 1];
        if (!right_sibling->is_leaf)
            right_sibling->children[i] = right_sibling->children[i + 1];
    }
    right_sibling->size--;
}

template <typename T>
void B_Tree<T>::fillChild(B_Tree_Node<T> *parent, int fill_child_index)
{
    if (fill_child_index > 0 && parent->children[fill_child_index - 1]->size >= min_degree)
        borrowFromLeft(parent, fill_child_index);
    else if (fill_child_index < parent->size && parent->children[fill_child_index + 1]->size >= min_degree)
        borrowFromRight(parent, fill_child_index);
    else
    {
        if (fill_child_index != parent->size)
            mergeChildren(parent, fill_child_index);
        else
            mergeChildren(parent, fill_child_index - 1);
    }
}

template <typename T>
bool B_Tree<T>::remove(B_Tree_Node<T> *&p_node, T remove_key) // when to update depth???
{
    bool ret;
    if (p_node->size == 0)
        p_node = p_node->children[0];
    // B_Tree_Node<T> *cursor = p_node;
    int pos = findFirstNotSmaller(p_node, remove_key);
    if (pos < p_node->size && remove_key == p_node->keys[pos])
    {
        if (p_node->is_leaf)
            ret = removeFromLeaf(p_node, pos);
        else
            ret = removeFromNonLeaf(p_node, pos);
    }
    else
    {
        if (p_node->is_leaf)
            return false;

        bool pos_at_end = pos == p_node->size;
        if (p_node->children[pos]->size < min_degree)
            fillChild(p_node, pos);
        if (pos_at_end && pos > p_node->size)
            ret = remove(p_node->children[pos - 1], remove_key);
        else
            ret = remove(p_node->children[pos], remove_key);
    }

    updateDepth(p_node);
    return ret;
}

template <typename T>
bool B_Tree<T>::insertNonFull_recursively(B_Tree_Node<T> *p_node, T insert_key)
{
    if (p_node->is_leaf)
        return insertToNode(p_node, insert_key);

    // cout << "point 1" << endl;
    int i = findFirstNotSmaller(p_node, insert_key);
    if (i < p_node->size && insert_key == p_node->keys[i])
        return false;

    B_Tree_Node<T> *target_child = p_node->children[i];

    // cout << "point 2" << endl;

    if (target_child->size == 2 * min_degree - 1)
    {
        splitChild(p_node, i);
        if (insert_key == p_node->keys[i])
            return false;
        if (insert_key > p_node->keys[i])
            target_child = p_node->children[i + 1];
    }

    // cout << "point 3" << endl;

    return insertNonFull_recursively(target_child, insert_key);
}
