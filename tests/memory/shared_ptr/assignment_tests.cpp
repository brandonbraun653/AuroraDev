/********************************************************************************
 *  File Name:
 *    assignment_tests.cpp
 *
 *  Description:
 *    Tests the various constructors for an Aurora shared_ptr
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
  TEST_F( DefaultSharedPtrFixture, CopyAssignment_Empty )
  {
    test_sptr<TrivialTestObject> obj0;
    test_sptr<TrivialTestObject> obj1;

    obj1 = obj0;
    GTEST_ASSERT_EQ( nullptr, obj0.get() );
    GTEST_ASSERT_EQ( nullptr, obj1.get() );
  }


  TEST_F( DefaultSharedPtrFixture, CopyAssignment_Data )
  {
    /*-------------------------------------------------
    Create some objects and fill them with data
    -------------------------------------------------*/
    test_sptr<TrivialTestObject> obj0( &mHeap );
    fill_random_data( obj0 );


    test_sptr<TrivialTestObject> obj1( &mHeap );
    fill_random_data( obj1 );

    /*-------------------------------------------------
    Validate the data is different
    -------------------------------------------------*/
    GTEST_ASSERT_EQ( 1, obj0.references() );
    GTEST_ASSERT_EQ( 1, obj1.references() );
    GTEST_ASSERT_NE( obj0.get(), obj1.get() );
    GTEST_ASSERT_NE( 0, memcmp( obj0.get(), obj1.get(), sizeof( TrivialTestObject ) ) );

    /*-------------------------------------------------
    Do the copy assignment operation
    -------------------------------------------------*/
    obj1 = obj0;

    /*-------------------------------------------------
    Validate the data is now the same
    -------------------------------------------------*/
    GTEST_ASSERT_EQ( 2, obj0.references() );
    GTEST_ASSERT_EQ( 2, obj1.references() );
    GTEST_ASSERT_EQ( obj0.get(), obj1.get() );
    GTEST_ASSERT_EQ( 0, memcmp( obj0.get(), obj1.get(), sizeof( TrivialTestObject ) ) );
  }


  TEST_F( DefaultSharedPtrFixture, MoveAssignment )
  {
    /*-------------------------------------------------
    Create some objects and fill them with data
    -------------------------------------------------*/
    TrivialTestObject obj0Copy;
    test_sptr<TrivialTestObject> obj0( &mHeap );
    fill_random_data( obj0 );
    memcpy( &obj0Copy, obj0.get(), sizeof( obj0Copy ) );

    test_sptr<TrivialTestObject> obj1( &mHeap );
    fill_random_data( obj1 );

    /*-------------------------------------------------
    Validate the data is different
    -------------------------------------------------*/
    GTEST_ASSERT_EQ( 1, obj0.references() );
    GTEST_ASSERT_EQ( 1, obj1.references() );
    GTEST_ASSERT_NE( obj0.get(), obj1.get() );
    GTEST_ASSERT_NE( 0, memcmp( obj0.get(), obj1.get(), sizeof( TrivialTestObject ) ) );

    /*-------------------------------------------------
    Do the copy assignment operation
    -------------------------------------------------*/
    obj1 = std::move( obj0 );

    /*-------------------------------------------------
    Validate the data has been moved and the original
    object was destroyed (only one reference)
    -------------------------------------------------*/
    GTEST_ASSERT_EQ( 0, obj0.references() );
    GTEST_ASSERT_EQ( 1, obj1.references() );
    GTEST_ASSERT_NE( obj0.get(), obj1.get() );
    GTEST_ASSERT_EQ( obj0.get(), nullptr );
    GTEST_ASSERT_EQ( 0, memcmp( &obj0Copy, obj1.get(), sizeof( TrivialTestObject ) ) );
  }


  TEST_F( DefaultSharedPtrFixture, MultipleReferences )
  {
    /*-------------------------------------------------
    Create data holders
    -------------------------------------------------*/
    std::array<test_sptr<TrivialTestObject>, 30> objReferences;
    test_sptr<TrivialTestObject> obj0( &mHeap );
    fill_random_data( obj0 );

    /*-------------------------------------------------
    Create the references to all the objects
    -------------------------------------------------*/
    objReferences.fill( obj0 );

    /*-------------------------------------------------
    Validate reference count and data is as expected
    -------------------------------------------------*/
    GTEST_ASSERT_EQ( obj0.references(), objReferences.size() + 1 );

    for ( size_t x = 0; x < objReferences.size(); x++ )
    {
      GTEST_ASSERT_EQ( objReferences[ x ].get(), obj0.get() );
      GTEST_ASSERT_EQ( objReferences[ x ].references(), objReferences.size() + 1 );
      GTEST_ASSERT_EQ( 0, memcmp( objReferences[ x ].get(), obj0.get(), sizeof( TrivialTestObject ) ) );
    }
  }

}  // namespace Aurora::Test::SharedPtr
