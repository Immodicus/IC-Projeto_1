#include <iostream>
#include <cstdlib>
#include <iostream>
#include "BitStream.h"
#include <random>

void SetBit(int8_t& b, int8_t pos, bool value)
{
	if (value)
	{
		b |= (1 << (7 - pos));
	}
	else
	{
		b &= ~(1 << (7 - pos));
	}
}

int main(int argc, char** argv)
{
	if (argc < 3)
	{
		std::cerr << "usage: " << argv[0] << " [-mode [encode|decode] def(encode)] <inFile> <outFile>" << std::endl;
		return EXIT_FAILURE;
	}

	bool encode =  true;

	for (int n = 1; n < argc; n++)
	{
		if (std::string(argv[n]) == "-mode")
		{
			if (std::string(argv[n + 1]) == "decode")
			{
				encode = false;
			}
			break;
		}
	}

	if(!encode)
	{
		BitStream in(argv[argc - 2], "r+b");
		BitStream out(argv[argc - 1], "w+b");

		bool b;
		while (in.ReadBit(b))
		{
			if (b)
			{
				BitSet b({ '1' });
				out.WriteNBits(b);
			}
			else
			{
				BitSet b({ '0' });
				out.WriteNBits(b);
			}
		}
	}
	else
	{
		BitStream in(argv[argc - 2], "r+b");
		BitStream out(argv[argc - 1], "w+b");

		bool b;
		bool s;
		int8_t c;
		int64_t i = 0;
		do
		{
			s = in.ReadBit(b);
			SetBit(c, i % 8, b);

			if (i % 8 == 7 && i > 0)
			{
				if (c == '0')
				{
					out.WriteBit(false);
				}
				else if(c == '1')
				{
					out.WriteBit(true);
				}
			}

			i++;
		} while (s);
	}

	return EXIT_SUCCESS;
}