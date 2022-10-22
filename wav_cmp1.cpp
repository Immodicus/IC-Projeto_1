#include <iostream>
#include <vector>
#include <sndfile.hh>
#include "wav_cmp.h"

using namespace std;

int main(int argc, char *argv[])
{
	if(argc < 3) 
    {
		cerr << "Usage: " << argv[0] << " <original file> <quantized file>\n";
		return 1;
	}

	SndfileHandle original { argv[1] };
    SndfileHandle quantized { argv[2] };

	if(original.error() || quantized.error()) 
    {
		cerr << "Error: invalid input file\n";
		return 1;
    }

	if(((original.format() & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV) || 
	   ((quantized.format() & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV)) 
    {
		cerr << "Error: file is not in WAV format\n";
		return 1;
	}

	if(((original.format() & SF_FORMAT_SUBMASK) != SF_FORMAT_PCM_16) || 
	   ((quantized.format() & SF_FORMAT_SUBMASK) != SF_FORMAT_PCM_16)) 
    {
		cerr << "Error: file is not in PCM_16 format\n";
		return 1;
	}

    WAVCmp::PrintSNR(original, quantized);

    return EXIT_SUCCESS;
}