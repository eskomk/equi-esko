/*
 * Eiger.h
 *
 *  Created on: Aug 7, 2022
 *      Author: esko02
 */

#ifndef EIGER_H_
#define EIGER_H_

#include <string>

#include "chunk.h"

using namespace std;

// uint CHUNK_MAX_SIZE_EIGER = 100;

enum DeltaWhich
{
	NA = 0,
	Orig = 1,
	Update = 2
};

typedef struct CHUNK_STRUCT
{
	// 0 = NA, 1 = orig buffer, 2 = update buffer
	// uint which = 0;
	DeltaWhich which;
	bool isInRes = false;
	uint id;
	size_t len = 0;
	unsigned char * data = nullptr;
	unsigned long hash;
} CHUNK;

typedef struct CHUNK_FILE_STRUCT
{
	uint id;
	string filename;
	size_t len;
	unsigned char * buffer;
	list<CHUNK> chunkList;
} CHUNK_FILE;

struct DELTA
{
	DeltaWhich which;
	uint hash;

	DELTA(DeltaWhich pwhich, uint phash)
	{
		which = pwhich;
		hash = phash;
	}

};

class Eiger {
public:
	Eiger();
	Eiger(uint pchunkmaxsize);
	virtual ~Eiger();

	int retrieveChunkFile(string pfilename, CHUNK_FILE & pcf);
	int calculateRollingHash(unsigned char * pdata, uint pdatalen, unsigned long & prhash, uint pwinsize = 10);
	int makeChunks(unsigned char * pfrom, size_t pfromlen, list<CHUNK> & pchlist, DeltaWhich pwhich);
	int iterateChunksForRH(list<CHUNK> & pchlist);
	int scanChunks(list<CHUNK> pchl1, list<CHUNK> pchl2, list<CHUNK> & pchlres);
	int iterateAndDeleteChunkBuffer(list<CHUNK> & pchlist);
	int composeDelta(vector<CHUNK> pchl1, vector<CHUNK> pchl2, list<DELTA> & preshashes);
	int iterateChunks(list<CHUNK> & pchlist);
	int iterateDelta(list<DELTA> pdelta);
	int iterateChunkListHashes(list<CHUNK> pchlist);

	int runThings(string pfile1, string pfile2);

	uint CHUNK_MAX_SIZE_EIGER;
};

#endif /* EIGER_H_ */
