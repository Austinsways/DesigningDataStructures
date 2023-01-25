/***********************************************************************
 * Component:
 *    SPY
 * Author:
 *    Br. Helfrich
 * Summary:
 *    A mock class designed to measure its usage: a spy!
 ************************************************************************/

#pragma once

#include <cassert>

enum { ALLOC,      // 0  allocations, number of times NEW is called
       DELETE,     // 1  deletions, number of times DELETE is called
       DEFAULT,    // 2  Spy::Spy()
       NONDEFAULT, // 3  Spy::Spy(int)
       COPY,       // 4  Spy::Spy(const Spy &)
       COPY_MOVE,  // 5  Spy::Spy(Spy &&)
       DESTRUCTOR, // 6  Spy::~Spy()
       ASSIGN,     // 7  Spy::operator=(const Spy &)
       ASSIGN_MOVE,// 8  Spy::operator=(Spy &&)
       EQUALS,     // 9  Spy::operator==(const Spy &)
       LESSTHAN,   // 10 Spy::operator<(const Spy &)
       SWAP,       // 11 Spy::swap()
       NUM_MARKERS};

/*************************************************************
 * SPY
 * A mock class that records how it was used
 *************************************************************/
class Spy
{
public:
   // the member variable
   int * p;
   
   // default constructor: allocate a spot and assign to zero
   Spy() : p(nullptr) { counters[DEFAULT]++; }
   
   // non-default constructor: allocate a spot and assign to the value
   Spy(int value) : p(nullptr)
   {
      allocate();
      *p = value;
      counters[NONDEFAULT]++;
   }
   
   // copy constructor: make a new copy
   Spy(const Spy & rhs) : p(nullptr)
   {
      if (!rhs.empty())
      {
         allocate();
         *p = rhs.get();
      }
      counters[COPY]++;
   }
   
   // move constructor: steal the data from the RHS
   Spy(Spy && rhs) noexcept
   {
      if (!rhs.empty())
      {
         p = rhs.p;
         rhs.p = nullptr;
      }
      else
         p = nullptr;
      counters[COPY_MOVE]++;
   }
   
   // delete - remove the instance
   ~Spy()
   {
      if (!empty())
         unallocate();
      counters[DESTRUCTOR]++;
   }

   // copy assignment operator
   Spy & operator=(const Spy & rhs)
   {
      if (!rhs.empty())
      {
         if (empty())
            allocate();
         *p = rhs.get();
      }
      else if (!empty())
         unallocate();
      counters[ASSIGN]++;
      return *this;
   }
   
   // move assignment operator
   Spy & operator=(Spy && rhs) noexcept
   {
      if (!empty())
         unallocate();
      p = rhs.p;
      rhs.p = nullptr;
      counters[ASSIGN_MOVE]++;
      return *this;
   }
   
   // swap
   void swap(Spy & rhs) noexcept
   {
      int * pTemp = rhs.p;
      rhs.p = p;
      p = pTemp;
      counters[SWAP]++;
   }
   
   // is this pointer empty?
   bool empty() const noexcept
   {
      return p == nullptr;
   }
   
   // fetch the value
   int get() const noexcept
   {
      return *p;
   }
   
   // set the value
   void set(int value)
   {
      if (empty())
         allocate();
      *p = value;
   }
   
   // compare the values
   bool operator==(const Spy & rhs) const noexcept
   {
      counters[EQUALS]++;
      if (rhs.empty() && empty())
         return true;
      if (!rhs.empty() && !empty())
         return get() == rhs.get();
      return false;
   }
   
   // a null value is assumed to be the smallest value
   bool operator<(const Spy & rhs) const noexcept
   {
      counters[LESSTHAN]++;
      if (rhs.empty() && empty())
         return false;
      if (!rhs.empty() && !empty())
         return get() < rhs.get();
      if (empty())
         return true;
      else
         return false;
   }
   
   // reset the counters for a new test
   static void reset() noexcept
   {
      for (int i = 0; i < NUM_MARKERS; i++)
         counters[i] = 0;
   }
   
   static int numAlloc()       { return counters[ALLOC];      }
   static int numDelete()      { return counters[DELETE];     }
   static int numDefault()     { return counters[DEFAULT];    }
   static int numNondefault()  { return counters[NONDEFAULT]; }
   static int numCopy()        { return counters[COPY];       }
   static int numCopyMove()    { return counters[COPY_MOVE];  }
   static int numDestructor()  { return counters[DESTRUCTOR]; }
   static int numAssign()      { return counters[ASSIGN];     }
   static int numAssignMove()  { return counters[ASSIGN_MOVE];}
   static int numEquals()      { return counters[EQUALS];     }
   static int numLessthan()    { return counters[LESSTHAN];   }
   static int numSwap()        { return counters[SWAP];       }

   // keep track of how it is used
   static int counters[NUM_MARKERS];
private:
   
   // allocate a new buffer
   void allocate()
   {
      assert(p == nullptr);
      p = new int;
      counters[ALLOC]++;
   }
   
   // free the buffer
   void unallocate()
   {
      assert(p != nullptr);
      delete p;
      p = nullptr;
      counters[DELETE]++;
   }
   
};

// non-member-function version of Spy.swap()
inline void swap(Spy & s1, Spy & s2)
{
   s1.swap(s2);
}
