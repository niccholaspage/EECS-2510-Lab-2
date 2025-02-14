//==============================================================================================
// File: Huffman.cpp - Huffman tree implementation
// c.f.: Huffman.h
//
// This class implements a Huffman tree, filled with nodes based on the frequency of characters
// appearing in input files. This class allows for the encoding of files, buildin of tree builder
// files, encoding files with tree builder files, and decoding files.
//
// Author:     Nicholas Nassar, University of Toledo
// Class:      EECS 2510-001 Non-Linear Data Structures, Spring 2020
// Instructor: Dr.Thomas
// Date:       Mar 17, 2020
// Copyright:  Copyright 2020 by Nicholas Nassar. All rights reserved.

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
	for (int i = 0; i < AMOUNT_OF_CHARACTERS; i++)
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

	for (int i = 0; i < AMOUNT_OF_CHARACTERS; i++) // Loop through every node in the nodes array
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
	for (int i = 0; i < AMOUNT_OF_CHARACTERS; i++) // We want to loop through every index in the nodes array,
	{
		treenode* node = new treenode; // construct a new treenode,

		node->symbol = i; // set its symbol to the index i, which gets implicitly casted to a unsigned character,
		node->weight = 0; // set the weight to 0, as we don't care about it since we already have the order we are pairing things up in,
		node->leftChild = nullptr; // set the left child to null,
		node->rightChild = nullptr; // set the right child to null,

		nodes[i] = node; // and set the index at i of the nodes array to our newly constructed node.
	}

	// TODO: WHAT HAPPENS IF WE DON'T HAVE A FILE THIS BIG?
	for (int i = 0; i < AMOUNT_OF_CHARACTERS - 1; i++) // Since we are combining nodes based on 510 bytes, we do 255 iterations, or 255 combinations.
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

void Huffman::buildTree(bool incrementBytesIn)
{
	// This method builds the Huffman tree by determining the frequencies of each character in the input file,
	// constructing tree nodes for each character, then combining the two smallest nodes until we are left with
	// one root node.
	//
	// We start by creating an array of unsigned integers that keep track of the amount
	// of times a character occurs in the input file.
	unsigned int frequencyTable[AMOUNT_OF_CHARACTERS];

	for (int i = 0; i < AMOUNT_OF_CHARACTERS; i++) // We want to loop through each index of the frequency table array,
	{
		frequencyTable[i] = 0; // and initialize its value to 0.
	}

	char character; // This variable will hold each character we read from the input stream

	 // While ifstream::get returns a non false value, the next character from the file will be put into our character variable.
	while (inputStream.get(character))
	{
		// By coercing the character into an unsigned char, we solve the issue with invalid array indexing - normally,
		// a char will range from -128 to 127, so without this, we may access the array at index -127 to -1!
		// By casting it to a unsigned char, it's value will range from 0 to 255, fixing any possible indexing issues.
		//
		unsigned char symbol = character;

		frequencyTable[symbol]++; // We increment the frequency at the index of the symbol by 1. In this case, symbol implicitly is casted into an int.

		if (incrementBytesIn) // If we should increment bytes in,
		{
			bytesIn++; // We increment our bytes in counter since we have read a byte.
		}
	}

	for (int i = 0; i < AMOUNT_OF_CHARACTERS; i++) // We now want to loop through every index of the nodes array.
	{
		unsigned char symbol = i;		// We set our symbol to i, which will implicitly cast the int into an unsigned char.

		treenode* node = new treenode;	// We construct a new tree node,

		node->symbol = symbol;			// set its symbol to our symbol from the for loop,
		node->weight = frequencyTable[symbol]; //set its weight to the frequency of the symbol,
		node->leftChild = nullptr;  // and set the left child to nullptr,
		node->rightChild = nullptr; // and the right child to nullptr.

		nodes[i] = node; // Now that we've finished building our node, we set the element at index i of the nodes array to our node.
	}

	for (int i = 0; i < AMOUNT_OF_CHARACTERS - 1; i++) // Since we are combining 256 nodes, we need to do 255 iterations.
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
	// This recursive method builds the encoding table for each character
	// by following the given node's left and right children, building a path
	// of 0's and 1's on the direction taken to reach a certain leaf node, or
	// symbol.
	//
	if (isLeaf(node)) // If the node is a leaf, we are at a node with a symbol,
	{
		encodingTable[node->symbol] = path; // so we need to set the encoding bits for the symbol.

		// If the path's length is greater than 7, it can be used as padding bits
		// if additional bits are needed during encoding for the last byte of a file.
		if (path.length() > 7)
		{
			paddingBits = path; // Set the padding bits to the path so that it can be used later during encoding
		}

		return; // Since this node was a leaf, we don't need to check its left or right child, so we just return
	}

	if (node->leftChild != nullptr) // If the left child of the node isn't null,
	{
		// We recursively call this method to build the encoding table for the left child, passing in the given
		// path + 0, since 0 represents moving to the left child
		buildEncodingTable(node->leftChild, path + "0");
	}

	if (node->rightChild != nullptr) // If the right child of the node isn't null,
	{
		// We recursively call this method to build the encoding table for the right child, passing in the given
		// path + 1, since 1 represents moving to the right child
		buildEncodingTable(node->rightChild, path + "1");
	}
}

bool Huffman::openStreams(string inputFile, string outputFile)
{
	// This method opens the input and output streams for the given input and output
	// file paths so that the program can read the input file and write to the
	// output file. If either stream fails to open, the method returns false,
	// otherwise, true.
	//
	// Open the input stream with the input file as the path. We need to open the stream in binary mode,
	// since we are going to read the file in binary instead of text.
	inputStream.open(inputFile, ios::binary);

	if (inputStream.fail()) // If the input stream fails to open,
	{
		cout << "Unable to open input file." << endl; // we print a message saying we couldn't open the input file,

		return false; // and return false since we failed to open the input stream.
	}

	// Open the output stream with the output file as the path. We need to open the stream in binary mode,
	// since we are going to write to the file in binary instead of text.
	outputStream.open(outputFile, ios::binary);

	if (outputStream.fail()) // If the output stream fails to open,
	{
		cout << "Unable to open output file." << endl; // we print a message saying we couldn't open the output file,

		inputStream.close(); // Since the input stream at this point has been opened, we need to be sure to close it.

		return false; // and return false since we failed to open the output stream.
	}

	return true; // Since we at this point have opened the input and output streams, we can return true because of success!
}

void Huffman::closeStreams()
{
	// This method simply closes the input and output streams as cleanup since we have
	// finished reading the input file and writing the output file.
	//
	inputStream.close(); // Close the input stream

	outputStream.close(); // Close the output stream
}

void Huffman::MakeTreeBuilder(string inputFile, string outputFile)
{
	// This method makes a tree builder file at the given output file path from the given input file.
	// To do this, we have to open our input streams, build our Huffman tree, which will write the bytes 
	// needed to our output file, then close the streams and print out our final info.
	//
	if (!openStreams(inputFile, outputFile)) // If we are unable to open the input and output streams,
	{
		return; // we return, since we can't do anything.
	}


	// We build the tree. This method will read the bytes of the input file, building a frequency table
	// and huffman tree. This method will write the bytes needed for the tree builder as it builds the
	// huffman tree to the output file. Since we want to increment the bytes in here, we pass in true.
	buildTree(true);

	closeStreams(); // We've finished building the tree builder file so we close our input and output streams.

	printFinalInfo(); // We're done, so we can print the elapsed time and amount of bytes in and out.
}

void Huffman::navigateTree(unsigned char byte, int bitToCheck, treenode*& node)
{
	// This method navigates to a child of the given node by checking if the given bit
	// is flipped on in the given byte. This is used during the decoding process to
	// find characters to write to the ouput file. Something important to note is the
	// use of a node pointer reference - the reason we are using this is so that we can
	// change the node pointer to point to a different node and have it properly update
	// for the caller of this method.
	//
	// First, we do a bitwise AND of the given byte and bit we are checking. If the result is a non-zero value,
	// it means that the specific bit of the byte was turned on, or 1, so we navigate to the right child. Otherwise,
	// we navigate to the left child.
	node = byte & bitToCheck ? node->rightChild : node->leftChild;

	if (isLeaf(node)) // If the node is a leaf, it will have a valid symbol,
	{
		outputStream.put(node->symbol); // so we write it to the output stream,

		bytesOut++; // increment our bytes out counter since we just wrote a byte,

		node = nodes[0]; // and reset the node pointer reference back to the head of the tree for future calls.
	}
}

void Huffman::decodeBytes()
{
	// This method decodes all of the bytes of the input stream. We do this
	// by reading in each byte of the input stream, and navigate the Huffman
	// tree repeatedly until we reach a leaf node, then write the node's symbol
	// to the output stream. We then start back at the root of the Huffman tree
	// and continue until we are out of bytes.
	//
	char character; // This variable will hold each character we read from the input stream.

	 // We set a variable for the current node at the 0th element of our nodes array,
	// since by this point, we will have built our Huffman tree and its root will always
	// be at nodes[0]. We use this pointer to navigate the Huffman tree, finding leaf
	// nodes so that we can write their symbols to the file.
	treenode* currentNode = nodes[0];

	while (inputStream.get(character)) // While the input stream successfully reads in a character,
	{
		bytesIn++; // We increment our bytes in counter since we have read a byte.

		// We are using our character as a an 8-bit byte with a value of 0 to 255, so we implicitly
		// cast it into a unsigned char.
		unsigned char byte = character;

		// An interesting thing to note is that I had tested writing this in a different way. I made a
		// member variables that was an int array with the powers of 2, from 1 to 128, and I would loop
		// through each power and I inlined the navigateTree method in the for loop. Somehow, that performed
		// worse (although only very slightly) than having a navigateTree method and calling it 8 times,
		// so I stuck with this method, even though there are quite a bit of repeated lines.
		navigateTree(byte, 128, currentNode); // Use the navigateTree method to go to the correct node's child based on the first bit from the left
		navigateTree(byte, 64, currentNode); // Use the navigateTree method to go to the correct node's child based on the second bit
		navigateTree(byte, 32, currentNode); // Use the navigateTree method to go to the correct node's child based on the third bit
		navigateTree(byte, 16, currentNode); // Use the navigateTree method to go to the correct node's child based on the fourth bit
		navigateTree(byte, 8, currentNode); // Use the navigateTree method to go to the correct node's child based on the fifth bit
		navigateTree(byte, 4, currentNode); // Use the navigateTree method to go to the correct node's child based on the sixth bit
		navigateTree(byte, 2, currentNode); // Use the navigateTree method to go to the correct node's child based on the seventh bit
		navigateTree(byte, 1, currentNode); // Use the navigateTree method to go to the correct node's child based on the eighth bit
	}
}

void Huffman::encodeBits(unsigned char& outputCharacter, int& currentBit, string& bits)
{
	// This method encodes the given string of bits into the output character.
	// The given currentBit keeps track of which bit we are changing in the
	// output character. Once we hit 8, we write to the output file, reset
	// our outputCharacter and currentBit, and continue.
	//
	for (unsigned int i = 0; i < bits.length(); i++) // Loop through every character of the bits string
	{
		// If the character at index i is equal to '1', it means that we need to encode a 1 at the current bit.
		// Otherwise, we will be encoding a 0 for off.
		bool on = bits[i] == '1';

		// Here, we are building the output character or byte bit by bit. To turn on a specific bit of of a byte,
		// we just need to use a bitwise OR operator with it and another byte that just has one bit turned on,
		// like 0000 1000, for example. Since we are using currentBit to represent the bit we are on, beginning from
		// the left, we can use it to determine which bit we will be changing. First, we use the bitwise OR equal
		// operator. We then take our boolean on, which is either true or false, which in, and use the shift
		// left operator to shift its value of 1 or 0 to the left a certain number of times to represent the
		// bit we are changing. Our boolean ends up getting implicitly casted as an integer when using this shift
		// operator. If on is true and currentBit is 0 for example, we would start with the byte 0000 0001, and
		// then shift it to the left 7 - 0 times, resulting in 1000 0000. Now we bitwise OR it with our output
		// character and put the result in our output character variable, successfully flipping a bit in our
		// output character.
		outputCharacter |= (on << (7 - currentBit));

		currentBit++; // We now increment the currentBit so that we can take care of the next bit inside of our output character.

		if (currentBit == 8) // If the current bit is 8, we have taken care of all bits inside of the output character.
		{
			outputStream.put(outputCharacter); // Since we've finished this character, we write it to the output stream,

			bytesOut++; // increment our bytes out by 1 since we've written a byte,

			outputCharacter = 0; // reset our output character back to 0,

			currentBit = 0; // and reset the current bit back to 0.
		}
	}
}

void Huffman::encodeBytes()
{
	// This method encodes each character of the input stream by finding its encoding bits
	// in the encoding table and writes them out to the output stream as bytes are formed from
	// each set of 8 bits.
	//
	// By this point, we may have read every byte of the file for our generation of our Huffman tree,
	// so we need to reset the input stream so we can read the input file again.
	inputStream.clear();	// We clear the internal error state flags of the input stream,
	inputStream.seekg(0);	// and we set the position in the input sequence to 0, or the beginning of the file.

	char character; // This variable will hold each character we read from the input stream

	unsigned char outputCharacter = 0; // This character will hold the byte we will be writing out to the output stream.

	int currentBit = 0; // The current bit we are changing in the output character

	while (inputStream.get(character)) // While the input stream successfully reads in a character, we will encode the character's bits.
	{
		// By coercing the character into an unsigned char, we solve the issue with invalid array indexing - normally,
		// a char will range from -128 to 127, so without this, we may access the array at index -127 to -1!
		// By casting it to a unsigned char, it's value will range from 0 to 255, fixing any possible indexing issues.
		//
		unsigned char symbol = character;

		string& bitString = encodingTable[symbol]; // We get the bit string for encoding the symbol we read from the file.

		encodeBits(outputCharacter, currentBit, bitString); // We encode the bits of the symbol into our output character.

		bytesIn++; // We increment the bytes in by one, since we have read another byte of the file.
	}

	// At this point, we may be in the middle of an output character, and we don't to forget to write some
	// bits to our file. To finish off the byte, we need to write some padding bits that won't inadvertently
	// be a valid character. Since we've already set our padding bits member variable, we can just encode
	// those bits and our file will be taken care of.
	if (currentBit != 0)
	{
		int currentBitInBitString = 0;

		while (currentBit != 8)
		{
			bool on = paddingBits[currentBitInBitString] == '1';

			outputCharacter |= (on << (7 - currentBit));

			currentBit++;
			currentBitInBitString++;
		}

		outputStream.put(outputCharacter); // Since we've finished this character, we write it to the output stream,

		bytesOut++; // increment our bytes out by 1 since we've written a byte,
	}
}

void Huffman::EncodeFile(string inputFile, string outputFile)
{
	// This method encodes the given input file into the given output file.
	// To do this, we open the streams, build a Huffman tree from the input file,
	// build our encoding table for each character, and encode the bytes. We then
	// finish up by closing the streams and printing our final info.
	//
	if (!openStreams(inputFile, outputFile)) // If we are unable to open the input and output streams,
	{
		return; // we return, since we can't do anything.
	}

	// We build the tree. This method will read the bytes of the input file, building a frequency table
	// and huffman tree. This method will write the bytes needed for the tree builder as it builds the
	// huffman tree to the output file. Since we don't want to increment the bytes here, we pass in
	// false.
	buildTree(false);

	buildEncodingTable(); // We build the encoding table so we can encode each character of the input file.

	encodeBytes(); // Now we encode each byte of the input stream.

	closeStreams(); // We've finished encoding each byte of the file, so we close our input and output streams.

	printFinalInfo(); // We're done, so we can print the elapsed time and amount of bytes in and out.
}

void Huffman::DecodeFile(string inputFile, string outputFile)
{
	// This method decodes the given input file into the given output file.
	// To do this, we open the streams, build a Huffman tree from the tree builder
	// in the input file, which is the first 510 bytes of the input file. We then
	// decode the remaining bytes of the input file. We then finish up by closing
	// the streams and printing our final info.
	//
	if (!openStreams(inputFile, outputFile)) // If we are unable to open the input and output streams,
	{
		return; // we return, since we can't do anything.
	}

	// We build the tree from the tree builder in the first 510 bytes of the input file.
	// This method will read the first 510 bytes of the input file, building a huffman tree,
	// that we will use to decode the file.
	buildTreeFromTreeBuilder(inputStream, false);

	// We need to add 510 bytes to the bytes we've read in, since we read the first 510 bytes.
	// The buildTreeFromTreeBuilder method does not do this, so I'm just doing it here instead.
	bytesIn += 510;

	decodeBytes(); // Now, we decode each remaining byte of the input stream.

	closeStreams(); // We've finished decoding each byte of the file, so we close our input and output streams.

	printFinalInfo(); // We're done, so we can print the elapsed time and amount of bytes in and out.
}

void Huffman::EncodeFileWithTree(string inputFile, string TreeFile, string outputFile)
{
	// This method encodes the given input file into the given output file, but
	// uses the given tree file to build the Huffman tree. To do this, we open the streams,
	// build a Huffman tree from the tree file, build our encoding table for each character,
	// and encode the bytes. We then finish up by closing the streams and printing our final info.
	//
	// If we don't open the tree stream first and make sure its valid, we will accidentally create
	// an empty output file on failure of opening the tree stream file.
	ifstream treeStream; // We declare another ifstream so we can read our tree file.

	// Open the tree stream with the tree file as the path. We need to open the stream in binary mode,
	// since we are going to read the file in binary instead of text.
	treeStream.open(TreeFile, ios::binary);

	if (treeStream.fail()) // If the tree stream fails to open,
	{
		cout << "Unable to open tree file." << endl; // we print a message saying we couldn't open the tree file,

		closeStreams(); // close out the other streams since they are open by this point,

		return; // and return because we are done at this point.
	}

	if (!openStreams(inputFile, outputFile)) // If we are unable to open the input and output streams,
	{
		return; // we return, since we can't do anything.
	}

	// We build the tree from the tree builder in the first 510 bytes of the input file.
	// This method will read the first 510 bytes of the tree stream file, building a huffman tree,
	// that we will use to encode the file. By passing true as the second parameter if this method,
	// it will write the tree builder bytes to the output stream so that the file can be decoded properly.
	buildTreeFromTreeBuilder(treeStream, true);

	treeStream.close(); // Close the tree stream since we've finished building our Huffman tree

	buildEncodingTable(); // We build the encoding table so we can encode each character of the input file.

	encodeBytes(); // Now we encode each byte of the input stream.

	closeStreams(); // We've finished encoding each byte of the file, so we close our input and output streams.

	printFinalInfo(); // We're done, so we can print the elapsed time and amount of bytes in and out.
}

void Huffman::printFinalInfo()
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
	// This method prints out the usage options of the Huffman program
	// available to the user.
	//
	cout << "Available flags:\n";
	cout << "-h|-?|-help - Prints out this help\n";
	cout << "-e file1 [file2] - Encodes file1, placing the encrypted version into file2. If file2 is not specified, file2 will have the same name as file1, minus the extension, which will be .huf.\n";
	cout << "-d file1 file 2 - Decodes file1, placing the decrypted version into file1.\n";
	cout << "-t file1 [file2] - Creates a 510 byte tree-builder file for file1, and places it into file2.\n";
	cout << "-et file1 file2 [file3] - Encodes file1 with the tree built from file2 and places it into file3. If file3 is not specified, the output file will have the same name as file1 with the .huf extension.\n";
}