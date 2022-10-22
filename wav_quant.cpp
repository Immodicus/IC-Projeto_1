#include <iostream>
#include <vector>
#include <sndfile.hh>
#include "wav_quant.h"

using namespace std;

int main(int argc, char *argv[]) {

	if(argc < 3) {
		cerr << "Usage: " << argv[0] << " [ -mode round|midriser|midtread|shift (def round) ]\n";
		cerr << "                         [-b outBits (def 8)] <input file> <output file>\n";
		return 1;
	}

	SndfileHandle sndFile { argv[argc - 2] };
	QuantizationMode mode = Round;
	uint8_t nBits = 8;

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

	for (int n = 1; n < argc; n++)
	{
		if(string(argv[n]) == "-mode")
		{
			if (string(argv[n + 1]) == "round")
			{
				mode = Round;
				break;
			}
			if (string(argv[n + 1]) == "midriser")
			{
				mode = MidRiser;
				break;
			}
			if (string(argv[n + 1]) == "midtread")
			{
				mode = MidTread;
				break;
			}
			if (string(argv[n + 1]) == "shift")
			{
				mode = Shift;
				break;
			}
		}
	}

	for (int n = 1; n < argc; n++)
	{
		if(string(argv[n]) == "-b")
		{
			nBits = (uint8_t)atoi(argv[n + 1]);
			break;
		}
	}

	WAVQuant qnt ( argv[argc - 1], sndFile );

    assert(nBits < 16);
    qnt.Quantize(nBits, mode);


	return EXIT_SUCCESS;
}