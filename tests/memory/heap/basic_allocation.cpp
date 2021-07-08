/********************************************************************************
 *  File Name:
 *    basic_allocation.cpp
 *
 *  Description:
 *    Tests basic allocation of functionality
 *
 *  2021 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

/* STL Includes */
#include <algorithm>
#include <vector>

/* Testing Includes */
#include <tests/headers.hpp>
#include <tests/memory/heap/heap_fixture.hpp>

namespace Aurora::Test::Heap
{
  TEST_F( DefaultHeapFixture, Creation )
  {
    initializeHeap( 512 );

    /*-------------------------------------------------
    On initialization, a first block is created at the
    start of the heap memory, limiting the amount that
    can be used at startup.
    -------------------------------------------------*/
    GTEST_ASSERT_EQ( 512 - sizeof( Aurora::Memory::BlockLink_t ), mHeap.available() );
  }

  TEST_F( DefaultHeapFixture, AllocateProperSize )
  {
    static constexpr size_t HEAP_MAX       = 1024;
    static constexpr size_t HEAP_ALLOC_MAX = 100 - sizeof( Aurora::Memory::BlockLink_t );

    initializeHeap( HEAP_MAX );
    const size_t allocSize  = random_value( HEAP_ALLOC_MAX );
    const size_t startSize  = mHeap.available();
    const size_t expectSize = startSize - ( allocSize + sizeof( Aurora::Memory::BlockLink_t ) );

    void *memory = mHeap.malloc( allocSize );

    GTEST_ASSERT_NE( memory, nullptr );
    GTEST_ASSERT_EQ( mHeap.available(), expectSize );
  }

  TEST_F( DefaultHeapFixture, AllocAndFree )
  {
    static constexpr size_t HEAP_MAX       = 1024;
    static constexpr size_t HEAP_ALLOC_MAX = 100 - sizeof( Aurora::Memory::BlockLink_t );

    initializeHeap( HEAP_MAX );
    const size_t allocSize  = random_value( HEAP_ALLOC_MAX );
    const size_t startSize  = mHeap.available();

    void *memory = mHeap.malloc( allocSize );
    GTEST_ASSERT_NE( memory, nullptr );

    mHeap.free( memory );
    GTEST_ASSERT_EQ( mHeap.available(), startSize );
    GTEST_ASSERT_EQ( mHeap.allocated(), mHeap.freed() );
  }


  TEST_F( DefaultHeapFixture, RandomSizedLinearAllocAndFree )
  {
    static constexpr size_t HEAP_MAX       = 5 * 1024 * 1024;
    static constexpr size_t HEAP_ALLOC_MAX = 512;

    initializeHeap( HEAP_MAX );
    std::vector<void*> memory_allocated;

    /*-------------------------------------------------
    Fill the entire memory space
    -------------------------------------------------*/
    void *ptr = mHeap.malloc( random_value( HEAP_ALLOC_MAX ) );
    while( ptr )
    {
      memory_allocated.push_back( ptr );
      ptr = mHeap.malloc( random_value( HEAP_ALLOC_MAX ) );
    }

    /*-------------------------------------------------
    Free the whole memory space
    -------------------------------------------------*/
    for( auto mem : memory_allocated )
    {
      mHeap.free( mem );
    }

    GTEST_ASSERT_EQ( mHeap.allocated(), mHeap.freed() );
  }


  TEST_F( DefaultHeapFixture, RandomSizedAllocAndRandomFree )
  {
    static constexpr size_t HEAP_MAX       = 5 * 1024 * 1024;
    static constexpr size_t HEAP_ALLOC_MAX = 512;

    initializeHeap( HEAP_MAX );
    std::vector<void*> memory_allocated;

    /*-------------------------------------------------
    Fill the entire memory space
    -------------------------------------------------*/
    void *ptr = mHeap.malloc( random_value( HEAP_ALLOC_MAX ) );

    while( ptr )
    {
      memory_allocated.push_back( ptr );
      ptr = mHeap.malloc( random_value( HEAP_ALLOC_MAX ) );

      /*-------------------------------------------------
      50/50 split chance of deleting a memory location
      -------------------------------------------------*/
      if( ( memory_allocated.size() > 50 ) && ( random_value( HEAP_ALLOC_MAX ) > 255 ) )
      {
        size_t random_location = random_value( memory_allocated.size() - 1 );
        void *mem_to_free      = memory_allocated[ random_location ];

        mHeap.free( mem_to_free );
        memory_allocated.erase( std::remove( memory_allocated.begin(), memory_allocated.end(), mem_to_free ), memory_allocated.end() );
      }
    }

    /*-------------------------------------------------
    Free the whole memory space
    -------------------------------------------------*/
    for( auto mem : memory_allocated )
    {
      mHeap.free( mem );
    }

    GTEST_ASSERT_EQ( mHeap.allocated(), mHeap.freed() );
  }

}  // namespace Aurora::Test::Heap
