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
#include <etl/string.h>
#include <string>
#include <tests/filesystem/fixture_file_system.hpp>
#include <tests/headers.hpp>

namespace FS = Aurora::FileSystem;

namespace Aurora::Test::FileSystem
{
  /*---------------------------------------------------------------------------
  Make sure calling the module init function doesn't have weird side effects
  ---------------------------------------------------------------------------*/
  TEST( FSInvalidUseCase, Initialize )
  {
    FS::initialize();
    GTEST_SUCCEED();
  }

  /*---------------------------------------------------------------------------
  Ensure the core API returns the expected error codes before initialization
  ---------------------------------------------------------------------------*/
  TEST( FSInvalidUseCase, BeforeInit_Open )
  {
    FS::FileId f = 0;
    char       fake_data[ 10 ];

    GTEST_ASSERT_LT( FS::fopen( "", FS::O_RDWR, f ), 0 );
    GTEST_ASSERT_LT( FS::fclose( f ), 0 );
    GTEST_ASSERT_LT( FS::fflush( f ), 0 );
    GTEST_ASSERT_EQ( FS::fread( fake_data, 10, 1, f ), 0 );
    GTEST_ASSERT_EQ( FS::fwrite( fake_data, 10, 1, f ), 0 );
    GTEST_ASSERT_LT( FS::fseek( f, 0, FS::F_SEEK_SET ), 0 );
    GTEST_ASSERT_EQ( FS::ftell( f ), 0 );
  }

  TEST( FSInvalidUseCase, BadInterface )
  {
    FS::Interface intf;
    intf.clear();

    FS::initialize();
    GTEST_ASSERT_EQ( FS::mount( "a", intf ), -1 );
  }

  TEST( FSInvalidUseCase, BadDriveLength )
  {
    FS::initialize();
    FS::Interface intf = FS::Generic::getInterface();
    GTEST_ASSERT_EQ( FS::mount( "asldfoiwueroasdful;aksjd;flkajl;skudfiowueosiufowuer", intf ), -1 );
  }

  /*---------------------------------------------------------------------------
  Tests the expected use case for mounting a drive
  ---------------------------------------------------------------------------*/
  TEST( FSMounting, ExpectedUseCase )
  {
    FS::initialize();
    FS::Interface intf = FS::Generic::getInterface();
    FS::VolumeId  vol  = FS::mount( "/", intf );

    GTEST_ASSERT_EQ( vol, 0 );
  }

  TEST( FSMounting, DriveLimits )
  {
    /*-------------------------------------------------------------------------
    Setup
    -------------------------------------------------------------------------*/
    etl::string<FS::MAX_VOLUMES> mnt_chars = "abcde";
    EXPECT_EQ( mnt_chars.full(), true );

    FS::initialize();

    for ( size_t idx = 0; idx < FS::MAX_VOLUMES; idx++ )
    {
      FS::Interface intf = FS::Generic::getInterface();
      FS::VolumeId  vol  = FS::mount( &mnt_chars[ idx ], intf );
      GTEST_ASSERT_EQ( vol, idx );
    }

    /*-------------------------------------------------------------------------
    Verify
    -------------------------------------------------------------------------*/
    FS::Interface intf = FS::Generic::getInterface();
    FS::VolumeId  vol  = FS::mount( "f", intf );
    GTEST_ASSERT_EQ( vol, -1 );
  }

  /*---------------------------------------------------------------------------
  Tests Open/Write/Read/Close operations
  ---------------------------------------------------------------------------*/
  TEST_F( StandardFSFixture, ExpectedUseCase )
  {
    FS::FileId  file;
    std::string filename = testPath( "tst.txt" );

    /*-------------------------------------------------------------------------
    Open the file
    -------------------------------------------------------------------------*/
    int ret = FS::fopen( filename.c_str(), ( FS::O_CREAT | FS::O_WRONLY ), file );

    GTEST_ASSERT_EQ( ret, 0 );
    GTEST_ASSERT_EQ( file, 0 );

    /*-------------------------------------------------------------------------
    Write some test data
    -------------------------------------------------------------------------*/
    write_buf.fill( 0 );
    snprintf( write_buf.data(), write_buf.size(), "Hello World!" );

    ret = FS::fwrite( write_buf.data(), 1, strlen( write_buf.data() ), file );
    GTEST_ASSERT_EQ( ret, strlen( write_buf.data() ) );

    /*-------------------------------------------------------------------------
    Close the file, then reopen it
    -------------------------------------------------------------------------*/
    ret = FS::fclose( file );
    GTEST_ASSERT_EQ( ret, 0 );

    ret = FS::fopen( filename.c_str(), FS::O_RDONLY, file );
    GTEST_ASSERT_EQ( ret, 0 );
    GTEST_ASSERT_GE( file, 1 );  // File index should've incremented

    /*-------------------------------------------------------------------------
    Read the data back
    -------------------------------------------------------------------------*/
    read_buf.fill( 0 );
    ret = FS::fread( read_buf.data(), 1, strlen( write_buf.data() ), file );

    GTEST_ASSERT_EQ( ret, strlen( write_buf.data() ) );
    GTEST_ASSERT_EQ( write_buf, read_buf );

    /*-------------------------------------------------------------------------
    Close the file
    -------------------------------------------------------------------------*/
    ret = FS::fclose( file );
    GTEST_ASSERT_EQ( ret, 0 );
  }

  TEST_F( LittleFSFixture, ExpectedUseCase )
  {
    FS::FileId  file;
    std::string filename = "/tst.txt";

    /*-------------------------------------------------------------------------
    Open the file
    -------------------------------------------------------------------------*/
    int ret = FS::fopen( filename.c_str(), ( FS::O_CREAT | FS::O_WRONLY ), file );

    GTEST_ASSERT_EQ( ret, 0 );
    GTEST_ASSERT_EQ( file, 0 );

    /*-------------------------------------------------------------------------
    Write some test data
    -------------------------------------------------------------------------*/
    write_buf.fill( 0 );
    snprintf( write_buf.data(), write_buf.size(), "Hello World!" );

    ret = FS::fwrite( write_buf.data(), 1, strlen( write_buf.data() ), file );
    GTEST_ASSERT_EQ( ret, strlen( write_buf.data() ) );

    /*-------------------------------------------------------------------------
    Close the file, then reopen it
    -------------------------------------------------------------------------*/
    ret = FS::fclose( file );
    GTEST_ASSERT_EQ( ret, 0 );

    ret = FS::fopen( filename.c_str(), FS::O_RDONLY, file );
    GTEST_ASSERT_EQ( ret, 0 );
    GTEST_ASSERT_GE( file, 1 );  // File index should've incremented

    /*-------------------------------------------------------------------------
    Read the data back
    -------------------------------------------------------------------------*/
    read_buf.fill( 0 );
    ret = FS::fread( read_buf.data(), 1, strlen( write_buf.data() ), file );

    GTEST_ASSERT_EQ( ret, strlen( write_buf.data() ) );
    GTEST_ASSERT_EQ( write_buf, read_buf );

    /*-------------------------------------------------------------------------
    Close the file
    -------------------------------------------------------------------------*/
    ret = FS::fclose( file );
    GTEST_ASSERT_EQ( ret, 0 );
  }

}  // namespace Aurora::Test::FileSystem
