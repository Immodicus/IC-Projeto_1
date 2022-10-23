#include <iostream>
#include <vector>
#include <math.h>
#include "AudioFile.h"

using namespace std;

typedef enum Effect
{
    SingleEcho,
    MultipleEchos,
    AmplitudeModulation
} Effect;

int main(int argc, char *argv[])
{
    // Command line arguments processing
    if (argc < 3)
    {
        cout << "\033[1;31mError: Usage sintax is ./wav_effects [-ef singleEcho|multEcho|am (def singleEcho)]\033[0m" << endl;
        cout << "\033[1;31m                                     [-g gain(def 0.75)] [-d delay(s) (def 0.5)]\033[0m" << endl;
        cout << "\033[1;31m                                     [-fc carrierFreq (def 500)] <input filename> <output filename>\033[0m" << endl;
        return 0;
    }

    char *inputfile = argv[argc-2];

    // Default settings
    Effect ef = SingleEcho;
    double gain = 0.75;
    double delay = 0.5;
    double fc = 500.0;

    for (int n = 1; n < argc; n++)
	{
		if(string(argv[n]) == "-ef")
		{
			if (string(argv[n + 1]) == "singleEcho")
			{
				ef = SingleEcho;
				break;
			}
            if (string(argv[n + 1]) == "multEcho")
			{
				ef = MultipleEchos;
				break;
			}
            if (string(argv[n + 1]) == "am")
			{
				ef = AmplitudeModulation;
				break;
			}
		}
	}

    for (int n = 1; n < argc; n++)
	{
		if(string(argv[n]) == "-g")
		{
			gain = atof(argv[n+1]);
            break;
		}
	}

    for (int n = 1; n < argc; n++)
	{
		if(string(argv[n]) == "-d")
		{
			delay = atof(argv[n+1]);
            break;
		}
	}

    for (int n = 1; n < argc; n++)
	{
		if(string(argv[n]) == "-fc")
		{
			fc = atof(argv[n+1]);
            break;
		}
	}

    // AudioFile
    AudioFile<double> af;
    af.shouldLogErrorsToConsole(false);

    AudioFile<double> out;

    // Load audiofile
    if (af.load(inputfile) == 0)
    {
        cout << "\033[1;31mERROR: File doesn't exist or otherwise can't load file\033[0m" << endl;
        return 0;
    }

    // Get audiofile meta information
    int numChannels = af.getNumChannels();
    int numSamples = af.getNumSamplesPerChannel();
    double sampleRate = static_cast<double>(af.getSampleRate());

    vector<vector<double>> samples;
    samples.resize(numChannels);

    int k = static_cast<int>(static_cast<double>(sampleRate) * delay);
    
    for (int i = 0; i < numChannels; i++)
    {
        for (int j = 0; j < numSamples; j++)
        {
            if(ef == SingleEcho)
            {
                if(j >= k) samples[i].push_back(af.samples[i][j] + (gain * af.samples[i][j - k]));
                else samples[i].push_back(af.samples[i][j]);
            }
            else if(ef == MultipleEchos)
            {
                if(j >= k) samples[i].push_back(af.samples[i][j] + gain * samples[i][j - k]);
                else samples[i].push_back(af.samples[i][j]);
            }
            else
            {
                // fc is the carrier frequency
            }
        }
    }
    
    out.setBitDepth(16);
    out.setSampleRate(af.getSampleRate());
    out.setNumChannels(af.getNumChannels());
    out.setNumSamplesPerChannel(af.getNumSamplesPerChannel());
    out.setAudioBuffer(samples);
    out.setAudioBufferSize(2, af.getNumSamplesPerChannel());
    out.save(argv[argc-1]);
}