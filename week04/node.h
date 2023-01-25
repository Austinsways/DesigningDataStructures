/***********************************************************************
 * Header:
 *    NODE
 * Summary:
 *    One node in a linked list (and the functions to support them).
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *
 *    This will contain the class definition of:
 *        Node         : A class representing a Node
 *    Additionally, it will contain a few functions working on Node
 * Author
 *    <your names here>
 ************************************************************************/

#pragma once

#include <cassert>     // for ASSERT
#include <iostream>    // for NULL

/*************************************************
 * NODE
 * the node class.  Since we do not validate any
 * of the setters, there is no point in making them
 * private.  This is the case because only the
 * List class can make validation decisions
 *************************************************/
template <class T>
class Node
{
public:

   //
   // Construct
   //

   Node()
   {
      data = T();
      pPrev = nullptr;
      pNext = nullptr;
   }
   Node(const T &  data)
   {
       this->data = data;
       pPrev = nullptr;
       pNext = nullptr;
   }
   Node(      T && data)
   {
       this->data = data;
       pPrev = nullptr;
       pNext = nullptr;
   }

   //
   // Member variables
   //

   T data;                 // user data
   Node <T> * pNext;       // pointer to next node
   Node <T> * pPrev;       // pointer to previous node
};

/***********************************************
 * COPY
 * Copy the list from the pSource and return
 * the new list
 *   INPUT  : the list to be copied
 *   OUTPUT : return the new list
 *   COST   : O(n)
 **********************************************/
template <class T>
inline Node <T> * copy(const Node <T> * pSource) 
{
    if (pSource != nullptr) {
        Node<T>* pDestination = new Node<T>(pSource->data);

        //creating a copy of pSource we can modify
        Node<T>* pSrc = new Node<T>;
        pSrc->data = pSource->data;
        pSrc->pNext = pSource->pNext;
        pSrc->pPrev = pSource->pPrev;

        Node<T>* pDes = pDestination; //we can iterate through this without losing the start of it.

        while (pSrc->pNext) {
            insert(pDes, pSrc->data, true);
            pSrc = pSrc->pNext;
            pDes = pDes->pNext;
        }
        return pDestination;
    }
    else {
        return nullptr;
    }
}

/***********************************************
 * Assign
 * Copy the values from pSource into pDestination
 * reusing the nodes already created in pDestination if possible.
 *   INPUT  : the list to be copied
 *   OUTPUT : return the new list
 *   COST   : O(n)
 **********************************************/
template <class T>
inline void assign(Node <T> * & pDestination, const Node <T> * pSource)
{
   
}

/***********************************************
 * SWAP
 * Swap the list from LHS to RHS
 *   COST   : O(1)
 **********************************************/
template <class T>
inline void swap(Node <T>* &pLHS, Node <T>* &pRHS)
{

}

/***********************************************
 * REMOVE
 * Remove the node pSource in the linked list
 *   INPUT  : the node to be removed
 *   OUTPUT : the pointer to the parent node
 *   COST   : O(1)
 **********************************************/
template <class T>
inline Node <T> * remove(const Node <T> * pRemove) 
{
   return new Node <T>;
}

/**********************************************
 * INSERT 
 * Insert a new node the the value in "t" into a linked
 * list immediately before the current position.
 *   INPUT   : t - the value to be used for the new node
 *             pCurrent - a pointer to the node before which
 *                we will be inserting the new node
 *             after - whether we will be inserting after
 *   OUTPUT  : return the newly inserted item
 *   COST    : O(1)
 **********************************************/
template <class T>
inline Node <T>* insert(Node <T>* pCurrent,
    const T& t,
    bool after = false)
{
    Node<T>* freshNode = new Node<T>; //fresh node to insert
    if (pCurrent != nullptr && after == false) { //place a new node with the intialized value into the last, and replace the next and previous pointers of the corresponding nodes.
       
        freshNode->data = t;
        freshNode->pPrev = pCurrent->pPrev;
        freshNode->pNext = pCurrent;
        pCurrent->pPrev = freshNode;
        if (freshNode->pPrev) //if the previous exists or isnt nullptr
        {
            freshNode->pPrev->pNext = freshNode;
        }
    }
    if (pCurrent != nullptr && after == true) { //if the items to be placed after makie the pNext nullptr
        freshNode->data = t;
        freshNode->pPrev = pCurrent;
        freshNode->pNext = nullptr;
        pCurrent->pNext = freshNode;
        
    }
    if (!freshNode) {
        freshNode->data = T();
    }
    return freshNode;

   

}

/******************************************************
 * SIZE
 * Find the size an unsorted linked list.  
 *  INPUT   : a pointer to the head of the linked list
 *            the value to be found
 *  OUTPUT  : number of nodes
 *  COST    : O(n)
 ********************************************************/
template <class T>
inline size_t size(const Node <T> * pHead)
{
   return 99;
}

/***********************************************
 * DISPLAY
 * Display all the items in the linked list from here on back
 *    INPUT  : the output stream
 *             pointer to the linked list
 *    OUTPUT : the data from the linked list on the screen
 *    COST   : O(n)
 **********************************************/
template <class T>
inline std::ostream & operator << (std::ostream & out, const Node <T> * pHead)
{
   return out;
}

/*****************************************************
 * CLEAR
 * Free all the data currently in the linked list
 *   INPUT   : pointer to the head of the linked list
 *   OUTPUT  : pHead set to NULL
 *   COST    : O(n)
 ****************************************************/
template <class T>
inline void clear(Node <T> * & pHead)
{

}


