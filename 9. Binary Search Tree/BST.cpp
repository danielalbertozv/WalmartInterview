// -------------------------------------------------------------- -*- C++ -*-
// File: BST.cpp
// --------------------------------------------------------------------------
// Developed by Daniel Zuniga
// Date: 10/04/2021
// Version 1.0.0
//
// C++ Version: Binary Search Tree
// --------------------------------------------------------------------------

// Libraries
#include <algorithm>	// For range of elements, subsets of elements
#include <assert.h>		// Diagnostic messages and verify program assumptions
#include <cmath>		// Perform math operations, sqrt(), log(), ...
#include <cstdlib>		// General purpose function, abs(), ...
#include <ctime>		// Converts time to character string
#include <fstream>		// Open file for writing
#include <ilcplex/ilocplex.h>	// CPLEX library
#include <iostream>		// To read and write
#include <time.h>		// C library for ctime 
#include <stdio.h>		// File input and output
#include <stdlib.h>		// Memory allocation, process control, conversion, ...
#include <string>		// String manipulation
//#include<bits/stdc++.h>

using namespace std;

ILOSTLBEGIN // IBM macro to allow run the application in c++ with STL (Standard Template Library)

// Creates AuthorDetials method, (void: no value returned)
void AuthorDetails() {
	cout << "--------------------------------------------------------" << endl;
	cout << "File: BST.cpp" << endl;
	cout << "--------------------------------------------------------" << endl;
	cout << "Developed by Daniel Zuniga" << endl;
	cout << "Date: 10/04/2021" << endl;
	cout << "Version 1.0.0\n" << endl;
	cout << "C++ Version: Binary Search Tree" << endl;
	cout << "--------------------------------------------------------\n" << endl;

} // End AuthorDetails

#pragma region Check If Binary Tree is Binary Search Tree
// Check if Binary Tree is BST

// Class for nodes
class node
{
public:
    int nodeValue;   // Vode value
    node* leftNode;  // Left node
    node* rightNode; // Right node

    // Constructor
    node(int nodeValue)
    {
        this->nodeValue = nodeValue;
        this->leftNode = NULL;
        this->rightNode = NULL;
    }
};

int checkNode(node* rootNode, int leftNodeValue, int rightNodeValue)
{
    // If there are no further nodes to check
    if (rootNode == NULL)
        return 1;
    // If rootNode value is less than leftNodeValue and more than rightNodeValue, it is not BST
    else if (rootNode->nodeValue < leftNodeValue || rootNode->nodeValue > rightNodeValue)
        return 0;

    //Recursively check next nodes
    return
        checkNode(rootNode->leftNode, leftNodeValue, rootNode->nodeValue - 1) &   // leftNode with maximum value of NodeValue -1
        checkNode(rootNode->rightNode, rootNode->nodeValue + 1, rightNodeValue);  // rightNode with minimum value of LeftNode + 1
}

int isBST(node* rootNode)
{
    return(checkNode(rootNode, INT_MIN, INT_MAX));
}

void printSolution(int solution) {
    if (solution)
        cout << "The BT IS BST" << endl;
    else
        cout << "The BT is NOT a BST" << endl;
}

void checkBTisBST() {

    // Example of BT that is a BST
    node* rootNodeA = new node(12);
    rootNodeA -> leftNode = new node(10);
    rootNodeA -> rightNode = new node(14);
    rootNodeA -> leftNode -> leftNode = new node(9);
    rootNodeA -> leftNode -> rightNode = new node(11);
    rootNodeA -> rightNode -> leftNode = new node(13);
    rootNodeA -> rightNode -> rightNode = new node(15);

    printSolution(checkNode(rootNodeA, INT_MIN, INT_MAX));

    // Example of BT that is not a BST
    node* rootNodeB = new node(4);
    rootNodeB->leftNode = new node(2);
    rootNodeB->rightNode = new node(6);
    rootNodeB->leftNode->leftNode = new node(1);
    rootNodeB->leftNode->rightNode = new node(3);
    rootNodeB->rightNode->leftNode = new node(7);
    rootNodeB->rightNode->rightNode = new node(5);

    printSolution(checkNode(rootNodeB, INT_MIN, INT_MAX));
}

#pragma endregion

// Creates Main method, entry point of C++ (int: returns integer)
int main() {

	AuthorDetails();	// calls AuthorDetails method/function 
    checkBTisBST();
	return 0;
}