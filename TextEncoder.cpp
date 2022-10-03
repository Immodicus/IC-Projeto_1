#include <iostream>
#include <cstdlib>
#include <iostream>
#include "BitStream.h"
#include <random>

//int main()
//{
//    BitStream in("./in.hex", "r+b");
//    BitStream out("./out.hex", "w+b");
//
//    /*for (int i = 0; i < (1000 * 1000 * 8); i++)
//    {
//        in.WriteBit(i % 2 == 0);
//    }
//
//    in.WriteBit(true);*/
//
//    //for (int i = 0; i < (1000 * 1000 * 8 * 25) + 39547; i++)
//    //{
//    //    in.WriteBit(rand() % 2);
//    //}
//
//    /*for (int i = 0; i < (1000 * 1000 * 8); i++)
//    {
//        bool b;
//
//        if (i > 1000 * 8 && i < 1000 * 8 * 2)
//        {
//            in.WriteBit(i % 2 == 0);
//        }
//        else
//        {
//            if (!in.ReadBit(b)) throw new std::runtime_error("ReadBit Failed");
//
//            if ((i % 2 == 0) != b) throw new std::runtime_error("ReadBit Failed");
//        }
//    }
//
//    in.WriteBit(true);*/
//
//    //unsigned long size = 3759;
//
//    //BitSet b = in.ReadNBits(size);
//    //int i = 0;
//    //do
//    //{
//    //    for (int j = 0; j < b.size(); i++, j++)
//    //    {
//    //        if ((i % 2 == 0) != b[j])
//    //        {
//    //            //std::cout << "Bit: " << i << " differs. Should it?\n";
//    //        }
//    //        if (i >= 8000000)
//    //        {
//    //            std::cout << "Bit: " << i << " " << b[j] << " \n";
//    //        }
//    //    }
//
//    //    b = in.ReadNBits(size);
//    //    if (b.size() != size)
//    //    {
//    //        std::cout << "Test" << std::endl;
//    //    }
//    //    //std::cout << b.size() << std::endl;
//    //    //std::cout << i << std::endl;
//    //} while (b.size());
//
//    unsigned long size = 9752;
//
//    BitSet buf(size);
//    int i = 0;
//    while ((buf = in.ReadNBits(size)).size() > 0)
//    {
//        i += buf.size();
//        //std::cout << i << " " << out.f() * 8 << std::endl;
//        if(!out.WriteNBits(buf))  throw new std::runtime_error("WriteNBits Failed");
//    }
//    //std::cout << i << std::endl;
//    //bool b;
//    //while (in.ReadBit(b))
//    //{
//    //    out.WriteBit(b);
//    //}
//
//    /*for (int i = 0; i < (1000 * 1000 * 8); i++)
//    {
//        bool b;
//        if (!in.ReadBit(b)) throw new std::runtime_error("ReadBit Failed");
//
//        if(b != (i % 2 == 0)) throw new std::runtime_error("Bad Bit Read");
//    }
//
//    bool b;
//    if (!in.ReadBit(b)) throw new std::runtime_error("ReadBit Failed");
//    if (!in.ReadBit(b)) throw new std::runtime_error("ReadBit Failed");
//    if (!in.ReadBit(b)) throw new std::runtime_error("ReadBit Failed");
//    if (!in.ReadBit(b)) throw new std::runtime_error("ReadBit Failed");
//    if (!in.ReadBit(b)) throw new std::runtime_error("ReadBit Failed");
//    if (!in.ReadBit(b)) throw new std::runtime_error("ReadBit Failed");
//    if (!in.ReadBit(b)) throw new std::runtime_error("ReadBit Failed");
//    if (!in.ReadBit(b)) throw new std::runtime_error("ReadBit Failed");*/
//}

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

//int main(int argc, char** argv)
//{
//	//if (argc < 3)
//	//{
//	//	std::cerr << "usage: " << argv[0] << " <textfile> <binaryfile>" << std::endl;
//	//	return EXIT_FAILURE;
//	//}
//
//	//BitStream test("text.hex", "w+b");
//
//	//BitSet tst({ '0', '1', '0', '0', '1', '0', '1', '1', '1', '0'});
//	//test.WriteNBits(tst);
//
//	BitStream in("text.hex", "r+b");
//	BitStream out("textout.hex", "w+b");
//
//	bool b;
//	bool s;
//	int8_t c;
//	int64_t i = 0;
//	do
//	{
//		s = in.ReadBit(b);
//		SetBit(c, i % 8, b);
//
//		if (i % 8 == 7 && i > 0)
//		{
//			std::cout << i << "\n";
//			if (c == '0')
//			{
//				std::cout << "Write false\n";
//				out.WriteBit(false);
//			}
//			else if(c == '1')
//			{
//				std::cout << "Write true\n";
//				out.WriteBit(true);
//			}
//		}
//
//		i++;
//	} while (s);
//
//	return EXIT_SUCCESS;
//}

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
					std::cout << "Write false\n";
					out.WriteBit(false);
				}
				else if(c == '1')
				{
					std::cout << "Write true\n";
					out.WriteBit(true);
				}
			}

			i++;
		} while (s);
	}

	return EXIT_SUCCESS;
}