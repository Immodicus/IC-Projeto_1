#include <iostream>
#include <vector>
#include <map>
#include <sndfile.hh>
#include <stdint.h>
#include <exception>
#include <cmath>
#include <sstream>

class WAVQuant
{
    private:
        SndfileHandle& original;
        const char* fileName;

        const uint64_t BufferSize = 256000;

        int16_t Quantize(int16_t s, uint8_t bBits)
        {
            if(bBits == 0 || bBits > 15) throw new std::runtime_error("Invalid number of bits");

            uint32_t srcNumBits = sizeof(int16_t) * 8;
            const uint32_t destNumBits = bBits;
            
            const uint32_t uBitDiff = srcNumBits - destNumBits;

            const uint16_t uDiff = pow(2, uBitDiff);
            const uint16_t uHalf = uDiff / 2;
            const int16_t uMax = (((int16_t)(pow(2, destNumBits))) / 2) - 1;

            int16_t result = s / uDiff;

            if(result < uMax)
            {
                if(s > 0)
                {
                    if(abs(s % uDiff) >= uHalf) result++;
                }
                else
                {
                    if(abs(s % uDiff) > uHalf) result--;
                }
            }

            return result;
        }
    
    public:
        WAVQuant(const char* outFileName, SndfileHandle& sfh) : original(sfh), fileName(outFileName) {};

        void Quantize(uint8_t bBits)
        {
            SndfileHandle quantized (fileName, SFM_WRITE, original.format(), original.channels(), original.samplerate());
        
            std::vector<int16_t> vReadBuffer (BufferSize * original.channels());
            std::vector<int16_t> vWriteBuffer (BufferSize * original.channels());

            size_t sRead;
            while((sRead = original.readf(vReadBuffer.data(), BufferSize)))
            {
                vReadBuffer.resize(sRead * original.channels());
                vWriteBuffer.resize(sRead * original.channels());

                for(uint64_t i = 0; i < vReadBuffer.size(); i++)
                {
                    vWriteBuffer[i] = Quantize(vReadBuffer[i], bBits);
                }

                quantized.writeRaw(vWriteBuffer.data(), vWriteBuffer.size() * sizeof(int16_t));
            }
            
        }
};