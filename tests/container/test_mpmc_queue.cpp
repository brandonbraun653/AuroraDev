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
}
