#ifndef WAVHIST_H
#define WAVHIST_H

#include <iostream>
#include <vector>
#include <map>
#include <sndfile.hh>

class WAVHist
{
private:
	std::vector<std::map<short, size_t>> counts;
	bool bStereo;

public:
	WAVHist(const SndfileHandle &sfh) : bStereo(false)
	{
		if (sfh.channels() == 2)
		{
			counts.resize(sfh.channels() + 2);
			bStereo = true;
		}
		else
		{
			counts.resize(sfh.channels());
		}
	}

	void update(const std::vector<short> &samples)
	{
		size_t n{};

		if(bStereo)
		{
			for (auto s : samples)
			{
				if (n % 2 == 1)
				{
					short left = samples[n - 1];
					short right = samples[n];

					counts[2][(left + right) / 2]++; // MID
					counts[3][(left - right) / 2]++; // SIDE

					//std::cout << "Left: " << left << " Right: " << right << " Mid: " <<  (left + right) / 2 << " Side: " << (left - right) / 2 << "\n";
				}

				counts[n++ % 2][s]++;
			}
		}
		else
		{
			for (auto s : samples)
			{
				counts[n++ % counts.size()][s]++;
			}
		}
	}

	void dump(const size_t channel) const
	{
		for (auto [value, counter] : counts[channel])
			std::cout << value << '\t' << counter << '\n';
	}

	void dump(size_t channel, const char* fileName)
	{
		FILE* histFile = fopen(fileName, "w");

		for(int64_t v = INT16_MIN; v <= INT16_MAX; v++)
		{
			size_t c = 0;
			if(counts[channel].count(v))
			{
				c = counts[channel][v];
			}
			
			fwrite(&c, sizeof(size_t), 1, histFile);
		}

		fclose(histFile);
	}
};

#endif
