#include <stdlib.h>
#include <stdio.h>
#include <chrono>
#include <iostream>
#include <assert.h>

#include "BitStream.h"

#define TESTFILESIZE 100 * 1000 * 1000 // test file size in bytes.
#define TESTFILENAME "bitstreamTestFile.test"
#define TESTFILENAME2 "bitstreamTestFile2.test"
#define TEST2BUFFERSIZE 2048
#define TEST3BUFFERSIZE 2347

int main()
{
    std::cout << "Creating test file....\n";
    FILE* testFile = fopen(TESTFILENAME, "w+");
    assert(testFile);

    for(size_t i = 0; i < TESTFILESIZE / sizeof(int); i++)
    {
        int c = rand();
        assert(fwrite(&c, sizeof(int), 1, testFile) == 1);
    }

    fclose(testFile);
    std::cout << "Done creating test file....\n";

    std::cout << "Beginning first performance test\n" << "Copying a " << TESTFILESIZE / 1000000 << " MB file bit by bit\n";
    
    auto begin = std::chrono::high_resolution_clock::now();

    BitStream* inBs = new BitStream(TESTFILENAME, "r");
    BitStream* outBs = new BitStream(TESTFILENAME2, "w");

    bool b;
    while(inBs->ReadBit(b))
    {
        outBs->WriteBit(b);
    }

    delete inBs;
    delete outBs;

    auto end = std::chrono::high_resolution_clock::now();
    auto timeTaken = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);

    std::cout << "First test took: " << (double)timeTaken.count() / 1000.0 
        << " seconds. Average of " << ((double)TESTFILESIZE / 1000000.0) / ((double)timeTaken.count() / 1000.0) << " MB/s\n";


    std::cout << "Beginning third performance test\n" << "Copying a " << TESTFILESIZE / 1000000 << " MB file in " << TEST2BUFFERSIZE << " bit blocks\n";

    begin = std::chrono::high_resolution_clock::now();

    inBs = new BitStream(TESTFILENAME, "r");
    outBs = new BitStream(TESTFILENAME2, "w");

    BitSet bitSet(TEST2BUFFERSIZE);
    while((bitSet = inBs->ReadNBits(TEST2BUFFERSIZE)).size() > 0)
    {
        outBs->WriteNBits(bitSet);
    }

    delete inBs;
    delete outBs;

    end = std::chrono::high_resolution_clock::now();
    timeTaken = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);

    std::cout << "Second test took: " << (double)timeTaken.count() / 1000.0 
        << " seconds. Average of " << ((double)TESTFILESIZE / 1000000.0) / ((double)timeTaken.count() / 1000.0) << " MB/s\n";

    std::cout << "Beginning third performance test\n" << "Copying a " << TESTFILESIZE / 1000000 << " MB file in " << TEST3BUFFERSIZE << " bit blocks\n";

    begin = std::chrono::high_resolution_clock::now();

    inBs = new BitStream(TESTFILENAME, "r");
    outBs = new BitStream(TESTFILENAME2, "w+");

    BitSet bitSet2(TEST3BUFFERSIZE);
    while((bitSet2 = inBs->ReadNBits(TEST3BUFFERSIZE)).size() > 0)
    {
        outBs->WriteNBits(bitSet2);
    }

    delete inBs;
    delete outBs;

    end = std::chrono::high_resolution_clock::now();
    timeTaken = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);

    std::cout << "Third test took: " << (double)timeTaken.count() / 1000.0 
        << " seconds. Average of " << ((double)TESTFILESIZE / 1000000.0) / ((double)timeTaken.count() / 1000.0) << " MB/s\n";
}