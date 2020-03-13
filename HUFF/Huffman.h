#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <chrono>

using namespace std;

class Huffman {
public:
	Huffman();
	~Huffman();

	void MakeTreeBuilder(string inputFile, string outputFile);	// Makes a tree builder file from the given input file in the specified output file
	void EncodeFile(string inputFile, string outputFile);		// Encodes the given input file into the given output file
	void DecodeFile(string inputFile, string outputFile);		// Decodes the given input file into the given output file
	void EncodeFileWithTree(string inputFile, string treeFile, string outputFile); // Encodes the given input file, using the given tree builder file, into the given output file
	void DisplayHelp(); // Displays information on how to use the program
private:
	struct treenode {
		unsigned char symbol = NULL;	// The symbol of the node
		unsigned int weight = 0;		// The weight of the node (amount of times the character appears in a file)
		treenode* leftChild = nullptr;	// A pointer to the left child of the node
		treenode* rightChild = nullptr;	// A pointer to the right child of the node
	};

	const static int amountOfCharacters = 256;

	treenode* nodes[amountOfCharacters];		// An array of node pointers used to build the Huffman tree and encode/decode files.
	string encodingTable[amountOfCharacters];	// A string array containing the encoding bits for each type of character
	string paddingBits;		// A string referring to padding bits that can be written at the end of a byte if extra bits are needed.
	ifstream inputStream;	// An input file stream used for the input file that will be encoded/decoded
	ofstream outputStream;	// An output file stream used for the file that will be written to
	unsigned int bytesIn;	// An unsigned integer that keeps track of the amount of bytes read in, so it can be displayed at the end of the operation.
	unsigned int bytesOut;	// An unsigned integer that keeps track of the amount of bytes written out, so it can be displayed at the end of the operation.
	chrono::high_resolution_clock::time_point start; // A point of time that will represent the very beginning of the operation

	void traverseDestruct(treenode* p); // Traverses through the given node and deletes its children recursively as well as itself
	bool openStreams(string inputFile, string outputFile); // Opens the input and output streams for the given input and output files
	void closeStreams(); // Closes out both the input and output streams
	int getIndexOfSmallestNode(int skipIndex); // Returns the smallest node index in the array, skipping the given index
	void buildTree(); // Builds the tree of nodes by reading the input file and determining frequencies and writes the combinations of nodes to the output stream
	void buildTreeFromTreeBuilder(ifstream& stream, bool writeToOutput); // Builds the tree of nodes by combining nodes based on the given stream.
	void buildEncodingTable(); // Builds the encoding table, which is used to encode each character in a file
	void buildEncodingTable(treenode* node, string currentPath); // Recursively builds encoding table by starting at the given node and traversing through its children
	void decodeBytes(); // Decodes the bytes of the input file
	void encodeBits(unsigned char& outputCharacter, int& currentBit, string& bits); // Encodes the given bits into the output file
	void encodeBytes(); // Encodes the bytes of the input file
	void navigateTree(unsigned char byte, int bitToCheck, treenode*& node); // Navigates through the tree by checking the given bit and navigating to the left and right child of the given node
	void printFinalInfo(string& input_file_path, string& output_file_path); // Prints the final information after the operation ran, like the time elapsed and bytes in and out
	string formatUnsignedInt(unsigned int number); // Formats an unsigned integer by inserting commas into it, returning a string
	bool isLeaf(treenode* node); // Checks if the given node is a leaf
};