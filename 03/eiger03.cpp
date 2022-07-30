#include <iostream>
#include <fstream>
#include <cstring>
#include <cmath>
#include "chunk.h"
#include "eiger03.h"

using namespace std;

int main(int argc, char **argv) {
	cout << "Hello world" << endl;

	if (argc < 2)
	{
		cout << "Usage: eiger03 <filename>" << endl;
		exit(0);
	}

	int cfOK = -10;
	CHUNK_FILE cf;
	// string strFilename = argv[1];

	cfOK = retrieveChunkFile(argv[1], cf);

	if (0 != cfOK)
	{
		cout << "Could not read buffer" << endl;

		return cfOK;
	}

	cout << "cf.len: " << cf.len << endl;
	cout << "cf Filename: " << cf.filename << endl;
	cout << "cf Data:" << endl << cf.buffer << endl;

	uint lenLeft = cf.len;
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

		memcpy(ch.data, cf.buffer + chPointer, ch.len);

		cf.chunkList.push_back(ch);

		chPointer += ch.len;
	}

	cout << "main #1" << endl;

	delete [] cf.buffer;

	cout << "main #2" << endl;

	for (list<CHUNK>::iterator it = cf.chunkList.begin(); it != cf.chunkList.end(); ++it)
	{
		// unsigned char outData[CHUNK_MAX_SIZE + 1];
		unsigned char outData[(*it).len + 1];

		memcpy(outData, (*it).data, (*it).len);
		outData[(*it).len] = 0;

	    // cout << "data: " << (*it).data << endl;
		cout << "outData: " << outData << endl;

		uint aHash = 0;
		int calcOK = -1;

		calcOK = calculateRollingHash((*it).data, (*it).len, aHash);

		if (0 == calcOK)
		{
			cout << "Hash: " << aHash << endl;
		}
		else
		{
			cout << "Hash NOT OK, return value: " << calcOK << endl;
		}

	    delete [] (*it).data;
	}

	cout << "THE END" << endl;

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

	for (uint i = 0; i + pwinsize < pdatalen; i++)
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
	// delete pbuf01;
	file01.close();

	return 0;
}


