#include <iostream>
#include <vector>
#include <map>
#include <sndfile.hh>
#include <stdint.h>
#include <exception>
#include <cmath>
#include <sstream>

constexpr const uint64_t FrameBufferSize = 256000;

class WAVCmp
{
    private:
        static constexpr int16_t Dequantize(int16_t s, uint8_t numBits)
        {
            return s << (16 - numBits);
        }

    public:
        static void PrintSNR(SndfileHandle& original, SndfileHandle& quantized, uint8_t numBits)
        {
            if(original.channels() != quantized.channels()
            || original.frames() != quantized.frames()
            || original.samplerate() != quantized.samplerate()
            || original.format() != quantized.format())
            {
                throw new std::runtime_error("Cannot calculate SNR for this 2 files.");
            }

            std::vector<int16_t> originalFrames;
            originalFrames.resize(FrameBufferSize * original.channels());

            std::vector<int16_t> quantizedFrames;
            quantizedFrames.resize(FrameBufferSize * quantized.channels());

            double dOriginalEnergy = 0.0;
            double dNoiseEnergy = 0.0;
            double dMaxAbsError = 0.0;

            size_t sOrReadFrames = 0;
            size_t sQtReadFrames = 0;

            while(((sOrReadFrames = original.readf(originalFrames.data(), FrameBufferSize)) == 
                   (sQtReadFrames = quantized.readf(quantizedFrames.data(), FrameBufferSize))) && sOrReadFrames != 0)
            {
                originalFrames.resize(sOrReadFrames * original.channels());
                quantizedFrames.resize(sQtReadFrames * quantized.channels());

                for(uint64_t f = 0; f < originalFrames.size(); f++)
                {
                    int16_t s = originalFrames[f];
                    int16_t b = quantizedFrames[f];

                    dOriginalEnergy += pow(abs((double)s), 2);

                    int16_t nb = Dequantize(b, numBits);

                    double dDiff = abs((double)s - (double)nb);
                    if(dDiff > dMaxAbsError) dMaxAbsError = dDiff;

                    dNoiseEnergy += pow(abs((double)s - (double)nb), 2);
                }
            }

            std::cout << "dOriginalEnergy is " << dOriginalEnergy << "\n";
            std::cout << "dNoiseEnergy is " << dNoiseEnergy << "\n";
            std::cout << "SNR is: " << 10.0 * log10(dOriginalEnergy / dNoiseEnergy) << " dB\n";
            std::cout << "Max per sample absolute error is: " << dMaxAbsError << "\n";
        }
};