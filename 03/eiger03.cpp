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

	return retval;
}



