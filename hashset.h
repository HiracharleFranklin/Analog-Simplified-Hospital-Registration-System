//
//  hashset.h
//  
//
//  Created by KD on 6.03.21.
//
#pragma once
#ifndef hashset_h
#define hashset_h



template<class T> class hashset
{
public:
    hashset(int size = 20);
//    virtual ~hashset();
    T &operator[](int index);
// The operation add inserts an element into the hashtable, remove deletes an
// element and member checks, if the element is in the hashtable
    void add(T item);
    void add_risk_status(T item);
    void remove(T item);
    bool member(T item);
// The function display is only used for testing purposes. It shows the size of
// the hashtable, the number of elements, and displays all entries
    void display(void);
//Return a pointer to a vector which stores all the ids wiht the same risk
    vector<int>*output_same_risk(int);
    vector<int>*output_same_risk_file(int);
//Store the ids of different risks(The size is designed for the specific relation MEDICAL_STATUS)
    int storage[10][3000]; 
    struct status_index{
        int status;
        int index;  //index of the vector with ids in it(in storage)
    };
//arr for storing the different MEDICAL_STATUS and the index of the data in the B+ tree
    status_index arr[1000];



    int numitems;           // contains the number of elements in the hashtable
    int **reprarray;
/* reprarray defines a pointer to (an array of) pointers to values of type T. We will use a null pointer, if an entry is undefined, and a fixed pointer as placeholder for free entries (after remove) */
    int *pt_nil;              // used as placeholder TTTT
    int maxsize;            // defines the size of the hashtable
    void rehash(int newsize);
// rehash is an internal function to enlarge or shrink the hashtable,
// if the fill factors 0.75 or 0.25 are exceeded or undershot, resp.
};

#endif /* hashset_h */
