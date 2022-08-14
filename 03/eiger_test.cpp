/*
 * hello_test.cpp
 *
 *  Created on: Aug 7, 2022
 *      Author: esko02
 */

#include <gtest/gtest.h>
#include <list>

#include "eiger03.h"

// extern uint CHUNK_MAX_SIZE_EIGER;

using namespace std;

/*
// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}
*/

class EigerTestFixture : public ::testing::Test
{
protected:
	// char * cArr1;
	// char * cArr2;

	void SetUp() override
	{

		// cArr1 = (char *) strArr1.c_str();
		// cArr2 = (char *) strArr2.c_str();
	}

};

TEST(EigerTest, DISABLED_TestBasicThingsTest)
{
	unsigned char cArr[] = {'h','e','l','l','o',' ','w','o','r','l','d'};

	size_t arrLen = (sizeof(cArr) / sizeof(char));

	EXPECT_EQ(11, arrLen);

	list<CHUNK> chList;

	int retval = -1;

	Eiger aEiger;

	retval = aEiger.makeChunks(cArr, arrLen, chList, DeltaWhich::Orig);

	EXPECT_EQ(0, retval);

	aEiger.iterateAndDeleteChunkBuffer(chList);
}

TEST(EigerTest, TestCharArray12Zeros)
{
	unsigned char cArr[] = {0,0,0,0,0,0,0,0,0,0,0,0};

	size_t arrLen = (sizeof(cArr) / sizeof(char));

	EXPECT_EQ(12, arrLen);

	list<CHUNK> chList;

	int retval = -1;

	Eiger aEiger;

	retval = aEiger.makeChunks(cArr, arrLen, chList, DeltaWhich::Orig);

	EXPECT_EQ(0, retval);

	retval = aEiger.iterateChunksForRH(chList);

	EXPECT_EQ(0, retval);

	int counter = 0;
	uint actualVal = 9999;

	for (list<CHUNK>::iterator it = chList.begin(); it != chList.end(); it++)
	{
		if (0 == counter)
		{
			// EXPECT_EQ(0, (*it).hash);

			actualVal = (*it).hash;
		}

		cout << "Counter: " << counter << endl;

		counter++;
	}

	cout << "Actual value: " << actualVal << endl;

	// EXPECT_EQ(0, actualVal);

	EXPECT_EQ(0, retval);

	aEiger.iterateAndDeleteChunkBuffer(chList);
}

TEST(EigerTest, calculateCompareRollingHash01)
{
	unsigned char cArr1[] = " which uses the rolling hash described below. Another popular";
	unsigned char cArr2[] = " which Uses the rolling hash described below. Another popular";

	size_t arrLen1 = (sizeof(cArr1) / sizeof(char));
	size_t arrLen2 = (sizeof(cArr2) / sizeof(char));

	Eiger aEiger;
	unsigned long hash1 = 0;
	unsigned long hash2 = 0;

	int retval = -1;

	retval = aEiger.calculateRollingHash(cArr1, arrLen1, hash1, 10);
	EXPECT_EQ(0, retval);

	retval = aEiger.calculateRollingHash(cArr2, arrLen2, hash2, 10);
	EXPECT_EQ(0, retval);

	cout << "hash1: " << hash1 << endl;
	cout << "hash2: " << hash2 << endl;


	EXPECT_NE(hash1, hash2);
}

TEST(EigerTest, calculateCompareRollingHash02)
{
	unsigned char cArr1[] = " which uses the rolling ha";
	unsigned char cArr2[] = " which Uses the rolling ha";

	size_t arrLen1 = (sizeof(cArr1) / sizeof(char));
	size_t arrLen2 = (sizeof(cArr2) / sizeof(char));

	cout << "cArr1: " << cArr1 << endl;
	cout << "cArr2: " << cArr2 << endl;

	cout << "arrLen1: " << arrLen1 << endl;
	cout << "arrLen2: " << arrLen2 << endl;

	Eiger aEiger;
	ulong hash1 = 0;
	ulong hash2 = 0;

	int retval = -1;

	retval = aEiger.calculateRollingHash(cArr1, arrLen1, hash1, 10);
	EXPECT_EQ(0, retval);

	retval = aEiger.calculateRollingHash(cArr2, arrLen2, hash2, 10);
	EXPECT_EQ(0, retval);

	cout << "hash1: " << hash1 << endl;
	cout << "hash2: " << hash2 << endl;

	EXPECT_NE(hash1, hash2);
}

TEST(EigerTest, calculateCompareRollingHash03)
{
	unsigned char cArr1[] = " which uses the rolling h";
	unsigned char cArr2[] = " which uses the Rolling h";

	size_t arrLen1 = (sizeof(cArr1) / sizeof(char));
	size_t arrLen2 = (sizeof(cArr2) / sizeof(char));

	cout << "cArr1: " << cArr1 << endl;
	cout << "cArr2: " << cArr2 << endl;

	cout << "arrLen1: " << arrLen1 << endl;
	cout << "arrLen2: " << arrLen2 << endl;

	Eiger aEiger;
	ulong hash1 = 0;
	ulong hash2 = 0;

	int retval = -1;

	retval = aEiger.calculateRollingHash(cArr1, arrLen1, hash1, 10);
	EXPECT_EQ(0, retval);

	retval = aEiger.calculateRollingHash(cArr2, arrLen2, hash2, 10);
	EXPECT_EQ(0, retval);

	cout << "hash1: " << hash1 << endl;
	cout << "hash2: " << hash2 << endl;

	EXPECT_NE(hash1, hash2);
}


TEST_F(EigerTestFixture, TestArraysAreNotEquals01)
{
	unsigned char cArr1[] = "A rolling hash (also known as recursive hashing or rolling"\
			" checksum) is a hash function where the input is hashed in a window"\
			" that moves through the input."\
			" A few hash functions allow a rolling hash to be computed very"\
			" quickly—the new hash value is rapidly calculated given only the old"\
			" hash value, the old value removed from the window, and the new"\
			" value added to the window—similar to the way a moving average"\
			" function can be computed much more quickly than other low-pass filters."\
			"One of the main applications is the Rabin–Karp string search algorithm,"\
			" which uses the rolling hash described below. Another popular"\
			" application is the rsync program, which uses a checksum based on"\
			" Mark Adler's adler-32 as its rolling hash."\
			" Low Bandwidth Network Filesystem (LBFS) uses a Rabin fingerprint"\
			" as its rolling hash. FastCDC (Fast Content-Defined Chunking) uses a"\
			" compute-efficient Gear fingerprint as its rolling hash.";

	unsigned char cArr2[] = "A rolling hash (also known as recursive hashing or rolling"\
			" checksum) is a hash function where the input is hashed in a window"\
			" that moves through the input."\
			" A few hash functions allow a rolling hash to be computed very"\
			" quickly—the new hash value is rapidly calculated given only the old"\
			" hash value, the old value removed from the window, and the new"\
			" value added to the window—similar to the way a moving average"\
			" function can be computed much more quickly than other low-pass filters."\
			"One of the main applications is the Rabin–Karp string search algorithm,"\
			" Ahich Uses the rolling hash described below. Another popular"\
			" application is the rsync program, which uses a checksum based on"\
			" Mark Adler's adler-32 as its rolling hash."\
			" Low Bandwidth Network Filesystem (LBFS) uses a Rabin fingerprint"\
			" as its rolling hash. FastCDC (Fast Content-Defined Chunking) uses a"\
			" compute-efficient Gear fingerprint as its rolling hash.";

	size_t arrLen1 = (sizeof(cArr1) / sizeof(char));
	size_t arrLen2 = (sizeof(cArr2) / sizeof(char));

	list<CHUNK> chList1;
	list<CHUNK> chList2;

	int retval = -1;

	Eiger aEiger;

	retval = aEiger.makeChunks(cArr1, arrLen1, chList1, DeltaWhich::Orig);
	EXPECT_EQ(0, retval);

	retval = aEiger.makeChunks(cArr2, arrLen2, chList2, DeltaWhich::Update);
	EXPECT_EQ(0, retval);

	retval = aEiger.iterateChunksForRH(chList1);
	EXPECT_EQ(0, retval);

	retval = aEiger.iterateChunksForRH(chList2);
	EXPECT_EQ(0, retval);

	retval = aEiger.iterateChunkListHashes(chList1);
	EXPECT_EQ(0, retval);

	retval = aEiger.iterateChunkListHashes(chList2);
	EXPECT_EQ(0, retval);

	list<DELTA> deltaList;

	vector<CHUNK> vcf1(chList1.begin(), chList1.end());
	vector<CHUNK> vcf2(chList2.begin(), chList2.end());

	// retval = aEiger.composeDelta(chList1, chList2, deltaList);
	retval = aEiger.composeDelta(vcf1, vcf2, deltaList);
	EXPECT_EQ(0, retval);

	retval = aEiger.iterateAndDeleteChunkBuffer(chList1);
	EXPECT_EQ(0, retval);

	retval = aEiger.iterateAndDeleteChunkBuffer(chList2);
	EXPECT_EQ(0, retval);

	retval = aEiger.iterateDelta(deltaList);
}


TEST(EigerTest, DISABLED_CharArray10ZerosAndOne1)
{
	unsigned char cArr[] = {1,0,0,0,0,0,0,0,0,0,0};

	size_t arrLen = (sizeof(cArr) / sizeof(char));

	EXPECT_EQ(11, arrLen);

	list<CHUNK> chList;

	int retval = -1;

	Eiger aEiger;

	retval = aEiger.makeChunks(cArr, arrLen, chList, DeltaWhich::Orig);

	EXPECT_EQ(0, retval);

	retval = aEiger.iterateChunksForRH(chList);

	EXPECT_EQ(0, retval);

	int counter = 0;
	uint actualVal = 9999;

	for (list<CHUNK>::iterator it = chList.begin(); it != chList.end(); it++)
	{
		if (0 == counter)
		{
			EXPECT_EQ(0, (*it).hash);

			actualVal = (*it).hash;
		}

		cout << "Counter: " << counter << endl;

		counter++;
	}

	cout << "Actual value: " << actualVal << endl;

	EXPECT_EQ(0, actualVal);

	EXPECT_EQ(0, retval);

	aEiger.iterateAndDeleteChunkBuffer(chList);
}

TEST(EigerTest, DISABLED_CharArray11ZerosAndOne1)
{
	unsigned char cArr[] = {1,0,0,0,0,0,0,0,0,0,0,0};

	size_t arrLen = (sizeof(cArr) / sizeof(char));

	EXPECT_EQ(12, arrLen);

	list<CHUNK> chList;

	int retval = -1;

	Eiger aEiger;

	retval = aEiger.makeChunks(cArr, arrLen, chList, DeltaWhich::Orig);

	EXPECT_EQ(0, retval);

	retval = aEiger.iterateChunksForRH(chList);

	EXPECT_EQ(0, retval);

	int counter = 0;
	uint actualVal = 9999;

	for (list<CHUNK>::iterator it = chList.begin(); it != chList.end(); it++)
	{
		if (0 == counter)
		{
			EXPECT_EQ(0, (*it).hash);

			actualVal = (*it).hash;
		}

		cout << "Counter: " << counter << endl;

		counter++;
	}

	cout << "Actual value: " << actualVal << endl;

	EXPECT_EQ(0, actualVal);

	EXPECT_EQ(0, retval);

	aEiger.iterateAndDeleteChunkBuffer(chList);
}

