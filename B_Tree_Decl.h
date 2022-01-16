#include <iostream>
using namespace std;

template <typename T>
class B_Tree_Node
{
public:
    bool is_leaf;
    int size;
    int depth;

    T *keys;
    B_Tree_Node<T> **children;
};

template <typename T>
class B_Tree
{
public:
    B_Tree(int x = 3);
    ~B_Tree();

    bool contain(T key) const;
    T *getHandle(T key) const;
    void display() const;
    bool insert(T key);
    bool remove(T key);

private:
    void printNodeInfo(B_Tree_Node<T> *p_node) const;
    B_Tree_Node<T> *createEmptyNode();
    void freeNode(B_Tree_Node<T> *p_node);
    int findFirstNotSmaller(B_Tree_Node<T> *p_node, T a_key) const;
    T *search(B_Tree_Node<T> *p_node, T key_to_search) const;
    void freeAll(B_Tree_Node<T> *p_node);
    void display(B_Tree_Node<T> *p_node) const;
    void updateDepth(B_Tree_Node<T> *p_node);
    T getPred(B_Tree_Node<T> *p_node, int index) const;
    T getSucc(B_Tree_Node<T> *p_node, int index) const;
    bool insertToNode(B_Tree_Node<T> *p_node, T new_key);
    void splitChild(B_Tree_Node<T> *parent, int full_child_index);
    void mergeChildren(B_Tree_Node<T> *parent, int merge_index);
    bool removeFromLeaf(B_Tree_Node<T> *p_node, int remove_index);
    bool insertNonFull_recursively(B_Tree_Node<T> *p_node, T insert_key);
    // when the corresponding child has less than min_degree keys, try to fill the child with more key.
    void fillChild(B_Tree_Node<T> *parent, int fill_child_index);
    bool removeFromNonLeaf(B_Tree_Node<T> *&p_node, int remove_index);
    bool remove(B_Tree_Node<T> *&p_node, T remove_key);
    void borrowFromLeft(B_Tree_Node<T>* parent, int borrow_child_index);
    void borrowFromRight(B_Tree_Node<T>* parent, int borrow_child_index);

private:
    int min_degree;
    int key_num;
    B_Tree_Node<T> *root;
};