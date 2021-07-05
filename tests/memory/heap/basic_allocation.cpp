/********************************************************************************
 *  File Name:
 *    basic_allocation.cpp
 *
 *  Description:
 *    Tests basic allocation of functionality
 *
 *  2021 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

/* Testing Includes */
#include <tests/headers.hpp>
#include <tests/memory/heap/heap_fixture.hpp>

namespace Aurora::Test::Heap
{
  TEST_F( DefaultFixture, Creation )
  {
    initializeHeap( 512 );

    /*-------------------------------------------------
    On initialization, a first block is created at the
    start of the heap memory, limiting the amount that
    can be used at startup.
    -------------------------------------------------*/
    GTEST_ASSERT_EQ( 512 - sizeof( TestBlockLink_t ), mHeap.available() );
  }

  TEST_F( DefaultFixture, AllocateProperSize )
  {
    static constexpr size_t HEAP_MAX       = 1024;
    static constexpr size_t HEAP_ALLOC_MAX = 100 - sizeof( TestBlockLink_t );

    initializeHeap( HEAP_MAX );
    const size_t allocSize  = random_value( HEAP_ALLOC_MAX );
    const size_t startSize  = mHeap.available();
    const size_t expectSize = startSize - ( allocSize + sizeof( TestBlockLink_t ) );

    void *memory = mHeap.malloc( allocSize );

    GTEST_ASSERT_NE( memory, nullptr );
    GTEST_ASSERT_EQ( mHeap.available(), expectSize );
  }

  TEST_F( DefaultFixture, AllocAndFree )
  {
    static constexpr size_t HEAP_MAX       = 1024;
    static constexpr size_t HEAP_ALLOC_MAX = 100 - sizeof( TestBlockLink_t );

    initializeHeap( HEAP_MAX );
    const size_t allocSize  = random_value( HEAP_ALLOC_MAX );
    const size_t startSize  = mHeap.available();
    const size_t startAlloc = mHeap.allocated();

    void *memory = mHeap.malloc( allocSize );
    GTEST_ASSERT_NE( memory, nullptr );

    mHeap.free( memory );
    GTEST_ASSERT_EQ( mHeap.available(), startSize );
    GTEST_ASSERT_EQ( mHeap.allocated(), mHeap.freed() );
  }

}  // namespace Aurora::Test::Heap
