#include <iostream>
#include <fstream>
#include <cstring>
#include <cmath>
#include "chunk.h"
#include "eiger03.h"

using namespace std;

int main(int argc, char **argv) {
	cout << "Hello world" << endl;

	if (argc < 3)
	{
		cout << "Usage: eiger03 <filename> <filename>" << endl;
		exit(0);
	}

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

	makeChunks(cf1.buffer, cf1.len, cf1.chunkList);
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

	makeChunks(cf2.buffer, cf2.len, cf2.chunkList);

	delete [] cf2.buffer;

	cout << "main #2" << endl;

	list<CHUNK> resList;

	iterateChunksForRH(cf1.chunkList);
	iterateChunksForRH(cf2.chunkList);

	cout << "main #3" << endl;

	// iterateChunks(resList);

	scanChunks(cf1.chunkList, cf2.chunkList, resList);

	cout << "main #4" << endl;

	iterateChunksForRH(resList);

	iterateAndDeleteChunkBuffer(cf1.chunkList);
	iterateAndDeleteChunkBuffer(cf2.chunkList);

	// iterateChunksForRH(resList);

	cout << "THE END, joko nyt taas payday ?" << endl;

	return 0;
}

int scanChunks(list<CHUNK> pchl1, list<CHUNK> pchl2, list<CHUNK> & pchlres)
{
	for (list<CHUNK>::iterator it2 = pchl2.begin(); it2 != pchl2.end(); ++it2)
	{
		for (list<CHUNK>::iterator it1 = pchl1.begin(); it1 != pchl1.end(); ++it1)
		{
			cout << "Res up: hash 1: " << (*it1).hash << endl;

			if ((*it2).hash == (*it1).hash)
			{
				pchlres.push_back((*it1));

				cout << "Res: hash 1: " << (*it1).hash << endl;
				cout << "Res: hash 2: " << (*it2).hash << endl;
			}
		}
	}

	return 0;
}


int iterateChunksForRH(list<CHUNK> & pchlist)
{
	cout << "iterateChunksForRH START" << endl;

	for (list<CHUNK>::iterator it = pchlist.begin(); it != pchlist.end(); ++it)
	{
		// unsigned char outData[CHUNK_MAX_SIZE + 1];
		unsigned char outData[(*it).len + 1];

		memcpy(outData, (*it).data, (*it).len);
		outData[(*it).len] = 0;

		cout << "(*it).len: " << (*it).len << endl;

	    // cout << "data: " << (*it).data << endl;
		cout << "outData: " << outData << endl;

		uint aHash = 0;
		int calcOK = -1;

		calcOK = calculateRollingHash((*it).data, (*it).len, aHash);

		if (0 == calcOK)
		{
			(*it).hash = aHash;
			cout << "Hash iter: " << (*it).hash << endl;
		}
		else
		{
			cout << "Hash NOT OK, return value: " << calcOK << endl;
		}

	    // delete [] (*it).data;
	}

	cout << "iterateChunksForRH END" << endl;

	return 0;
}

int iterateAndDeleteChunkBuffer(list<CHUNK> & pchlist)
{
	for (list<CHUNK>::iterator it = pchlist.begin(); it != pchlist.end(); ++it)
	{
		delete [] (*it).data;
	}

	return 0;
}

int makeChunks(char * pfrom, size_t pfromlen, list<CHUNK> & pchlist)
{
	cout << "makeChunks START" << endl;

	uint lenLeft = pfromlen;
	uint chPointer = 0;

	while (lenLeft > 0)
	{
		CHUNK ch;

		if (lenLeft >= CHUNK_MAX_SIZE)
		{
			ch.len = CHUNK_MAX_SIZE;
			lenLeft -= CHUNK_MAX_SIZE;
		}
		else
		{
			ch.len = lenLeft;
			lenLeft = 0;
		}

		ch.data = new unsigned char[ch.len];

		cout << "while #1, chPointer: " << chPointer << endl;
		cout << "while #2, ch.len: " << ch.len << endl;

		memcpy(ch.data, pfrom + chPointer, ch.len);

		pchlist.push_back(ch);

		chPointer += ch.len;
	}

	cout << "makeChunks END" << endl;

	return 0;
}

int calculateRollingHash(unsigned char * pdata, uint pdatalen, uint & prhash, uint pwinsize)
{
	if (pdata == nullptr)
	{
		cout << "pdata was NULL" << endl;

		return -1;
	}

	if (pdatalen < 1)
	{
		cout << "pdata length < 1" << endl;

		return -2;
	}

	if (pwinsize < 1)
	{
		cout << "Window size < 1" << endl;

		return -3;
	}

	if (pwinsize > pdatalen)
	{
		pwinsize = pdatalen;
	}

	uint H = 0;
	uint p = 11;
	uint m = pow(10, 9) + 7;

	for (uint k = 0; k < pwinsize; k++)
	{
		H += ((uint) (pdata[k] * pow(p, k))) % m;
	}

	H = H % m;

	for (uint i = 1; i + pwinsize < pdatalen; i++)
	{
		H = (H / p) - (((uint) (pdata[i] * pow(p, 0))) % m) + (((uint) (pdata[i + pwinsize] * pow(p, pwinsize))) % m);
		H = H % m;
	}

	prhash = H;

	return 0;
}

int retrieveChunkFile(string pfilename, CHUNK_FILE & pcf)
{
	ifstream file01;
	filebuf * pbuf01 = nullptr;
	size_t size01;
	char * buffer01 = nullptr;
	bad_alloc ba;

	try
	{
		file01.open(pfilename, fstream::binary | fstream::in);
		pbuf01 = file01.rdbuf();
		size01 = pbuf01->pubseekoff(0, file01.end, file01.in);
		pbuf01->pubseekpos (0, file01.in);
		buffer01 = new char[size01];
	}
	catch (bad_alloc & ba)
	{
		cout << "ba: " << ba.what() << endl;
		cout << "Bad filename: " << pfilename << endl;
		// cout << "Exiting ..." << endl;
		// exit(1);

		return -1;
	}

	pcf.len = size01;

	pbuf01->sgetn(buffer01, pcf.len);

	cout << "#1" << endl;

	pcf.filename = pfilename;
	// pcf.buffer = buffer01;
	pcf.buffer = new char[pcf.len];
	memcpy(pcf.buffer, buffer01, pcf.len);

	cout << "#2" << endl;

	delete [] buffer01;
	file01.close();

	return 0;
}


