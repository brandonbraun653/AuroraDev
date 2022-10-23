/******************************************************************************
 *  File Name:
 *    fixture_file_system.hpp
 *
 *  Description:
 *    Fixtures for testing the filesystem interface
 *
 *  2022 | Brandon Braun | brandonbraun653@protonmail.com
 *****************************************************************************/

#pragma once
#ifndef AURORA_TESTING_FILESYSTEM_HPP
#define AURORA_TESTING_FILESYSTEM_HPP

/*-----------------------------------------------------------------------------
Includes
-----------------------------------------------------------------------------*/
#include <Aurora/filesystem>
#include "gtest/gtest.h"

namespace Aurora::Test::FileSystem
{
  /*---------------------------------------------------------------------------
  Classes
  ---------------------------------------------------------------------------*/
  class StandardFileSystem : public ::testing::Test
  {
  protected:
    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }
  };
}  // namespace Aurora::Test::FileSystem

#endif  /* !AURORA_TESTING_FILESYSTEM_HPP */
