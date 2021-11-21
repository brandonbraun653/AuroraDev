/********************************************************************************
 *  File Name:
 *    fixture_mpmc_queue.hpp
 *
 *  Description:
 *    Test fixture for MPMC Queue
 *
 *  2021 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

#pragma once
#ifndef AURORA_TEST_MPMC_QUEUE_FIXTURE_HPP
#define AURORA_TEST_MPMC_QUEUE_FIXTURE_HPP

/*-----------------------------------------------------------------------------
Includes
-----------------------------------------------------------------------------*/
#include <cstdlib>
#include <Aurora/container>
#include "gtest/gtest.h"


namespace Aurora::Test::MPMCQueue
{
  /*---------------------------------------------------------------------------
  Constants
  ---------------------------------------------------------------------------*/
  static constexpr size_t MIN_TEST_ELEMENTS = 10;

  /*---------------------------------------------------------------------------
  Structures
  ---------------------------------------------------------------------------*/
  struct TestObj
  {
    float item0;
    TestObj *item1;
  };


  /*---------------------------------------------------------------------------
  Classes
  ---------------------------------------------------------------------------*/
  class MPMCFixture : public ::testing::Test
  {
  protected:
    virtual void SetUp()
    {
      test_sig = Chimera::Interrupt::SIGNAL_INVALID;
      static_init = rand() % 2;
      if( static_init )
      {
        static_init = false;

        GTEST_ASSERT_EQ( test_circ_buf.init( test_array, ARRAY_COUNT( test_array ) ), true );
        GTEST_ASSERT_EQ( test_attr.init( &test_circ_buf, &test_mtx, test_sig ), true );
      }
      else
      {
        static_init = true;
        GTEST_ASSERT_EQ( test_attr.init( ( rand() % 100 ) + MIN_TEST_ELEMENTS ), true );
      }


      GTEST_ASSERT_EQ( true, test_queue.init( test_attr ) );
    }

    virtual void TearDown()
    {
      test_attr.destroy();
    }

    Aurora::Container::MPMCAttr<TestObj> test_attr;
    Aurora::Container::MPMCQueue<TestObj> test_queue;
    Chimera::Interrupt::Signal_t test_sig;

  private:
    bool static_init;
    TestObj test_array[ MIN_TEST_ELEMENTS + 50 ];
    Aurora::Container::CircularBuffer<TestObj> test_circ_buf;
    Chimera::Thread::RecursiveMutex test_mtx;
  };
}  // namespace Aurora::Test::Container

#endif  /* !AURORA_TEST_MPMC_QUEUE_FIXTURE_HPP */
