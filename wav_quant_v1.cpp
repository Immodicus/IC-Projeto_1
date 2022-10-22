#include <iostream>
#include <vector>
#include <math.h>
#include "AudioFile.h"

using namespace std;

int main(int argc, char *argv[])
{
    // Command line arguments processing
    if (argc != 2)
    {
        cout << "\033[1;31mError: Usage sintax is ./wav_quant <input filename>\033[0m" << endl;
        return 0;
    }
    char *inputfile = argv[1];

    // AudioFile
    AudioFile<double> af;
    af.shouldLogErrorsToConsole(false);
    // Load audiofile
    if (af.load(inputfile) == 0)
    {
        cout << "\033[1;31mERROR: File doesn't exist or otherwise can't load file\033[0m" << endl;
        return 0;
    }
    // Vectors to plots ---> opcional
    vector<double> originalsamples;
    vector<double> samples;
    vector<int> nums;
    int cnt = 0;

    int Amax = 1, Amin = -1;
    int nLevels;
    cout << "Number of Level (8, 16, 32): ";
    cin >> nLevels;
    if (nLevels > 33)
    {
        cout << "\033[1;31mERROR: Ther number Level must be less than 33\033[0m" << endl;
        return 0;
    }
    if (nLevels % 2 != 0 && nLevels != 1)
    {
        cout << "\033[1;31mERROR: Ther number level must be the power of 2 (8, 16, 32)\033[0m" << endl;
        return 0;
    }
    if (nLevels < 1)
    {
        cout << "\033[1;31mERROR: Ther number of bit Level must positive integer\033[0m" << endl;
        return 0;
    }
    double delta = (Amax - Amin) / pow(nLevels, 2);

    int option = -1;
    while (option != 0 && option != 1)
    {
        cout << "Choose quantization option (0 for Mid-riser or 1 for Mid-tread):";
        cin >> option;
    }

    // Get audiofile metainfo
    int numChannels = af.getNumChannels();
    int numSamples = af.getNumSamplesPerChannel();

    for (int j = 0; j < numChannels; j++)
    {
        for (int i = 0; i < numSamples; i++)
        {
            originalsamples.push_back(af.samples[j][i]);

            if (option == 0) // Mid-riser quantization
                af.samples[j][i] = delta * (floor((af.samples[j][i] / delta)) + 0.5);
            else // Mid-tread quantization
                af.samples[j][i] = delta * floor((af.samples[j][i] / delta) + 0.5);

            samples.push_back(af.samples[j][i]);
            nums.push_back(cnt);
            cnt++;
        }
    }

    af.setBitDepth(nLevels);
    af.save("wav_quant_out.wav");

    return 0;
}