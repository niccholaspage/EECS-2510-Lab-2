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
        cout << "encrypt tree" << endl;
    }
    else {
        cout << "Invalid flag!" << endl;
    }
}

int main(int argc, char* argv[])
{
    Huffman* huffman = new Huffman();

    handleCommandLineParameters(argc, argv, huffman);

    delete huffman;

    return 0;
}