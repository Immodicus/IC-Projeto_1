#include <iostream>
#include <vector>
#include <map>
#include <sndfile.hh>
#include <stdint.h>
#include <exception>
#include <cmath>
#include <sstream>
#include <assert.h>
#include <algorithm>

class WAVQuant
{
    private:
        SndfileHandle& original;
        const char* fileName;

        const uint64_t BufferSize = 256000;

        int16_t Quantize(int16_t s, uint8_t bBits)
        {
            assert(bBits < 16);

            double n = static_cast<double>(s);

            int16_t max = (INT16_MAX + 1) / pow(2, 16 - bBits) - 1;
            int16_t min = (INT16_MIN) / pow(2, 16 - bBits);

            n /= pow(2, 16 - bBits);

            return std::clamp(static_cast<int16_t>(std::round(n)), min, max) << (16 - bBits);
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