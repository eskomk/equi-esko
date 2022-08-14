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

Eiger::Eiger(uint pchunkmaxsize)
{
	this->CHUNK_MAX_SIZE_EIGER = pchunkmaxsize;
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

	cfOK = this->retrieveChunkFile(pfile1, cf1);

	if (0 != cfOK)
	{
		cout << "Could not read buffer #1" << endl;

		return cfOK;
	}

	cout << "cf1.len: " << cf1.len << endl;
	cout << "cf1 Filename: " << cf1.filename << endl;
	// cout << "cf1 Data:" << endl << cf1.buffer << endl;

	this->makeChunks(cf1.buffer, cf1.len, cf1.chunkList, DeltaWhich::Orig);
	cout << "main #1" << endl;

	delete [] cf1.buffer;

	cfOK = this->retrieveChunkFile(pfile2, cf2);

	if (0 != cfOK)
	{
		cout << "Could not read buffer #2" << endl;

		return cfOK;
	}

	cout << "cf2.len: " << cf2.len << endl;
	cout << "cf2 Filename: " << cf2.filename << endl;
	// cout << "cf2 Data:" << endl << cf2.buffer << endl;

	this->makeChunks(cf2.buffer, cf2.len, cf2.chunkList, DeltaWhich::Update);

	delete [] cf2.buffer;

	cout << "main #2" << endl;

	list<DELTA> deltaList;

	this->iterateChunksForRH(cf1.chunkList);
	this->iterateChunksForRH(cf2.chunkList);

	cout << "main #3" << endl;

	this->iterateChunks(cf1.chunkList);
	this->iterateChunks(cf2.chunkList);

	vector<CHUNK> vcf1(cf1.chunkList.begin(), cf1.chunkList.end());
	vector<CHUNK> vcf2(cf2.chunkList.begin(), cf2.chunkList.end());

	// this->composeDelta(cf1.chunkList, cf2.chunkList, deltaList);
	this->composeDelta(vcf1, vcf2, deltaList);

	cout << "main #4" << endl;

	this->iterateChunkListHashes(cf1.chunkList);
	this->iterateChunkListHashes(cf2.chunkList);

	// iterateChunksForRH(resList);
	// iterateChunks(resList);

	this->iterateAndDeleteChunkBuffer(cf1.chunkList);
	this->iterateAndDeleteChunkBuffer(cf2.chunkList);

	// iterateChunksForRH(resList);

	this->iterateDelta(deltaList);

	cout << "THE END, joko nyt taas payday oliona ?" << endl;

	return 0;
}

int Eiger::iterateDelta(list<DELTA> pdelta)
{
	cout << "iterateDelta BEGIN" << endl;

	int counter = 0;

	for (list<DELTA>::iterator it = pdelta.begin(); it != pdelta.end(); it++)
	{
		cout << counter << ": " << (*it).which << ": " << (*it).hash << endl;
		counter++;
	}

	cout << "iterateDelta END" << endl;

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

int Eiger::iterateChunkListHashes(list<CHUNK> pchlist)
{
	cout << "iterateChunkListHashes START" << endl;

	int counter = 0;

	for (list<CHUNK>::iterator it = pchlist.begin(); it != pchlist.end(); it++)
	{
		cout << counter << ": " << (*it).hash << endl;

		counter++;
	}

	cout << "iterateChunkListHashes END" << endl;

	return 0;
}

int Eiger::composeDelta(vector<CHUNK> pchl1, vector<CHUNK> pchl2, list<DELTA> & preshashes)
{
	cout << "composeDelta START" << endl;

	uint size1 = pchl1.size();
	uint size2 = pchl2.size();

	cout << "Size1: " << size1 << endl;
	cout << "Size2: " << size2 << endl;

	// for (list<CHUNK>::iterator it2 = pchl2.begin(); it2 != pchl2.end(); it2++)
	for (uint i2 = 0; i2 < size2; i2++)
	{
		// for (list<CHUNK>::iterator it1 = pchl1.begin(); it1 != pchl1.end(); it1++)
		for (uint i1 = 0; i1 < size1; i1++)
		{
			// if ((*it2).hash == (*it1).hash)
			if (pchl2[i2].hash == pchl1[i1].hash)
			{
				// DELTA aDelta((*it1).which, (*it1).hash);
				DELTA aDelta((pchl1[i1]).which, (pchl1[i1]).hash);

				preshashes.push_back(aDelta);

				bool it1StuffEnded = false;

				// while (it2 != pchl2.end() && it1 != pchl1.end())
				while (i2 < size2 && i1 < size1)
				{
					// it2++;
					// it1++;
					i2++;
					i1++;

					// if ((*it2).hash == (*it1).hash)
					if (pchl2[i2].hash == pchl1[i1].hash)
					{
						// DELTA bDelta((*it1).which, (*it1).hash);
						DELTA bDelta(pchl1[i1].which, pchl1[i1].hash);
						preshashes.push_back(bDelta);
					}
					else
					{
						break;
					}

					/*
					if ((*it2).hash == (*it1).hash)
					{
						preshashes.push_back(0);
					}
					else
					{
						it1StuffEnded = true;

						// preshashes.push_back((*it1).hash);
						// preshashes.push_back((*it2).hash);

						break;
					}
					*/
				}
			}
		}

		// if (it2 != pchl2.end())
		if (i2 < size2)
		{
			// DELTA cDelta((*it2).which, (*it2).hash);
			DELTA cDelta(pchl2[i2].which, pchl2[i2].hash);

			preshashes.push_back(cDelta);
		}
	}

	/*
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

	*/

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

		unsigned long aHash = 0;
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
		if ((*it).data)
		{
			delete [] (*it).data;
		}
	}

	return 0;
}

int Eiger::makeChunks(unsigned char * pfrom, size_t pfromlen, list<CHUNK> & pchlist, DeltaWhich pwhich)
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

int Eiger::calculateRollingHash(unsigned char * pdata, uint pdatalen, unsigned long & prhash, uint pwinsize)
{
	cout << "calculateRollingHash START" << endl;

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

	unsigned long H = 0;
	uint p = 11;
	uint m = pow(10, 9) + 7;

	uint p0_pow = 1;

	// cout << "Data 1: " << endl;

	for (uint k = 0; k < pwinsize; k++)
	{
		p0_pow = (p0_pow * p) % m;
		// H += ((uint) ((pdata[k] + 1) * pow(p, k))) % m;
		H += (((pdata[k] + 1) * p0_pow)) % m;

		// cout << (pdata[k] + 1) << " pow: " << p0_pow << " H: " << H << endl;
	}

	H = H % m;

	uint p1_pow = 1;
	uint p1_pow_winsize = 1;

	for (uint k = 0; k < pwinsize; k++)
	{
		p1_pow_winsize = (p1_pow_winsize * p) % m;
	}

	// cout << "Data 2: "  << endl;;

	for (uint i = 1; i + pwinsize < pdatalen; i++)
	{
		p1_pow *= p;
		// H = (H / p) - (((uint) ((pdata[i] + 1) * pow(p, 1))) % m) + (((uint) ((pdata[i + pwinsize] + 1) * pow(p, pwinsize))) % m);
		// H = (H / p) - ((((pdata[i] + 1) * p1_pow)) % m) + ((((pdata[i + pwinsize] + 1) * p1_pow_winsize)) % m);
		H = H - ((((pdata[i] + 1) * p1_pow)) % m) + ((((pdata[i + pwinsize] + 1) * p1_pow_winsize)) % m);
		H = H % m;

		// cout << (pdata[i] + 1) << "pow: " << p1_pow << " " << (pdata[i + pwinsize] + 1) << " H: " << H << endl;
	}

	prhash = H;

	cout << "calculateRollingHash END" << endl;

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
	pcf.buffer = new unsigned char[pcf.len];
	memcpy(pcf.buffer, buffer01, pcf.len);

	cout << "#2" << endl;

	delete [] buffer01;
	file01.close();

	return 0;
}

