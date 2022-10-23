#include <iostream>
#include <math.h>
#include "AudioFile.h"

using namespace std;

int main(int argc, char *argv[])
{
    // Command line arguments processing
    if (argc != 3)
    {
        cout << "\033[1;31mError: Usage sintax is ./wav_cmp <orignal filename> <quantized filename>\033[0m" << endl;
        return 0;
    }
    char *originalfile = argv[1];
    char *compressedfile = argv[2];

    AudioFile<double> originalaf;
    originalaf.shouldLogErrorsToConsole(false);
    // Load input audio file
    if (originalaf.load(originalfile) == 0)
    {
        cout << "\033[1;31mERROR: File doesn't exist or otherwise can't load file\033[0m" << endl;
        return 0;
    }

    AudioFile<double> compressedaf;
    compressedaf.shouldLogErrorsToConsole(false);
    // Load input audio file
    if (compressedaf.load(compressedfile) == 0)
    {
        cout << "\033[1;31mERROR: File doesn't exist or otherwise can't load file\033[0m" << endl;
        return 0;
    }
    // Both files must have the same number of samples
    if (originalaf.getNumChannels() * originalaf.getNumSamplesPerChannel() != compressedaf.getNumChannels() * compressedaf.getNumSamplesPerChannel())
    {
        cout << "\033[1;31mError: Both files must have the same number of samples\033[0m" << endl;
        return 0;
    }

    // Variables to calculate the maximum per sample absolute error
    double maxError = 0, tmpError;
    // Obtain total number of samples
    double N = (double)originalaf.getNumChannels() * (double)originalaf.getNumSamplesPerChannel();

    // Get audiofile metainfo
    int numChannels = originalaf.getNumChannels();
    int numSamples = originalaf.getNumSamplesPerChannel();

    // Calculating Noise Power (Distortion) and Signal Power
    double D = 0, S = 0;
    for (int j = 0; j < numChannels; j++)
    {
        for (int i = 0; i < numSamples; i++)
        {
            D += pow(originalaf.samples[j][i] - compressedaf.samples[j][i], 2);
            S += pow(originalaf.samples[j][i], 2);
            tmpError = abs(originalaf.samples[j][i] - compressedaf.samples[j][i]);
            // Update maxError if necessary
            if (tmpError > maxError)
                maxError = tmpError;
        }
    }

    D = (1 / N) * D;
    S = (1 / N) * S;

    // Calculating SNR (expressed in dB)
    double SNR = 10 * log10(S / D);
    cout << "\033[1;34mSNR: " << SNR << " dB\nMaximum per sample absolute error: " << maxError << "\033[0m" << endl;

    return 0;
}