/***********************************************************************
 * Header:
 *    TEST NODE
 * Summary:
 *    Unit tests for node
 * Author
 *    Br. Helfrich
 ************************************************************************/

#pragma once

#ifdef DEBUG

#include "node.h"
#include "spy.h"

#include <cassert>
#include <memory>

class TestNode : public UnitTest
{
public:
   void run()
   {
      reset();

      // Construct
      test_create_default();
      test_create_value();
      test_copy_nullptr();
      test_copy_one();
      test_copy_standard();
      
      // Assign
      test_assign_emptyToEmpty();
      test_assign_standardToEmpty();
      test_assign_emptyToStandard();
      test_assign_smallToBig();
      test_assign_bigToSmall();
      test_swap_emptyEmpty();
      test_swap_emptyStandard();
      test_swap_standardEmpty();
      test_swap_oneTwo();

      // Insert
      test_insert_emptyBefore();
      test_insert_emptyAfter();
      test_insert_frontBefore();
      test_insert_frontAfter();
      test_insert_backBefore();
      test_insert_backAfter();
      test_insert_middleBefore();
      test_insert_middleAfter();

      // Remove
      test_remove_nullptr();
      test_remove_front();
      test_remove_back();
      test_remove_middle();
      test_clear_nullptr();
      test_clear_one();
      test_clear_standard();

      // Status
      test_size_empty();
      test_size_standard();
      test_size_standardMiddle();
      
      report("Node");
   }
   
   /***************************************
    * CONSTRUCTOR
    ***************************************/
   
   // default constructor
   void test_create_default()
   {  // setup
      std::allocator<Node <Spy>> alloc;
      Node <Spy> n;
      n.data = Spy(999);
      n.pNext = (Node <Spy> *)0xBADF00D1;
      n.pPrev = (Node <Spy> *)0xBADF00D2;
      Spy::reset();
      // exercise
      alloc.construct(&n); // the constructor is called explicitly
      // verify
      assertUnit(Spy::numDefault() == 1);      // a default spy is created
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertEmptyFixture(&n);
   }  // teardown
   
   // non-default constructor that initializes with a value.
   void test_create_value()
   {  // setup
      std::allocator<Node <Spy>> alloc;
      Node <Spy> n;
      n.data = Spy(999);
      n.pNext = (Node <Spy> *)0xBADF00D1;
      n.pPrev = (Node <Spy> *)0xBADF00D2;
      Spy s(99);
      Spy::reset();
      // exercise
      alloc.construct(&n, s); // the constructor is called explicitly
      // verify
      assertUnit(Spy::numCopy() == 1);       // copy 99 to n
      assertUnit(Spy::numAlloc() == 1);      // allocate the new 99
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      //    +----+
      //    | 99 |
      //    +----+
      assertUnit(n.data == Spy(99));
      n.data = Spy();
      assertEmptyFixture(&n);
   }  // teardown
   
   /***************************************
    * COPY
    ***************************************/
   
   // copy with a nullptr pointer
   void test_copy_nullptr()
   {  // setup
      Node <Spy>* pSrc = nullptr;
      Node <Spy>* pDes = nullptr;
      Spy::reset();
      // exercise
      pDes = copy(pSrc);
      // verify
      assertUnit(Spy::numDefault() == 0);      
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);      
      assertUnit(pDes == nullptr);
      assertUnit(pSrc == nullptr);
   }  // teardown

   // copy a single node in insolation
   void test_copy_one()
   {  // setup
      //    +----+
      //    | 26 |
      //    +----+
      Node <Spy>* pSrc = new Node <Spy> (Spy(26));
      Node <Spy>* pDes = nullptr;
      Spy::reset();
      // exercise
      pDes = copy(pSrc);
      // verify
      assertUnit(Spy::numCopy() == 1);        // copy [26] to pDes    
      assertUnit(Spy::numAlloc() == 1);       // the copy involves an allocation
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(pSrc != pDes);
      //    +----+
      //    | 26 |
      //    +----+
      assertUnit(pSrc != nullptr);
      if (pSrc)
      {
         assertUnit(pSrc->data == Spy(26));
         assertUnit(pSrc->pNext == nullptr);
         assertUnit(pSrc->pPrev == nullptr);
      }
      //    +----+
      //    | 26 |
      //    +----+
      assertUnit(pDes != nullptr);
      if (pDes)
      {
         assertUnit(pDes->data == Spy(26));
         assertUnit(pDes->pNext == nullptr);
         assertUnit(pDes->pPrev == nullptr);
      }
      // teardown
      teardownStandardFixture(pSrc);
      teardownStandardFixture(pDes);
   }
   
   // copy the standard fixture: three nodes.
   void test_copy_standard()
   {  // setup
      //     p11      p26      p31
      //    +----+   +----+   +----+
      //    | 11 | - | 26 | - | 31 |
      //    +----+   +----+   +----+
      Node <Spy>* p11, * p26, * p31;
      setupStandardFixture(p11, p26, p31);
      Node <Spy>* pDes = nullptr;
      Spy::reset();
      // exercise
      pDes = copy(p11);
      // verify
      assertUnit(Spy::numCopy() == 3);        // copy [11][26][31] to pDes    
      assertUnit(Spy::numAlloc() == 3);       // the copy involves an allocation
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(p11 != pDes);
      if(pDes)
      { 
         assertUnit(p26 != pDes->pNext);
         if (p31 && pDes->pNext)
            assertUnit(p31->pNext != pDes->pNext->pNext);
      }
      //     p11   
      //    +----+   +----+   +----+
      //    | 11 | - | 26 | - | 31 |
      //    +----+   +----+   +----+
      assertStandardFixture(p11);
      //     pDes
      //    +----+   +----+   +----+
      //    | 11 | - | 26 | - | 31 |
      //    +----+   +----+   +----+
      assertStandardFixture(pDes);
      // teardown
      teardownStandardFixture(p11);
      teardownStandardFixture(pDes);
   }

   /***************************************
    * ASSIGN
    ***************************************/

   // assign an empty list to an empty list
   void test_assign_emptyToEmpty()
   {  // setup
      Node <Spy>* pSrc = nullptr;
      Node <Spy>* pDes = nullptr;
      Spy::reset();
      // exercise
      assign(pDes, pSrc);
      // verify
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numSwap() == 0);
      assertUnit(pDes == nullptr);
      assertUnit(pSrc == nullptr);
   }  // teardown

   // assign the standard fixture onto an empty linked list
   void test_assign_standardToEmpty()
   {  // setup
      //     p11      p26      p31
      //    +----+   +----+   +----+
      //    | 11 | - | 26 | - | 31 |
      //    +----+   +----+   +----+
      Node <Spy>* p11, * p26, * p31;
      setupStandardFixture(p11, p26, p31);
      Node <Spy>* pDes = nullptr;
      Spy::reset();
      // exercise
      assign(pDes, p11);
      // verify
      assertUnit(Spy::numCopy() == 3);      // copy [11][26][31]
      assertUnit(Spy::numAlloc() == 3);     // allocate [11][26][31]
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numSwap() == 0);
      assertUnit(pDes != p11);
      //     p11   
      //    +----+   +----+   +----+
      //    | 11 | - | 26 | - | 31 |
      //    +----+   +----+   +----+
      assertStandardFixture(p11);
      //     pDes
      //    +----+   +----+   +----+
      //    | 11 | - | 26 | - | 31 |
      //    +----+   +----+   +----+
      assertStandardFixture(pDes);
      // teardown
      teardownStandardFixture(p11);
      teardownStandardFixture(pDes);
   }

   // assign an empty linked list onto a standard fixturew
   void test_assign_emptyToStandard()
   {  // setup
      const Node <Spy>* pSrc = nullptr;
      //     p11      p26      p31
      //    +----+   +----+   +----+
      //    | 11 | - | 26 | - | 31 |
      //    +----+   +----+   +----+
      Node <Spy>* p11, * p26, * p31;
      setupStandardFixture(p11, p26, p31);
      Spy::reset();
      // exercise
      assign(p11, pSrc);
      // verify
      assertUnit(Spy::numDestructor() == 3);  // destroy [11][26][31]
      assertUnit(Spy::numDelete() == 3);      // delete  [11][26][31]
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numSwap() == 0);
      assertUnit(p11 == nullptr);
      assertUnit(pSrc == nullptr);
   }  // teardown

   // assign a small linked list to a big one
   void test_assign_smallToBig()
   {  // setup
      //     p11      p26      p31
      //    +----+   +----+   +----+
      //    | 11 | - | 26 | - | 31 |
      //    +----+   +----+   +----+
      Node <Spy>* p11, * p26, * p31;
      setupStandardFixture(p11, p26, p31);
      //     p67      p89  
      //    +----+   +----+
      //    | 67 | - | 89 |
      //    +----+   +----+
      Node <Spy>* p67, * p89;
      p67 = new Node <Spy>(Spy(67));
      p89 = new Node <Spy>(Spy(89));
      p67->pNext = p89;
      p89->pPrev = p67;
      Spy::reset();
      // exercise
      assign(p11, p67);
      // verify
      assertUnit(Spy::numDestructor() == 1);  // destroy [31]
      assertUnit(Spy::numDelete() == 1);      // delete  [31]
      assertUnit(Spy::numAssign() == 2);      // assign [67][89] onto [11][26];
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numSwap() == 0);
      assertUnit(p11 != p67);
      //     p11      p26  
      //    +----+   +----+
      //    | 67 | - | 89 |
      //    +----+   +----+
      assertUnit(p11 != nullptr);
      if (p11)
      {
         assertUnit(p11->data == Spy(67));
         assertUnit(p11->pPrev == nullptr);
         assertUnit(p11->pNext == p26);
      }
      assertUnit(p26 != nullptr);
      if (p26)
      {
         assertUnit(p26->data == Spy(89));
         assertUnit(p26->pPrev == p11);
         assertUnit(p26->pNext == nullptr);
      }
      //     p67      p89  
      //    +----+   +----+
      //    | 67 | - | 89 |
      //    +----+   +----+
      assertUnit(p67 != nullptr);
      if (p67)
      {
         assertUnit(p67->data == Spy(67));
         assertUnit(p67->pPrev == nullptr);
         assertUnit(p67->pNext == p89);
      }
      assertUnit(p89 != nullptr);
      if (p89)
      {
         assertUnit(p89->data == Spy(89));
         assertUnit(p89->pPrev == p67);
         assertUnit(p89->pNext == nullptr);
      }
      // teardown
      teardownStandardFixture(p11);
      teardownStandardFixture(p67);
   }

   // assign a big linked list to a small one
   void test_assign_bigToSmall()
   {  // setup
      //     p11      p26      p31
      //    +----+   +----+   +----+
      //    | 11 | - | 26 | - | 31 |
      //    +----+   +----+   +----+
      Node <Spy>* p11, * p26, * p31;
      setupStandardFixture(p11, p26, p31);
      //     p67      p89  
      //    +----+   +----+
      //    | 67 | - | 89 |
      //    +----+   +----+
      Node <Spy>* p67, * p89;
      p67 = new Node <Spy>(Spy(67));
      p89 = new Node <Spy>(Spy(89));
      p67->pNext = p89;
      p89->pPrev = p67;
      Spy::reset();
      // exercise
      assign(p67, p11);
      // verify
      assertUnit(Spy::numAssign() == 2);      // assign [11][26] onto [67][89]
      assertUnit(Spy::numCopy() == 1);        // copy-create [31]
      assertUnit(Spy::numAlloc() == 1);       // allocate [31]
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);      
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numSwap() == 0);
      assertUnit(p11 != p67);
      //     p11   
      //    +----+   +----+   +----+
      //    | 11 | - | 26 | - | 31 |
      //    +----+   +----+   +----+
      assertStandardFixture(p11);
      //     p67
      //    +----+   +----+   +----+
      //    | 11 | - | 26 | - | 31 |
      //    +----+   +----+   +----+
      assertStandardFixture(p67);
      // teardown
      teardownStandardFixture(p11);
      teardownStandardFixture(p67);
   }

   /***************************************
    * SWAP
    ***************************************/

   // swap two empty pointers
   void test_swap_emptyEmpty()
   {  // setup
      Node <Spy>* pLHS = nullptr;
      Node <Spy>* pRHS = nullptr;
      Spy::reset();
      // exercise
      swap(pLHS, pRHS);
      // verify
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(pLHS == nullptr);
      assertUnit(pRHS == nullptr);
   }  // teardown

   // swap an empty fixture with the standard fixture
   void test_swap_emptyStandard()
   {  // setup
      Node <Spy>* pLHS = nullptr;
      //     p11      p26      p31
      //    +----+   +----+   +----+
      //    | 11 | - | 26 | - | 31 |
      //    +----+   +----+   +----+
      Node <Spy>* p11, * p26, * p31;
      setupStandardFixture(p11, p26, p31);
      Spy::reset();
      // exercise
      swap(pLHS, p11);
      // verify
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      //     pLHS
      //    +----+   +----+   +----+
      //    | 11 | - | 26 | - | 31 |
      //    +----+   +----+   +----+
      assertStandardFixture(pLHS);
      assertUnit(p11 == nullptr);
      // teardown
      teardownStandardFixture(pLHS);
   }

   // swap an empty fixture with the standard fixture
   void test_swap_standardEmpty()
   {  // setup
      //     p11      p26      p31
      //    +----+   +----+   +----+
      //    | 11 | - | 26 | - | 31 |
      //    +----+   +----+   +----+
      Node <Spy>* p11, * p26, * p31;
      setupStandardFixture(p11, p26, p31);
      Node <Spy>* pRHS = nullptr;
      Spy::reset();
      // exercise
      swap(p11, pRHS);
      // verify
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      //     pRHS
      //    +----+   +----+   +----+
      //    | 11 | - | 26 | - | 31 |
      //    +----+   +----+   +----+
      assertStandardFixture(pRHS);
      assertUnit(p11 == nullptr);
      // teardown
      teardownStandardFixture(pRHS);
   } 
   
   // swap two non-empty lists
   void test_swap_oneTwo()
   {  // setup
      //     p11      p26      p31
      //    +----+   +----+   +----+
      //    | 11 | - | 26 | - | 31 |
      //    +----+   +----+   +----+
      Node <Spy>* p11, * p26, * p31;
      setupStandardFixture(p11, p26, p31);
      //     p67      p89  
      //    +----+   +----+
      //    | 67 | - | 89 |
      //    +----+   +----+
      Node <Spy>* p67, * p89;
      p67 = new Node <Spy>(Spy(67));
      p89 = new Node <Spy>(Spy(89));
      p67->pNext = p89;
      p89->pPrev = p67;
      Spy::reset();
      // exercise
      swap(p11, p67);
      // verify
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      //     p67
      //    +----+   +----+   +----+
      //    | 11 | - | 26 | - | 31 |
      //    +----+   +----+   +----+
      assertStandardFixture(p67);
      //     p11
      //    +----+   +----+
      //    | 67 | - | 89 |
      //    +----+   +----+
      assertUnit(p11 != nullptr);
      if (p11)
      {
         assertUnit(p11->data == Spy(67));
         assertUnit(p11->pPrev == nullptr);
         assertUnit(p11->pNext == p89);
      }

      assertUnit(p89 != nullptr);
      if (p89)
      {
         assertUnit(p89->data == Spy(89));
         assertUnit(p89->pPrev == p11);
         assertUnit(p89->pNext == nullptr);
      }
      // teardown
      teardownStandardFixture(p67);
      teardownStandardFixture(p11);
   } 

   /***************************************
    * REMOVE
    ***************************************/
   
   // attempt to remove from a nullptr pointer
   void test_remove_nullptr()
   {  // setup
      Node <Spy>* pHead = nullptr;
      Node <Spy>* pReturn = nullptr;
      Spy::reset();
      // exercise
      pReturn = remove(pHead);
      // verify
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(pHead == nullptr);
      assertUnit(pReturn == nullptr);
   }  // teardown
   
   // attempt to remove from the front of the standard fixture
   void test_remove_front()
   {  // setup
      //     p11      p26      p31
      //    +----+   +----+   +----+
      //    | 11 | - | 26 | - | 31 |
      //    +----+   +----+   +----+
      //     pRemove
      Node <Spy>* p11, * p26, * p31;
      setupStandardFixture(p11, p26, p31);
      Node <Spy>* pReturn = nullptr;
      Node <Spy>* pRemove = p11;
      Spy::reset();
      // exercise
      pReturn = remove(pRemove);
      // verify
      assertUnit(Spy::numDestructor() == 1);   // destroy [11]
      assertUnit(Spy::numDelete() == 1);       // delete the node [11]
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      //    pReturn
      //      p26      p31
      //    +----+   +----+
      //    | 26 | - | 31 |
      //    +----+   +----+
      assertUnit(pReturn == p26);
      assertUnit(nullptr != p26);
      if (p26)
      { 
         assertUnit(p26->data == Spy(26));
         assertUnit(p26->pNext == p31);
         assertUnit(p26->pPrev == nullptr);
      }
      assertUnit(nullptr != p31);
      if (p31)
      {
         assertUnit(p31->data == Spy(31));
         assertUnit(p31->pNext == nullptr);
         assertUnit(p31->pPrev == p26);
      }
      // teardown
      teardownStandardFixture(p26);
   }
   
   // remove from the back of the standard fixture
   void test_remove_back()
   {  // setup
      //     p11      p26      p31
      //    +----+   +----+   +----+
      //    | 11 | - | 26 | - | 31 |
      //    +----+   +----+   +----+
      //                       pRemove
      Node <Spy>* p11, * p26, * p31;
      setupStandardFixture(p11, p26, p31);
      Node <Spy>* pReturn = nullptr;
      Node <Spy>* pRemove = p31;
      Spy::reset();
      // exercise
      pReturn = remove(pRemove);
      // verify
      assertUnit(Spy::numDestructor() == 1);   // destroy [31]
      assertUnit(Spy::numDelete() == 1);       // delete the node [31]
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      //             pReturn
      //      p11      p26
      //    +----+   +----+
      //    | 11 | - | 26 |
      //    +----+   +----+
      assertUnit(pReturn == p26);

      assertUnit(p26 != nullptr);
      if (p26)
      {
         assertUnit(p26->data == Spy(26));
         assertUnit(p26->pNext == nullptr);
         assertUnit(p26->pPrev == p11);
      }
      assertUnit(p11 != nullptr);
      if (p11)
      {
         assertUnit(p11->data == Spy(11));
         assertUnit(p11->pNext == p26);
         assertUnit(p11->pPrev == nullptr);
      }
      // teardown
      teardownStandardFixture(p11);
   }

   // remove from the middle of the standard fixture
   void test_remove_middle()
   {  // setup
      //     p11      p26      p31
      //    +----+   +----+   +----+
      //    | 11 | - | 26 | - | 31 |
      //    +----+   +----+   +----+
      //             pRemove
      Node <Spy>* p11, * p26, * p31;
      setupStandardFixture(p11, p26, p31);
      Node <Spy>* pReturn = nullptr;
      Node <Spy>* pRemove = p26;
      Spy::reset();
      // exercise
      pReturn = remove(pRemove);
      // verify
      assertUnit(Spy::numDestructor() == 1);   // destroy [26]
      assertUnit(Spy::numDelete() == 1);       // delete the node [26]
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      //    pReturn
      //    +----+   +----+
      //    | 11 | - | 31 |
      //    +----+   +----+
      assertUnit(pReturn == p11);

      assertUnit(p11 != nullptr);
      if (p11)
      {
         assertUnit(p11->data == Spy(11));
         assertUnit(p11->pNext == p31);
         assertUnit(p11->pPrev == nullptr);
      }

      assertUnit(p31 != nullptr);
      if (p31)
      {
         assertUnit(p31->data == Spy(31));
         assertUnit(p31->pNext == nullptr);
         assertUnit(p31->pPrev == p11);
      }
      // teardown
      teardownStandardFixture(p11);
   }
   
   /***************************************
    * INSERT
    ***************************************/
   
   // insert into an empty linked list with the before option set
   void test_insert_emptyBefore()
   {  // setup
      Node <Spy>* pInsert = nullptr;
      Node <Spy>* pReturn = nullptr;
      Spy s(99);
      Spy::reset();
      // exercise
      pReturn = insert(pInsert, s, false /*after*/);
      // verify
      assertUnit(Spy::numCopy() == 1);          // copy-create [99]
      assertUnit(Spy::numAlloc() == 1);         // allocate [99]
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);      
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      //    pReturn
      //    +----+
      //    | 99 |
      //    +----+
      assertUnit(pReturn != nullptr);
      if (pReturn)
      {
         assertUnit(pReturn->data == Spy(99));
         assertUnit(pReturn->pPrev == nullptr);
         assertUnit(pReturn->pNext == nullptr);
      }
      assertUnit(pInsert == nullptr);
      // teardown
      teardownStandardFixture(pReturn);
   }

   // insert into an empty linked list with the after option set
   void test_insert_emptyAfter()
   {  // setup
      Node <Spy>* pInsert = nullptr;
      Node <Spy>* pReturn = nullptr;
      Spy s(99);
      Spy::reset();
      // exercise
      pReturn = insert(pInsert, s, true /*after*/);
      // verify
      assertUnit(Spy::numCopy() == 1);          // copy-create [99]
      assertUnit(Spy::numAlloc() == 1);         // allocate [99]
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      //    pReturn
      //    +----+
      //    | 99 |
      //    +----+
      assertUnit(pReturn != nullptr);
      if (pReturn)
      {
         assertUnit(pReturn->data == Spy(99));
         assertUnit(pReturn->pPrev == nullptr);
         assertUnit(pReturn->pNext == nullptr);
      }
      assertUnit(pInsert == nullptr);
      // teardown
      teardownStandardFixture(pReturn);
   }

   // insert into the front of the standard fixture with the after option set
   void test_insert_frontBefore()
   {  // setup
      //     p11      p26      p31
      //    +----+   +----+   +----+
      //    | 11 | - | 26 | - | 31 |
      //    +----+   +----+   +----+
      Node <Spy>* p11, * p26, * p31;
      setupStandardFixture(p11, p26, p31);
      Node <Spy>* pReturn = nullptr;
      Spy s(99);
      Spy::reset();
      // exercise
      pReturn = insert(p11, s, false /*after*/);
      // verify
      assertUnit(Spy::numCopy() == 1);          // copy-create [99]
      assertUnit(Spy::numAlloc() == 1);         // allocate [99]
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      //     pReturn    p11      p26      p31
      //     +----+   +----+   +----+   +----+
      //     | 99 | - | 11 | - | 26 | - | 31 |
      //     +----+   +----+   +----+   +----+
      assertUnit(pReturn != nullptr);
      if (pReturn)
      {
         assertUnit(pReturn->data == Spy(99));
         assertUnit(pReturn->pNext == p11);
         assertUnit(pReturn->pPrev == nullptr);
      }

      assertUnit(p11 != nullptr);
      if (p11)
      {
         assertUnit(p11->data == Spy(11));
         assertUnit(p11->pPrev == pReturn);
         assertUnit(p11->pNext == p26);
      }

      assertUnit(p26 != nullptr);
      if (p26)
      {
         assertUnit(p26->data == Spy(26));
         assertUnit(p26->pPrev == p11);
         assertUnit(p26->pNext == p31);
      }

      assertUnit(p31 != nullptr);
      if (p31)
      {
         assertUnit(p31->data == Spy(31));
         assertUnit(p31->pPrev == p26);
         assertUnit(p31->pNext == nullptr);
      }
      // teardown
      delete pReturn;
      delete p11;
      delete p26;
      delete p31;
   }
   
   // insert into the front of the standard fixture with the before option set
   void test_insert_frontAfter()
   {  // setup
      //     p11      p26      p31
      //    +----+   +----+   +----+
      //    | 11 | - | 26 | - | 31 |
      //    +----+   +----+   +----+
      Node <Spy>* p11, * p26, * p31;
      setupStandardFixture(p11, p26, p31);
      Node <Spy>* pReturn = nullptr;
      Spy s(99);
      Spy::reset();
      // exercise
      pReturn = insert(p11, s, true /*after*/);
      // verify
      assertUnit(Spy::numCopy() == 1);          // copy-create [99]
      assertUnit(Spy::numAlloc() == 1);         // allocate [99]
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      //       p11    pReturn   p26      p31
      //     +----+   +----+   +----+   +----+
      //     | 11 | - | 99 | - | 26 | - | 31 |
      //     +----+   +----+   +----+   +----+
      assertUnit(p11 != nullptr);
      if (p11)
      {
         assertUnit(p11->data == Spy(11));
         assertUnit(p11->pPrev == nullptr);
         assertUnit(p11->pNext == pReturn);
      }

      assertUnit(pReturn != nullptr);
      if (pReturn)
      {
         assertUnit(pReturn->data == Spy(99));
         assertUnit(pReturn->pNext == p26);
         assertUnit(pReturn->pPrev == p11);
      }

      assertUnit(p26 != nullptr);
      if (p26)
      {
         assertUnit(p26->data == Spy(26));
         assertUnit(p26->pPrev == pReturn);
         assertUnit(p26->pNext == p31);
      }

      assertUnit(p31 != nullptr);
      if (p31)
      {
         assertUnit(p31->data == Spy(31));
         assertUnit(p31->pPrev == p26);
         assertUnit(p31->pNext == nullptr);
      }
      // teardown
      delete pReturn;
      delete p11;
      delete p26;
      delete p31;
   }

   // insert into the back of the standard fixture with the before option set
   void test_insert_backBefore()
   {  // setup
      //     p11      p26      p31
      //    +----+   +----+   +----+
      //    | 11 | - | 26 | - | 31 |
      //    +----+   +----+   +----+
      Node <Spy>* p11, * p26, * p31;
      setupStandardFixture(p11, p26, p31);
      Node <Spy>* pReturn = nullptr;
      Spy s(99);
      Spy::reset();
      // exercise
      pReturn = insert(p31, s, false /*after*/);
      // verify
      assertUnit(Spy::numCopy() == 1);          // copy-create [99]
      assertUnit(Spy::numAlloc() == 1);         // allocate [99]
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      //       p11      p26    pReturn   p31
      //     +----+   +----+   +----+   +----+
      //     | 11 | - | 26 | - | 99 | - | 31 |
      //     +----+   +----+   +----+   +----+
      assertUnit(p11 != nullptr);
      if (p11)
      {
         assertUnit(p11->data == Spy(11));
         assertUnit(p11->pPrev == nullptr);
         assertUnit(p11->pNext == p26);
      }

      assertUnit(p26 != nullptr);
      if (p26)
      {
         assertUnit(p26->data == Spy(26));
         assertUnit(p26->pPrev == p11);
         assertUnit(p26->pNext == pReturn);
      }

      assertUnit(pReturn != nullptr);
      if (pReturn)
      {
         assertUnit(pReturn->data == Spy(99));
         assertUnit(pReturn->pNext == p31);
         assertUnit(pReturn->pPrev == p26);
      }

      assertUnit(p31 != nullptr);
      if (p31)
      {
         assertUnit(p31->data == Spy(31));
         assertUnit(p31->pPrev == pReturn);
         assertUnit(p31->pNext == nullptr);
      }
      // teardown
      delete pReturn;
      delete p11;
      delete p26;
      delete p31;
   }

   // insert into the back of the standard fixture with the after option set
   void test_insert_backAfter()
   {  // setup
      //     p11      p26      p31
      //    +----+   +----+   +----+
      //    | 11 | - | 26 | - | 31 |
      //    +----+   +----+   +----+
      Node <Spy>* p11, * p26, * p31;
      setupStandardFixture(p11, p26, p31);
      Node <Spy>* pReturn = nullptr;
      Spy s(99);
      Spy::reset();
      // exercise
      pReturn = insert(p31, s, true /*after*/);
      // verify
      assertUnit(Spy::numCopy() == 1);          // copy-create [99]
      assertUnit(Spy::numAlloc() == 1);         // allocate [99]
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      //       p11      p26      p31    pReturn  
      //     +----+   +----+   +----+   +----+
      //     | 11 | - | 26 | - | 31 | - | 99 |
      //     +----+   +----+   +----+   +----+
      assertUnit(p11 != nullptr);
      if (p11)
      {
         assertUnit(p11->data == Spy(11));
         assertUnit(p11->pPrev == nullptr);
         assertUnit(p11->pNext == p26);
      }

      assertUnit(p26 != nullptr);
      if (p26)
      {
         assertUnit(p26->data == Spy(26));
         assertUnit(p26->pPrev == p11);
         assertUnit(p26->pNext == p31);
      }

      assertUnit(p31 != nullptr);
      if (p31)
      {
         assertUnit(p31->data == Spy(31));
         assertUnit(p31->pPrev == p26);
         assertUnit(p31->pNext == pReturn);
      }

      assertUnit(pReturn != nullptr);
      if (pReturn)
      {
         assertUnit(pReturn->data == Spy(99));
         assertUnit(pReturn->pNext == nullptr);
         assertUnit(pReturn->pPrev == p31);
      }

      // teardown
      delete pReturn;
      delete p11;
      delete p26;
      delete p31;
   }
   
   // insert into the middle of the standard fixture with the before option set
   void test_insert_middleBefore()
   {  // setup
      //     p11      p26      p31
      //    +----+   +----+   +----+
      //    | 11 | - | 26 | - | 31 |
      //    +----+   +----+   +----+
      Node <Spy>* p11, * p26, * p31;
      setupStandardFixture(p11, p26, p31);
      Node <Spy>* pReturn = nullptr;
      Spy s(99);
      Spy::reset();
      // exercise
      pReturn = insert(p26, s, false /*after*/);
      // verify
      assertUnit(Spy::numCopy() == 1);          // copy-create [99]
      assertUnit(Spy::numAlloc() == 1);         // allocate [99]
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      //      p11     pReturn   p26      p31
      //     +----+   +----+   +----+   +----+
      //     | 11 | - | 99 | - | 26 | - | 31 |
      //     +----+   +----+   +----+   +----+
      assertUnit(p11 != nullptr);
      if (p11)
      {
         assertUnit(p11->data == Spy(11));
         assertUnit(p11->pPrev == nullptr);
         assertUnit(p11->pNext == pReturn);
      }

      assertUnit(pReturn != nullptr);
      if (pReturn)
      {
         assertUnit(pReturn->data == Spy(99));
         assertUnit(pReturn->pPrev == p11);
         assertUnit(pReturn->pNext == p26);
      }

      assertUnit(p26 != nullptr);
      if (p26)
      {
         assertUnit(p26->data == Spy(26));
         assertUnit(p26->pPrev == pReturn);
         assertUnit(p26->pNext == p31);
      }

      assertUnit(p31 != nullptr);
      if (p31)
      {
         assertUnit(p31->data == Spy(31));
         assertUnit(p31->pPrev == p26);
         assertUnit(p31->pNext == nullptr);
      }
      // teardown
      delete pReturn;
      delete p11;
      delete p26;
      delete p31;
   }
   
   // insert into the middle of the standard fixture with the after option set
   void test_insert_middleAfter()
   {  // setup
      //     p11      p26      p31
      //    +----+   +----+   +----+
      //    | 11 | - | 26 | - | 31 |
      //    +----+   +----+   +----+
      Node <Spy>* p11, * p26, * p31;
      setupStandardFixture(p11, p26, p31);
      Node <Spy>* pReturn = nullptr;
      Spy s(99);
      Spy::reset();
      // exercise
      pReturn = insert(p26, s, true /*after*/);
      // verify
      assertUnit(Spy::numCopy() == 1);          // copy-create [99]
      assertUnit(Spy::numAlloc() == 1);         // allocate [99]
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      //      p11       p26    pReturn   p31
      //     +----+   +----+   +----+   +----+
      //     | 11 | - | 26 | - | 99 | - | 31 |
      //     +----+   +----+   +----+   +----+
      assertUnit(p11 != nullptr);
      if (p11)
      {
         assertUnit(p11->data == Spy(11));
         assertUnit(p11->pPrev == nullptr);
         assertUnit(p11->pNext == p26);
      }

      assertUnit(p26 != nullptr);
      if (p26)
      {
         assertUnit(p26->data == Spy(26));
         assertUnit(p26->pPrev == p11);
         assertUnit(p26->pNext == pReturn);
      }

      assertUnit(pReturn != nullptr);
      if (pReturn)
      {
         assertUnit(pReturn->data == Spy(99));
         assertUnit(pReturn->pPrev == p26);
         assertUnit(pReturn->pNext == p31);
      }
      
      assertUnit(p31 != nullptr);
      if (p31)
      {
         assertUnit(p31->data == Spy(31));
         assertUnit(p31->pPrev == pReturn);
         assertUnit(p31->pNext == nullptr);
      }
      // teardown
      delete pReturn;
      delete p11;
      delete p26;
      delete p31;
   }
  
   /***************************************
    * FIND
    ***************************************/

   // attempt to find an element in an empty linked list
   void test_size_empty()
   {  // setup
      const Node <Spy>* p = nullptr;
      size_t s;
      Spy::reset();
      // exercise
      s = size(p);
      // verify
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(Spy::numCopy() == 0); 
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(s == 0);
   }  // teardown
   
   // attempt to find an element at the head of the standard fixture
   void test_size_standard()
   {  // setup
      //     p11      p26      p31
      //    +----+   +----+   +----+
      //    | 11 | - | 26 | - | 31 |
      //    +----+   +----+   +----+
      //     pHead
      Node <Spy>* p11, * p26, * p31;
      setupStandardFixture(p11, p26, p31);
      const Node <Spy>* pHead = p11;
      size_t s;
      Spy::reset();
      // exercise
      s = size(pHead);
      // verify
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      //     p11      p26      p31
      //    +----+   +----+   +----+
      //    | 11 | - | 26 | - | 31 |
      //    +----+   +----+   +----+
      assertUnit(s == 3);
      assertStandardFixture(p11);
      // teardown
      teardownStandardFixture(p11);
   }

   // attempt to find an element at the tail of the standard fixture
   void test_size_standardMiddle()
   {  // setup
      //     p11      p26      p31
      //    +----+   +----+   +----+
      //    | 11 | - | 26 | - | 31 |
      //    +----+   +----+   +----+
      //               pHead
      Node <Spy>* p11, * p26, * p31;
      setupStandardFixture(p11, p26, p31);
      const Node <Spy>* pHead = p26;
      size_t s;
      Spy::reset();
      // exercise
      s = size(pHead);
      // verify
      assertUnit(Spy::numEquals() == 0);  
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      //     p11      p26      p31
      //    +----+   +----+   +----+
      //    | 11 | - | 26 | - | 31 |
      //    +----+   +----+   +----+
      assertUnit(s == 2);
      assertStandardFixture(p11);
      // teardown
      teardownStandardFixture(p11);
   }

      
   /***************************************
    * FREE
    ***************************************/
  
   // attempt to free an empty linked list
   void test_clear_nullptr()
   {  // setup
      Node <Spy>* pList = nullptr;
      Spy::reset();
      // exercise
      clear(pList);
      // verify
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(pList == nullptr);
   }  // teardown
   
   // attempt to free a linked list consisting of one node
   void test_clear_one()
   {  // setup
      //     p26
      //    +----+
      //    | 26 |
      //    +----+
      Node <Spy>* p26 = new Node <Spy>(26);
      Spy::reset();
      // exercise
      clear(p26);
      // verify
      assertUnit(Spy::numDestructor() == 1);   // destroy [26]
      assertUnit(Spy::numDelete() == 1);       // delete [26]    
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(p26 == nullptr);
   }  // teardown

   // attempt to free the standard fixture
   void test_clear_standard()
   {  // setup
      //     p11      p26      p31
      //    +----+   +----+   +----+
      //    | 11 | - | 26 | - | 31 |
      //    +----+   +----+   +----+
      Node <Spy>* p11, * p26, * p31;
      setupStandardFixture(p11, p26, p31);
      Spy::reset();
      // exercise
      clear(p11);
      // verify
      assertUnit(Spy::numDestructor() == 3);   // destroy [11][26][31]
      assertUnit(Spy::numDelete() == 3);       // delete [11][26][31]    
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(p11 == nullptr);
   }  // teardown


   /*************************************************************
    * SETUP STANDARD FIXTURE
    *    +----+   +----+   +----+
    *    | 11 | - | 26 | - | 31 |
    *    +----+   +----+   +----+
    *************************************************************/
   void setupStandardFixture(Node <int>*& p11, Node <int>*& p26, Node <int>*& p31)
   {
      // allocate
      p11 = new Node <int>(11);
      p26 = new Node <int>(26);
      p31 = new Node <int>(31);

      // set next pointers
      p11->pNext = p26;
      p26->pNext = p31;

      // set previous pointers
      p26->pPrev = p11;
      p31->pPrev = p26;
   }

   /*************************************************************
    * SETUP STANDARD FIXTURE
    *    +----+   +----+   +----+
    *    | 11 | - | 26 | - | 31 |
    *    +----+   +----+   +----+
    *************************************************************/
   void setupStandardFixture(Node <Spy>*& p11, Node <Spy>*& p26, Node <Spy>*& p31)
   {
      // allocate
      p11 = new Node <Spy>(Spy(11));
      p26 = new Node <Spy>(Spy(26));
      p31 = new Node <Spy>(Spy(31));

      // set next pointers
      p11->pNext = p26;
      p26->pNext = p31;

      // set previous pointers
      p26->pPrev = p11;
      p31->pPrev = p26;
   }

   /*************************************************************
    * VERIFY EMPTY FIXTURE
    *************************************************************/
   void assertEmptyFixtureParameters(const Node <Spy>* p, int line, const char* function)
   {
      assertIndirect(p != nullptr);

      // Verify empty
      if (p)
      {
         assertIndirect(p->data == Spy());
         assertIndirect(p->pPrev == nullptr);
         assertIndirect(p->pNext == nullptr);
      }
   }

   /*************************************************************
    * VERIFY STANDARD FIXTURE
    *    +----+   +----+   +----+
    *    | 11 | - | 26 | - | 31 |
    *    +----+   +----+   +----+
    *************************************************************/
   void assertStandardFixtureParameters(const Node <int>* p, int line, const char* function)
   {
      assertIndirect(p != nullptr);

      // Verify 11
      if (p)
      { 
         assertIndirect(p->data == 11);
         assertIndirect(p->pPrev == nullptr);
         assertIndirect(p->pNext != nullptr);

         // verify 26
         if (p->pNext)
         {
            assertIndirect(p->pNext->data == 26);
            assertIndirect(p->pNext->pPrev == p);
            assertIndirect(p->pNext->pNext != nullptr);

            // verify 31
            if (p->pNext->pNext)
            {
               assertIndirect(p->pNext->pNext->data == 31);
               assertIndirect(p->pNext->pNext->pPrev == p->pNext);
               assertIndirect(p->pNext->pNext->pNext == nullptr);
            }
         }
      }
   }

   /*************************************************************
    * VERIFY STANDARD FIXTURE
    *    +----+   +----+   +----+
    *    | 11 | - | 26 | - | 31 |
    *    +----+   +----+   +----+
    *************************************************************/
   void assertStandardFixtureParameters(const Node <Spy>* p, int line, const char* function)
   {
      assertIndirect(p != nullptr);

      // Verify 11
      if (p)
      {
         assertIndirect(p->data == Spy(11));
         assertIndirect(p->pPrev == nullptr);
         assertIndirect(p->pNext != nullptr);

         // verify 26
         if (p->pNext)
         {
            assertIndirect(p->pNext->data == Spy(26));
            assertIndirect(p->pNext->pPrev == p);
            assertIndirect(p->pNext->pNext != nullptr);

            // verify 31
            if (p->pNext->pNext)
            {
               assertIndirect(p->pNext->pNext->data == Spy(31));
               assertIndirect(p->pNext->pNext->pPrev == p->pNext);
               assertIndirect(p->pNext->pNext->pNext == nullptr);
            }
         }
      }
   }

   /*************************************************************
    * TEARDOWN STANDARD FIXTURE
    *    +----+   +----+   +----+
    *    |    | - |    | - |    |
    *    +----+   +----+   +----+
    *************************************************************/
   void teardownStandardFixture(Node <int>* pRoot)
   {
      if (nullptr != pRoot)
      {
         if (nullptr != pRoot->pNext)
         {
            if (nullptr != pRoot->pNext->pNext)
               delete pRoot->pNext->pNext;
            delete pRoot->pNext;
         }
         delete pRoot;
      }
   }

   /*************************************************************
    * TEARDOWN STANDARD FIXTURE
    *    +----+   +----+   +----+
    *    |    | - |    | - |    |
    *    +----+   +----+   +----+
    *************************************************************/
   void teardownStandardFixture(Node <Spy>* pRoot)
   {
      if (nullptr != pRoot)
      {
         if (nullptr != pRoot->pNext && pRoot->pNext != pRoot)
         {
            if (nullptr != pRoot->pNext->pNext && pRoot->pNext->pNext != pRoot->pNext && pRoot->pNext->pNext != pRoot)
               delete pRoot->pNext->pNext;
            delete pRoot->pNext;
         }
         delete pRoot;
      }
   }

};

#endif // DEBUG
