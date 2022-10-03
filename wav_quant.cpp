#include <iostream>
#include <vector>
#include <sndfile.hh>
#include "wav_quant.h"

using namespace std;

int main(int argc, char *argv[]) {

	if(argc < 3) {
		cerr << "Usage: " << argv[0] << " <input file> <output file> [outBits]\n";
		return 1;
	}

	SndfileHandle sndFile { argv[1] };
	if(sndFile.error()) {
		cerr << "Error: invalid input file\n";
		return 1;
    }

	if((sndFile.format() & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV) {
		cerr << "Error: file is not in WAV format\n";
		return 1;
	}

	if((sndFile.format() & SF_FORMAT_SUBMASK) != SF_FORMAT_PCM_16) {
		cerr << "Error: file is not in PCM_16 format\n";
		return 1;
	}

	WAVQuant qnt ( argv[2], sndFile );
    if(argc == 4)
    {
        std::cout << (uint32_t)atoi(argv[3]) << std::endl;
        qnt.Quantize((uint8_t)atoi(argv[3]));
    }
    else
    {
        qnt.Quantize(8);
    }


	return EXIT_SUCCESS;
}