#include "Huffman.h"

Huffman::Huffman() : nodes{ nullptr }
{
	// The constructor. We just need to intialize all of our member variables:
	//
	bytesIn = 0;	// Initialize our bytes in to zero, as we haven't read any bytes yet.

	bytesOut = 0;	// Initialize our bytes out to zero as well, as we haven't written any bytes either.

	start = chrono::high_resolution_clock::now(); // We set the starting time position to the current time.
}

Huffman::~Huffman()
{
	// In the destructor, we need to make sure we properly clean up the nodes we had.
	// To do this, we loop through node in the nodes array.
	//
	for (int i = 0; i < amountOfCharacters; i++)
	{
		if (nodes[i] != nullptr) // If a node exists at element i in the nodes array...
		{
			traverseDestruct(nodes[i]); // We call the traverseDestruct method to take care of the node and its children, recursively.
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

int Huffman::getIndexOfSmallestNode(int skipIndex)
{
	// This method returns the index of the smallest node in the array (based on a node's weight).
	// To do this, we will loop through every node in the nodes array, keeping track of
	// the index and weight of the smallest node.
	int smallestNodeIndex = -1; // Set up a variable to keep track of the index of the smallest node
	unsigned int smallestWeight = UINT_MAX; // Initialize the smallest weight to the maximum unsigned integer.

	for (int i = 0; i < amountOfCharacters; i++) // Loop through every node in the nodes array
	{
		if (skipIndex == i) // If i is the index we want to skip,
		{
			continue;		// we continue, as we don't want to check the node at this index.
		}

		treenode* node = nodes[i]; // Get the node pointer at index i of the nodes array

		if (node == nullptr)	// If the node pointer is nullptr, we have no weight to check,
		{
			continue;			// so we continue to the next node.
		}

		if (node->weight < smallestWeight)	// If the node's weight is smaller than the smallest weight,
		{
			smallestNodeIndex = i;			// we set the smallest node index to i,
			smallestWeight = node->weight;	// and we set the smallest weight to the node's weight.
		}
	}

	// Since we've checked every node in the array and have the
	// smallest node index, we can now return it.
	return smallestNodeIndex;
}

void Huffman::buildTreeFromTreeBuilder(ifstream& stream, bool writeToOutput)
{
	// This method builds the Huffman tree by combining nodes based
	// on the first 510 bytes of the input stream passed in. If the
	// write to output parameter is on, the bytes will be written
	// to the output stream.
	//
	for (int i = 0; i < amountOfCharacters; i++) // We want to loop through every index in the nodes array,
	{
		treenode* node = new treenode; // construct a new treenode,

		node->symbol = i; // set its symbol to the index i, which gets implicitly casted to a unsigned character,
		node->weight = 0; // set the weight to 0, as we don't care about it since we already have the order we are pairing things up in,
		node->leftChild = nullptr; // set the left child to null,
		node->rightChild = nullptr; // set the right child to null,

		nodes[i] = node; // and set the index at i of the nodes array to our newly constructed node.
	}

	// TODO: WHAT HAPPENS IF WE DON'T HAVE A FILE THIS BIG?
	for (int i = 0; i < amountOfCharacters - 1; i++) // Since we are combining nodes based on 510 bytes, we do 255 iterations, or 255 combinations.
	{
		unsigned char leftIndex = stream.get();		// We get the character representing the left index of the nodes we are going to combine,
		unsigned char rightIndex = stream.get();	// and we also get the character representing the right index.

		if (writeToOutput) // If we are writing to the output stream,
		{
			outputStream.put(leftIndex);	// we write the left index
			outputStream.put(rightIndex);	// as well as the right index.

			bytesOut += 2; // Since we've written two characters to the output stream, we need to increment our bytes out by two.
		}

		treenode* parent = new treenode; // We construct a new node that will act as the parent of the nodes at the left and right index.

		treenode* leftNode = nodes[leftIndex];		// We get the node at the left index
		treenode* rightNode = nodes[rightIndex];	// as well as the right index.

		parent->symbol = NULL; // Parent nodes don't need to have a valid symbol, so we just set it to NULL.
		parent->weight = 0; // We don't care about the weight, since just like before, we already have the order we are pairing things up in.

		parent->leftChild = leftNode;	// We set the parent's left child to the left node
		parent->rightChild = rightNode;	// and the parent's right child to the right node.

		// Now that we've finished setting up the parent,
		nodes[leftIndex] = parent;		// we can set the node at the left index in the nodes array to the parent,
		nodes[rightIndex] = nullptr;	// and set the right index in the nodes array to nullptr.
	}
}

void Huffman::buildTree()
{
	// This method builds the Huffman tree by determining the frequencies of each character in the input file,
	// constructing tree nodes for each character, then combining the two smallest nodes until we are left with
	// one root node.
	//
	// We start by creating an array of unsigned integers that keep track of the amount
	// of times a character occurs in the input file.
	unsigned int frequencyTable[amountOfCharacters];

	for (int i = 0; i < amountOfCharacters; i++) // We want to loop through each index of the frequency table array,
	{
		frequencyTable[i] = 0; // and initialize its value to 0.
	}

	char character; // We declare this variable so that it can hold each character we read from the file

	 // While ifstream::get returns a non false value, the next character from the file will be put into our character variable.
	while (inputStream.get(character))
	{
		// By coercing the character into an unsigned char, we solve the issue with invalid array indexing - normally,
		// a char will range from -128 to 127, so without this, we may access the array at index -127 to -1!
		// By casting it to a unsigned char, it's value will range from 0 to 255, fixing any possible indexing issues.
		//
		unsigned char symbol = character;

		frequencyTable[symbol]++; // We increment the frequency at the index of the symbol by 1. In this case, symbol implicitly is casted into an int.
	}

	for (int i = 0; i < amountOfCharacters; i++) // We now want to loop through every index of the nodes array.
	{
		unsigned char symbol = i;		// We set our symbol to i, which will implicitly cast the int into an unsigned char.

		treenode* node = new treenode;	// We construct a new tree node,
		
		node->symbol = symbol;			// set its symbol to our symbol from the for loop,
		node->weight = frequencyTable[symbol]; //set its weight to the frequency of the symbol,
		node->leftChild = nullptr;  // and set the left child to nullptr,
		node->rightChild = nullptr; // and the right child to nullptr.

		nodes[i] = node; // Now that we've finished building our node, we set the element at index i of the nodes array to our node.
	}

	for (int i = 0; i < amountOfCharacters - 1; i++) // Since we are combining 256 nodes, we need to do 255 iterations.
	{
		// We first get the smallest node index, passing -1 as our skip index since we don't want to skip any node.
		int smallestNodeIndex = getIndexOfSmallestNode(-1);

		// Now we get the next smallest node index, passing in our smallest node index so that we skip it.
		int nextSmallestNodeIndex = getIndexOfSmallestNode(smallestNodeIndex);

		treenode* parent = new treenode; // We now need to construct a parent node that will have these smallest nodes as children.

		treenode* smallestNode = nodes[smallestNodeIndex];			// We get the node at the smallest node index
		treenode* nextSmallestNode = nodes[nextSmallestNodeIndex];	// and the node at the next smallest node index.

		parent->symbol = NULL; // In a Huffman tree, a parentnode's symbol doesn't matter, so we set it to NULL.
		parent->weight = smallestNode->weight + nextSmallestNode->weight; // The parent's weight is the sum of its two childrens' weights.

		// We always want the node with the smallest index to be the left child,
		// so we check if the smallest node's index is less than the next smallest
		// node's index. We also always want the parent node to be put into the
		// smaller index of the two node indices.
		//
		if (smallestNodeIndex < nextSmallestNodeIndex)
		{
			parent->leftChild = smallestNode;		// In this case, the left child is the smallest node,
			parent->rightChild = nextSmallestNode;	// and the right child is the next smallest node.

			// Since the smallest node's index is smaller than the next smallest node's index, we put the
			// parent node in that index of the nodes array.
			nodes[smallestNodeIndex] = parent;
			nodes[nextSmallestNodeIndex] = nullptr; // We then set the element at the other index to nullptr.

			// Finally, we write the smallest node index and next smallest node index to the
			// output stream so that our program can properly rebuild the tree by knowing
			// which nodes to combine.
			outputStream.put(smallestNodeIndex);
			outputStream.put(nextSmallestNodeIndex);
		}
		else
		{
			parent->leftChild = nextSmallestNode;	// In this case, the left child is the next smallest node,
			parent->rightChild = smallestNode;		// and the right child is the smallest node.

			// Since the next smallest node's index is smaller than the smallest node's index, we put the
			// parent node in that index of the nodes array.
			nodes[nextSmallestNodeIndex] = parent;
			nodes[smallestNodeIndex] = nullptr; // We then set the element at the other index to nullptr.

			// Finally, we write the next smallest node index and smallest node index to the
			// output stream so that our program can properly rebuild the tree by knowing
			// which nodes to combine.
			outputStream.put(nextSmallestNodeIndex);
			outputStream.put(smallestNodeIndex);
		}

		bytesOut += 2; // We increment the bytes out by 2, as we have written two characters to the file.
	}
}

bool Huffman::isLeaf(treenode* node)
{
	// This method simply checks if the given node is a leaf,
	// meaning it has no left or right child.
	//
	return node->leftChild == nullptr && node->rightChild == nullptr;
}

void Huffman::buildEncodingTable()
{
	// This method builds the encoding table so that each character
	// in a file will have an encoding string for it inside of the
	// encoding table array. It is simply a convenience method that
	// calls the recursive function buildEncodingTable. Unfortunately,
	// non-static member variables can't be used as default parameters,
	// so this method is required instead of just having default parameters
	// of the buildEncodingTable method below.
	//
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

		bytesOut++;

		node = nodes[0];
	}
}

void Huffman::decodeBytes()
{
	char character;

	treenode* currentNode = nodes[0];

	while (inputStream.get(character))
	{
		bytesIn++;

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

			bytesOut++;

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
		bytesIn++;

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

void Huffman::printFinalInfo(string& input_file_path, string& output_file_path)
{
	// This method prints out the time elapsed and the bytes in from the
	// size of the file at the input file path as well as the bytes out
	// from the size of the file at the output file path.
	//
	auto end = chrono::high_resolution_clock::now(); // Get a time point representing the current time.

	// This takes the difference between the end time and start time and casts it into a duration with the double
	// type, returning the seconds between the end and start time including decimals.
	//
	auto elapsed_seconds = chrono::duration_cast<chrono::duration<double>>(end - start);

	cout << "Time: " << elapsed_seconds.count() << " seconds.\t"; // Print out the time elapsed in seconds and a tab
	cout << formatUnsignedInt(bytesIn) << " bytes in / " << formatUnsignedInt(bytesOut) << " bytes out\n"; // Print the bytes in and out, formatted
}

string Huffman::formatUnsignedInt(unsigned int number)
{
	// This method formats the given unsigned integer by
	// inserting commas into it, starting from the right of the string
	// going to the left, inserting commas every 3 numbers.
	//
	string str = to_string(number); // Turn the number into a string

	for (int i = str.length() - 3; i > 0; i -= 3) // Loop through the entire string, starting from the end - 3, decrementing by 3.
	{
		// At this point, i is the position we want to put a comma at.
		// We simply take a substring of the string from the beginning to i,
		// add a comma, and add the substring from i to the end of the string,
		// and set str to it.
		//
		str = str.substr(0, i) + "," + str.substr(i);
	}

	return str; // Return the formatted string
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