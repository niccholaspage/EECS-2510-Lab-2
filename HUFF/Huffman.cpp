#include "Huffman.h"

Huffman::Huffman() : nodes{ nullptr }
{
	start = chrono::high_resolution_clock::now(); // This sets the starting time position to the current time.
}

Huffman::~Huffman()
{
	for (int i = 0; i < amountOfCharacters; i++)
	{
		if (nodes[i] != nullptr)
		{
			traverseDestruct(nodes[i]);
		}
	}
}

void Huffman::traverseDestruct(treenode* p) {
	// This recursive method can be called on a given node to delete
	// its children and itself. Doing this recursively ensures that
	// every node will get deleted as long as this method is initially
	// called on the root node.
	//
	if (p->leftChild != nullptr) // Check if the left child of the node is not null
	{
		traverseDestruct(p->leftChild); // Since the node does have a left child, we will call the traverseDestruct method on the node
	}

	if (p->rightChild != nullptr) // Check if the right child is not null
	{
		traverseDestruct(p->rightChild); // Since the node does have a right child, we will call the traverseDestruct method on the node
	}

	// Now that we have properly deleted the children of the given node,
	// we are safe to delete the node since we have already taken care of
	// its children.
	//
	delete p;
}

int Huffman::getIndexOfSmallestNode(treenode* nodes[amountOfCharacters], int skipIndex)
{
	int smallestNodeIndex = -1;
	unsigned int smallestWeight = UINT_MAX;

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

void Huffman::buildTreeFromTreeBuilder(ifstream& stream, bool writeToOutput)
{
	for (int i = 0; i < amountOfCharacters; i++)
	{
		unsigned char symbol = i;

		treenode* node = new treenode;

		node->symbol = symbol;
		node->weight = 0; // We don't care about the weight, as we already have the order we are pairing things up in
		node->leftChild = nullptr;
		node->rightChild = nullptr;

		nodes[i] = node;
	}

	// TODO: WHAT HAPPENS IF WE DON'T HAVE A FILE THIS BIG?
	for (int i = 0; i < amountOfCharacters - 1; i++)
	{
		unsigned char leftIndex = stream.get();
		unsigned char rightIndex = stream.get();

		if (writeToOutput)
		{
			outputStream.put(leftIndex);
			outputStream.put(rightIndex);
		}

		treenode* parent = new treenode;

		treenode* firstNode = nodes[leftIndex];
		treenode* secondNode = nodes[rightIndex];

		parent->symbol = NULL;
		parent->weight = 0; // we don't care about weight

		parent->leftChild = firstNode;
		parent->rightChild = secondNode;

		nodes[leftIndex] = parent;
		nodes[rightIndex] = nullptr;
	}
}

void Huffman::buildTree()
{
	unsigned int frequencyTable[amountOfCharacters];

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

			outputStream.put(smallestNodeIndex);
			outputStream.put(nextSmallestNodeIndex);
		}
		else
		{
			parent->leftChild = nextSmallestNode;
			parent->rightChild = smallestNode;
			nodes[nextSmallestNodeIndex] = parent;
			nodes[smallestNodeIndex] = nullptr;

			outputStream.put(nextSmallestNodeIndex);
			outputStream.put(smallestNodeIndex);
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

		if (path.length() > 7)
		{
			paddingBits = path;
		}

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

bool Huffman::openStreams(string inputFile, string outputFile)
{
	inputStream.open(inputFile, ios::binary);

	if (inputStream.fail())
	{
		cout << "Unable to open input file." << endl;

		return false;
	}

	outputStream.open(outputFile, ios::binary); // THIS IS PROBABLY NOT IT CHIEF.

	if (outputStream.fail())
	{
		cout << "Unable to open output file." << endl;

		return false;
	}

	return true;
}

void Huffman::closeStreams()
{
	inputStream.close();

	outputStream.close();
}

void Huffman::MakeTreeBuilder(string inputFile, string outputFile)
{
	if (!openStreams(inputFile, outputFile))
	{
		return;
	}


	buildTree();

	buildEncodingTable();

	closeStreams();
}

void Huffman::navigateTree(unsigned char byte, int bitToCheck, treenode*& node)
{
	node = byte & bitToCheck ? node->rightChild : node->leftChild;

	if (isLeaf(node))
	{
		outputStream.put(node->symbol);

		node = nodes[0];
	}
}

void Huffman::decodeBytes()
{
	char character;

	treenode* currentNode = nodes[0];

	while (inputStream.get(character))
	{
		unsigned char byte = character;

		navigateTree(byte, 128, currentNode);
		navigateTree(byte, 64, currentNode);
		navigateTree(byte, 32, currentNode);
		navigateTree(byte, 16, currentNode);
		navigateTree(byte, 8, currentNode);
		navigateTree(byte, 4, currentNode);
		navigateTree(byte, 2, currentNode);
		navigateTree(byte, 1, currentNode);
	}
}

void Huffman::encodeBits(unsigned char& outputCharacter, int& currentBit, string& bits)
{
	for (unsigned int i = 0; i < bits.length(); i++)
	{
		bool on = bits[i] == '1';

		outputCharacter |= (on << (7 - currentBit));

		currentBit++;

		if (currentBit == 8)
		{
			outputStream.put(outputCharacter);

			outputCharacter = 0;

			currentBit = 0;
		}
	}
}

void Huffman::encodeBytes()
{
	inputStream.clear();
	inputStream.seekg(0);

	char character;

	unsigned char outputCharacter = 0; // Character we are working on writing

	int currentBit = 0; // The current bit we are on

	while (inputStream.get(character))
	{
		unsigned char symbol = character;

		string& bitString = encodingTable[symbol];

		encodeBits(outputCharacter, currentBit, bitString);
	}

	// Handling case here, where we need to finish writing a byte, so we finish the byte with padding bits.
	if (currentBit != 0)
	{
		encodeBits(outputCharacter, currentBit, paddingBits);
	}
}

void Huffman::EncodeFile(string inputFile, string outputFile)
{
	if (!openStreams(inputFile, outputFile))
	{
		return;
	}

	buildTree();

	buildEncodingTable();

	encodeBytes();

	closeStreams();

	printFinalInfo(inputFile, outputFile);
}

void Huffman::DecodeFile(string inputFile, string outputFile)
{
	if (!openStreams(inputFile, outputFile))
	{
		return;
	}

	buildTreeFromTreeBuilder(inputStream, false);

	decodeBytes();

	closeStreams();

	printFinalInfo(inputFile, outputFile);
}

void Huffman::EncodeFileWithTree(string inputFile, string TreeFile, string outputFile)
{
	if (!openStreams(inputFile, outputFile))
	{
		return;
	}

	ifstream treeStream;

	treeStream.open(TreeFile, ios::binary);

	if (treeStream.fail())
	{
		cout << "Unable to open tree file." << endl;

		closeStreams();

		return;
	}

	buildTreeFromTreeBuilder(treeStream, true);

	treeStream.close();

	buildEncodingTable();

	encodeBytes();

	closeStreams();

	printFinalInfo(inputFile, outputFile);
}

unsigned int Huffman::getFileSize(string& file_path)
{
	// This method returns the size a file in bytes at the given file path.
	// It uses an ifstream to do this, by opening a file in binary mode
	// at the end position and returns the position, which will be the
	// amount of bytes in the file.
	//
	ifstream stream = ifstream(); // Construct a new ifstream

	if (stream.fail()) // If we can't open the input stream,
	{
		return -1;	   // we return -1, as we can't find the size if we can't open the file.
	}

	// Open the stream at the given file path
	// We open it in binary mode with the binary flag,
	// and the ate flag, which means "at end." This sets
	// the stream's position to the end of the stream
	// when opening.
	//
	stream.open(file_path, ios::binary | ios::ate);

	streampos end = stream.tellg(); // Since we are already at the end of the stream, we can use tellg to grab the position of the end

	stream.close(); // Cleanup by closing out the stream.

	return end; // Return the end position, which will be the file size in bytes.
}

void Huffman::printFinalInfo(string& input_file_path, string& output_file_path)
{
	auto end = chrono::high_resolution_clock::now();

	auto elapsed_seconds = chrono::duration_cast<chrono::duration<double>>(end - start);

	unsigned int bytesIn = getFileSize(input_file_path);

	unsigned int bytesOut = getFileSize(output_file_path);

	cout << "Time: " << elapsed_seconds.count() << " seconds.\t" << formatUnsignedInt(bytesIn) << " bytes in / " << formatUnsignedInt(bytesOut) << " bytes out";
}

string Huffman::formatUnsignedInt(unsigned int number)
{
	string str = to_string(number);

	for (int i = str.length() - 3; i > 0; i -= 3)
	{
		str = str.substr(0, i) + "," + str.substr(i, str.length());
	}

	return str;
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