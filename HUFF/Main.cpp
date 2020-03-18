//==============================================================================================
// Main.cpp - Huffman program
//
// This program implements the Huffman code algorithm, allowing for users to encode and decode
// files. The program encodes files by building a Huffman tree based on the frequency of
// characters in the file, and encodes each character of the file. In doing so, files may
// be smaller than their original size, so Huffman coding can be used as a form of compression.
//
// Author:     Nicholas Nassar, University of Toledo
// Class:      EECS 2510-001 Non-Linear Data Structures, Spring 2020
// Instructor: Dr.Thomas
// Date:       Mar 17, 2020
// Copyright:  Copyright 2020 by Nicholas Nassar. All rights reserved.

#include <iostream>

#include "Huffman.h"

using namespace std;

string replaceExtension(const string& fileName, const string& extension)
{
	// This method simply replaces the extension of the given filename with
	// the given file extension. If a file doesn't have an extension, the
	// given extension will just be added on to the file's name.
	//

	// We first need to find the position of the first dot in the file name.
	size_t dotPosition = fileName.find('.');

	if (dotPosition == string::npos)		// If there is no dot in the file name,
	{
		return fileName + "." + extension;	// we simply add a dot and the extension to the file name and return it.
	}
	else
	{
		// Otherwise, we take a substring of the file name from the beginning to the dot,
		// and add a dot and the extension to it, then return it
		return fileName.substr(0, dotPosition) + "." + extension;
	}
}


void handleCommandLineParameters(int argc, char* argv[], Huffman* huffman)
{
	// This method handles the commandline parameters and runs the proper
	// method of the Huffman class. It also automatically passes in output
	// file names automatically for encoding commands.
	//
	// If there is only one argument, which is the path of the executable, the user did not provide any flags.
	if (argc < 2)
	{
		cout << "No flags given! Here is some help!" << endl; // Print out that no flags were given,

		huffman->DisplayHelp(); // Display help to the user so they can see how to use the program.

		return; // We're done here, so now we return.
	}

	string flag = argv[1]; // The first argument is the flag the user passed in.

	if (flag[0] != '-' || flag.length() < 2) // If the flag doesn't start with a - or is only one character long,
	{
		cout << "Invalid flag format!" << endl; // it is incorrectly formatted, so print that out.

		return; // We're done here, so now we return.
	}

	for (unsigned int i = 0; i < flag.length(); i++) // Loop through every character in the flag string,
	{
		flag[i] = tolower(flag[i]); // and make it lowercase, therefore making the entire string lowercase.
	}

	string command = flag.substr(1); // Since the flag will always start with a dash, we strip it out and just focus on the command.

	if (command == "h" || command == "?" || command == "help") // If the command is h, ?, or help,
	{
		huffman->DisplayHelp(); // we just display the help and we're done.
	}
	else if (command == "e") // If the command is e, we are going to encode a file.
	{
		if (argc < 3) // If we have less than 3 arguments, we are missing the input file path,
		{
			cout << "Missing arguments!" << endl; // so we print that we are missing arguments.
		}
		else // Otherwise,
		{
			string input_path = argv[2]; // we get our input path,

			// get our output path, which is either the 3rd argument, or automatically
			// calculated from the input path.
			string output_path = argc < 4 ? replaceExtension(input_path, "huf") : argv[3];

			// We then tell our Huffman instance to encode the file at the input path to the given output path.
			huffman->EncodeFile(input_path, output_path);
		}
	}
	else if (command == "d") // If the command is d, we are going to decode a file.
	{
		if (argc < 4) // If we have less than 3 arguments, we are missing the input or output file path,
		{
			cout << "Missing arguments!" << endl; // so we print that we are missing arguments.
		}
		else // otherwise,
		{
			// we tell our Huffman instance to decode the file, passing in the input and output file paths.
			huffman->DecodeFile(argv[2], argv[3]);
		}
	}
	else if (command == "t") // If the command is t, we are going to make the tree builder file.
	{
		if (argc < 3) // If we have less than 3 arguments, we are missing the input file path,
		{
			cout << "Missing arguments!" << endl; // so we print that we are missing arguments.
		}
		else // otherwise,
		{
			string input_path = argv[2]; // we get our input path,

			// get our output path, which is either the 3rd argument, or automatically
			// calculated from the input path.
			string output_path = argc < 4 ? replaceExtension(input_path, "htree") : argv[3];

			// We then tell our Huffman instance to make a tree builder file, using the
			// input path and writing to the output path.
			huffman->MakeTreeBuilder(input_path, output_path);
		}
	}
	else if (command == "et") // Finally, if the command is et, we are going to encode a file with the tree builder file.
	{
		if (argc < 4) // If we have less than 3 arguments, we are missing the input or tree builder file path,
		{
			cout << "Missing arguments!" << endl; // so we print that we are missing arguments.
		}
		else
		{
			string input_path = argv[2]; // we get our input path,

			// get our output path, which is either the 4rd argument, or automatically
			// calculated from the input path.
			string output_path = argc < 5 ? replaceExtension(input_path, "huf") : argv[4];

			// We then tell our Huffman instance to encode the input file with our tree file path
			// into the output file path.
			huffman->EncodeFileWithTree(input_path, argv[3], output_path);
		}
	}
	else
	{
		cout << "Invalid flag!" << endl; // Otherwise, the user didn't give a valid flag, so we say so.
	}
}

int main(int argc, char* argv[])
{
	// This is the entry point of the application, so we first set the parameters of cout for proper output,
	// construct the huffman tree and handle the commandline parameters given to the program.
	//
	cout.setf(ios::fixed); // Set the formatting of cout to be fixed for floating point numbers
	cout.setf(ios::showpoint); // Set the formatting of cout to display the decimal point for all floating point values
	cout.precision(3); // Sets the decimal precision of cout

	Huffman* huffman = new Huffman(); // Construct a new Huffman instance

	// Handle the commandline parameters, passing in the amount of arguments, arguments themselves, and Huffman instance
	handleCommandLineParameters(argc, argv, huffman);

	delete huffman; // We delete the huffman instace before ending the program

	return 0; // We return 0, as we have not experienced an error, and our program has exited!
}