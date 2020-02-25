#include <fstream>
#include "Huffman.h"

Huffman::Huffman()
{

}

Huffman::~Huffman()
{

}

int Huffman::getIndexOfSmallestNode(treenode* nodes[amountOfCharacters], int skipIndex)
{
	int smallestNodeIndex = -1;
	int smallestWeight = INT_MAX;

	for (int i = 0; i < amountOfCharacters; i++)
	{
		if (skipIndex == i)
		{
			continue;
		}

		treenode* node = nodes[i];

		if (node == nullptr)
		{
			continue;
		}

		if (node->weight < smallestWeight)
		{
			smallestNodeIndex = i;
			smallestWeight = node->weight;
		}
	}

	return smallestNodeIndex;
}

void Huffman::MakeTreeBuilder(string inputFile, string outputFile)
{
	ifstream inputStream;

	ofstream outputStream;

	inputStream.open(inputFile);

	if (inputStream.fail())
	{
		cout << "Input file failed to open!" << endl;

		return;
	}

	outputStream.open(outputFile);

	if (outputStream.fail())
	{
		cout << "Output file failed to open!" << endl;

		return;
	}

	int frequencyTable[amountOfCharacters];

	for (int i = 0; i < amountOfCharacters; i++)
	{
		frequencyTable[i] = 0;
	}

	int charactersRead = 0;

	unsigned char character;

	while (inputStream >> std::noskipws >> character)
	{
		frequencyTable[character]++;

		charactersRead++;
	}

	treenode* nodes[amountOfCharacters];

	for (int i = 0; i < amountOfCharacters; i++)
	{
		unsigned char symbol = i;

		treenode* node = new treenode;

		node->symbol = symbol;
		node->weight = frequencyTable[symbol];
		node->leftChild = nullptr;
		node->rightChild = nullptr;

		nodes[i] = node;
	}

	while (nodes[0]->weight != charactersRead)
	{
		int smallestNodeIndex = getIndexOfSmallestNode(nodes, -1);
		int nextSmallestNodeIndex = getIndexOfSmallestNode(nodes, smallestNodeIndex);

		treenode* parent = new treenode;

		treenode* smallestNode = nodes[smallestNodeIndex];
		treenode* nextSmallestNode = nodes[nextSmallestNodeIndex];

		//outputStream << "We are looking at indexes " << smallestNodeIndex << " & " << nextSmallestNodeIndex << ":" << endl;

		//outputStream << "Symbol: " << smallestNode->symbol << " - " << smallestNode->weight << endl;

		//outputStream << "Symbol: " << nextSmallestNode->symbol << " - " << nextSmallestNode->weight << endl;

		parent->symbol = NULL;
		parent->weight = smallestNode->weight + nextSmallestNode->weight;

		if (smallestNodeIndex < nextSmallestNodeIndex)
		{
			parent->leftChild = smallestNode;
			parent->rightChild = nextSmallestNode;
			nodes[smallestNodeIndex] = parent;
			nodes[nextSmallestNodeIndex] = nullptr;
		}
		else
		{
			parent->leftChild = nextSmallestNode;
			parent->rightChild = smallestNode;
			nodes[nextSmallestNodeIndex] = parent;
			nodes[smallestNodeIndex] = nullptr;
		}

		//outputStream << "Set " << smallestNodeIndex << " to parent, and " << nextSmallestNodeIndex << " to null" << endl;
		//outputStream << "Combined weight: " << parent->weight << endl;
	}

	for (int i = 0; i < amountOfCharacters; i++)
	{
		treenode* node = nodes[i];

		if (node != nullptr)
		{
			printStuff(node, "");
		}
	}

	cout << "Amount of characters read: " << charactersRead << endl;

	inputStream.close();

	outputStream.close();
}

void Huffman::printStuff(treenode* node, string spaces)
{
	if (node->weight == 0)
	{
		return;
	}

	cout << spaces;

	if (node->symbol != NULL)
	{
		cout << "Symbol: " << node->symbol << "(" << (unsigned int)node->symbol << ")" << " - ";
	}

	cout << node->weight << endl;

	if (node->leftChild != nullptr)
	{
		printStuff(node->leftChild, spaces + " ");
	}

	if (node->rightChild != nullptr)
	{
		printStuff(node->rightChild, spaces + " ");
	}
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