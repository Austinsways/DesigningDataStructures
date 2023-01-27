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

   Node() : data(T())
   {
      pPrev = nullptr;
      pNext = nullptr;
   }
   Node(const T &  data) : data(data)
   {
       this->pPrev = nullptr;
       this->pNext = nullptr;
   }
   Node(      T && data) : data(data)
   {
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
   Node<T>* pDes = nullptr;
   if (pSource == nullptr) {
        return pDes;
   }
   else if (pSource->pNext==nullptr) {
       pDes = new Node<T>(pSource->data);
       return pDes;
   }
   
   if (pSource->pNext != nullptr) {
       Node<T>* pSrc = pSource->pNext->pPrev;
       Node<T>* pStart = new Node<T>(pSrc->data);
       pDes = pStart;
       pSrc = pSrc->pNext;
       while (pSrc!=nullptr)
       {
           pDes->pNext = new Node<T>(pSrc->data);
           pDes->pNext->pPrev = pDes;
           pDes = pDes->pNext;
           pSrc = pSrc->pNext;
           
       }
       return pStart;
   }
   
   return pDes;
    
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
    
    if (pSource == nullptr) {
        if (pDestination != nullptr) {
            while (pDestination->pNext != nullptr)
            {
                pDestination = pDestination->pNext;
                delete pDestination->pPrev;
            }
            Node<T>* deleteableData = pDestination;
            pDestination = nullptr;
            delete deleteableData;
        }
    }
    else {
        Node<T>* pSrc = nullptr;
        Node<T>* pDesPrevious = nullptr;
        Node<T>* pDes = pDestination;
        
        //we do the first step with the constant variable.
        if (pDes != nullptr) {
            pDes->data = pSource->data;
            pDes = pDes->pNext;
            pSrc = pSource->pNext;
            while (pDes != nullptr && pSrc != nullptr) { //itterate through the lists until on of them ends

                pDes->data = pSrc->data;
                if (pDes->pNext == nullptr) {
                    pDesPrevious = pDes;
                }
                pDes = pDes->pNext;
                pSrc = pSrc->pNext;
            }
        }
        else {
            if (pSource != nullptr) {
                pSrc = pSource->pNext->pPrev;
            }
        }
        if (pSrc != nullptr) {  //if the destination list ended first.
            pDes = pDesPrevious;

            if (pSrc == nullptr) {
                pSrc = pSource->pNext->pPrev;
            }
            if (pDes == nullptr && pSrc != nullptr) {
                pDestination = new Node<T>(pSrc->data);
                pDes = pDestination;
                pSrc = pSrc->pNext;
            }

            while (pSrc != nullptr) {
                Node<T>* freshNode = new Node<T>(pSrc->data);
                pDes->pNext = freshNode;
                freshNode->pPrev = pDes;
                pSrc = pSrc->pNext;
                pDes = pDes->pNext;
            }
        }

        else if (pDes != nullptr) { //if the source list ended first

            Node<T>* pDeleter = pDes;
            pDes = pDes->pPrev;
            while (pDeleter->pNext != nullptr) {
                pDeleter = pDeleter->pNext;
            }
            while (pDeleter != pDes) {
                pDeleter = pDeleter->pPrev;
                delete pDeleter->pNext;
            }
            pDes->pNext = nullptr;
        }
    }
}

/***********************************************
 * SWAP
 * Swap the list from LHS to RHS
 *   COST   : O(1)
 **********************************************/
template <class T>
inline void swap(Node <T>* &pLHS, Node <T>* &pRHS)
{
    std::swap(pLHS, pRHS);
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
    if (pRemove == nullptr)
        return nullptr;

    if (pRemove->pPrev)
        pRemove->pPrev->pNext = pRemove->pNext;
    if (pRemove->pNext)
        pRemove->pNext->pPrev = pRemove->pPrev;

    Node <T>* pReturn = nullptr;
    if (pRemove->pPrev)
        pReturn = pRemove->pPrev;
    else pReturn = pRemove->pNext;

    delete pRemove;
    return pReturn;
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
    Node<T>* freshNode = new Node<T>(t); //fresh node to insert
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
    else if (pCurrent != nullptr && after == true) { //if the items to be placed after make the pNext the old pNext of the Current node
        freshNode->pPrev = pCurrent;
        freshNode->pNext = pCurrent->pNext;
        pCurrent->pNext = freshNode;
        if (freshNode->pNext != nullptr) {
            freshNode->pNext->pPrev = freshNode;
        }
        
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
    int s = 0;
    for (auto p = pHead; p != nullptr; p = p->pNext)
        s++;

    return s;
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
    for (auto p = pHead; p != nullptr; p = p->pNext)
        out << p->data;
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
    while (pHead != nullptr)
    {
        Node<T> * pDelete = pHead;
        pHead = pHead->pNext;
        delete pDelete;
    }
}


