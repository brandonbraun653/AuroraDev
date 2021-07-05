/********************************************************************************
 *  File Name:
 *    heap_fixture.hpp
 *
 *  Description:
 *    Fixture for testing a heap
 *
 *  2021 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

#pragma once
#ifndef AURORA_TEST_MEMORY_HEAP_FIXTURE_HPP
#define AURORA_TEST_MEMORY_HEAP_FIXTURE_HPP

/* STL Includes */
#include <random>

/* Aurora Includes */
#include <Aurora/memory>

/* ETL Includes */
#include <etl/random.h>

/* Testing Includes */
#include "gtest/gtest.h"

namespace Aurora::Test::Heap
{
  /*-------------------------------------------------------------------------------
  Structures
  -------------------------------------------------------------------------------*/
  /**
   * @brief Copy of BlockLink_t for sizing calculations
   * See original definition in heap.hpp
   */
  struct TestBlockLink_t
  {
    TestBlockLink_t *next;
    size_t size;
  };

  /*-------------------------------------------------------------------------------
  Classes
  -------------------------------------------------------------------------------*/
  class DefaultFixture : public ::testing::Test
  {
  protected:
    virtual void SetUp()
    {
      mHeapMemory = nullptr;
      mRNG.initialise( static_cast<uint32_t>( rand() ) );
    }

    virtual void TearDown()
    {
      if ( mHeapMemory )
      {
        delete[] mHeapMemory;
      }
    }

    void initializeHeap( const size_t bytes )
    {
      mHeapMemory = new uint8_t[ bytes ];
      mHeap.assignPool( mHeapMemory, bytes );
    }

    size_t random_value( const size_t max )
    {
      return mRNG() % max;
    }

    Aurora::Memory::Heap mHeap;

  private:
    uint8_t *mHeapMemory;
    etl::random_xorshift mRNG;
  };
}  // namespace Aurora::Test::Heap

#endif /* !AURORA_TEST_MEMORY_HEAP_FIXTURE_HPP */
