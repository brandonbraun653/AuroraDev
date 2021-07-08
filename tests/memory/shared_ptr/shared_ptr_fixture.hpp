/********************************************************************************
 *  File Name:
 *    shared_ptr_fixture.hpp
 *
 *  Description:
 *    Shared pointer test fixtures
 *
 *  2021 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

#pragma once
#ifndef AURORA_TEST_SHARED_PTR_FIXTURES_HPP
#define AURORA_TEST_SHARED_PTR_FIXTURES_HPP

/* STL Includes */
#include <cstdint>
#include <cstddef>
#include <type_traits>

/* Aurora Includes */
#include <Aurora/memory>

/* Testing Includes */
#include "gtest/gtest.h"

namespace Aurora::Test::SharedPtr
{
  /*-------------------------------------------------------------------------------
  Constants
  -------------------------------------------------------------------------------*/
  static constexpr size_t DEFAULT_HEAP_SIZE = 1 * 1024 * 1024;

  /* clang-format off */
  static constexpr size_t BASE_SPTR_ALLOC_SIZE =
    sizeof( size_t ) +                            /* Object counter data */
    sizeof( Chimera::Thread::Mutex );             /* Thread lock data */

  static constexpr size_t BASE_SPTR_OBJECT_SIZE =
    sizeof( Aurora::Memory::IHeapAllocator* ) +   /* Heap pointer */
    sizeof( size_t * ) +                          /* Object counter pointer */
    sizeof( void * ) +                            /* Object data pointer */
    sizeof( Chimera::Thread::Mutex * );           /* Object lock pointer */
  /* clang-format on */

  /*-------------------------------------------------------------------------------
  Aliases
  -------------------------------------------------------------------------------*/
  template<typename T, const size_t BUFSIZE = 0>
  using test_sptr = ::Aurora::Memory::shared_ptr<T, BUFSIZE>;

  /*-------------------------------------------------------------------------------
  Structures
  -------------------------------------------------------------------------------*/
  struct TrivialTestObject
  {
    uint8_t member_uint8_t_0;
    uint8_t member_uint8_t_1;
    uint8_t member_uint8_t_2;
    uint8_t member_uint8_t_3;
    uint16_t member_uint16_t_0;
    uint16_t member_uint16_t_1;
    uint32_t member_uint32_t;
    uint64_t member_uint64_t;
  };
  static_assert( std::is_trivial<TrivialTestObject>::value );
  static_assert( sizeof( TrivialTestObject ) == 24 );

  struct TrivialLinkedList
  {
    test_sptr<TrivialLinkedList> next;
    bool valid;
    uint32_t data;
  };

  /*-------------------------------------------------------------------------------
  Public Functions
  -------------------------------------------------------------------------------*/
  void initialize();
  void fill_random_data( test_sptr<TrivialTestObject> &obj );

  /*-------------------------------------------------------------------------------
  Classes
  -------------------------------------------------------------------------------*/
  class DefaultSharedPtrFixture : public ::testing::Test
  {
  protected:
    virtual void SetUp()
    {
      initialize();
      mHeapMemory = new uint8_t[ DEFAULT_HEAP_SIZE ];
      mHeap.assignMemoryPool( mHeapMemory, DEFAULT_HEAP_SIZE );
    }

    virtual void TearDown()
    {
      if ( mHeapMemory )
      {
        delete[] mHeapMemory;
      }
    }

    Aurora::Memory::Heap mHeap;

  private:
    uint8_t *mHeapMemory;
  };


}  // namespace Aurora::Test::SharedPtr

#endif  /* !AURORA_TEST_SHARED_PTR_FIXTURES_HPP */
