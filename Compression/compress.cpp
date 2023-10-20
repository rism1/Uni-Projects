#include "HuffmanHandler.h"
#include "LZWHandler.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main(int argc, char* argv[])
{
	if (argc == 4)
	{
		if (std::string(argv[1]) == "-lzw")
		{
			LZWHandler lzwHandler = LZWHandler(std::string(argv[2]), std::string(argv[3]));
			lzwHandler.saveEncoded();
		}
		else if (std::string(argv[1]) == "-huffman")
		{
			HuffmanHandler huffmanHandler = HuffmanHandler(std::string(argv[2]), std::string(argv[3]));
			huffmanHandler.saveEncoded();
		}
		else if (std::string(argv[1]) == "-d")
		{
			std::ifstream input(std::string(argv[2]), std::ios::binary);
			if (!input.is_open())
			{
				std::cout << "ERROR: CANNOT OPEN AN INPUT FILE!";
				return -1;
			}
			char type;
			input.get(type);
			input.close();
			if (type == 'L')
			{
				LZWHandler lzwHandler = LZWHandler(std::string(argv[2]), std::string(argv[3]));
				lzwHandler.saveDecoded();
			}
			else if (type == 'H')
			{
				HuffmanHandler huffmanHandler = HuffmanHandler(std::string(argv[2]), std::string(argv[3]));
				huffmanHandler.saveDecoded();
			}
			else
			{
				std::cout << "ERROR: INVALID INPUT FILE!";
				return -1;
			}
		}
	}

	else if (argc == 2 && (std::string(argv[1]) == "?" || std::string(argv[1]) == "-h"))
	{
		std::cout << argv[0] << " [-lzw][-huffman] [inputFile] [outputFile] : Encode inputFile to outputFile using LZW or Huffman encoding\n";
		std::cout << argv[0] << " [-d] : Decode inputFile to outputFile from LZW or Huffman encoding\n";
		std::cout << argv[0] << " [-h][?] : Print help information\n";		
	}
	
}