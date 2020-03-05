#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <chrono>

using namespace std;

class Huffman {
public:
	struct treenode {
		unsigned char symbol = NULL;
		unsigned int weight = 0;
		treenode* leftChild = nullptr;
		treenode* rightChild = nullptr;
	};

	Huffman();
	~Huffman();

	void MakeTreeBuilder(string inputFile, string outputFile);
	void EncodeFile(string inputFile, string outputFile);
	void DecodeFile(string inputFile, string outputFile);
	void EncodeFileWithTree(string inputFile, string treeFile, string outputFile);
	void DisplayHelp();
private:
	const static int amountOfCharacters = 256;

	treenode* nodes[amountOfCharacters];
	string encodingTable[amountOfCharacters];
	string paddingBits;
	ifstream inputStream;
	ofstream outputStream;
	chrono::high_resolution_clock::time_point start;

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
	void printFinalInfo();
	bool isLeaf(treenode* node);
};