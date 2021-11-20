/********************************************************************************
 *  File Name:
 *    circular_buffer_fixture.hpp
 *
 *  Description:
 *    Fixtures for testing circular buffer
 *
 *  2021 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

#pragma once
#ifndef AURORA_TESTING_CIRCULAR_BUFFER_HPP
#define AURORA_TESTING_CIRCULAR_BUFFER_HPP

/*-----------------------------------------------------------------------------
Includes
-----------------------------------------------------------------------------*/
#include <Aurora/container>
#include "gtest/gtest.h"


namespace Aurora::Test::CircularBuffer
{
  /*---------------------------------------------------------------------------
  Structures
  ---------------------------------------------------------------------------*/
  struct DummyObject
  {
    uint8_t dummy0;
    size_t dummy1;
    uint64_t dummy2;
    float dummy3;
  };

  /*---------------------------------------------------------------------------
  Classes
  ---------------------------------------------------------------------------*/
  class BufferFixture : public ::testing::Test
  {
  protected:
    virtual void SetUp()
    {
      test_element_size = static_cast<size_t>( ( rand() % 100 ) + 5 );
      test_data = new DummyObject[ test_element_size ];
    }

    virtual void TearDown()
    {
      if( test_data )
      {
        delete[] test_data;
      }
    }

    DummyObject *test_data;
    size_t test_element_size;
  };
}  // namespace Aurora::Test::CircularBuffer

#endif  /* !AURORA_TESTING_CIRCULAR_BUFFER_HPP */
