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
	if (argc < 4)
	{
		std::cerr << "usage: " << argv[0] << " encode/decode <file1> <file2>" << std::endl;
		return EXIT_FAILURE;
	}

	if(strcmp(argv[1], "decode") == 0)
	{
		BitStream in(argv[2], "r+b");
		BitStream out(argv[3], "w+b");

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
		BitStream in(argv[2], "r+b");
		std::cout << argv[2] << std::endl;
		BitStream out(argv[3], "w+b");
		std::cout << argv[3] << std::endl;

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
				std::cout << i << "\n";
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