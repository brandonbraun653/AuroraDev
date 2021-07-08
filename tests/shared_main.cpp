/********************************************************************************
 *  File Name:
 *    shared_main.cpp
 *
 *  Description:
 *    Common file for providing the test's entry point.
 *
 *  2021 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

/* Chimera Includes */
#include <Chimera/common>

/* Testing Includes */
#include "gtest/gtest.h"

int main(void)
{
  testing::InitGoogleTest();
  return RUN_ALL_TESTS();
}
