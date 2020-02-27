#pragma once

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

class Huffman {
public:
	struct treenode {
		unsigned char symbol = NULL;
		int weight = 0;
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
	ifstream inputStream;
	ofstream outputStream;

	bool openStreams(string inputFile, string outputFile);
	void closeStreams();
	int getIndexOfSmallestNode(treenode* nodes[amountOfCharacters], int skipIndex);
	void buildTree();
	void buildEncodingTable();
	void buildEncodingTable(treenode* node, string currentPath);
	void encodeBytes();
	bool isLeaf(treenode* node);
};