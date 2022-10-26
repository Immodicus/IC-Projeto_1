#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <assert.h>
#include <fftw3.h>
#include <sndfile.hh>

#include "BitStream.h"

using namespace std;

// https://stackoverflow.com/a/12399290
template <typename T>
inline std::vector<size_t> vec_sort_index(const std::vector<T> &v)
{
	static_assert(std::is_fundamental<T>::value);

	std::vector<size_t> vInd(v.size());

	// fill vInd with index values for v (0 to s.size() - 1);
	std::iota(vInd.begin(), vInd.end(), 0);

	std::sort(
		vInd.begin(), vInd.end(), [&v](size_t i1, size_t i2)
		{ return abs(v[i1]) > abs(v[i2]); });

	return vInd;
}

template <typename T>
inline double vec_norm(const std::vector<T> &v)
{
	static_assert(std::is_fundamental<T>::value);

	double result = 0.0;

	for (auto d : v)
	{
		result += d * d;
	}

	return sqrt(result);
}

inline constexpr void Split(double* x0, double* x1, double x, uint32_t nBits)
{
    double d = x * (pow(2, nBits) + 1);
    double t = d - x;
    *x0 = d - t;
    *x1 = x - *x0;
}

int main(int argc, char *argv[])
{
	size_t sampleRate = 0;
	size_t nChannels = 0;
	size_t nFrames = 0;
	size_t blockSize = 1024;
	size_t nBlocks = 0;
	double dDctFrac = 0.9999;

	uint32_t nBits = 16;

	bool verbose = false;
	bool encode = true;

	if (argc < 3)
	{
		cerr << "Usage: wav_dct [ -v (verbose) ]\n";
		cerr << "               [ -mode [enc/dec] (def enc) ]\n";
		cerr << "               [ -bs blockSize (def 1024) ]\n";
		cerr << "               [ -frac dctFraction (def 0.9999) ]\n";
		cerr << "               [ -b nBits (def 16) ]\n";
		cerr << "               fileIn fileOut\n";
		return 1;
	}

	for (int n = 1; n < argc; n++)
	{
		if (string(argv[n]) == "-v")
		{
			verbose = true;
			break;
		}
	}

	for (int n = 1; n < argc; n++)
	{
		if (string(argv[n]) == "-mode")
		{
			if (string(argv[n + 1]) == "dec")
			{
				encode = false;
			}
			break;
		}
	}

	for (int n = 1; n < argc; n++)
	{
		if (string(argv[n]) == "-bs")
		{
			blockSize = atoi(argv[n + 1]);
			break;
		}
	}

	for (int n = 1; n < argc; n++)
	{
		if (string(argv[n]) == "-frac")
		{
			dDctFrac = atof(argv[n + 1]);
			break;
		}
	}

	for (int n = 1; n < argc; n++)
	{
		if (string(argv[n]) == "-b")
		{
			nBits = atoi(argv[n + 1]);
			break;
		}
	}

	if (encode)
	{
		SndfileHandle sfhIn{argv[argc - 2]};
		if (sfhIn.error())
		{
			cerr << "Error: invalid input file\n";
			return 1;
		}

		if ((sfhIn.format() & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV)
		{
			cerr << "Error: file is not in WAV format\n";
			return 1;
		}

		if ((sfhIn.format() & SF_FORMAT_SUBMASK) != SF_FORMAT_PCM_16)
		{
			cerr << "Error: file is not in PCM_16 format\n";
			return 1;
		}

		nChannels = static_cast<size_t>(sfhIn.channels());
		nFrames = static_cast<size_t>(sfhIn.frames());
		nBlocks = static_cast<size_t>(ceil(static_cast<double>(nFrames) / blockSize));
		sampleRate = static_cast<size_t>(sfhIn.samplerate());

		BitStream outBs{argv[argc - 1], "w+"};

		vector<short> samples(nChannels * nFrames);
		sfhIn.readf(samples.data(), nFrames);
		samples.resize(nBlocks * blockSize * nChannels);

		if (verbose)
		{
			std::cout << "sampleRate: " << sampleRate << "\n";
			std::cout << "nChannels: " << nChannels << "\n";
			std::cout << "nFrames: " << nFrames << "\n";
			std::cout << "blockSize: " << blockSize << "\n";
			std::cout << "dDctFrac: " << dDctFrac << "\n";
			std::cout << "nBits: " << nBits << "\n";
			std::cout << "nBlocks: " << nBlocks << "\n";
		}

		assert(outBs.Write(sampleRate));
		assert(outBs.Write(nChannels));
		assert(outBs.Write(nFrames));
		assert(outBs.Write(blockSize));
		assert(outBs.Write(nBits));

		vector<double> x(blockSize);

		fftw_plan plan_d = fftw_plan_r2r_1d(blockSize, x.data(), x.data(), FFTW_REDFT10, FFTW_ESTIMATE);
		for (size_t n = 0; n < nBlocks; n++)
		{
			for (size_t c = 0; c < nChannels; c++)
			{
				for (size_t k = 0; k < blockSize; k++)
				{
					x[k] = samples[(n * blockSize + k) * nChannels + c];
					// x[k] /= (INT16_MAX + 1);
					// //std::cout << x[k] << "\n";
				}

				fftw_execute(plan_d);

				double xNorm = vec_norm(x);
				auto sorted = vec_sort_index(x);

				size_t needed = 0;

				if(dDctFrac >= 1.0)
				{
					needed = blockSize;
				}
				else
				{
					double prev = 0.0;
					while (prev / xNorm < dDctFrac && needed <= blockSize)
					{
						prev = sqrt(pow(prev, 2) + pow(x[sorted[needed]], 2));

						needed++;
					}
				}

				//std::cout << "Block: " << n << " Channel: " << c << " xNorm: " << xNorm << " Need: " << needed << " coefficients Percent: " << (double)needed / x.size() * 100 << "\n";

				BitSet bitmap(blockSize);

				for (size_t b = 0; b < needed; b++)
				{
					bitmap.SetBit(sorted[b], true);
				}

				assert(outBs.WriteNBits(bitmap));

				// Keep only "dctFrac" of the "low frequency" coefficients
				for (size_t k = 0; k < blockSize; k++)
				{
					if (bitmap.GetBit(k))
					{
						double d = x[k] / (blockSize << 1);
						double d1;
						double d2;

						Split(&d1, &d2, d, nBits);

						int64_t i = 0;
						memcpy(&i, &d1, sizeof(double));
						i >>= (64 - nBits);

						for (size_t b = (64 - nBits); b < 64; b++)
						{
							outBs.WriteBit((i >> (63 - (b % 64))) & 1);
						}
					}
				}
			}
		}

		fftw_destroy_plan(plan_d);
		fftw_cleanup();
	}
	else
	{
		BitStream inBs{argv[argc - 2], "r"};

		assert(inBs.Read(sampleRate));
		assert(inBs.Read(nChannels));
		assert(inBs.Read(nFrames));
		assert(inBs.Read(blockSize));
		assert(inBs.Read(nBits));

		nBlocks = static_cast<size_t>(ceil(static_cast<double>(nFrames) / blockSize));

		if (verbose)
		{
			std::cout << "sampleRate: " << sampleRate << "\n";
			std::cout << "nChannels: " << nChannels << "\n";
			std::cout << "nFrames: " << nFrames << "\n";
			std::cout << "blockSize: " << blockSize << "\n";
			std::cout << "nBits: " << nBits << "\n";
			std::cout << "nBlocks: " << nBlocks << "\n";
		}

		SndfileHandle sfhOut{argv[argc - 1], SFM_WRITE, SF_FORMAT_PCM_16 | SF_FORMAT_WAV, static_cast<int>(nChannels), static_cast<int>(sampleRate)};
		if (sfhOut.error())
		{
			cerr << "Error: invalid output file\n";
			return 1;
		}

		vector<double> x(blockSize);

		vector<short> samples(nChannels * nFrames);
		samples.resize(nBlocks * blockSize * nChannels);

		fftw_plan plan_i = fftw_plan_r2r_1d(blockSize, x.data(), x.data(), FFTW_REDFT01, FFTW_ESTIMATE);
		for (size_t n = 0; n < nBlocks; n++)
		{
			for (size_t c = 0; c < nChannels; c++)
			{
				memset(x.data(), 0, x.size() * sizeof(double));

				BitSet bitmap = inBs.ReadNBits(blockSize);

				for (size_t k = 0; k < blockSize; k++)
				{
					if (bitmap.GetBit(k))
					{
						int64_t i = 0;

						for (size_t b = (64 - nBits); b < 64; b++)
						{
							bool tb;
							assert(inBs.ReadBit(tb));

							if (tb)
							{
								i |= (1 << (63 - b));
							}
							else
							{
								i &= ~(1 << (63 - b));
							}
						}
						i <<= (64 - nBits);

						double d;
						memcpy(&d, &i, sizeof(double));
						x[k] = d;
					}
				}

				fftw_execute(plan_i);

				for (size_t k = 0; k < blockSize; k++)
				{
					samples[(n * blockSize + k) * nChannels + c] = static_cast<short>(round(x[k]));
				}
			}
		}

		fftw_destroy_plan(plan_i);
		fftw_cleanup();

		sfhOut.writef(samples.data(), nFrames);
	}

	return EXIT_SUCCESS;
}