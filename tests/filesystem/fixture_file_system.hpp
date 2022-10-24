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
#include "gtest/gtest.h"
#include <Aurora/filesystem>
#include <array>
#include <filesystem>

namespace Aurora::Test::FileSystem
{
  /*---------------------------------------------------------------------------
  Aliases
  ---------------------------------------------------------------------------*/
  namespace FS = ::Aurora::FileSystem;

  /*---------------------------------------------------------------------------
  Classes
  ---------------------------------------------------------------------------*/
  class StandardFSFixture : public ::testing::Test
  {
  protected:
    const std::string     mnt_path = "/tmp/aurora/filesystem/";
    FS::VolumeId          mnt_vol;
    std::array<char, 128> write_buf;
    std::array<char, 128> read_buf;

    virtual void SetUp()
    {
      /*-----------------------------------------------------------------------
      Ensure the directory we mount actually exists
      -----------------------------------------------------------------------*/
      std::filesystem::create_directories( mnt_path );
      GTEST_ASSERT_EQ( std::filesystem::exists( mnt_path ), true );

      /*-----------------------------------------------------------------------
      Initialize the Aurora filesystem drivers
      -----------------------------------------------------------------------*/
      FS::initialize();
      FS::Interface intf    = FS::Generic::getInterface();
      FS::VolumeId  mnt_vol = FS::mount( mnt_path, intf );
      GTEST_ASSERT_EQ( mnt_vol, 0 );

      /*-----------------------------------------------------------------------
      Initialize utility variables
      -----------------------------------------------------------------------*/
      write_buf.fill( 0 );
      read_buf.fill( 0 );
    }

    virtual void TearDown()
    {
      /*-----------------------------------------------------------------------
      Destroy Aurora filesystem drivers
      -----------------------------------------------------------------------*/
      FS::unmount( mnt_vol );

      /*-----------------------------------------------------------------------
      Clean the mount directory of all data
      -----------------------------------------------------------------------*/
      for ( const auto &entry : std::filesystem::directory_iterator( mnt_path ) )
      {
        std::filesystem::remove_all( entry.path() );
      }
    }

    std::string testPath( std::string pth )
    {
      return mnt_path + pth;
    }
  };


  class LittleFSFixture : public ::testing::Test
  {
  protected:
    const std::string     mnt_path     = "";
    const std::string     backing_file = "/tmp/aurora/lfs_tests/lfs_nor_file.bin";
    FS::VolumeId          mnt_vol;
    FS::LFS::Volume       lfs_vol;
    std::array<char, 128> write_buf;
    std::array<char, 128> read_buf;

    virtual void SetUp()
    {
      /*-----------------------------------------------------------------------
      Initialize Chimera
      -----------------------------------------------------------------------*/
      Chimera::SPI::initialize();

      /*-----------------------------------------------------------------------
      Initialize the LittleFS drivers
      -----------------------------------------------------------------------*/
      FS::LFS::initialize();

      auto props = Aurora::Flash::NOR::getProperties( Aurora::Flash::NOR::Chip::AT25SF081 );
      lfs_vol.clear();

      lfs_vol.cfg.read_size      = 16;
      lfs_vol.cfg.prog_size      = 16;
      lfs_vol.cfg.block_size     = props->blockSize;
      lfs_vol.cfg.block_count    = props->endAddress / props->blockSize;
      lfs_vol.cfg.cache_size     = 64;
      lfs_vol.cfg.lookahead_size = 16;
      lfs_vol.cfg.block_cycles   = 500;
      lfs_vol._dataFile          = backing_file;

      GTEST_ASSERT_EQ( lfs_vol.flash.configure( Aurora::Flash::NOR::Chip::AT25SF081, Chimera::SPI::Channel::SPI1 ), true );
      GTEST_ASSERT_EQ( FS::LFS::attachVolume( &lfs_vol ), true );

      /*-----------------------------------------------------------------------
      Initialize the Aurora filesystem drivers
      -----------------------------------------------------------------------*/
      auto intf    = FS::LFS::getInterface();
      intf.context = &lfs_vol;

      FS::initialize();
      FS::VolumeId mnt_vol = FS::mount( mnt_path, intf );
      if( mnt_vol < 0 )
      {
        FS::LFS::formatVolume( &lfs_vol );
        mnt_vol = FS::mount( mnt_path, intf );
      }
      GTEST_ASSERT_EQ( mnt_vol, 0 );

      //int create_dir_error = lfs_mkdir( &lfs_vol.fs, "/" );

      /*-----------------------------------------------------------------------
      Initialize utility variables
      -----------------------------------------------------------------------*/
      write_buf.fill( 0 );
      read_buf.fill( 0 );
    }

    virtual void TearDown()
    {
      /*-----------------------------------------------------------------------
      Destroy Aurora filesystem drivers
      -----------------------------------------------------------------------*/
      FS::unmount( mnt_vol );
    }

    std::string testPath( std::string pth )
    {
      return mnt_path + pth;
    }
  };
}  // namespace Aurora::Test::FileSystem

#endif /* !AURORA_TESTING_FILESYSTEM_HPP */
