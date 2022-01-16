//
//  alist.cpp
//  
//
//  Created by KD on 14.01.21.
//
#pragma once
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include "alist.h"
using std::cout;
using std::cin;

template<class T> T &AList<T>::operator[](int index)
{
    return reprarray[index - 1];
}

template<class T> int AList<T>::getlength(void)
{
    return numitems;
}

template<class T> void AList<T>::setitem(int index, T value)
{
    if ((index > 0) && (index <= numitems))
    {
        reprarray[index - 1] = value;
        return;
    }
    else
        cout << "Index error: index out or range\n";
}

template<class T> T AList<T>::getitem(int index)
{
    if ((index > 0) && (index <= numitems))
    {
        return reprarray[index - 1];
    }
    else
    {
        cout << "Index error: index out or range\n";
        exit(EXIT_FAILURE);
    }
}

template<class T> void AList<T>::append(T value)
{
    if (numitems == maxsize)
        allocate();
    reprarray[numitems] = value;
    ++numitems;
    return;
}

template<class T> void AList<T>::insert(int index, T value)
{
    if (numitems == maxsize)
        allocate();
    if (index < numitems)
    {
        for (int j = numitems - 1; j >= index; --j)
        {
            reprarray[j+1] = reprarray[j];
        }
        reprarray[index] = value;
        ++numitems;
        return;
    }
    else
    {
        append(value);
        return;
    }
}

template<class T> void AList<T>::allocate(void)
{
    int newsize = 2 * maxsize;
    T *newarray = new T[newsize];
    for (int i = 0; i < numitems; ++i)
    {
        newarray[i] = reprarray[i];
    }
    delete[] reprarray;
    reprarray = newarray;
    maxsize = newsize;
    return;
}

template<class T> void AList<T>::remove(int index)
{
    if ((numitems == maxsize / 4) && (maxsize > minsize))
        deallocate();
    if (index <= numitems)
    {
        for (int j = index; j < numitems; j++)
        {
            reprarray[j-1] = reprarray[j];
        }
        --numitems;
        return;
    }
    else
        cout << "Index error: list index out of range\n";
}

template<class T> void AList<T>::deallocate(void)
{
    int newsize = maxsize / 2;
    T *newarray = new T[newsize];
    for (int i = 0; i < numitems; ++i)
    {
        newarray[i] = reprarray[i];
    }
    delete[] reprarray;
    reprarray = newarray;
    maxsize = newsize;
    return;
}

template<class T> void AList<T>::concat(AList<T> &list)
{
    int length = list.getlength();
    for (int i = 1; i <= length; ++i)
    {
        append(list[i]);
    }
    return;
}

template<class T> bool AList<T>::member(T value)
{
    bool result = false;
    for (int i = 0; i < numitems; ++i)
    {
        if (reprarray[i] == value)
        {
            result = true;
            break;
        }
    }
    return result;
}

template<class T> bool AList<T>::equal(AList<T> &list)
{
    bool result = true;
    if (numitems == list.getlength())
    {
        for (int i = 0; i < numitems; ++i)
        {
            if (reprarray[i] == list[i + 1])
                continue;
            else
            {
                result = false;
                break;
            }
        }
    }
    else
        result = false;
    return result;
}

template<class T> bool AList<T>::sublist(AList<T> &list)
{
    int length = list.getlength();
    bool result = true;
    for (int i = 1, j = 0; (i <= length) && (j < numitems); ++i, ++j)
    {
        if (list[i] == reprarray[j])
        {
            if ((j == numitems - 1) && (i < length))
            {
                result = false;
                break;
            }
        }
        else
            if (j == numitems - 1)
            {
                result = false;
                break;
            }
            else
                --i;
    }
    return result;
}

template<class T> AList<T>::AList(int size)
{
    maxsize = size;
    if (size < 20)
        minsize = size;
    else
        minsize = 20;
    numitems = 0;
    reprarray = new T[maxsize];
}

// Helper function for the programming assignment
template<class T> void AList<T>::report(int halfday, string filename)
{
    ofstream outfile;
    outfile.open(filename,ios::app);
    outfile << "The people who have been treated during this week.\n" ;
    outfile.close();
    for (int i = 0; i < numitems; i++)
    {
        if ((*(getitem(i+1))).treat_halfday > halfday - 14 && (*(getitem(i+1))).treat_halfday <= halfday)
        {
            file_print_person_s1mple(*(getitem(i+1)), filename);
            outfile.open(filename,ios::app);
            outfile << "Waiting time: " << (*(getitem(i+1))).treat_halfday - (*(getitem(i+1))).regist_halfday << " halfday(s)." << "\n";
            outfile.close();
        }
    }
    outfile.close();
}

template<class T> void AList<T>::risk_2_report(int halfday, string filename)
{
    ofstream outfile;
    outfile.open(filename,ios::app);
    outfile.close();
    if (numitems != 0)
    {
        outfile << "The people who have been moved into risk 2 buffer during this week.\n";
        for (int i = 0; i < numitems; i++)
        {
            if ((*(getitem(i+1))).risk2halfday > halfday - 14 && (*(getitem(i+1))).risk2halfday <= halfday)
            {
                file_print_person_s1mple(*(getitem(i+1)), filename);
                outfile.open(filename,ios::app);
                outfile << endl;
                outfile.close();
            }
        }
    }
    else
    {
        outfile.open(filename,ios::app);
        outfile << "No people have been moved into risk 2 buffer during this week.\n";
        outfile.close();
    }
    outfile.close();
}

template<class T> void AList<T>::reinsert_report(int halfday, string filename)
{
    ofstream outfile;
    outfile.open(filename,ios::app);
    outfile.close();
    if (numitems != 0)
    {
        outfile << "The people who have been moved into reinsert buffer after withdrawn during this week.\n";
        for (int i = 0; i < numitems; i++)
        {
            if ((*(getitem(i+1))).reinsert_halfday > halfday - 14 && (*(getitem(i+1))).reinsert_halfday <= halfday)
            {
                file_print_person_s1mple(*(getitem(i+1)), filename);
                outfile.open(filename,ios::app);
                outfile << endl;
                outfile.close();
            }
        }
    }
    else
    {
        outfile.open(filename,ios::app);
        outfile << "No people have been moved into reinsert buffer during this week.\n";
        outfile.close();
    }
    outfile.close();
}

template<class T> void AList<T>::treat_report_all(int halfday)
{
    cout << "The people who have been treated.\n" ;
    for (int i = 0; i < numitems; i++)
    {
        print_person_s1mple(*(getitem(i+1)));
        cout << "Waiting time: " << (*(getitem(i+1))).treat_halfday - (*(getitem(i+1))).regist_halfday << " halfday(s)." << "\n";
    }
}

template<class T> void AList<T>::risk_report_all(int halfday)
{
    if (numitems != 0)
    {
        cout << "The people who are in the risk 2 buffer:\n" ;
        for (int i = 0; i < numitems; i++)
        {
            print_person_s1mple(*(getitem(i+1)));
            cout << " the halfday turned to risk 2: " << getitem(i+1)->risk2halfday << endl;
        }
    }
    else
    {
        cout << "The risk 2 buffer is empty.\n";
    }
}

template<class T> void AList<T>::reinsert_report_all(int halfday)
{
    if (numitems != 0)
    {
        cout << "The people who are in the reinsert buffer:\n" ;
        for (int i = 0; i < numitems; i++)
        {
            print_person_s1mple(*(getitem(i+1)));
            cout << endl;
        }
    }
    else
    {
        cout << "The reinsert buffer is empty.\n";
    }
}

template<class T> int AList<T>::total_halfday(int halfday)
{
    int total_halfdays = 0;
    for (int i = 0; i < numitems; i++)
    {
        int time = halfday - (*(getitem(i+1))).regist_halfday;
        total_halfdays += time;
    }
    return total_halfdays;
}

template<class T> int AList<T>::average_halfday(int halfday)
{
    int average_halfdays = (total_halfday(halfday))/numitems;
    return average_halfdays;
}


