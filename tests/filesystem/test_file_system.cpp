/******************************************************************************
 *  File Name:
 *    test_file_system.cpp
 *
 *  Description:
 *    Tests the high level interface to the filesystem module
 *
 *  2022 | Brandon Braun | brandonbraun653@protonmail.com
 *****************************************************************************/

/*-----------------------------------------------------------------------------
Includes
-----------------------------------------------------------------------------*/
#include <Aurora/filesystem>
#include <tests/headers.hpp>

namespace Aurora::Test::FileSystem
{
  TEST( CB_Pass, PassTest )
  {
    GTEST_ASSERT_EQ( true, true );
  }
}  // namespace Aurora::Test::FileSystem
