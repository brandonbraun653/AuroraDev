/********************************************************************************
 *  File Name:
 *    destructor_tests.cpp
 *
 *  Description:
 *    Tests the destructor of an Aurora shared_ptr
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
  Tests
  -------------------------------------------------------------------------------*/
  TEST_F( DefaultSharedPtrFixture, SimpleDestruction_AllocReleased )
  {
    /*-------------------------------------------------
    Record the amount of memory that existed before
    -------------------------------------------------*/
    const size_t starting_mem = mHeap.available();

    /*-------------------------------------------------
    Create a new scope and validate allocation
    -------------------------------------------------*/
    {
      test_sptr<TrivialTestObject> obj0( &mHeap );
      fill_random_data( obj0 );

      GTEST_ASSERT_LT( mHeap.available(), starting_mem );
      GTEST_ASSERT_EQ( obj0.references(), 1 );
    }

    /*-------------------------------------------------
    The data should now be freed
    -------------------------------------------------*/
    GTEST_ASSERT_EQ( mHeap.available(), starting_mem );
  }


  TEST_F( DefaultSharedPtrFixture, ChainedDestructions )
  {
    /*-------------------------------------------------
    Record available memory before chaining
    -------------------------------------------------*/
    const size_t starting_mem = mHeap.available();

    test_sptr<TrivialLinkedList> obj0( &mHeap );
    test_sptr<TrivialLinkedList> obj1( &mHeap );
    test_sptr<TrivialLinkedList> obj2( &mHeap );
    test_sptr<TrivialLinkedList> obj3( &mHeap );

    /*-------------------------------------------------
    Chain the items together in new scope
    -------------------------------------------------*/
    {
      test_sptr<TrivialLinkedList> rootObj( &mHeap );
      rootObj->next = obj0;
      obj0->next    = obj1;
      obj1->next    = obj2;
      obj2->next    = obj3;

      GTEST_ASSERT_EQ( 2, obj0.references() );
      GTEST_ASSERT_EQ( 2, obj1.references() );
      GTEST_ASSERT_EQ( 2, obj2.references() );
      GTEST_ASSERT_EQ( 2, obj3.references() );
      GTEST_ASSERT_LT( mHeap.available(), starting_mem );

      /*-------------------------------------------------
      Destroy old references
      -------------------------------------------------*/
      obj0 = test_sptr<TrivialLinkedList>();
      obj1 = test_sptr<TrivialLinkedList>();
      obj2 = test_sptr<TrivialLinkedList>();
      obj3 = test_sptr<TrivialLinkedList>();
    }

    /*-------------------------------------------------
    Verify the memory has returned to normal
    -------------------------------------------------*/
    GTEST_ASSERT_EQ( 0, obj0.references() );
    GTEST_ASSERT_EQ( 0, obj1.references() );
    GTEST_ASSERT_EQ( 0, obj2.references() );
    GTEST_ASSERT_EQ( 0, obj3.references() );
    GTEST_ASSERT_EQ( mHeap.available(), starting_mem );
  }

}  // namespace Aurora::Test::SharedPtr
