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

	void MakeTreeBuilder(string inputFile, string outputFile);
	void EncodeFile(string inputFile, string outputFile);
	void DecodeFile(string inputFile, string outputFile);
	void EncodeFileWithTree(string inputFile, string treeFile, string outputFile);
	void DisplayHelp();
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
	string paddingBits;							// A string referring to padding bits that can be written at the end of a byte if extra bits are needed.
	ifstream inputStream;
	ofstream outputStream;
	unsigned int bytesIn;
	unsigned int bytesOut;
	chrono::high_resolution_clock::time_point start; // A point of time that will represent the very beginning of the operation

	void traverseDestruct(treenode* p); // Traverses through the given node and deletes its children recursively as well asitself
	bool openStreams(string inputFile, string outputFile);
	void closeStreams();
	int getIndexOfSmallestNode(treenode* nodes[amountOfCharacters], int skipIndex);
	void buildTree();
	void buildTreeFromTreeBuilder(ifstream& stream, bool writeToOutput);
	void buildEncodingTable();
	void buildEncodingTable(treenode* node, string currentPath);
	void decodeBytes();
	void encodeBits(unsigned char& outputCharacter, int& currentBit, string& bits);
	void encodeBytes();
	void navigateTree(unsigned char byte, int bitToCheck, treenode*& node);
	void printFinalInfo(string& input_file_path, string& output_file_path);
	string formatUnsignedInt(unsigned int number);
	bool isLeaf(treenode* node);
};