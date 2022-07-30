/*
 * eiger03.h
 *
 *  Created on: Jul 28, 2022
 *      Author: esko02
 */

#ifndef EIGER03_H_
#define EIGER03_H_

#include <string>

#include "chunk.h"

using namespace std;

int retrieveChunkFile(string pfilename, CHUNK_FILE & pcf);
int calculateRollingHash(unsigned char * pdata, uint pdatalen, uint & prhash, uint pwinsize = 10);

#endif /* EIGER03_H_ */