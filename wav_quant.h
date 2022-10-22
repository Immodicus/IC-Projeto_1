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

typedef enum QuantizationMode
{
    Round,
    Shift,
    MidRiser,
    MidTread
} QuantizationMode;

class WAVQuant
{
    private:
        SndfileHandle& original;
        const char* fileName;

        const uint64_t BufferSize = 256000;

        int16_t QuantizeRnd(int16_t s, uint8_t bBits)
        {
            double n = static_cast<double>(s);

            int16_t max = (INT16_MAX + 1) / pow(2, 16 - bBits) - 1;
            int16_t min = (INT16_MIN) / pow(2, 16 - bBits);

            n /= pow(2, 16 - bBits);

            return std::clamp(static_cast<int16_t>(std::round(n)), min, max) << (16 - bBits);
        }

        int16_t QuantizeShift(int16_t s, uint8_t bBits)
        {
            return (s >> (16 - bBits)) << (16 - bBits);
        }

        int16_t MidRiserQuantize(int16_t s, uint8_t bBits)
        {
            double sample = static_cast<double>(s);
            double delta = pow(2, 16 - bBits);

            double result = delta * (std::floor(sample / delta) + 0.5);

            return static_cast<int16_t>(result);
        }

        int16_t MidTreadQuantize(int16_t s, uint8_t bBits)
        {
            double sample = static_cast<double>(s);
            double delta = pow(2, 16 - bBits);

            double result = std::clamp(delta * std::floor((sample / delta) + 0.5), -32768.0, 32767.0);

            return static_cast<int16_t>(result);
        }
    
    public:
        WAVQuant(const char* outFileName, SndfileHandle& sfh) : original(sfh), fileName(outFileName) {};

        void Quantize(uint8_t bBits, QuantizationMode mode)
        {
            SndfileHandle quantized (fileName, SFM_WRITE, original.format(), original.channels(), original.samplerate());
        
            std::vector<int16_t> vReadBuffer (BufferSize * original.channels());
            std::vector<int16_t> vWriteBuffer (BufferSize * original.channels());

            size_t sRead;
            while((sRead = original.readf(vReadBuffer.data(), BufferSize)))
            {
                vReadBuffer.resize(sRead * original.channels());
                vWriteBuffer.resize(sRead * original.channels());

                QuantizeInternal(bBits, vReadBuffer, vWriteBuffer, mode);

                quantized.writeRaw(vWriteBuffer.data(), vWriteBuffer.size() * sizeof(int16_t));
            }
            
        }

    private:
        void QuantizeInternal(uint8_t bBits, std::vector<int16_t>& vReadBuffer, std::vector<int16_t>& vWriteBuffer, QuantizationMode mode)
        {
            switch(mode)
            {
                case Round:
                {
                    for(uint64_t i = 0; i < vReadBuffer.size(); i++)
                    {
                        vWriteBuffer[i] = QuantizeRnd(vReadBuffer[i], bBits);
                    }
                    
                    break;
                }
                case Shift:
                {
                    for(uint64_t i = 0; i < vReadBuffer.size(); i++)
                    {
                        vWriteBuffer[i] = QuantizeShift(vReadBuffer[i], bBits);
                    }
                    
                    break;
                }
                case MidRiser:
                {
                    for(uint64_t i = 0; i < vReadBuffer.size(); i++)
                    {
                        vWriteBuffer[i] = MidRiserQuantize(vReadBuffer[i], bBits);
                    }
                    
                    break;
                }
                case MidTread:
                {
                    for(uint64_t i = 0; i < vReadBuffer.size(); i++)
                    {
                        vWriteBuffer[i] = MidTreadQuantize(vReadBuffer[i], bBits);
                    }
                    
                    break;
                }
            }
        }
};