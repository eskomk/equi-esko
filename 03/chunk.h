/*
 * chunk.h
 *
 *  Created on: Jul 28, 2022
 *      Author: esko02
 */

#ifndef CHUNK_H_
#define CHUNK_H_

#include <list>

using namespace std;

uint CHUNK_MAX_SIZE = 100;

typedef struct CHUNK_STRUCT
{
	uint id;
	size_t len;
	unsigned char * data;
} CHUNK;

typedef struct CHUNK_FILE_STRUCT
{
	uint id;
	string filename;
	size_t len;
	char * buffer;
	list<CHUNK> chunkList;
} CHUNK_FILE;

#endif /* CHUNK_H_ */
