/********************************************************************************
 *  File Name:
 *    basic_tests.cpp
 *
 *  Description:
 *    Basic testing for the shared_ptr interface
 *
 *  2021 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

/* Aurora Includes */
#include <Aurora/memory>

/* Testing Includes */
#include <tests/headers.hpp>
#include <tests/memory/shared_ptr/shared_ptr_fixture.hpp>

namespace Aurora::Test::SharedPtr
{
  /*-------------------------------------------------------------------------------
  Aliases
  -------------------------------------------------------------------------------*/
  template<typename T, const size_t BUFSIZE = 0>
  using test_sptr = ::Aurora::Memory::shared_ptr<T, BUFSIZE>;

  /*-------------------------------------------------------------------------------
  Constants
  -------------------------------------------------------------------------------*/
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
  Tests
  -------------------------------------------------------------------------------*/
  /**
   * @brief Validate the raw object is of the expected size
   */
  static_assert( BASE_SPTR_OBJECT_SIZE == sizeof( test_sptr<TrivialTestObject> ) );


  /**
   * @brief Validates a default constructed object is empty
   */
  TEST_F( DefaultSharedPtrFixture, DefaultConstructor )
  {
    test_sptr<TrivialTestObject> default_object;

    GTEST_ASSERT_EQ( false, static_cast<bool>( default_object ) );
    GTEST_ASSERT_EQ( nullptr, default_object.get() );
  }


  /**
   * @brief Validates a default constructed object reports the proper size
   */
  TEST_F( DefaultSharedPtrFixture, SizeCheckTrivialType )
  {
    test_sptr<TrivialTestObject> default_object;
    size_t expected_size = BASE_SPTR_ALLOC_SIZE;

    expected_size += sizeof( TrivialTestObject );

    GTEST_ASSERT_EQ( expected_size, default_object.size() );
  }


  /**
   * @brief Validates a constructed object reports the proper size
   */
  TEST_F( DefaultSharedPtrFixture, SizeCheckBufferTypeNULL )
  {
    static constexpr size_t BUF_BYTES = 0;

    test_sptr<void*, BUF_BYTES> default_object;
    size_t expected_size = BASE_SPTR_ALLOC_SIZE;

    expected_size += sizeof( void *);
    expected_size += BUF_BYTES;

    GTEST_ASSERT_EQ( expected_size, default_object.size() );
  }


  /**
   * @brief Validates a constructed object reports the proper size
   */
  TEST_F( DefaultSharedPtrFixture, SizeCheckBufferTypeWithData )
  {
    static constexpr size_t BUF_BYTES = 53;

    test_sptr<void*, BUF_BYTES> default_object;
    size_t expected_size = BASE_SPTR_ALLOC_SIZE;

    expected_size += sizeof( void *);
    expected_size += BUF_BYTES;

    GTEST_ASSERT_EQ( expected_size, default_object.size() );
  }


  /**
   * @brief Validates a trivial object can be created with an allocator
   * Also checks that the proper amount of memory is allocated.
   */
  TEST_F( DefaultSharedPtrFixture, AllocatorConstructor )
  {
    /*-------------------------------------------------
    Construct the object, tracking available memory
    -------------------------------------------------*/
    const size_t start_size = mHeap.available();
    test_sptr<TrivialTestObject> default_object( &mHeap );
    const size_t end_size = mHeap.available();

    /*-------------------------------------------------
    Calculate some properties.
    -------------------------------------------------*/
    /*                                Basic Size      +         Managed Object      +     Allocator overhead */
    const size_t expected_size = BASE_SPTR_ALLOC_SIZE + sizeof( TrivialTestObject ) + sizeof( Aurora::Memory::BlockLink_t );
    const size_t actual_size   = start_size - end_size;

    GTEST_ASSERT_EQ( true, static_cast<bool>( default_object ) );
    GTEST_ASSERT_NE( nullptr, default_object.get() );
    GTEST_ASSERT_EQ( expected_size, actual_size );
  }
}