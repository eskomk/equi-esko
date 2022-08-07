#include <iostream>
#include <fstream>
#include <cstring>
#include <cmath>
// #include "chunk.h"
#include "eiger03.h"

// extern uint CHUNK_MAX_SIZE_EIGER;

using namespace std;

int main(int argc, char **argv) {
	cout << "Hello world" << endl;

	if (argc < 3)
	{
		cout << "Usage: eiger03 <filename> <filename>" << endl;
		exit(0);
	}

	int retval = -1;

	Eiger theEig;

	retval = theEig.runThings(
			"/home/esko02/eclipse-workspace/eiger02/build/default/compile_commands.json",
			"/home/esko02/eclipse-workspace/eiger03/build/default/kissa.txt");

	/*

	int cfOK = -10;
	CHUNK_FILE cf1;
	CHUNK_FILE cf2;
	// string strFilename = argv[1];

	cfOK = retrieveChunkFile(argv[1], cf1);

	if (0 != cfOK)
	{
		cout << "Could not read buffer #1" << endl;

		return cfOK;
	}

	cout << "cf1.len: " << cf1.len << endl;
	cout << "cf1 Filename: " << cf1.filename << endl;
	// cout << "cf1 Data:" << endl << cf1.buffer << endl;

	makeChunks(cf1.buffer, cf1.len, cf1.chunkList, 1);
	cout << "main #1" << endl;

	delete [] cf1.buffer;

	cfOK = retrieveChunkFile(argv[2], cf2);

	if (0 != cfOK)
	{
		cout << "Could not read buffer #2" << endl;

		return cfOK;
	}

	cout << "cf2.len: " << cf2.len << endl;
	cout << "cf2 Filename: " << cf2.filename << endl;
	// cout << "cf2 Data:" << endl << cf2.buffer << endl;

	makeChunks(cf2.buffer, cf2.len, cf2.chunkList, 2);

	delete [] cf2.buffer;

	cout << "main #2" << endl;

	list<CHUNK> resList;

	iterateChunksForRH(cf1.chunkList);
	iterateChunksForRH(cf2.chunkList);

	cout << "main #3" << endl;

	// iterateChunks(resList);

	// scanChunks(cf1.chunkList, cf2.chunkList, resList);
	composeDelta(cf1.chunkList, cf2.chunkList, resList);

	cout << "main #4" << endl;

	// iterateChunksForRH(resList);
	iterateChunks(resList);

	iterateAndDeleteChunkBuffer(cf1.chunkList);
	iterateAndDeleteChunkBuffer(cf2.chunkList);

	// iterateChunksForRH(resList);

	cout << "THE END, joko nyt taas payday ?" << endl;

	*/

	return retval;
}



