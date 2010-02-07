/*
  $Id: array.h 4100 2008-11-07 21:50:39Z abehm $

  Copyright (C) 2007 by The Regents of the University of California

  Redistribution of this file is permitted under the terms of the 
  BSD license

  An array that dynamically grows.  Reasons to write our own: (1) STL
  Vector doubles its array size every time we need to do a realloc, which
  causes too much storage overhead; (2) BOOST Array has a static size.

  WARNING: Since internally we use "malloc" and "realloc" to allocate
  memory, it only works for basic types such as int, unsigned, float,
  bool, and address pointer. DO NOT USE THIS CLASS FOR CLASSES SUCH AS
  STRING. FOR THESE CLASSES, YOU SHOULD USE STL VECTOR.

  Date: 05/16/2007
  Author: Chen Li <chenli (at) ics.uci.edu>
*/

#ifndef _keyarray_h_
#define _keyarray_h_

#include <cstdlib>
#include <cassert>
#include <iostream>
#include <vector>
#include <tr1/unordered_set>

using namespace std;
using namespace tr1;

template <typename T>
class KeyArray {
 public:
  typedef T* iterator;

  KeyArray() {
    init(10, 10);
  };

//  // constructor
//  KeyArray(bool useHashSet) {
//    init(10, 10, useHashSet);
//  };
//
//  // constructor
//  KeyArray(const unsigned initCapacity) {
//    init(initCapacity, 10);
//  };
//
//  // constructor
//  KeyArray(const unsigned initCapacity, const unsigned incrementalSize) {
//    init(initCapacity, incrementalSize);
//  };

  // destructor
  ~KeyArray() {
    if (data != NULL)
      free(data);

    if (elementSet != NULL)
      delete elementSet;
  };

  void clear(){
 	  if (data != NULL)
 	       free(data);

 	     if (elementSet != NULL)
 	       delete elementSet;
   }

  unsigned size() const {
    return elementNum;
  };

  void setSize(unsigned size) {
    capacity = size;
    data = (T *)realloc(data, sizeof(T) * capacity);
//    assert(data != NULL);
    elementNum = size;
  };

  unsigned getCapacity() const {
    return capacity;
  };

  void extendCapacity() {
    capacity += incrementalSize; // allocate more space
    data = (T *)realloc(data, sizeof(T) * capacity);
    assert(data != NULL);
  }

  void append(const T &element) {
    if (elementNum >= capacity)
      extendCapacity();

    // store this element
    data[elementNum] = element;
    elementNum++;

    // if there are too many elements, we are willing to build a hashset
    // for this array in order to improve the performance of lookups
    //if (elementNum > 10000) {
    //
    //    if (elementNum == 50000 && elementSet == NULL &&  && rand() % 10
    //    <= 2 ) {
    /*if (elementNum == 10000 && elementSet == NULL) {
      elementSet = new unordered_set<T>;
      for (unsigned i = 0; i < elementNum - 1; i ++)
	elementSet->insert(data[i]);
	}*/

    // store the latest one in the hashset
    if (elementSet != NULL){
      //cout<<"insert to hastable " <<endl;
      elementSet->insert(element);
    }//end if
  };


  // delete an element at a given position (starting from 0)
  void erase(const unsigned position) {

    // check corner cases
    if (elementNum == 0 || position >= elementNum)
      return;

    // shift elements to the left
    // be careful about the usage of the unsigned type, and make sure
    // "elementNum -1" is never negative.
    for (register unsigned i = position + 1; i <= elementNum - 1; i ++) {
      data[i-1] = data[i];
    }

    elementNum --;
  }

  ///vijay-changed code to insert in a sorted list

  unsigned insert(const T &element){
	// allocate space
	if (elementNum >= capacity)
	extendCapacity();

	unsigned i=elementNum;
	while(i>0 && data[i-1] > element){
	  data[i]=data[i-1];
	  --i;
	}
	data[i]=element;
	++elementNum;

	return i;

}
  // added by ALEX to save memory when final size of array is known
  void finalize() {
    data = (T *)realloc(data, sizeof(T) * elementNum);
    assert(data != NULL);
  }

  //added by jiaheng for delete the last element
  void removeLastElement(){
    if(elementNum>0)
      elementNum--;
  };

  T front() const {
    return data[0];
  };

  T back() const {
    return data[elementNum - 1];
  };

  bool empty() const {
    return elementNum == 0;
  };

  T at(unsigned pos) const {
    return data[pos];
  };

  const T& operator[] (unsigned pos) const {
    return data[pos];
  }

  T& operator[] (unsigned pos) {
    return data[pos];
  }

  //
  // Returns the position of the element between [first, last] that is the
  // smallest among those that are >= the given element.  If the element
  // exists in the array, returns that position.  If the key is greater
  // than all the elements in the array, returns the current element
  // number + 1.
  //
  // WARNING: We assume the array is already sorted in an ascending
  // order. it's the caller's responsibility to keep this order.
  //
  // http://www.fredosaurus.com/notes-cpp/algorithms/searching/binarysearch.html

  unsigned binarySearch(T key,  unsigned first) const {
    //assert(capacity > 0);
	  if(elementNum==0)return first;
    unsigned last  = elementNum - 1;

    while (first <= last) {
      unsigned mid = (first + last) / 2;  // compute mid point.
      if (key > data[mid])
    first = mid + 1;  // repeat search in top half.
      else if (key < data[mid])
       {
        if (mid==0) return first; // avoid last == -1
    last = mid - 1; // repeat search in bottom half.
       }
      else
    return mid;     // found it. return the position
    }

    return first;    // failed to find key

  };

  unsigned binarySearch(T key) const {

    return binarySearch(key,0); // default: starting from 0
  }

  /*
   * Returns the position of the first element equal or smaller than the key.
   * Assumes that the array is sorted in ascending order.
   * The search is done form end to start.
   * The algorithm used is a variant of the Jump Search
   * http://www.nist.gov/dads/HTML/jumpsearch.html
   * where the jump step is initially 1 and it is incremented by 1 after each jump
   * Starts the search at element pos - 1, so the maximum value for pos is
   * the length of the array.
   */
  unsigned jumpIncRevSearch(T key, unsigned pos) const {
    unsigned t = 1;
    // t designates the jump step
    // the jump step is incremented by one every time
    while (pos > t && data[pos - t] > key)
      pos -= t++;

    if (pos > 0)
      pos--;
    // t designates the end position
    // end position is the end of current block or the end of the array
    t = pos > t? pos - t:0;
    while (pos > t && data[pos] > key)
      pos--;
    return pos;

  }

  bool has(const T key) const {

    if (elementSet != NULL)
	return elementSet->find(key) != elementSet->end();

    // if no hash_set is available, we have to do a binary search
    unsigned pos = binarySearch(key, 0);
    if (pos < size() && at(pos) == key)
      return true;

    return false;
  }

  iterator begin() {
    return data;
  }

  iterator end() {
    return &data[elementNum];
  }

 private:

  void init(const unsigned initCapacity, const unsigned incrementalSize) {

    // if we don't want to use hashset, just use the following
    init(initCapacity,  incrementalSize, false);

    // if we DO want to use hashset, just use the following
    // init(initCapacity,  incrementalSize, true);
  }

  // helper for the constructor
  void init(const unsigned initCapacity, const unsigned incrementalSize,
	    bool useHashSet) {
    capacity = initCapacity;
    if (initCapacity == 0) this->capacity = 10;
    this->incrementalSize = incrementalSize;
    if (incrementalSize == 0) // make sure it's positive
      this->incrementalSize = 10;

    data = (T *) malloc(sizeof(T) * initCapacity);
    assert(data != NULL);

    elementNum = 0;

    if (useHashSet)
      elementSet = new unordered_set<T>;
    else
      elementSet = NULL;
  };

private:
  T*        data;
  unsigned  capacity;
  unsigned  elementNum;
  unsigned  incrementalSize;

  unordered_set<T> *elementSet;

  template <typename W> friend ostream& operator<<(ostream& out,
                                                   const KeyArray<W> &array);
};

template <typename T>
ostream& operator<<(ostream& out, const KeyArray<T> &array)
{
  out << '[';
  for (unsigned i = 0; i < array.elementNum; i++) {
    if (i != 0)
      out << ", ";
    out << array.data[i];
  }
  out << ']';
  return out;
}

template <typename T>
bool operator== (const KeyArray<T> &a, const KeyArray<T> &b)
{
  if (&a == &b)
    return true;
  if (a.size() != b.size())
    return false;
  for (unsigned i = 0; i < a.size(); i++)
    if (a[i] != b[i])
      return false;
  return true;
}
//
// the following functions is for Array operations as intersection, union, difference
// add by Wang Bin on Dec 13, 2007
//

// Array union operation input sorted array
void arrayUnion(KeyArray<unsigned> * srcArray, KeyArray<unsigned> * otherArray, KeyArray<unsigned> *& resultArray);

// Array intersection operation is between two sorted array
void arrayIntersection(KeyArray<unsigned> *& array1, KeyArray<unsigned> *& array2, KeyArray<unsigned> *& res);

// Array difference operation is from 'srcArray' minus 'subArray'
void arrayDifference(KeyArray<unsigned> * srcArray, KeyArray<unsigned> *& subArray);

// Array difference oepration outputs stored in 'resultArray' is from 'srcArray' minus 'subArray'
void arrayDifference(KeyArray<unsigned> * srcArray, KeyArray<unsigned> * subArray, KeyArray<unsigned> *& resultArray);
void intersect(KeyArray<unsigned> * srcArray, KeyArray<unsigned> *& otherArray);


#endif
