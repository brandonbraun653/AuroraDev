/********************************************************************************
 *  File Name:
 *    test_generic_utils.cpp
 *
 *  Description:
 *    Tests the generic memory utility functions
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

/* Aurora Includes */
#include <Aurora/memory>

/* GTest Includes */
#include <gtest/gtest.h>


namespace Aurora::Memory
{
  /*-------------------------------------------------------------------------------
  Test Helpers
  -------------------------------------------------------------------------------*/
  // static const Properties inv_inverted_sizing_1 = {
  //   .pageSize = 256,
  //   .numPages = 1,
  //   .blockSize = 128,
  //   .numBlocks = 1,
  //   .sectorSize = 64,
  //   .numSectors = 1,
  //   .jedec = 10,
  //   .
  // };

  /*-------------------------------------------------------------------------------
  FUT: validateProperties()
  -------------------------------------------------------------------------------*/
  TEST( Robust_ValidateProperties, InvalidInputs )
  {
    EXPECT_EQ(true, true);
  }

  TEST( Normal_ValidateProperties, ValidInputs )
  {
    EXPECT_EQ( true, true );
  }
}  // namespace Aurora::Memory
