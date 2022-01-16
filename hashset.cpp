//
//  hashset.cpp
//  
//
//  Created by KD on 6.03.21.
//
#pragma once
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <vector>
using namespace std;
#include "person.h"
#include "hashset.h"


/* This defines a constructor */
template<class T> hashset<T>::hashset(int size)
{
    maxsize = size;
    if (size < 20)
        maxsize = 20; // default size is 20; no smaller hashtables are considered
    numitems = 0; // initially, the hashtable is empty
    reprarray = new int*[maxsize];  // allocate space for the array of pointers
    // the for loop initialises all table entries to be undefined
    for(int i=0; i<10; i++){
        for(int j=0; j< 3000; j++){
            storage[i][j] = -1;
        }
    }
    for(int i=0; i<1000;i++){
        arr[i].status = -1;
    }
    
    for (int i = 0; i < maxsize; i++)
    {
        reprarray[i] = 0;
    }
    int *pt0 = new int;  // This defines the fixed placeholder pointer
    *pt0 = 0;
    pt_nil = pt0;
}

// template<class T> hashset<T>::~hashset(void)
// {
//     delete[] reprarray;
// }

template<class T> T & hashset<T>::operator[](int index)
{
    if (index <= 0 || index > maxsize)
    {
        cout << "Index error: index out of range\n";
        exit(EXIT_FAILURE);
    }
    if (reprarray[index - 1] != 0 && reprarray[index - 1] != pt_nil)
        return *reprarray[index - 1];
    else
    {
        cout << "Index not used in hashtable";
        return 0;
    }
}

template<class T> void hashset<T>::add(T item)
{
    //hash<T> hashfunction; // use the predefined hashfunction to get "key" values
    int index;
    //index = hashfunction(item) % maxsize; // First determine the position index in the hash table, where the new value is stored, if free.
    index = item % maxsize;
    int location = -1;  // used to distinguish between undefined entries (null pointer) and placeholders
    while (reprarray[index] != 0) // We first check, if the item is already in the hashtable
    {
        if (reprarray[index] != pt_nil && *reprarray[index] == item)
            return;   // item found; no insertion
        if (location < 0 && reprarray[index] == pt_nil) // a placeholder object is found; i.e. if the item is not in the hashtable, this will be the place for the insertion
            location = index;
        index = (index + 1) % maxsize;
    }
    // after leaving the while loop we either have location < 1, i.e. we store the item at the last examined index (which contains a null pointer),
    // otherwise, if location >= 0 holds, we found a placeholder, so the item will be stored at the location of this placeholder
    if (location < 0)
        location = index;
    T *pt = new T;
    *pt = item;
    reprarray[location] = pt;   // store item in the hashtable
    ++ numitems;
    int load = 100 * numitems / maxsize;
    if (load >= 75)             // max load factor is exceeded; double the size
    {
        int newsize = 2 * numitems;
        rehash(newsize);
    }
    return;
}
//New function added for the programming assignment 2
template<class T> void hashset<T>::add_risk_status(T item)
{
    //hash<T> hashfunction; // use the predefined hashfunction to get "key" values
    int index;

    //Find the place by using the secondary key
    int ID = (int)item.id;
    int STATUS = (int)item.risk;
    index = STATUS % maxsize; // First determine the position index in the hash table, where the new value is stored, if free.
    int location = -1;  // used to distinguish between undefined entries (null pointer) and placeholders
    // for(int i = 0; i < numitems ; i++){
    //     cout<<*reprarray[i];
    // }
    // cout<<"\n";
    //There exist repeated elements with the same id
    if (reprarray[index] != NULL &&*reprarray[index] == STATUS){
        for(int i = 0; i < numitems; i++){
            if(arr[i].status == STATUS ){
                for(int j=0; j <3000;j++){
                    if(storage[arr[i].index][j] == -1){      //If the storage is empty, store the person ID
                        storage[arr[i].index][j] = ID;
                        return;
                    }
                }
            }
        }
        cout<<"Can not find the item in the storage with its appearance before";
        return;
    }
    // after leaving the while loop we either have location < 1, i.e. we store the item at the last examined index (which contains a null pointer),
    // otherwise, if location >= 0 holds, we found a placeholder, so the item will be stored at the location of this placeholder
    if (location < 0)
        location = index;

    int *pt = new int;
    *pt = STATUS;
    reprarray[location] = pt;   // store item in the hashtable
    arr[numitems].status = STATUS;  //New type of MEDICAL_STATUS and store it in a new index of arr
    arr[numitems].index = numitems;
    storage[numitems][0] = ID;  //Add the new id for the same risk status array(storage[numitems] stores all the person with same risk status and now store the first oen of them)
    ++ numitems;
    int load = 100 * numitems / maxsize;
    if (load >= 75)             // max load factor is exceeded; double the size
    {
        int newsize = 2 * numitems;
        rehash(newsize);
    }
    return;
}

template<class T> void hashset<T>::remove(T item)
{
    //hash<T> hashfunction;  // use again the predefined hashfunction
    int index;
    index = item % maxsize;
    //index = hashfunction(item) % maxsize;
    while (reprarray[index] != 0)  // same as for add we search for item in the hashtable; the search starts at the designated hash value, and stops when we find a null pointer
    {
        if (reprarray[index] != pt_nil && *reprarray[index] == item)
                    // item found
        {
            int nextindex = (index + 1) % maxsize;
            /* check the next entry, if it is a null pointer; if yes, we can overwrite item by a null pointer; otherwise we use a placeholder, i.e. the pointer pt_nil */
            if (reprarray[nextindex] == 0)
                reprarray[index] = 0;
            else
                reprarray[index] = pt_nil;
            --numitems;
            int load = 100 * numitems / maxsize;
            if (load <= 25)
                // if the min load factor is undershot, we shrink the hashtable, but the size shall always be >= 20
            {
                int newsize = numitems;
                if (newsize < 20)
                    newsize = 20;
                rehash(newsize);
            }
            return;
        }
        index = (index + 1) % maxsize;
    }
    cout << item << " is not in the hashtable.\n";
    return;
}

template<class T> bool hashset<T>::member(T item)
{
    // hash<T> hashfunction;  // use the "key" function for the type T (if defined)
    // int index;
    // index = hashfunction(item) % maxsize;
    int index;
    index = item % maxsize;
    while (reprarray[index] != 0) // again we search for item starting from the index position until we find a null pointer
    {
        if (reprarray[index] != pt_nil && *reprarray[index] == item)
            return true;  // item was found
        index = (index + 1) % maxsize;
    }
    return false;  // a null pointer was found, so item is not in the hashtable
}

template<class T> void hashset<T>::rehash(int newsize)
{
    if (newsize < 20)
        newsize = 20;
    int newnum = 0;
    int **newarray = new int*[newsize];   // allocate space for a new hashtable of the given new size
    for (int i = 0; i < newsize; i++)
    {
        newarray[i] = 0;   // initialise the new hashtable with only null pointers
    }
    for (int i = 0; i < maxsize; i++)  // we need to copy all existing entries to the new hash table
    {
        if (reprarray[i] != 0 && reprarray[i] != pt_nil)
        {
            int item = *reprarray[i];
            //hash<T> hashfunction;
            //int index = hashfunction(item) % newsize;
            int index = item % newsize;
                // recompute the new hash value
            while (newarray[index] != 0)  // find the first free place, where the entry can be stored in the new hashtable
            {
                index = (index + 1) % newsize;
            }
            newarray[index] = reprarray[i];  // do the actual copying
            ++ newnum;
        }
    }
    numitems = newnum;   // change the number of stored elements
    reprarray = newarray;   // make the new hashtable the one of the hashset object
    maxsize = newsize;   // update the size
}

template<class T> void hashset<T>::display(void) // only for test purposes
{
    cout << "The size of the hashtable is: " << maxsize << "\n";
    cout << "The number of elements in the hashtable is: " << numitems << "\n";
    for (int i = 0; i < maxsize; ++i)
    {
        if (reprarray[i] == 0)
            cout << "The entry " << i + 1 << " is undefined.\n";
        else if (reprarray[i] == pt_nil)
            cout << "The entry " << i + 1 << " is free.\n";
        else
            cout << "The entry " << i + 1 << " is " << *reprarray[i] << " .\n";
    }
    return;
}

template<class T> vector<int>* hashset<T>::output_same_risk(int status) // only for test purposes
{
    int find = -1;
    int place = -1;
    vector<int>* res = new vector<int>;
    for(int i=0; i<numitems; i++){
        if(arr[i].status == status){
            find = 1;
            place = i;
            break;
        }
    }
    if(find == -1){
        cout<<"Can not find people with risk status:"<<status<<"\n"<<endl;
        return NULL;

    }  
    int counter = 0;
    if(place == -1){
        cout<<"Invalid place"<<endl;
        return NULL;
    }
    cout<<"ID:"<<endl;
    for(int i=0; storage[arr[place].index][i] != -1;i++){
        cout<<" "<<storage[arr[place].index][i];
        counter += 1;
        (*res).push_back(storage[arr[place].index][i]);
    }
    cout<<"\n";
    return res;
}

template<class T> vector<int>* hashset<T>::output_same_risk_file(int status) // only for test purposes
{
    int find = -1;
    int place = -1;
    vector<int>* res = new vector<int>;
    for(int i=0; i<numitems; i++){
        if(arr[i].status == status){
            find = 1;
            place = i;
            break;
        }
    }
    if(find == -1){
        cout<<"Can not find people with risk status:"<<status<<"\n"<<endl;
        return NULL;

    }  
    if(place == -1){
        cout<<"Invalid place"<<endl;
        return NULL;
    }
    // cout<<"ID:"<<endl;
     for(int i=0; storage[arr[place].index][i] != -1;i++){
    //     cout<<" "<<storage[arr[place].index][i];
    //     counter += 1;
         (*res).push_back(storage[arr[place].index][i]);
     }
    // cout<<"\n";
    return res;
}
