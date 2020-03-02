#include <chrono>
#include <iostream>

#include "Huffman.h"

using namespace std;

void handleCommandLineParameters(int argc, char* argv[], Huffman* huffman)
{
	if (argc < 2)
	{
		cout << "No flags given! Here is some help!" << endl;

		huffman->DisplayHelp();

		return;
	}

	string flag = argv[1];

	if (flag[0] != '-' || flag.length() < 2)
	{
		cout << "Invalid flag format!" << endl;

		return;
	}

	for (unsigned int i = 0; i < flag.length(); i++)
	{
		flag[i] = tolower(flag[i]);
	}

	string command = flag.substr(1);

	if (command == "h" || command == "?" || command == "help")
	{
		huffman->DisplayHelp();
	}
	else if (command == "e")
	{
		if (argc < 4)
		{
			cout << "not enough arguments!" << endl;
		}
		else
		{
			huffman->EncodeFile(argv[2], argv[3]);
		}
	}
	else if (command == "d")
	{
		huffman->DecodeFile(argv[2], argv[3]);
	}
	else if (command == "t")
	{
		if (argc < 4)
		{
			cout << "not enough arguments!" << endl;
		}
		else
		{
			huffman->MakeTreeBuilder(argv[2], argv[3]);
		}
	}
	else if (command == "et")
	{
		if (argc < 5)
		{
			cout << "not enough arguments!" << endl;
		}
		else
		{
			huffman->EncodeFileWithTree(argv[2], argv[3], argv[4]);
		}
	}
	else {
		cout << "Invalid flag!" << endl;
	}
}

int main(int argc, char* argv[])
{
	auto start = chrono::system_clock::now();

	Huffman* huffman = new Huffman();

	handleCommandLineParameters(argc, argv, huffman);

	delete huffman;

	auto end = chrono::system_clock::now();

	chrono::duration<double> elapsed_seconds = end - start;

	cout << "Elapsed time: " << elapsed_seconds.count() << "s\n";

	return 0;
}