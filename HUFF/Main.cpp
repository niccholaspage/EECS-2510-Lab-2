#include <iostream>

#include "Huffman.h"

using namespace std;

int main(int argc, char* argv[])
{
    Huffman* huffman = new Huffman();

    if (argc < 2)
    {
        cout << "No flags given! Here is some help!" << endl;

        huffman->DisplayHelp();

        delete huffman;

        return 0;
    }

    string flag = argv[1];

    if (flag[0] != '-' || flag.length() < 2)
    {
        cout << "Invalid flag format!" << endl;

        delete huffman;

        return 0;
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
        cout << "encrypt..." << endl;
    }
    else if (command == "d")
    {
        cout << "decrypt..." << endl;
    }
    else if (command == "t")
    {
        cout << "tree" << endl;
    }
    else if (command == "et")
    {
        cout << "encrypt tree" << endl;
    }
    else {
        cout << "Invalid flag!" << endl;
    }

    delete huffman;

    return 0;
}