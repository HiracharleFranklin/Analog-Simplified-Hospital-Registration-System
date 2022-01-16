#pragma once
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::string;

//Declaration of the struct node
template <class T>
struct node
{
    T n; // The value of the node
    T ID_recorder;
    person p;
    int degree; // Number of the child
    node<T> *parent;
    node<T> *child;
    node<T> *left;
    node<T> *right;
    char mark;  //Use 'F' or 'T' to show if the node is marked
    char check; //Use 'Y' or 'N' to show if the node is checked when we try to find a particular node
};

template <class T>
class FibHeap
{
public:

    //Constructor
    //After initializing, H is NULL.
    FibHeap()
    {
        H = Initialize();
        numnode = 0;
    }
    //Number of nodes in the Fibonacci heap
    int numnode;
    //The pointer to the minimun element in the root list
    node<T> *H;
    //The following part are the member functions

    //Initialize the heap
    node<T> *Initialize()
    {
        node<T> *Nullptr;
        Nullptr = NULL;
        return Nullptr;
    }
    //Create a new node and allocate space for it
    //input is the value of the new node
    node<T> *New_node(T value)
    {
        node<T> *N = new node<T>;
        N->n = value;
        return N;
    }
    //Insert a new node with an intended value into the root list of fibonacci heap
    //The return value is the min node of the root list
    node<T> *Insert_val(T value)
    {
        node<T> *N = New_node(value);
        N->degree = 0;    //It has zero child
        N->parent = NULL; //It has no parent
        N->child = NULL;  //It has no child
        N->left = N;
        N->right = N;
        N->mark = 'F';  //It is not marked
        N->check = 'N'; //It has not been checked(for Find function)
        if (H != NULL)
        {
            //Add n to the root list of heap
            (H->left)->right = N;
            N->left = H->left;
            N->right = H;
            H->left = N;
            if (N->n < H->n) //Check whther the key of the new node is smaller than the min of the heap
                H = N;
        }
        else
        {
            H = N;
        }
        //Number of nodes increases by 1 after insertion.
        numnode++;
        return H;
    }
    //Insert a new node with a pointer to node into the fibonacci heap
    node<T> *Insert_ptr(node<T>* N)
    {
        N->degree = 0;    //It has zero child
        N->parent = NULL; //It has no parent
        N->child = NULL;  //It has no child
        N->left = N;
        N->right = N;
        N->mark = 'F';  //It is not marked
        N->check = 'N'; //It has not been checked(for Find function)
        if (H != NULL)
        {
            //Add n to the root list of heap
            (H->left)->right = N;
            N->left = H->left;
            N->right = H;
            H->left = N;
            if (N->n < H->n) //Check whther the key of the new node is smaller than the min of the heap
                H = N;
        }
        else
        {
            H = N;
        }
        //Number of nodes increases by 1 after insertion.
        numnode++;
        return H;
    }
    //Link two nodes into the relationship of parent and child
    //(Make one node(node<T>* y) the parent of the other node(node<T>* x))
    //These two nodes are both roots
    //No return value
    void Fibonacci_link(node<T> *x, node<T> *y)
    {
        //Following two lines delete the node x from the root list
        (x->left)->right = x->right;
        (x->right)->left = x->left;
        //If there is only one node in the root list, than H points to y
        if (y->right == y)
        {
            H = y;
        }
        //Make x the child of y
        x->left = x;
        x->right = x;
        x->parent = y;
        if (y->child == NULL)
            y->child = x;
        //Add x to the child list of y
        //Plud x between the direct child of x and the left node of direct child
        x->right = y->child;
        x->left = (y->child)->left;
        ((y->child)->left)->right = x;
        (y->child)->left = x;
        //If x is smaller than the direct child, than make x as the direct child
        if (x->n < (y->child)->n)
            y->child = x;
        //The number of child of y increases by 1
        y->degree++;
    }

    //Merge two fibonacci heaps together
    //User must adopt the total number of node after calling this fucntion
    //Another way is compare the min node of the two heaps and call the Union function in the heap with smaller min
    node<T> *Union(node<T> *H1, node<T> *H2)
    {
        node<T> *ptr;
        node<T> *H_new = Initialize(); //H_new is NULL pointer now

        (H1->left)->right = H2;
        (H2->left)->right = H1;
        ptr = H1->left;
        H1->left = H2->left;
        H2->left = ptr;
        H_new = H1;
        if (H1->n > H2->n)
            H_new = H2;
        return H_new;
    }
    //Remove and return a the smallest element in the fibonacci heap
    //The return value is the pointer to the deleted min node
    node<T> *Extract_Min(void)
    {
        node<T> *ret = H;
        if (H == NULL)
        {
            cout << "The heap is empty\n";
            return NULL;
        }
        else if (numnode == 1)
        {
            numnode--;
            H = NULL;
            //cout << "The node with key:" << min << " is deleted\n";
            cout << "There are " << numnode << " person left in the Fib_heap\n";
            return ret;
        }
        node<T> *ptr1 = H;
        node<T> *ptr2 = H;
        node<T> *ptr3 = NULL;

        if (ptr1->child != NULL)
        {
            ptr3 = ptr1->child; //ptr3 points to the child of H
            do
            {
                ptr2 = ptr3->right;
                (H->left)->right = ptr3;
                ptr3->right = H;
                ptr3->left = H->left;
                H->left = ptr3;
                ptr3->parent = NULL;
                ptr3 = ptr2;
            } while (ptr3 != ptr1->child);
        }
        (H->left)->right = H->right; //Cut the minimal node from the root list
        (H->right)->left = H->left;
        int yn = (ptr1 == ptr1->right && ptr1->child == NULL);
        if (yn != 1)
        {
            H = H->right;
            Consolidate();
        }
        numnode--;
        //cout << "The node with key:" << min << " is deleted\n";
        cout << "There are " << numnode << " person left in the Fib_heap\n";
        return ret;
    }
    //Consolidate the nodes with same degree
    //Use a list arr[j] to store the nodes with same degree and combine them
    //Finally restore them to the root list of the fibonacci heap
    void Consolidate()
    {
        int num1;
        float num2 = (log(numnode)) / (log(2));
        int num3 = num2 + 2; //num3 is the max number of degrees
        int temp1 = 0;
        int temp2 = 0;
        node<T> *arr[numnode]; //Creat an  array with enought space to store the nodes with different degrees
        for (int i = 0; i < num3; i++)
            arr[i] = NULL;
        node<T> *ptr1 = H;
        node<T> *ptr2;
        node<T> *ptr3;
        node<T> *ptr4;
        do
        {
            temp1++;            //Number of node in the root list
            ptr1 = ptr1->right; //Count the number of nodes in the root list
        } while (ptr1 != H);
        do
        {
            ptr4 = ptr1->right;
            num1 = ptr1->degree; //Record the number of degree from the minimal node
            while (arr[num1] != NULL)
            {
                ptr2 = arr[num1];
                if (ptr1->n > ptr2->n)
                {
                    ptr3 = ptr1; //If the key value of the ptr1 is larger than ptr2, than swap these two nodes
                    ptr1 = ptr2;
                    ptr2 = ptr3;
                }
                if (ptr2 == H)
                    H = ptr1;
                Fibonacci_link(ptr2, ptr1); //Make ptr1 the parent of ptr2
                temp1--;                    //Number of node in the root list is decreased by 1
                if (ptr1->right == ptr1)
                    H = ptr1;
                arr[num1] = NULL;
                temp2--;
                num1++;
            }
            arr[num1] = ptr1;
            temp2++;
            ptr1 = ptr4;
        } while (temp1 != temp2);
        H = NULL;
        for (int j = 0; j < num3; j++)
        { //Add the nodes in the arr to the root list
            if (arr[j] != NULL)
            {
                arr[j]->left = arr[j]; //Cut the arr[j] from the previous list
                arr[j]->right = arr[j];
                if (H != NULL)
                { //When the root is not empty, add the new node arr[j] next to H.
                    (H->left)->right = arr[j];
                    arr[j]->right = H;
                    arr[j]->left = H->left;
                    H->left = arr[j];
                    if (arr[j]->n < H->n)
                        H = arr[j];
                }
                else
                    H = arr[j];
            }
        }
    }
    //Find the node with the target value
    //If the node with the target value is found, then return the pointer to the node
    //If the node is not found, return NULL
    node<T> *Find(node<T> *H1, T target)
    {
        H1->check = 'Y'; //Start from the first element in the heap
        node<T> *ptr1 = NULL;
        node<T> *ptr2 = NULL; //this pointer is used for storing the return value
        if (H1->n == target)
        {
            H1->check = 'N'; //If the checked node has same value as target, then return the pointer and unmark it.
            return H1;
        }
        //Check other roots and its child recursively
        else
        {
            if (H1->child != NULL)
            { //check its child recursively
                ptr1 = Find(H1->child, target);
            }
            if ((H1->right)->check == 'N')
            { //check the nodes in the same level recursively
                ptr2 = Find(H1->right, target);
            }
        }
        H1->check = 'N';
        if (ptr1 != NULL)
            return ptr1;
        else if (ptr2 != NULL)
            return ptr2;
        else
            return NULL;
    }
    //Use the ID to find the target person
    node<T> *Find_ID(node<T> *H1, T target)
    {
        H1->check = 'Y'; //Start from the first element in the heap
        node<T> *ptr1 = NULL;
        node<T> *ptr2 = NULL; //this pointer is used for storing the return value
        if (H1->ID_recorder == target)
        {
            H1->check = 'N'; //If the checked node has same value as target, then return the pointer and unmark it.
            return H1;
        }
        //Check other roots and its child recursively
        else
        {
            if (H1->child != NULL)
            { //check its child recursively
                ptr1 = Find_ID(H1->child, target);
            }
            if ((H1->right)->check == 'N')
            { //check the nodes in the same level recursively
                ptr2 = Find_ID(H1->right, target);
            }
        }
        H1->check = 'N';
        if (ptr1 != NULL)
            return ptr1;
        else if (ptr2 != NULL)
            return ptr2;
        else
            return NULL;
    }
    //Decrease the key value of the node
    void Decrease_key(T old_val, T new_val)
    {
        node<T> *z;
        if (H == NULL)
        {
            cout << "The heap is empty\n";
            return;
        }
        node<T> *p = Find(H, old_val);
        if (p == NULL)
        {
            cout << "There is no node with old_val\n";
            return;
        }
        if (p->n < new_val)
        {
            cout << "New value is greater than current value. Do you want to increse key?\n";
            return;
        }
        p->n = new_val;
        z = p->parent;
        if (z != NULL && p->n < z->n)
        {
            Cut(p, z); //Use cascade cut to maintain the structure of the fibonacci heap
            Cascade_cut(z);
        }
        if (p->n < H->n)
        {
            H = p;
        }
    }
    //Increase the key of a certain node
    void Increase_key(T old_val, T new_val)
    {
        if (H == NULL)
        {
            cout << "The heap is empty\n";
            return;
        }
        node<T> *p = Find(H, old_val);
        if (p == NULL)
        {
            cout << "There is no node with old_val\n";
            return;
        }
        if (p->n > new_val)
        {
            cout << "New value is smaller than current value. Do you want to decrease key?\n";
            return;
        }
        Decrease_key(old_val, -922337203685477580); //Decerase the key to minus infinity
        Extract_Min();                              //Extract the node
        Insert(new_val);                            //Insert a new node with the intended value
    }
    //Delete the node with the given key value
    void Delete_key(T val)
    {
        if (H == NULL)
        {
            cout << "The heap is empty\n";
        }
        Decrease_key(val, -922337203685477580); //Decerase the key to minus infinity
        Extract_Min();                          //Extract the node
    }
    //Cut a certain node from the fibonacci heap
    //x is the parent node of y
    void Cut(node<T> *x, node<T> *y)
    {
        if (x == x->right)
            y->child = NULL;
        (x->left)->right = x->right;
        (x->right)->left = x->left;
        if (x == y->child)
            y->child = x->right;
        y->degree = y->degree - 1;
        x->right = x;
        x->left = x;
        (H->left)->right = x;
        x->right = H;
        x->left = H->left;
        H->left = x;
        x->parent = NULL;
        x->mark = 'F';
    }
    //If the cutted node 
    void Cascade_cut(node<T> *y)
    {
        node<T> *z = y->parent;
        if (z != NULL)
        {
            if (y->mark == 'F')
                y->mark = 'T';
            else
            {
                Cut(y, z);
                Cascade_cut(z);
            }
        }
    }



    //Display all the ID of the patients in the Fibonacci heap in the format of Fibonacci heap
    void Display()
    {
        node<T> *ptr = H;
        if (ptr == NULL)
        {
            cout << "The Heap is Empty" << endl;
            cout << "The heap has " << numnode << " nodes" << endl
                << endl;
            return;
        }
        node<T>*temp = Extract_Min();
        if(temp != NULL)Insert_ptr(temp);
        visual_tree(H, "");
        cout << endl
            << "The heap has " << numnode << " nodes" << endl
            << endl;
    }
    
    void visual_tree(node<T> *n, string previous)
    {
        string s = "|  ";
        node<T> *x = n;
        do
        {
            if (x->right != n)
            {
                cout << previous << "|__";
            }
            else
            {
                cout << previous << "|__";
                s = "    ";
            }
            if (x->child == nullptr)
            {
                cout << "_" << x->ID_recorder << endl;
            }
            else
            {
                cout << "_" << x->ID_recorder << endl;
                visual_tree(x->child, previous + s);
            }
            x = x->right;
        } while (x != n);
    }



    //This function will display the information of all the waiting patients
    //!!Only used for weekly report
    void Display_all(int half_day, string filename){
    ofstream outfile;
    outfile.open(filename,ios::app);
    
    node<T> *ptr = H;
    if (ptr == NULL)
    {
        outfile << "The Heap is Empty" << endl;
        outfile << "The heap has " << numnode << " nodes" << endl
                << endl;
        return;
    }
    file_visual_heap(H, half_day,filename);
    
    outfile << endl
            << "The heap has " << numnode << " nodes" << endl
            << endl;
    
    outfile.close();
    }

    void visual_heap(node<T>* n, int half_day){
        node<T> *x = n;
        do
        {
            if (x->child == nullptr)
            {
                print_person_s1mple(x->p);
                cout<<"Waiting time: "<<half_day - x->p.regist_halfday<<" halfday(s)." <<endl;
            }
            else
            {
                print_person_s1mple(x->p);
                cout<<"Waiting time: "<<half_day - x->p.regist_halfday<<" halfday(s)." << endl;
                visual_heap(x->child,half_day);
            }
            x = x->right;
        } while (x != n);

    }

    void file_visual_heap(node<T>* n, int half_day, string filename){
        ofstream outfile;
        outfile.open(filename,ios::app); 
        node<T> *x = n;
        do
        {
            if (x->child == nullptr)
            {
                file_print_person_s1mple(x->p, filename);
                outfile<<"Waiting time: "<<half_day - x->p.regist_halfday<<" halfday(s)." <<endl;
            }
            else
            {
                file_print_person_s1mple(x->p, filename);
                outfile<<"Waiting time: "<<half_day - x->p.regist_halfday<<" halfday(s)." << endl;
                file_visual_heap(x->child,half_day, filename);
                
            }
            x = x->right;
        } while (x != n);
        outfile.close();
    }



    
};
