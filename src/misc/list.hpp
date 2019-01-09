//
// list.hpp, simple intrusive list management
//
// Copyright Marius Greuel 1996. All rights reserved.
//

#ifndef _LIST_HPP_INCLUDED
#define _LIST_HPP_INCLUDED

#include "types.h"

// CISLink
template <class T>
class CISLink {
friend class CISList;
friend class CISIter;
  T *Next;                      // next element
};

// CIDLink
template <class T>
class CIDLink {
friend class CIDList;
friend class CIDIter;
  T *Next, *Prev;               // next & previous element
};

// CISList: intrusive singly linked list
template <class T>
class CISList {
  T *Last;                      // last element, first element = last->next
public:
  // default constructor
  CISList()
  {
    Last = NULL;
  }
  // insert element at top of list
  void Insert( T *Element )
  {
    if( Last )
      Element->Next = Last->Next;
    else
      Last = Element;
    Last->Next = Element;
  }
  // append element at bottom of list
  void Append( T *Element )
  {
    if( Last ) {
      Element->Next = Last->Next;
      Last = Last->Next = Element;
    } else
      Last = Element->Next = Element;
  }
  // get first element
  T *Find() const
  {
    return Last ? Last->Next : NULL;
  }
  // get last element
  T *FindLast() const
  {
    return Last;
  }
  // get first element and remove it
  T *Get()
  {
    if( Last == NULL )
      return NULL;
    T *Element = Last->Next;
    if( Element == Last )
      Last = NULL;
    else
      Last->Next = Element->Next;
    return Element;
  }
  // test if list is empty
  BOOL IsEmpty() const
  {
    return Last == NULL ? TRUE : FALSE;
  }
  // count number of elements in list
  int Count() const
  {
    int NumOfElements = 0;
    for( T *Element = Find(); Element != Last; Element = Element->Next )
      NumOfElements++;
    return NumOfElements + 1;
  }
};

// CIDList: intrusive doubly linked list
template <class T>
class CIDList {
  T *Root, *Last;               // first & last element
public:
  // default constructor
  CIDList()
  {
    Root = Last = NULL;
  }
  // insert element at top of list
  void Insert( T *Element )
  {
    Element->Next = Root;
    Element->Prev = NULL;
    if( Root )
      Root->Prev = Element;
    Root = Element;
    if( Last == NULL )
      Last = Element;
  }
  // append element at bottom of list
  void Append( T *Element )
  {
    Element->Next = NULL;
    Element->Prev = Last;
    if( Last )
      Last->Next = Element;
    Last = Element;
    if( Root == NULL )
      Root = Element;
  }
  // get first element
  T *Find() const
  {
    return Root;
  }
  // get last element of list
  T *FindLast() const
  {
    return Last;
  }
  // get first (or given) element and remove it
  T *Get( T *Element = NULL )
  {
    if( Element == NULL ) {
      Element = Root;
      if( Element == NULL )
        return NULL;
    }
    if( Element->Next )
      Element->Next->Prev = Element->Prev;
    else
      Last = Element->Prev;
    if( Element->Prev )
      Element->Prev->Next = Element->Next;
    else
      Root = Element->Next;
    return Element;
  }
  // test if list is empty
  BOOL IsEmpty() const
  {
    return Root == NULL ? TRUE : FALSE;
  }
  // count number of elements in list
  int Count() const
  {
    int NumOfElements = 0;
    for( T *Element = Find(); Element; Element = Element->Next )
      NumOfElements++;
    return NumOfElements;
  }
};

// CISIter: singly linked list iterator
template <class T>
class CISIter {
  const CISList<T> *List;
  int Index;
  T *Element;
public:
  CISIter( const CISList<T> &_List, int StartIndex = 0 )
  {
    List = &_List;
    Index = 0;
    Element = List->Find();
    while( Index < StartIndex && Element != NULL ) {
      Index++;
      Element = Element->Next;
    }
  }
  T *operator ++(int)
  {
    if( Element == NULL )
      return NULL;
    Index++;
    if( Element == List->FindLast() )
      return Element = NULL;
    else
      return Element = Element->Next;
  }
  T *operator []( int i )
  {
    if( i < Index || Element == NULL ) {
      Index = 0;
      Element = List->Find();
    }
    while( Index < i && Element != NULL ) {
      Index++;
      Element = Element->Next;
    }
    return Element;
  }
  T *operator ()() const
  {
    return Element;
  }
};

// CIDIter: doubly linked list iterator
template <class T>
class CIDIter {
  const CIDList<T> *List;
  int Index;
  T *Element;
public:
  CIDIter( const CIDList<T> &_List, int Dir = 1, int StartIndex = 0 )
  {
    List = &_List;
    Index = 0;
    Element = Dir > 0 ? List->Find() : List->FindLast();
    while( Index < StartIndex && Element != NULL ) {
      Index++;
      Element = Element->Next;
    }
  }
  T *operator ++(int)
  {
    if( Element == NULL )
      return NULL;
    Index++;
    return Element = Element->Next;
  }
  T *operator --(int)
  {
    if( Element == NULL )
      return NULL;
    Index--;
    return Element = Element->Prev;
  }
  T *operator []( int i )
  {
    if( Element == NULL ) {
      Index = 0;
      Element = List->Find();
    }
    while( Index < i && Element != NULL ) {
      Index++;
      Element = Element->Next;
    }
    while( Index > i && Element != NULL ) {
      Index--;
      Element = Element->Prev;
    }
    return Element;
  }
  T *operator ()() const
  {
    return Element;
  }
};

#endif // _LIST_HPP_INCLUDED

