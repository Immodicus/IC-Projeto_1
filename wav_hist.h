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
		for (auto s : samples)
		{
			if (bStereo && n % 2 == 1)
			{
				short left = samples[n - 1];
				short right = samples[n];

				counts[2][(left + right) / 2]++; // MID
				counts[3][(left - right) / 2]++; // SIDE
			}

			counts[n++ % counts.size()][s]++;
		}
	}

	void dump(const size_t channel) const
	{
		for (auto [value, counter] : counts[channel])
			std::cout << value << '\t' << counter << '\n';
	}
};

#endif
