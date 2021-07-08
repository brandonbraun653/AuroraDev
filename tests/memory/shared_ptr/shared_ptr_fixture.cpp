/********************************************************************************
 *  File Name:
 *    shared_ptr_fixture.cpp
 *
 *  Description:
 *    Test fixture utilities
 *
 *  2021 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

/* ETL Includes */
#include <etl/random.h>

/* Testing Includes */
#include <tests/memory/shared_ptr/shared_ptr_fixture.hpp>

namespace Aurora::Test::SharedPtr
{
  /*-------------------------------------------------------------------------------
  Constants
  -------------------------------------------------------------------------------*/
  static constexpr size_t RANDOM_SEED = 0x5e0776e7;

  /*-------------------------------------------------------------------------------
  Static Data
  -------------------------------------------------------------------------------*/
  static etl::random_xorshift rng;

  /*-------------------------------------------------------------------------------
  Public Functions
  -------------------------------------------------------------------------------*/
  void initialize()
  {
    rng.initialise( RANDOM_SEED );
  }


  void fill_random_data( test_sptr<TrivialTestObject> &obj )
  {
    obj->member_uint8_t_0  = static_cast<uint8_t>( rng() );
    obj->member_uint8_t_1  = static_cast<uint8_t>( rng() );
    obj->member_uint8_t_2  = static_cast<uint8_t>( rng() );
    obj->member_uint8_t_3  = static_cast<uint8_t>( rng() );
    obj->member_uint16_t_0 = static_cast<uint16_t>( rng() );
    obj->member_uint16_t_1 = static_cast<uint16_t>( rng() );
    obj->member_uint32_t   = static_cast<uint32_t>( rng() );
    obj->member_uint64_t   = static_cast<uint64_t>( rng() ) + static_cast<uint64_t>( rng() );
  }
}  // namespace Aurora::Test::SharedPtr
