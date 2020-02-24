#include "Huffman.h"

Huffman::Huffman()
{

}

Huffman::~Huffman()
{

}

void Huffman::MakeTreeBuilder(string inputFile, string OutputFile)
{
}

void Huffman::EncodeFile(string inputFile, string outputFile)
{
}

void Huffman::DecodeFile(string inputFile, string outputFile)
{
}

void Huffman::EncodeFileWithTree(string inputFile, string TreeFile, string outputFile)
{
}
void Huffman::DisplayHelp()
{
	cout << "Welcome to the Huffman Project!\n";
	cout << "Available flags:\n";
	cout << "-h|-?|-help - Prints out this help\n";
	cout << "-e file1 [file2] - Encodes file1, placing the encrypted version into file2. If file2 is not specified, file2 will have the same name as file1, minus the extension, which will be .huf.\n";
	cout << "-d file1 file 2 - Decodes file1, placing the decrypted version into file1.\n";
	cout << "-t file1 [file2] - Creates a 510 byte tree-builder file for file1, and places it into file2.\n";
	cout << "–et file1 file2 [file3] - Encodes file1 with the tree built from file2 and places it into file3. If file3 is not specified, the output file will have the same name as file1 with the .huf extension.\n";
}