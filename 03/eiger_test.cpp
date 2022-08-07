/*
 * hello_test.cpp
 *
 *  Created on: Aug 7, 2022
 *      Author: esko02
 */

#include <gtest/gtest.h>

#include "eiger03.h"

// extern uint CHUNK_MAX_SIZE_EIGER;

using namespace std;

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}

TEST(EigerTest, BasicThings)
{
	char cArr[] = {'h','e','l','l','o',' ','w','o','r','l','d'};

	size_t arrLen = (sizeof(cArr) / sizeof(char));

	EXPECT_EQ(11, arrLen);

	list<CHUNK> chList;

	int retval = -1;

	Eiger aEiger;

	retval = aEiger.makeChunks(cArr, arrLen, chList, 1);

	EXPECT_EQ(0, retval);
}
