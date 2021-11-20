/********************************************************************************
 *  File Name:
 *    fixture_mpmc_queue.hpp
 *
 *  Description:
 *    Test fixture for MPMC Queue
 *
 *  2021 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

#pragma once
#ifndef AURORA_TEST_MPMC_QUEUE_FIXTURE_HPP
#define AURORA_TEST_MPMC_QUEUE_FIXTURE_HPP

/*-----------------------------------------------------------------------------
Includes
-----------------------------------------------------------------------------*/
#include <Aurora/container>
#include "gtest/gtest.h"


namespace Aurora::Test::MPMCQueue
{
  /*---------------------------------------------------------------------------
  Structures
  ---------------------------------------------------------------------------*/
  struct TestObj
  {
    float item0;
    TestObj *item1;
  };


  /*---------------------------------------------------------------------------
  Classes
  ---------------------------------------------------------------------------*/
  class MPMCFixture : public ::testing::Test
  {
  protected:
    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }
  };
}  // namespace Aurora::Test::Container

#endif  /* !AURORA_TEST_MPMC_QUEUE_FIXTURE_HPP */
