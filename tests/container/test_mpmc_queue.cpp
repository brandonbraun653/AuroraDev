/********************************************************************************
 *  File Name:
 *    stream_buffer_tests.cpp
 *
 *  Description:
 *    Basic testing for the Stream Buffer interface
 *
 *  2021 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

/* Aurora Includes */
#include <Aurora/container>

/* Testing Includes */
#include <tests/headers.hpp>
#include <tests/container/fixture_mpmc_queue.hpp>

namespace Aurora::Test::MPMCQueue
{
  TEST( MPMC_RobustTests, InvalidInit )
  {
    auto attr = Aurora::Container::MPMCAttr<TestObj>();
    auto queue = Aurora::Container::MPMCQueue<TestObj>();

    GTEST_ASSERT_EQ( false, queue.init( attr ) );
  }

  TEST( MPMC_RobustTests, AttrNullElements )
  {
    auto attr = Aurora::Container::MPMCAttr<TestObj>();
    GTEST_ASSERT_EQ( false, attr.init( 0 ) );
  }

  TEST( MPMC_RobustTests, AttrMissingMem )
  {
    auto cb   = Aurora::Container::CircularBuffer<TestObj>();
    auto mtx  = Chimera::Thread::RecursiveMutex();
    auto sig  = Chimera::Interrupt::SIGNAL_INVALID;
    auto attr = Aurora::Container::MPMCAttr<TestObj>();

    GTEST_ASSERT_EQ( false, attr.init( nullptr, nullptr, sig ) );
    GTEST_ASSERT_EQ( false, attr.init( nullptr, &mtx, sig ) );
    GTEST_ASSERT_EQ( false, attr.init( &cb, nullptr, sig ) );
  }

  TEST_F( MPMCFixture, GeneralSizeAttributes )
  {
    GTEST_ASSERT_EQ( true, test_attr.valid() );
    GTEST_ASSERT_EQ( true, test_queue.empty() );
    GTEST_ASSERT_EQ( true, test_queue.empty( false ) );
    GTEST_ASSERT_GE( test_queue.capacity(), MIN_TEST_ELEMENTS );
    GTEST_ASSERT_GE( test_queue.capacity( false ), MIN_TEST_ELEMENTS );
    GTEST_ASSERT_EQ( test_queue.available(), test_queue.capacity() );
    GTEST_ASSERT_EQ( test_queue.available(), test_queue.capacity( false ) );
    GTEST_ASSERT_EQ( 0, test_queue.size() );
    GTEST_ASSERT_EQ( 0, test_queue.size( false ) );
  }

  TEST_F( MPMCFixture, PushBadInputs )
  {
    TestObj dummy;
    GTEST_ASSERT_EQ( test_queue.push( nullptr, 10 ), 0 );
    GTEST_ASSERT_EQ( test_queue.push( &dummy, 0 ), 0 );
  }

  TEST_F( MPMCFixture, PopBadInputs )
  {
    TestObj dummy;
    GTEST_ASSERT_EQ( test_queue.pop( nullptr, 10 ), 0 );
    GTEST_ASSERT_EQ( test_queue.pop( &dummy, 0 ), 0 );
  }

  TEST_F( MPMCFixture, PushPopConditions )
  {
    TestObj dummy;
    dummy.item0 = 1.0f;

    /*-------------------------------------------------------------------------
    Validate pushing until full is ok
    -------------------------------------------------------------------------*/
    while( !test_queue.full() )
    {
      GTEST_ASSERT_EQ( test_queue.push( &dummy, 1 ), 1 );
    }

    /*-------------------------------------------------------------------------
    Once full, validate reported sizing
    -------------------------------------------------------------------------*/
    GTEST_ASSERT_EQ( test_queue.full(), true );
    GTEST_ASSERT_EQ( test_queue.size(), test_queue.capacity() );

    /*-------------------------------------------------------------------------
    Try to push one more time and validate it fails
    -------------------------------------------------------------------------*/
    GTEST_ASSERT_EQ( test_queue.push( &dummy, 1 ), 0 );

    /*-------------------------------------------------------------------------
    Pop all the data off
    -------------------------------------------------------------------------*/
    while( !test_queue.empty() )
    {
      GTEST_ASSERT_EQ( test_queue.pop( &dummy, 1 ), 1 );
    }

    /*-------------------------------------------------------------------------
    Once empty, validate sizing
    -------------------------------------------------------------------------*/
    GTEST_ASSERT_EQ( test_queue.full(), false );
    GTEST_ASSERT_EQ( test_queue.empty(), true );
    GTEST_ASSERT_EQ( test_queue.size(), 0 );

    /*-------------------------------------------------------------------------
    Pop one more time to validate empty behavior
    -------------------------------------------------------------------------*/
    GTEST_ASSERT_EQ( test_queue.pop( &dummy, 1 ), 0  );
  }
}
