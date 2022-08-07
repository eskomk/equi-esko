/*
 * Eiger.cpp
 *
 *  Created on: Aug 7, 2022
 *      Author: esko02
 */

#include <iostream>
#include <fstream>
#include <cstring>
#include <cmath>

#include "Eiger.h"

Eiger::Eiger() {
	// TODO Auto-generated constructor stub
	CHUNK_MAX_SIZE_EIGER = 100;
}

Eiger::~Eiger() {
	// TODO Auto-generated destructor stub
}

int Eiger::runThings(string pfile1, string pfile2)
{
	int cfOK = -10;
	CHUNK_FILE cf1;
	CHUNK_FILE cf2;
	// string strFilename = argv[1];

	cfOK = retrieveChunkFile(pfile1, cf1);

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

	cfOK = retrieveChunkFile(pfile2, cf2);

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

	cout << "THE END, joko nyt taas payday oliona ?" << endl;

	return 0;

}

int Eiger::scanChunks(list<CHUNK> pchl1, list<CHUNK> pchl2, list<CHUNK> & pchlres)
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


int Eiger::composeDelta(list<CHUNK> pchl1, list<CHUNK> pchl2, list<CHUNK> & pchlres)
{
	cout << "composeDelta START" << endl;

	for (list<CHUNK>::iterator it1 = pchl1.begin(); it1 != pchl1.end(); ++it1)
	{
		pchlres.push_back((*it1));
	}

	for (list<CHUNK>::iterator itRes = pchlres.begin(); itRes != pchlres.end(); ++itRes)
	{
		for (list<CHUNK>::iterator it2 = pchl2.begin(); it2 != pchl2.end(); ++it2)
		{
			if ((*itRes).hash == (*it2).hash)
			{
				(*it2).isInRes = true;

				break;
			}
		}
	}

	for (list<CHUNK>::iterator it2 = pchl2.begin(); it2 != pchl2.end(); ++it2)
	{
		if (!(*it2).isInRes)
		{
			pchlres.push_back((*it2));
		}
	}

	cout << "composeDelta END" << endl;

	return 0;
}


int Eiger::iterateChunks(list<CHUNK> & pchlist)
{
	cout << "iterateChunks START" << endl;

	for (list<CHUNK>::iterator it = pchlist.begin(); it != pchlist.end(); ++it)
	{
		// unsigned char outData[CHUNK_MAX_SIZE + 1];
		unsigned char outData[(*it).len + 1];

		memcpy(outData, (*it).data, (*it).len);
		outData[(*it).len] = 0;

		cout << "(*it).len: " << (*it).len << endl;

	    // cout << "data: " << (*it).data << endl;
		cout << "outData: " << outData << endl;

		cout << "Hash iter: " << (*it).hash << endl;
	}

	cout << "iterateChunks END" << endl;

	return 0;

}


int Eiger::iterateChunksForRH(list<CHUNK> & pchlist)
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

int Eiger::iterateAndDeleteChunkBuffer(list<CHUNK> & pchlist)
{
	for (list<CHUNK>::iterator it = pchlist.begin(); it != pchlist.end(); ++it)
	{
		delete [] (*it).data;
	}

	return 0;
}

int Eiger::makeChunks(char * pfrom, size_t pfromlen, list<CHUNK> & pchlist, uint pwhich)
{
	cout << "makeChunks START" << endl;

	uint lenLeft = pfromlen;
	uint chPointer = 0;

	while (lenLeft > 0)
	{
		CHUNK ch;

		ch.which = pwhich;

		if (lenLeft >= CHUNK_MAX_SIZE_EIGER)
		// if (lenLeft >= 100)
		{
			ch.len = CHUNK_MAX_SIZE_EIGER;
			lenLeft -= CHUNK_MAX_SIZE_EIGER;
			// ch.len = 100;
			// lenLeft -= 100;
		}
		else
		{
			ch.len = lenLeft;
			lenLeft = 0;
		}

		ch.data = new unsigned char[ch.len];

		cout << "while #1, chPointer: " << chPointer << endl;
		cout << "while #2, ch.len: " << ch.len << endl;
		cout << "while #2, ch.which: " << ch.which << endl;

		memcpy(ch.data, pfrom + chPointer, ch.len);

		pchlist.push_back(ch);

		chPointer += ch.len;
	}

	cout << "makeChunks END" << endl;

	return 0;
}

int Eiger::calculateRollingHash(unsigned char * pdata, uint pdatalen, uint & prhash, uint pwinsize)
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

int Eiger::retrieveChunkFile(string pfilename, CHUNK_FILE & pcf)
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

