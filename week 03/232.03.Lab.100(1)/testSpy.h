/***********************************************************************
 * Header:
 *    TEST SPY
 * Summary:
 *    Unit tests for the spy
 * Author
 *    Br. Helfrich
 ************************************************************************/

#pragma once
#ifdef DEBUG

#include "spy.h"        // class under test
#include "unitTest.h"   // unit test baseclass

/***********************************************
 * TEST SPY
 * Unit tests for the Spy class
 ***********************************************/
class TestSpy : public UnitTest
{
   
public:
   void run()
   {
      reset();
      
      // Constructor
      test_constructorDefault();
      test_constructorNondefault();
      
      // Destructor
      test_destructor_empty();
      test_destructor_full();
      
      // Copy Constructor
      test_constructorCopy_empty();
      test_constructorCopy_full();
      
      // Move Constructor
      test_constructorMove_empty();
      test_constructorMove_full();
      
      // Copy Assignment Operator
      test_assignCopy_emptyToEmpty();
      test_assignCopy_fullToEmpty();
      test_assignCopy_emptyToFull();
      test_assignCopy_fullToFull();

      // Assign Move
      test_assignMove_emptyToEmpty();
      test_assignMove_fullToEmpty();
      test_assignMove_emptyToFull();
      test_assignMove_fullToFull();
      
      // Equivalence
      test_equivalence_emptyToEmpty();
      test_equivalence_fullToEmpty();
      test_equivalence_emptyToFull();
      test_equivalence_same();
      test_equivalence_firstSmaller();
      test_equivalence_firstLarger();
      
      // Less Than
      test_lessthan_emptyToEmpty();
      test_lessthan_fullToEmpty();
      test_lessthan_emptyToFull();
      test_lessthan_same();
      test_lessthan_firstSmaller();
      test_lessthan_firstLarger();
  
      report("Spy");
   }
   
   
   /***************************************
    * CONSTRUCTOR
    *    Spy::Spy()
    *    Spy::Spy(int)
    ***************************************/
   
   // default constructor: create a default spy without allocation
   void test_constructorDefault()
   {  // setup
      Spy::reset();
      // exercise
      Spy s;
      // verify
      assertUnit(Spy::numAlloc() == 0);     // nothing allocated
      assertUnit(Spy::numDelete() == 0);    // nothing deleted
      assertUnit(Spy::numDefault() == 1);   // s
      assertUnit(Spy::numNondefault() == 0);// non-default constructor not called
      assertUnit(s.p == nullptr);
   }  // teardown
   
   // non-default constructor: create a spy with the value 99 initialized.
   void test_constructorNondefault()
   {  // setup
      Spy::reset();
      // exercise
      Spy s(99);
      // verify
      assertUnit(Spy::numAlloc() == 1);     // s
      assertUnit(Spy::numDelete() == 0);    // nothing deleted
      assertUnit(Spy::numNondefault() == 1);// s
      assertUnit(Spy::numDefault() == 0);   // default constructor not called
      assertUnit(s.p != nullptr);
      if (s.p)
         assertUnit(*(s.p) == 99);
      // teardown
      if (s.p)
         delete s.p;
      s.p = nullptr;
   }

   /***************************************
    * Destructor
    *    Spy::~Spy()
    ***************************************/
   
   // delete a default and empty spy
   void test_destructor_empty()
   {  // setup
      {
         Spy s;
         Spy::reset();
      }  // exercise
      // verify
      assertUnit(Spy::numAlloc() == 0);      // nothing allocated
      assertUnit(Spy::numDelete() == 0);     // nothing deleted
      assertUnit(Spy::numDestructor() == 1); // s
   }  // teardown
   
   // delete a full spy
   void test_destructor_full()
   {  // setup
      {
         Spy s(99);
         Spy::reset();
      }  // exercise
      // verify
      assertUnit(Spy::numAlloc() == 0);      // nothing allocated
      assertUnit(Spy::numDelete() == 1);     // s
      assertUnit(Spy::numDestructor() == 1); // s
   }  // teardown
   
   /***************************************
    * COPY Constructor
    * The regular (non-move) edition of the copy constructor
    *   Spy::Spy(const Spy &)
    ***************************************/
   
   // copy a default and empty spy
   void test_constructorCopy_empty()
   {  // setup
      Spy sSrc;
      Spy::reset();
      // exercise
      Spy sDes(sSrc);
      // verify
      assertUnit(Spy::numAlloc() == 0);      // nothing allocated
      assertUnit(Spy::numDelete() == 0);     // nothing deleted
      assertUnit(Spy::numDefault() == 0);    // default constructor not called
      assertUnit(Spy::numCopyMove() == 0);   // default constructor not called
      assertUnit(Spy::numAssign() == 0);     // assignment operator not called
      assertUnit(Spy::numAssignMove() == 0); // move version of the assignment operator not called
      assertUnit(Spy::numCopy() == 1);       // sDes
      assertUnit(sSrc.p == nullptr);
      assertUnit(sDes.p == nullptr);
   }  // teardown
   
   // copy with a filled spy
   void test_constructorCopy_full()
   {  // setup
      Spy sSrc(99);
      Spy::reset();
      // exercise
      Spy sDes(sSrc);
      // verify
      assertUnit(Spy::numAlloc() == 1);      // sDes
      assertUnit(Spy::numDelete() == 0);     // nothing deleted
      assertUnit(Spy::numDefault() == 0);    // default constructor not called
      assertUnit(Spy::numCopyMove() == 0);   // default constructor not called
      assertUnit(Spy::numAssign() == 0);     // assignment operator not called
      assertUnit(Spy::numAssignMove() == 0); // move version of the assignment operator not called
      assertUnit(Spy::numCopy() == 1);       // sDes
      assertUnit(sSrc.p != nullptr);
      if (sSrc.p)
         assertUnit(*(sSrc.p) == 99);
      assertUnit(sDes.p != nullptr);
      if (sDes.p)
         assertUnit(*(sDes.p) == 99);
      // teardown
      if (sSrc.p)
         delete sSrc.p;
      if (sDes.p)
         delete sDes.p;
      sSrc.p = sDes.p = nullptr;
   }

   /***************************************
    * MOVE CONSTRUCTOR
    * The move (steal) edition of the copy constructor
    *     Spy::Spy(Spy &&)
    ***************************************/
   
   // move constructor a default spy
   void test_constructorMove_empty()
   {  // setup
      Spy sSrc;
      Spy::reset();
      // exercise
      Spy sDes(std::move(sSrc));
      // verify
      assertUnit(Spy::numAlloc() == 0);      // nothing allocated
      assertUnit(Spy::numDelete() == 0);     // nothing deleted
      assertUnit(Spy::numDefault() == 0);    // default constructor not called
      assertUnit(Spy::numCopyMove() == 1);   // sDes
      assertUnit(Spy::numAssign() == 0);     // assignment operator not called
      assertUnit(Spy::numAssignMove() == 0); // move version of the assignment operator not called
      assertUnit(sSrc.p == nullptr);
      assertUnit(sDes.p == nullptr);
   }  // teardown
   
   // move constructor with a filled rhs
   void test_constructorMove_full()
   {  // setup
      Spy sSrc(99);
      Spy::reset();
      // exercise
      Spy sDes(std::move(sSrc));
      // verify
      assertUnit(Spy::numAlloc() == 0);      // nothing allocated
      assertUnit(Spy::numDelete() == 0);     // nothing deleted
      assertUnit(Spy::numDefault() == 0);    // default constructor not called
      assertUnit(Spy::numCopyMove() == 1);   // sDes
      assertUnit(Spy::numAssign() == 0);     // assignment operator not called
      assertUnit(Spy::numAssignMove() == 0); // move version of the assignment operator not called
      assertUnit(sSrc.p == nullptr);
      assertUnit(sDes.p != nullptr);
      if (sDes.p)
         assertUnit(*(sDes.p) == 99);
      // teardown
      if (sDes.p)
         delete sDes.p;
      sDes.p = nullptr;
   }

   /***************************************
    * MOVE
    * The move (steal) edition of the assignment operator
    *     Spy::operator=(Spy &&)
    ***************************************/
   
   // assign-move an empty spy onto an empty spy
   void test_assignMove_emptyToEmpty()
   {  // setup
      Spy sSrc;
      Spy sDes;
      Spy::reset();
      // exercise
      sDes = std::move(sSrc);
      // verify
      assertUnit(Spy::numAlloc() == 0);      // nothing allocated
      assertUnit(Spy::numDelete() == 0);     // nothing deleted
      assertUnit(Spy::numDefault() == 0);    // default constructor not called
      assertUnit(Spy::numCopyMove() == 0);   // move version of the copy constructor not called
      assertUnit(Spy::numAssign() == 0);     // assignment operator not called
      assertUnit(Spy::numAssignMove() == 1); // sSrc --> sDes
      assertUnit(sSrc.p == nullptr);
      assertUnit(sDes.p == nullptr);
   }  // teardown
   
   // assign-move a full spy to an empty spy
   void test_assignMove_fullToEmpty()
   {  // setup
      Spy sSrc(99);
      Spy sDes;
      Spy::reset();
      // exercise
      sDes = std::move(sSrc);
      // verify
      assertUnit(Spy::numAlloc() == 0);      // nothing allocated
      assertUnit(Spy::numDelete() == 0);     // nothing deleted
      assertUnit(Spy::numDefault() == 0);    // default constructor not called
      assertUnit(Spy::numCopyMove() == 0);   // move version of the copy constructor not called
      assertUnit(Spy::numAssign() == 0);     // assignment operator not called
      assertUnit(Spy::numAssignMove() == 1); // sSrc --> sDes
      assertUnit(sSrc.p == nullptr);
      assertUnit(sDes.p != nullptr);
      if (sDes.p)
         assertUnit(*(sDes.p) == 99);
      // teardown
      if (sDes.p)
         delete sDes.p;
      sDes.p = nullptr;
   }

   // assign-move an empty spy onto a full spy
   void test_assignMove_emptyToFull()
   {  // setup
      Spy sSrc;
      Spy sDes(99);
      Spy::reset();
      // exercise
      sDes = std::move(sSrc);
      // verify
      assertUnit(Spy::numAlloc() == 0);      // nothing allocated
      assertUnit(Spy::numDelete() == 1);     // sDes
      assertUnit(Spy::numDefault() == 0);    // default constructor not called
      assertUnit(Spy::numCopyMove() == 0);   // move version of the copy constructor not called
      assertUnit(Spy::numAssign() == 0);     // assignment operator not called
      assertUnit(Spy::numAssignMove() == 1); // sSrc --> sDes
      assertUnit(sSrc.p == nullptr);
      assertUnit(sDes.p == nullptr);
   }  // teardown

   // assign-move a full spy onto one that is already full
   void test_assignMove_fullToFull()
   {  // setup
      Spy sSrc(99);
      Spy sDes(55);
      Spy::reset();
      // exercise
      sDes = std::move(sSrc);
      // verify
      assertUnit(Spy::numAlloc() == 0);      // nothing allocated
      assertUnit(Spy::numDelete() == 1);     // sDes
      assertUnit(Spy::numDefault() == 0);    // default constructor not called
      assertUnit(Spy::numCopyMove() == 0);   // move version of the copy constructor not called
      assertUnit(Spy::numAssign() == 0);     // assignment operator not called
      assertUnit(Spy::numAssignMove() == 1); // sSrc --> sDes
      assertUnit(sSrc.p == nullptr);
      assertUnit(sDes.p != nullptr);
      if (sDes.p)
         assertUnit(*(sDes.p) == 99);
      // teardown
      if (sDes.p)
         delete sDes.p;
      sDes.p = nullptr;
   }

   /***************************************
    * COPY ASSIGNMENT OPERATOR
    * The regular non-move version of the assignment operator
    *     Spy::operator=(const Spy &)
    ***************************************/
   
   // assign an empty spy onto an empty spy
   void test_assignCopy_emptyToEmpty()
   {  // setup
      Spy sSrc;
      Spy sDes;
      Spy::reset();
      // exercise
      sDes = sSrc;
      // verify
      assertUnit(Spy::numAlloc() == 0);      // nothing allocated
      assertUnit(Spy::numDelete() == 0);     // nothing deleted
      assertUnit(Spy::numDefault() == 0);    // default constructor not called
      assertUnit(Spy::numCopyMove() == 0);   // move version of the copy constructor not called
      assertUnit(Spy::numAssign() == 1);     // sSrc --> sDes
      assertUnit(Spy::numAssignMove() == 0); // move version of the assignment operator not called
      assertUnit(sSrc.p == nullptr);
      assertUnit(sDes.p == nullptr);
   }  // teardown
   
   // assign a full spy to an empty spy
   void test_assignCopy_fullToEmpty()
   {  // setup
      Spy sSrc(99);
      Spy sDes;
      Spy::reset();
      // exercise
      sDes = sSrc;
      // verify
      assertUnit(Spy::numAlloc() == 1);      // sDes
      assertUnit(Spy::numDelete() == 0);     // nothing deleted
      assertUnit(Spy::numDefault() == 0);    // default constructor not called
      assertUnit(Spy::numCopyMove() == 0);   // move version of the copy constructor not called
      assertUnit(Spy::numAssign() == 1);     // sSrc --> sDes
      assertUnit(Spy::numAssignMove() == 0); // move version of the assignment operator not called
      assertUnit(sSrc.p != nullptr);
      if (sSrc.p)
         assertUnit(*(sSrc.p) == 99);
      assertUnit(sDes.p != nullptr);
      if (sDes.p)
         assertUnit(*(sDes.p) == 99);
      // teardown
      if (sDes.p)
         delete sDes.p;
      if (sSrc.p)
         delete sSrc.p;
      sDes.p = sSrc.p = nullptr;
   }
   
   // assign an empty spy onto a full spy
   void test_assignCopy_emptyToFull()
   {  // setup
      Spy sSrc;
      Spy sDes(99);
      Spy::reset();
      // exercise
      sDes = sSrc;
      // verify
      assertUnit(Spy::numAlloc() == 0);      // nothing allocated
      assertUnit(Spy::numDelete() == 1);     // sDes
      assertUnit(Spy::numDefault() == 0);    // default constructor not called
      assertUnit(Spy::numCopyMove() == 0);   // move version of the copy constructor not called
      assertUnit(Spy::numAssign() == 1);     // sSrc --> sDes
      assertUnit(Spy::numAssignMove() == 0); // move version of the assignment operator not called
      assertUnit(sSrc.p == nullptr);
      assertUnit(sDes.p == nullptr);
   }  // teardown
   
   // assign a full spy onto one that is already full
   void test_assignCopy_fullToFull()
   {  // setup
      Spy sSrc(99);
      Spy sDes(55);
      Spy::reset();
      // exercise
      sDes = sSrc;
      // verify
      assertUnit(Spy::numAlloc() == 0);      // nothing allocated
      assertUnit(Spy::numDelete() == 0);     // nothing deleted
      assertUnit(Spy::numDefault() == 0);    // default constructor not called
      assertUnit(Spy::numCopyMove() == 0);   // move version of the copy constructor not called
      assertUnit(Spy::numAssign() == 1);     // sSrc --> sDes
      assertUnit(Spy::numAssignMove() == 0); // move version of the assignment operator not called
      assertUnit(sSrc.p != nullptr);
      if (sSrc.p)
         assertUnit(*(sSrc.p) == 99);
      assertUnit(sDes.p != nullptr);
      if (sDes.p)
         assertUnit(*(sDes.p) == 99);
      // teardown
      if (sSrc.p)
         delete sSrc.p;
      if (sDes.p)
         delete sDes.p;
      sDes.p = sSrc.p = nullptr;
   }

   /***************************************
    * EQUIVALENCE
    * See if two things are the same
    *     Spy::operator==(const Spy &)
    ***************************************/
   
   // empty == empty
   void test_equivalence_emptyToEmpty()
   {  // setup
      Spy sSrc;
      Spy sDes;
      Spy::reset();
      // exercise
      bool value = (sSrc == sDes);
      // verify
      assertUnit(value == true);
      assertUnit(Spy::numEquals() == 1);     // sSrc == sDes
      assertUnit(Spy::numAlloc() == 0);      // nothing allocated
      assertUnit(Spy::numDelete() == 0);     // nothing deleted
      assertUnit(Spy::numDefault() == 0);    // default constructor not called
      assertUnit(Spy::numCopyMove() == 0);   // move version of the copy constructor not called
      assertUnit(Spy::numAssign() == 0);     // the assignment operator not called
      assertUnit(Spy::numAssignMove() == 0); // move version of the assignment operator not called
   }  // teardown
   
   // 99 == empty
   void test_equivalence_fullToEmpty()
   {  // setup
      Spy sSrc(99);
      Spy sDes;
      Spy::reset();
      // exercise
      bool value = (sSrc == sDes);
      // verify
      assertUnit(value == false);
      assertUnit(Spy::numEquals() == 1);     // sSrc == sDes
      assertUnit(Spy::numAlloc() == 0);      // nothing allocated
      assertUnit(Spy::numDelete() == 0);     // nothing deleted
      assertUnit(Spy::numDefault() == 0);    // default constructor not called
      assertUnit(Spy::numCopyMove() == 0);   // move version of the copy constructor not called
      assertUnit(Spy::numAssign() == 0);     // the assignment operator not called
      assertUnit(Spy::numAssignMove() == 0); // move version of the assignment operator not called
      // teardown
      if (sSrc.p)
         delete sSrc.p;
      sSrc.p = nullptr;
   }

   // empty == 99
   void test_equivalence_emptyToFull()
   {
      Spy sSrc;
      Spy sDes(99);
      Spy::reset();
      // exercise
      bool value = (sSrc == sDes);
      // verify
      assertUnit(value == false);
      assertUnit(Spy::numEquals() == 1);     // sSrc == sDes
      assertUnit(Spy::numAlloc() == 0);      // nothing allocated
      assertUnit(Spy::numDelete() == 0);     // nothing deleted
      assertUnit(Spy::numDefault() == 0);    // default constructor not called
      assertUnit(Spy::numCopyMove() == 0);   // move version of the copy constructor not called
      assertUnit(Spy::numAssign() == 0);     // the assignment operator not called
      assertUnit(Spy::numAssignMove() == 0); // move version of the assignment operator not called
      // teardown
      if (sDes.p)
         delete sDes.p;
      sDes.p = nullptr;
   }
   
   // 99 == 99
   void test_equivalence_same()
   {
      Spy sSrc(99);
      Spy sDes(99);
      Spy::reset();
      // exercise
      bool value = (sSrc == sDes);
      // verify
      assertUnit(value == true);
      assertUnit(Spy::numEquals() == 1);     // sSrc == sDes
      assertUnit(Spy::numAlloc() == 0);      // nothing allocated
      assertUnit(Spy::numDelete() == 0);     // nothing deleted
      assertUnit(Spy::numDefault() == 0);    // default constructor not called
      assertUnit(Spy::numCopyMove() == 0);   // move version of the copy constructor not called
      assertUnit(Spy::numAssign() == 0);     // the assignment operator not called
      assertUnit(Spy::numAssignMove() == 0); // move version of the assignment operator not called
      // teardown
      if (sSrc.p)
         delete sSrc.p;
      if (sDes.p)
         delete sDes.p;
      sDes.p = sSrc.p = nullptr;
   }
   
   // 9 == 99
   void test_equivalence_firstSmaller()
   {
      Spy sSrc(9);
      Spy sDes(99);
      Spy::reset();
      // exercise
      bool value = (sSrc == sDes);
      // verify
      assertUnit(value == false);
      assertUnit(Spy::numEquals() == 1);     // sSrc == sDes
      assertUnit(Spy::numAlloc() == 0);      // nothing allocated
      assertUnit(Spy::numDelete() == 0);     // nothing deleted
      assertUnit(Spy::numDefault() == 0);    // default constructor not called
      assertUnit(Spy::numCopyMove() == 0);   // move version of the copy constructor not called
      assertUnit(Spy::numAssign() == 0);     // the assignment operator not called
      assertUnit(Spy::numAssignMove() == 0); // move version of the assignment operator not called
      // teardown
      if (sSrc.p)
         delete sSrc.p;
      if (sDes.p)
         delete sDes.p;
      sDes.p = sSrc.p = nullptr;
   }
   
   // 99 == 9
   void test_equivalence_firstLarger()
   {
      Spy sSrc(99);
      Spy sDes(9);
      Spy::reset();
      // exercise
      bool value = (sSrc == sDes);
      // verify
      assertUnit(value == false);
      assertUnit(Spy::numEquals() == 1);     // sSrc == sDes
      assertUnit(Spy::numAlloc() == 0);      // nothing allocated
      assertUnit(Spy::numDelete() == 0);     // nothing deleted
      assertUnit(Spy::numDefault() == 0);    // default constructor not called
      assertUnit(Spy::numCopyMove() == 0);   // move version of the copy constructor not called
      assertUnit(Spy::numAssign() == 0);     // the assignment operator not called
      assertUnit(Spy::numAssignMove() == 0); // move version of the assignment operator not called
      // teardown
      if (sSrc.p)
         delete sSrc.p;
      if (sDes.p)
         delete sDes.p;
      sDes.p = sSrc.p = nullptr;
   }

   /***************************************
    * LESS-THAN
    * See if two things are the same
    *     Spy::operator<(const Spy &)
    ***************************************/
   
   // empty < empty
   void test_lessthan_emptyToEmpty()
   {  // setup
      Spy sSrc;
      Spy sDes;
      Spy::reset();
      // exercise
      bool value = (sSrc < sDes);
      // verify
      assertUnit(value == false);
      assertUnit(Spy::numLessthan() == 1);   // sSrc < sDes
      assertUnit(Spy::numAlloc() == 0);      // nothing allocated
      assertUnit(Spy::numDelete() == 0);     // nothing deleted
      assertUnit(Spy::numDefault() == 0);    // default constructor not called
      assertUnit(Spy::numCopyMove() == 0);   // move version of the copy constructor not called
      assertUnit(Spy::numAssign() == 0);     // the assignment operator not called
      assertUnit(Spy::numAssignMove() == 0); // move version of the assignment operator not called
   }  // teardown
   
   // 99 < empty
   void test_lessthan_fullToEmpty()
   {  // setup
      Spy sSrc(99);
      Spy sDes;
      Spy::reset();
      // exercise
      bool value = (sSrc < sDes);
      // verify
      assertUnit(value == false);
      assertUnit(Spy::numLessthan() == 1);   // sSrc < sDes
      assertUnit(Spy::numAlloc() == 0);      // nothing allocated
      assertUnit(Spy::numDelete() == 0);     // nothing deleted
      assertUnit(Spy::numDefault() == 0);    // default constructor not called
      assertUnit(Spy::numCopyMove() == 0);   // move version of the copy constructor not called
      assertUnit(Spy::numAssign() == 0);     // the assignment operator not called
      assertUnit(Spy::numAssignMove() == 0); // move version of the assignment operator not called
      // teardown
      if (sSrc.p)
         delete sSrc.p;
      sSrc.p = nullptr;
   }
   
   // empty < 99
   void test_lessthan_emptyToFull()
   {
      Spy sSrc;
      Spy sDes(99);
      Spy::reset();
      // exercise
      bool value = (sSrc < sDes);
      // verify
      assertUnit(value == true);
      assertUnit(Spy::numLessthan() == 1);   // sSrc < sDes
      assertUnit(Spy::numAlloc() == 0);      // nothing allocated
      assertUnit(Spy::numDelete() == 0);     // nothing deleted
      assertUnit(Spy::numDefault() == 0);    // default constructor not called
      assertUnit(Spy::numCopyMove() == 0);   // move version of the copy constructor not called
      assertUnit(Spy::numAssign() == 0);     // the assignment operator not called
      assertUnit(Spy::numAssignMove() == 0); // move version of the assignment operator not called
      // teardown
      if (sDes.p)
         delete sDes.p;
      sDes.p = nullptr;
   }
   
   // 99 < 99
   void test_lessthan_same()
   {
      Spy sSrc(99);
      Spy sDes(99);
      Spy::reset();
      // exercise
      bool value = (sSrc < sDes);
      // verify
      assertUnit(value == false);
      assertUnit(Spy::numLessthan() == 1);   // sSrc < sDes
      assertUnit(Spy::numAlloc() == 0);      // nothing allocated
      assertUnit(Spy::numDelete() == 0);     // nothing deleted
      assertUnit(Spy::numDefault() == 0);    // default constructor not called
      assertUnit(Spy::numCopyMove() == 0);   // move version of the copy constructor not called
      assertUnit(Spy::numAssign() == 0);     // the assignment operator not called
      assertUnit(Spy::numAssignMove() == 0); // move version of the assignment operator not called
      // teardown
      if (sSrc.p)
         delete sSrc.p;
      if (sDes.p)
         delete sDes.p;
      sDes.p = sSrc.p = nullptr;
   }
   
   // 9 < 99
   void test_lessthan_firstSmaller()
   {
      Spy sSrc(9);
      Spy sDes(99);
      Spy::reset();
      // exercise
      bool value = (sSrc < sDes);
      // verify
      assertUnit(value == true);
      assertUnit(Spy::numLessthan() == 1);   // sSrc < sDes
      assertUnit(Spy::numAlloc() == 0);      // nothing allocated
      assertUnit(Spy::numDelete() == 0);     // nothing deleted
      assertUnit(Spy::numDefault() == 0);    // default constructor not called
      assertUnit(Spy::numCopyMove() == 0);   // move version of the copy constructor not called
      assertUnit(Spy::numAssign() == 0);     // the assignment operator not called
      assertUnit(Spy::numAssignMove() == 0); // move version of the assignment operator not called
      // teardown
      if (sSrc.p)
         delete sSrc.p;
      if (sDes.p)
         delete sDes.p;
      sDes.p = sSrc.p = nullptr;
   }
   
   // 99 < 9
   void test_lessthan_firstLarger()
   {
      Spy sSrc(99);
      Spy sDes(9);
      Spy::reset();
      // exercise
      bool value = (sSrc < sDes);
      // verify
      assertUnit(value == false);
      assertUnit(Spy::numLessthan() == 1);   // sSrc < sDes
      assertUnit(Spy::numAlloc() == 0);      // nothing allocated
      assertUnit(Spy::numDelete() == 0);     // nothing deleted
      assertUnit(Spy::numDefault() == 0);    // default constructor not called
      assertUnit(Spy::numCopyMove() == 0);   // move version of the copy constructor not called
      assertUnit(Spy::numAssign() == 0);     // the assignment operator not called
      assertUnit(Spy::numAssignMove() == 0); // move version of the assignment operator not called
      // teardown
      if (sSrc.p)
         delete sSrc.p;
      if (sDes.p)
         delete sDes.p;
      sDes.p = sSrc.p = nullptr;
   }
};

#endif // DEBUG


