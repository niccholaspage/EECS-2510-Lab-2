#pragma once

#include <iostream>
#include <string>

using namespace std;

class Huffman {
public:
	struct treenode {
		char symbol = NULL;
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

	int getIndexOfSmallestNode(treenode* nodes[amountOfCharacters], int skipIndex);

	void printStuff(treenode* node, string spaces);
};