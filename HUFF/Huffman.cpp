#include <fstream>
#include "Huffman.h"

Huffman::Huffman()
{
	for (int i = 0; i < amountOfCharacters; i++)
	{
		nodes[i] = nullptr;
	}
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

void Huffman::buildTree(ifstream& inputStream, ofstream* outputStream)
{
	int frequencyTable[amountOfCharacters];

	for (int i = 0; i < amountOfCharacters; i++)
	{
		frequencyTable[i] = 0;
	}

	char character;

	while (inputStream.get(character))
	{
		// BY COERCING INTO A UNSIGNED CHAR, WE FIX THE ARRAY INDEX ACCESS OTHERWISE,
		// WE ACCESS ARRAY INDEXES OUT OF THE FREQUENCY TABLE SIZE AND MESS THINGS UP!
		unsigned char symbol = character;

		frequencyTable[symbol]++;
	}

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

	for (int i = 0; i < amountOfCharacters - 1; i++)
	{
		int smallestNodeIndex = getIndexOfSmallestNode(nodes, -1);
		int nextSmallestNodeIndex = getIndexOfSmallestNode(nodes, smallestNodeIndex);

		treenode* parent = new treenode;

		treenode* smallestNode = nodes[smallestNodeIndex];
		treenode* nextSmallestNode = nodes[nextSmallestNodeIndex];

		parent->symbol = NULL;
		parent->weight = smallestNode->weight + nextSmallestNode->weight;

		if (smallestNodeIndex < nextSmallestNodeIndex)
		{
			parent->leftChild = smallestNode;
			parent->rightChild = nextSmallestNode;
			nodes[smallestNodeIndex] = parent;
			nodes[nextSmallestNodeIndex] = nullptr;

			if (outputStream != nullptr)
			{
				*outputStream << (char)smallestNodeIndex;
				*outputStream << (char)nextSmallestNodeIndex;
			}
		}
		else
		{
			parent->leftChild = nextSmallestNode;
			parent->rightChild = smallestNode;
			nodes[nextSmallestNodeIndex] = parent;
			nodes[smallestNodeIndex] = nullptr;
			if (outputStream != nullptr)
			{
				*outputStream << (char)nextSmallestNodeIndex;
				*outputStream << (char)smallestNodeIndex;
			}
		}
	}
}

bool Huffman::isLeaf(treenode* node)
{
	return node->leftChild == nullptr && node->rightChild == nullptr;
}

void Huffman::buildEncodingTable()
{
	buildEncodingTable(nodes[0], "");
}

void Huffman::buildEncodingTable(treenode* node, string path)
{
	if (isLeaf(node))
	{
		encodingTable[node->symbol] = path;

		return;
	}

	if (node->leftChild != nullptr)
	{
		buildEncodingTable(node->leftChild, path + "0");
	}

	if (node->rightChild != nullptr)
	{
		buildEncodingTable(node->rightChild, path + "1");
	}
}

void Huffman::MakeTreeBuilder(string inputFile, string outputFile)
{
	ifstream inputStream;

	ofstream outputStream;

	inputStream.open(inputFile, ios::binary);

	if (inputStream.fail())
	{
		cout << "Input file failed to open!" << endl;

		return;
	}

	outputStream.open(outputFile, ios::binary); // THIS IS PROBABLY NOT IT CHIEF.

	if (outputStream.fail())
	{
		cout << "Output file failed to open!" << endl;

		return;
	}

	buildTree(inputStream, &outputStream);

	for (int i = 0; i < amountOfCharacters; i++)
	{
		treenode* node = nodes[i];

		/*if (node != nullptr)
		{
			printStuff(node, "");
		}*/
	}

	buildEncodingTable();

	/*cout << "Encoding Table: ";

	for (int i = 0; i < amountOfCharacters; i++)
	{
		cout << "Symbol: (" << i << ") - " << encodingTable[i] << endl;
	}*/

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
		cout << "Symbol: " << node->symbol << " (" << (unsigned int)node->symbol << ")" << " - ";
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