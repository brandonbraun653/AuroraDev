/********************************************************************************
 *  File Name:
 *    circular_buffer_tests.cpp
 *
 *  Description:
 *    Tests the circular buffer module
 *
 *  2021 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

/* Aurora Includes */
#include <Aurora/container>

/* Testing Includes */
#include <tests/headers.hpp>
#include <tests/container/circular_buffer_fixture.hpp>

namespace Aurora::Test::CircularBuffer
{
  TEST( RobustTests, InvalidInit )
  {
    /*-------------------------------------------------------------------------
    Setup
    -------------------------------------------------------------------------*/
    DummyObject fakeData[ 3 ];
    auto cb = Aurora::Container::CircularBuffer<DummyObject>();

    /*-------------------------------------------------------------------------
    Validate
    -------------------------------------------------------------------------*/
    GTEST_ASSERT_EQ( false, cb.init( nullptr, 10 ) );
    GTEST_ASSERT_EQ( false, cb.init( fakeData, 0 ) );
  }

  TEST( RobustTests, EmptyBufferReferences )
  {
    /*-------------------------------------------------------------------------
    Setup
    -------------------------------------------------------------------------*/
    DummyObject fakeData[ 3 ];
    auto cb = Aurora::Container::CircularBuffer<DummyObject>();
    cb.init( fakeData, 3 );

    /*-------------------------------------------------------------------------
    Validate
    -------------------------------------------------------------------------*/
    GTEST_ASSERT_EQ( nullptr, cb.front() );
    GTEST_ASSERT_EQ( nullptr, cb.back() );
  }

  TEST_F( BufferFixture, ValidAllocation )
  {
    auto cb = Aurora::Container::CircularBuffer<DummyObject>();

    /*-------------------------------------------------------------------------
    Validate initialization
    -------------------------------------------------------------------------*/
    GTEST_ASSERT_EQ( true, cb.init( this->test_data, this->test_element_size ) );
    GTEST_ASSERT_EQ( this->test_element_size, cb.capacity() );

    /*-------------------------------------------------------------------------
    Validate default state
    -------------------------------------------------------------------------*/
    GTEST_ASSERT_EQ( 0, cb.size() );
    GTEST_ASSERT_EQ( true, cb.empty() );
    GTEST_ASSERT_EQ( false, cb.full() );
    GTEST_ASSERT_EQ( this->test_data, cb.data() );

    /*-------------------------------------------------------------------------
    Validate reset state
    -------------------------------------------------------------------------*/
    cb.reset();

    GTEST_ASSERT_EQ( 0, cb.size() );
    GTEST_ASSERT_EQ( true, cb.empty() );
    GTEST_ASSERT_EQ( false, cb.full() );
    GTEST_ASSERT_EQ( this->test_data, cb.data() );
  }

  TEST_F( BufferFixture, PushThenReset )
  {
    DummyObject dummy;
    auto cb = Aurora::Container::CircularBuffer<DummyObject>();

    /*-------------------------------------------------------------------------
    Setup
    -------------------------------------------------------------------------*/
    cb.init( this->test_data, this->test_element_size );
    for ( auto x = 0; x < ( cb.capacity() - 1 ); x++ )
    {
      GTEST_ASSERT_EQ( true, cb.push( dummy ) );
    }

    /*-------------------------------------------------------------------------
    Validate loaded state
    -------------------------------------------------------------------------*/
    GTEST_ASSERT_EQ( cb.capacity() - 1, cb.size() );

    /*-------------------------------------------------------------------------
    Validate reset state
    -------------------------------------------------------------------------*/
    cb.reset();

    GTEST_ASSERT_EQ( 0, cb.size() );
    GTEST_ASSERT_EQ( true, cb.empty() );
    GTEST_ASSERT_EQ( false, cb.full() );
    GTEST_ASSERT_EQ( this->test_data, cb.data() );
  }

  TEST_F( BufferFixture, PushThenPop )
  {
    DummyObject dummy;
    auto cb = Aurora::Container::CircularBuffer<DummyObject>();

    /*-------------------------------------------------------------------------
    Setup
    -------------------------------------------------------------------------*/
    cb.init( this->test_data, this->test_element_size );
    for ( auto x = 0; x < ( cb.capacity() - 1 ); x++ )
    {
      GTEST_ASSERT_EQ( true, cb.push( dummy ) );
    }

    /*-------------------------------------------------------------------------
    Validate loaded state
    -------------------------------------------------------------------------*/
    GTEST_ASSERT_EQ( cb.capacity() - 1, cb.size() );

    /*-------------------------------------------------------------------------
    Clear out the buffer
    -------------------------------------------------------------------------*/
    for ( auto x = 0; x < ( cb.capacity() - 1 ); x++ )
    {
      dummy = cb.pop();
    }

    /*-------------------------------------------------------------------------
    Validate cleared stated
    -------------------------------------------------------------------------*/
    GTEST_ASSERT_EQ( 0, cb.size() );
    GTEST_ASSERT_EQ( true, cb.empty() );
    GTEST_ASSERT_EQ( false, cb.full() );
  }

  TEST_F( BufferFixture, PushTillFull )
  {
    DummyObject dummy;
    auto cb = Aurora::Container::CircularBuffer<DummyObject>();

    /*-------------------------------------------------------------------------
    Setup
    -------------------------------------------------------------------------*/
    cb.init( this->test_data, this->test_element_size );
    for ( auto x = 0; x < cb.capacity(); x++ )
    {
      GTEST_ASSERT_EQ( true, cb.push( dummy ) );
    }

    /*-------------------------------------------------------------------------
    Validate loaded state
    -------------------------------------------------------------------------*/
    GTEST_ASSERT_EQ( cb.capacity(), cb.size() );
    GTEST_ASSERT_EQ( true, cb.full() );
  }

  TEST_F( BufferFixture, PushAndOverwrite )
  {
    DummyObject dummy1, dummy2, dummy3;
    auto cb = Aurora::Container::CircularBuffer<DummyObject>();

    /*-------------------------------------------------------------------------
    Setup
    -------------------------------------------------------------------------*/
    dummy1.dummy0 = 3;
    dummy2.dummy0 = 8;
    dummy3.dummy0 = 10;

    cb.init( this->test_data, this->test_element_size );
    cb.push( dummy1 ); // This one will get overwritten

    while( !cb.full() )
    {
      GTEST_ASSERT_EQ( true, cb.push( dummy2 ) );
    }

    /*-------------------------------------------------------------------------
    Make sure the front still contains the original data, then overwrite it.
    -------------------------------------------------------------------------*/
    GTEST_ASSERT_EQ( 0, memcmp( &dummy1, cb.front(), sizeof( DummyObject ) ) );
    cb.pushOverwrite( dummy3 );

    /*-------------------------------------------------------------------------
    Validate the front has moved
    -------------------------------------------------------------------------*/
    GTEST_ASSERT_EQ( 0, memcmp( &dummy2, cb.front(), sizeof( DummyObject ) ) );
  }

  TEST_F( BufferFixture, PushAndValidateFrontBack )
  {
    DummyObject dummy1, dummy2, dummy3;
    auto cb = Aurora::Container::CircularBuffer<DummyObject>();

    /*-------------------------------------------------------------------------
    Setup
    -------------------------------------------------------------------------*/
    cb.init( this->test_data, this->test_element_size );
    cb.push( dummy1 );
    cb.push( dummy2 );
    cb.push( dummy3 );

    /*-------------------------------------------------------------------------
    Validate loaded state
    -------------------------------------------------------------------------*/
    GTEST_ASSERT_EQ( 0, memcmp( &dummy1, cb.front(), sizeof( dummy1 ) ) );
    GTEST_ASSERT_EQ( 0, memcmp( &dummy3, cb.back(), sizeof( dummy3 ) ) );
  }

  TEST_F( BufferFixture, ValidateReverseWrap )
  {
    DummyObject dummy;
    auto cb = Aurora::Container::CircularBuffer<DummyObject>();

    /*-------------------------------------------------------------------------
    Setup. Force the write index to zero and the buffer to not be full.
    -------------------------------------------------------------------------*/
    cb.init( this->test_data, this->test_element_size );
    for ( size_t x = 0; x < cb.capacity(); x++ )
    {
      dummy.dummy1 = x;
      GTEST_ASSERT_EQ( true, cb.push( dummy ) );
    }

    dummy = cb.pop();

    /*-------------------------------------------------------------------------
    Validate loaded state. The last written element should contain the highest
    zero indexed value of the buffer's capacity.
    -------------------------------------------------------------------------*/
    GTEST_ASSERT_NE( nullptr, cb.back() );
    GTEST_ASSERT_EQ( cb.capacity() - 1, cb.back()->dummy1 );
  }

  TEST_F( BufferFixture, PushAndValidateOrder )
  {
    DummyObject dummy1, dummy2, dummy3;
    auto cb = Aurora::Container::CircularBuffer<DummyObject>();

    /*-------------------------------------------------------------------------
    Setup
    -------------------------------------------------------------------------*/
    dummy1.dummy0 = 10;
    dummy2.dummy0 = 13;
    dummy3.dummy0 = 18;

    cb.init( this->test_data, this->test_element_size );
    cb.push( dummy1 );
    cb.push( dummy2 );
    cb.push( dummy3 );

    /*-------------------------------------------------------------------------
    Pop and verify
    -------------------------------------------------------------------------*/
    GTEST_ASSERT_EQ( 0, memcmp( &dummy1, cb.front(), sizeof( DummyObject ) ) );
    cb.pop();

    GTEST_ASSERT_EQ( 0, memcmp( &dummy2, cb.front(), sizeof( DummyObject ) ) );
    cb.pop();

    GTEST_ASSERT_EQ( 0, memcmp( &dummy3, cb.front(), sizeof( DummyObject ) ) );
    cb.pop();
  }
}  // namespace Aurora::Test::CircularBuffer
