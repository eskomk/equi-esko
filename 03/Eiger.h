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
	DeltaWhich which;
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
	ulong hash;

	DELTA(DeltaWhich pwhich, ulong phash)
	{
		which = pwhich;
		hash = phash;
	}
};

class Eiger {
public:

	/**
	 * Sets CHUNK_MAX_SIZE_EIGER = 100
	 */
	Eiger();

	/**
	 * @param pchunkmaxsize Sets CHUNK_MAX_SIZE_EIGER
	 */
	Eiger(uint pchunkmaxsize);

	virtual ~Eiger();

	/**
	 * Used in standalone app (runThings calls)
	 * @return 0 if successful, < 0 otherwise
	 */
	int retrieveChunkFile(string pfilename, CHUNK_FILE & pcf);

	/**
	 * Actual rolling hash calculation
	 * @return 0 if successful, < 0 otherwise
	 */
	int calculateRollingHash(unsigned char * pdata, uint pdatalen, unsigned long & prhash, uint pwinsize = 10);

	/**
	 * Makes chunks aout of pfrom buffer
	 * @return 0 if successful, < 0 otherwise
	 */
	int makeChunks(unsigned char * pfrom, size_t pfromlen, list<CHUNK> & pchlist, DeltaWhich pwhich);

	/**
	 * Calls calculateRollingHash() for each chunk
	 * @return 0 if successful, < 0 otherwise
	 */
	int iterateChunksForRH(list<CHUNK> & pchlist);

	/**
	 * Not actually used anywhere. Left here only for "historical reasons"
	 * @return 0 if successful, < 0 otherwise
	 */
	int scanChunks(list<CHUNK> pchl1, list<CHUNK> pchl2, list<CHUNK> & pchlres);

	/**
	 * If smart pointers were used, this function would render obsolete
	 * @return 0 if successful, < 0 otherwise
	 */
	int iterateAndDeleteChunkBuffer(list<CHUNK> & pchlist);

	/**
	 * Produces actual delta list
	 * @return 0 if successful, < 0 otherwise
	 */
	int composeDelta(vector<CHUNK> pchl1, vector<CHUNK> pchl2, list<DELTA> & preshashes);

	/**
	 * For printing info about chunks
	 * @return 0 if successful, < 0 otherwise
	 */
	int iterateChunks(list<CHUNK> & pchlist);

	/**
	 * For printing info about delta
	 * @return 0 if successful, < 0 otherwise
	 */
	int iterateDelta(list<DELTA> pdelta);

	/**
	 * For printing info about hashes
	 * @return 0 if successful, < 0 otherwise
	 */
	int iterateChunkListHashes(list<CHUNK> pchlist);

	/**
	 * Used in standalone app (runThings calls)
	 * Entries format in pfilename depends whether chunk is from original file or update file:
	 * Each entry
	 * - start with "which" info (4 bytes)
	 * -- 1: chunk is from original file
	 * -- 2: chunk comes from update file
	 * - next is hash (8 bytes)
	 * If chunk is from update file, entry has two additional fields:
	 * - len: length of chunk data buffer (8 bytes)
	 * - chunk data buffer (len number of bytes)
	 * @return 0 if successful, < 0 otherwise
	 */
	int writeDeltaFile(string pfilename, list<DELTA> pdlist, list<CHUNK> pclist);

	/**
	 * If you run standalone app (eiger03), this function handles all the stuff
	 * @return 0 if successful, < 0 otherwise
	 */
	int runThings(string pfile1, string pfile2, string pdeltafile);

	uint CHUNK_MAX_SIZE_EIGER;
};

#endif /* EIGER_H_ */
