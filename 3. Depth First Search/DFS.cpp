// -------------------------------------------------------------- -*- C++ -*-
// File: Example_Walmart_DFS.cpp
// --------------------------------------------------------------------------
// Developed by Daniel Zuniga
// Date: 09/18/2021
// Version 1.0.0
//
// C++ Version: DFS Examples
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
#include <list>
#include <vector>
#include <stack>

ILOSTLBEGIN // IBM macro to allow run the application in c++ with STL (Standard Template Library)

// Creates AuthorDetials method, (void: no value returned)
void AuthorDetails() {
	cout << "--------------------------------------------------------" << endl;
	cout << "File: Example_Walmart_DFS.cpp" << endl;
	cout << "--------------------------------------------------------" << endl;
	cout << "Developed by Daniel Zuniga" << endl;
	cout << "Date: 09/18/2021" << endl;
	cout << "Version 1.0.0\n" << endl;
	cout << "C++ Version: DFS Examples" << endl;
	cout << "--------------------------------------------------------\n" << endl;

} // End AuthorDetails

#pragma region Helper Functions

#define rowNumber 8
#define colNumber 8

// Class for graph
class Graph
{
    // Nounmber of vertices
    int V;

    // Pointer to an array containing adjacency list
    list<int>* adj;

    // Pointer to an array containing adjacency vector
    vector<int>* vec;

    // Recursive check for cycles
    bool isCycleRecursiveCheck(int v, bool visited[], int parent);

    // Recursive DFS for mother vertex
    void motherVecterDFS(int v, vector<bool>& visited);

    // A recursive function used by printAllPaths()
    void printPathsDFS(int, int, vector<bool>&, int[], int&);

public:

    // Constructor
    Graph(int V);

    // Add directed edge to graph as list
    void addUndirectedEdge(int v, int w);

    // Add undirected edge as list
    void addDirectedEdge(int v, int w);

    // Add directed edge as vector
    void addDirectedEdgeVector(int v, int w);

    // Returns true if there is a cycle
    bool isCyclic();

    // Prints all mother vertices of a graph
    void findMother();

    ///  Recursive DFS to print all paths from a source to a destination
    void printPaths(int s, int d);

    ///  Given a directed graph and a source, identify the DFS
    void graphDFS(int s);

    ///  Recursive DFS to identify the longest path
    void longestPathDFS(int v, int longestPathPerNode[], bool visited[]);
};

Graph::Graph(int V)
{
    this->V = V;
    adj = new list<int>[V];
}

void Graph::addUndirectedEdge(int v, int w)
{
    // Add w to v’s list.
    adj[v].push_back(w);

    // Add v to w’s list.
    adj[w].push_back(v);
}

void Graph::addDirectedEdge(int v, int w)
{
    adj[v].push_back(w); // Add w to v’s list.
}

void Graph::addDirectedEdgeVector(int v, int w)
{
    vec[v].push_back(w); // Add w to v’s list.
}

#pragma endregion

#pragma region Graph Depth First Search from a Specified Node

/// <summary>
///  Given a directed graph and a source, identify the DFS
/// </summary>
/// <param name="source"> Source node </param>
void Graph::graphDFS(int source)
{
    vector<bool> visited(V, false);  // Initialze visited vector in False
    stack<int> dfsNodes;    // Stack to store the nodes of the DFS

    dfsNodes.push(source);  // Add first node

    while (!dfsNodes.empty()){
        int source = dfsNodes.top();
        dfsNodes.pop();    //Remove top node from stack

        if (!visited[source]){  //print node if not visited
            cout << source << " "; 
            visited[source] = true;   //update visited vector
        }

        // Loop in adjacent node list and push (add on top) if not visited
        list<int>::iterator i;
        for (i = adj[source].begin(); i != adj[source].end(); ++i)
            if (!visited[*i])
                dfsNodes.push(*i);
    }
}

/// <summary>
///  Given a directed graph and a source, identify the DFS
/// </summary>
/// <param name="source"> Source node </param>
void graphDFS(int source)
{
    // Display used cycle graph example
    int edges[][2] = { {0,3},
                       {1,0},
                       {2,5},
                       {3,1},
                       {4,1},
                       {4,2},
                       {4,6},
                       {6,7},
                       {7,3},
                       {7,4} };

    Graph g(colNumber);

    cout << "The following directed graph example with a cycle is used:" << endl;
    for (int i = 0; i < size(edges); i++) {
        cout << edges[i][0] << " " << edges[i][1] << endl;
        g.addDirectedEdge(edges[i][0], edges[i][1]);     //Saves edge into graph
    }

    cout << "DFS from node " << source << " is:" << endl;
    g.graphDFS(source);

}
#pragma endregion

#pragma region Print All Paths from Source to Destination

/// <summary>
///  Recursive DFS to print all paths from a source to a destination
/// </summary>
/// <param name="source"> Source node </param>
/// <param name="destination"> Destintion node </param>
void Graph::printPathsDFS(int source, int destination, vector<bool>& visited, int path[], int& indexPath)
{
    // Mark the current node and store it in path[]
    visited[source] = true;
    path[indexPath] = source;
    indexPath++;

    // End of recursion for a path, case source is destination 
    if (source == destination) {
        for (int i = 0; i < indexPath; i++)
            cout << path[i] << " ";     // Print saved path
        cout << endl;
    }
    else // If current vertex is not destination
    {
        // Recursion for all adjacent nodes
        list<int>::iterator i;
        for (i = adj[source].begin(); i != adj[source].end(); ++i)
            if (!visited[*i])
                printPathsDFS(*i, destination, visited, path, indexPath);
    }

    // Reverse parameters for DFS considering next adjacent node
    indexPath--;
    visited[source] = false;
}

/// <summary>
/// Given a source, a destination and a directed graph, print all their paths
/// Initialize path and call recursive DFS
/// </summary>
/// <param name="source"> Source node </param>
/// <param name="destination"> Destintion node </param>
void Graph::printPaths(int source, int destination)
{
    vector <bool> visited(V, false);  // Visited node vector initialized as false

    // Create an array to store paths
    int* path = new int[V];
    int indexPath = 0;

    // Recursive functions for DFS
    printPathsDFS(source, destination, visited, path, indexPath);
}

/// <summary>
///  Given a source, a destination and a directed graph, print all their paths
/// </summary>
/// <param name="source"> Source node </param>
/// <param name="destination"> Destintion node </param>
void printPathSourceDestination(int source, int destination)
{
    // Display used cycle graph example
    int edges[][2] = { {0,3},
                       {1,0},
                       {2,5},
                       {3,1},
                       {4,1},
                       {4,2},
                       {4,6},
                       {6,7},
                       {7,3},
                       {7,4} };

    Graph g(colNumber);

    cout << "The following directed graph example with a cycle is used:" << endl;
    for (int i = 0; i < size(edges); i++) {
        cout << edges[i][0] << " " << edges[i][1] << endl;
        g.addDirectedEdge(edges[i][0], edges[i][1]);     //Saves edge into graph
    }

    cout << "Following are all different paths from " << source << " to " << destination << endl;
    g.printPaths(source, destination);

    cout << "\nPress [Enter] to continue..." << endl;
    cin.get();    // used to stop console from closing

}
#pragma endregion

#pragma region Mother Vertex

/// <summary>
///  Given a node and boolean vector, checks if all other nodes can be reached
/// </summary>
/// <param name="v"> Node to be checked </param>
/// <param name="visited"> Visited node array </param>
void Graph::motherVecterDFS(int v, vector<bool>& visited)
{
    visited[v] = true;

    list<int>::iterator i;
    for (i = adj[v].begin(); i != adj[v].end(); ++i)
        if (!visited[*i])
            motherVecterDFS(*i, visited);
}

/// <summary>
///  Find and prints mother vertices of a graph
/// </summary>
void Graph::findMother()
{   
    vector <bool> visited(V, false);  // Visited node vector
    vector <int> mother;              // Vector to store mother vertices
    bool isMother;                       

    // Loop through all vertices
    for (int i = 0; i < V; i++)
    {
        fill(visited.begin(), visited.end(), false);  //Initiate visited as purely false
        motherVecterDFS(i, visited);     // Recursive DFS
        isMother = true;
        for (int j = 0; j < V; j++)   // Check is vertex is mother
            if (visited[j] == false)
                isMother = false;

        if (isMother) {          // Add mother vertex
            mother.push_back(i);
        }
    }

    // Prints mother vertices
    cout << "Number of mother vertices: " << mother.size() << endl;
    for (int i = 0; i < mother.size(); i++)
        cout << mother[i] << " ";
    cout << endl;
}

/// <summary>
///  Prints mother vertices of a graph
/// </summary>
void motherVertex()
{
    // Display used cycle graph example
    int edges[][2] = { {0,3},
                       {1,0},
                       {2,5},
                       {3,1},
                       {4,1},
                       {4,2},
                       {4,6},
                       {6,7},
                       {7,4} };

    Graph g(colNumber);

    cout << "The following directed graph example with a cycle is used:" << endl;
    for (int i = 0; i < size(edges); i++) {
        cout << edges[i][0] << " " << edges[i][1] << endl;
        g.addDirectedEdge(edges[i][0], edges[i][1]);     //Saves edge into graph
    }

    g.findMother();

    cout << "\nPress [Enter] to continue..." << endl;
    cin.get();    // used to stop console from closing
}

#pragma endregion

#pragma region Longest Path in Graph

/// <summary>
///  Recursive DFS to identify the longest path
/// </summary>
/// <param name="v"> Node to be checked </param>
/// <param name="parent"> Specified parent of the node </param>
/// <param name="visvisitedited"> Visited node array </param>
void Graph::longestPathDFS(int v, int longestPathPerNode[], bool visited[])
{
    // Mark as visited
    visited[v] = true;

    list<int>::iterator i;
    for (i = adj[v].begin(); i != adj[v].end(); ++i) {

        if (!visited[*i])
            longestPathDFS(*i, longestPathPerNode, visited);

        // Store the longest paths
        longestPathPerNode[v] = max(longestPathPerNode[v], 1 + longestPathPerNode[*i]);
    }
}

/// <summary>
///  Identifies the longest path size per node and prints the longest one of the graph
/// </summary>
void longestPath()
{
    // Display used cycle graph example
    int edges[][2] = { {0,2},
                       {0,3},
                       {0,6},
                       {1,0},
                       {3,4},
                       {5,1},
                       {6,7} };

    Graph g(colNumber);

    cout << "The following directed graph example with a cycle is used:" << endl;
    for (int i = 0; i < size(edges); i++) {
        cout << edges[i][0] << " " << edges[i][1] << endl;
        g.addDirectedEdge(edges[i][0], edges[i][1]);     //Saves edge into graph
    }
    // Create an array to store the longest path per node and initialize it with 0s
    int longestPathPerNode[rowNumber];               
    memset(longestPathPerNode, 0, sizeof longestPathPerNode);

    bool visitedArray[rowNumber];                    // Create and initialize visited array in FALSE
    memset(visitedArray, false, sizeof visitedArray);

    // Recursive DFS for longest path
    for (int i = 0; i < rowNumber; i++) {
        if (!visitedArray[i])
            g.longestPathDFS(i, longestPathPerNode, visitedArray);
    }

    int longestPathGraph = 0;

    // Loop to identify the longest path from all of them
    for (int i = 1; i <= rowNumber; i++) {
        longestPathGraph = max(longestPathGraph, longestPathPerNode[i]);
    }
    cout << "The longest path in the graph is: " << longestPathGraph;

    cout << "\nPress [Enter] to continue..." << endl;
    cin.get();    // used to stop console from closing
}

#pragma endregion

#pragma region Detect Cycle in Directed Graph

/// <summary>
///  Recursive check for cycles in a graph
/// </summary>
/// <param name="v"> Node to be checked </param>
/// <param name="visited"> Visited node array </param>
/// <param name="parent"> Specified parent of the node </param>
bool Graph::isCycleRecursiveCheck(int v, bool visited[], int parent)
{
    // Update visited node
    visited[v] = true;

    // List iterator
    list<int>::iterator i;

    // Loop for adjacent nodes list
    for (i = adj[v].begin(); i != adj[v].end(); ++i){
        if (!visited[*i]){
            if (isCycleRecursiveCheck(*i, visited, v))
                return true;
        }
        else if (*i != parent)
            return true;
    }
    return false;
}

/// <summary>
///  Given a directed graph, identifies if there is a cycle
/// </summary>
bool Graph::isCyclic()
{
    // Bool pointer array for visited nodes
    bool* visited = new bool[V];

    // Start with 0 visited nodes
    for (int v = 0; v < V; v++)
        visited[v] = false;

    for (int v = 0; v < V; v++){
        if (!visited[v])
            // Initialize recursive check with parent -1
            if (isCycleRecursiveCheck(v, visited, -1))
                return true;
    }
    return false;
}

/// <summary>
///  Given a directed graph, identifies if there is a cycle
/// </summary>
/// <param name="matrixTransitiveClosure"> Transitive closure matrix </param>
void detectCycleDirectedGraph()
{
    // Display used cycle graph example
    int edgesA[][2] = {{0,2},
                       {0,3},
                       {0,6},
                       {1,0},
                       {2,5},
                       {3,4},
                       {4,2},
                       {5,1},
                       {6,7}};

    Graph gA(colNumber);

    cout << "The following directed graph example with a cycle is used:" << endl;
    for (int i = 0; i < size(edgesA); i++){
         cout << edgesA[i][0] << " " << edgesA[i][1] <<endl;
         gA.addDirectedEdge(edgesA[i][0], edgesA[i][1]);     //Saves edge into graph
    }

    // Conditional operator (if else)
    gA.isCyclic() ?
        cout << "Graph contains cycle\n" << endl :
        cout << "Graph doesn't contain cycle\n" << endl;

    // Display used cycle graph example
    int edgesB[][2] = {{0,1},
                       {0,4},
                       {1,2},
                       {2,3},
                       {2,5},
                       {1,6},
                       {6,7} };
    
    Graph gB(colNumber);

    cout << "The following directed graph example with NO cycle is used:" << endl;
    for (int i = 0; i < size(edgesB); i++) {
        cout << edgesB[i][0] << " " << edgesB[i][1] << endl;
        gB.addDirectedEdge(edgesB[i][0], edgesB[i][1]);     //Saves edge into graph
    }

    // Conditional operator (if else)
    gB.isCyclic() ?
        cout << "Graph contains cycle\n" << endl :
        cout << "Graph doesn't contain cycle\n" << endl;

    cout << "\nPress [Enter] to continue..." << endl;
    cin.get();    // used to stop console from closing
}

#pragma endregion

#pragma region Transitive Closure

/// <summary>
///  Given an adjacency matrix, uses Floyd Warshall algorithm to identify the transitive closure
/// </summary>
/// <param name="matrixTransitiveClosure"> Transitive closure matrix </param>
void transitiveClosure(int adjacencyMatrix[][colNumber])
{

    //Note: code is formulated for square adjacency matrices
    //Applies Floyd Warshall algorithm
    for (int k = 0; k < rowNumber; k++) {
        for (int i = 0; i < rowNumber; i++) {
            for (int j = 0; j < colNumber; j++) {
                adjacencyMatrix[i][j] = adjacencyMatrix[i][j] | (adjacencyMatrix[i][k] & adjacencyMatrix[k][j]);
            }
        }
    }

    cout << "Its transitive closure is:" << endl;;
    for (int i = 0; i < rowNumber; i++) {
        for (int j = 0; j < colNumber; j++) {
            if (i == j)
                cout << "1 ";
            else
                cout << adjacencyMatrix[i][j] << " ";
        }
        cout << endl;
    }
}



/// <summary>
///  For a specified adjacency matrix, the transitive closure is identified
/// </summary>
void transitiveClosure()
{
    int adjacencyMatrix[][colNumber] = { { 0, 1, 0, 0, 0, 0, 0, 1 },
                                           { 0, 0, 0, 0, 0, 0, 0, 1 },
                                           { 0, 0, 1, 0, 0, 0, 0, 0 },
                                           { 1, 0, 0, 0, 0, 0, 1, 1 },
                                           { 1, 0, 0, 0, 1, 0, 0 ,0 },
                                           { 0, 0, 1, 0, 0, 0, 0, 0 },
                                           { 0, 0, 0, 0, 0, 0, 1, 1 },
                                           { 0, 0, 0, 0, 0, 0, 0 ,1 }, };

    // Display used 2D matrix
    cout << "The following adjacency matrix is used:" << endl;
    for (int i = 0; i < rowNumber; ++i) {
        for (int j = 0; j < colNumber; ++j) {
            cout << adjacencyMatrix[i][j] << " ";
        } cout << endl;
    }

    // Identify the transitive closure
    transitiveClosure(adjacencyMatrix);

    cout << "\nPress [Enter] to continue..." << endl;
    cin.get();    // used to stop console from closing
}

#pragma endregion

#pragma region Number of Islands

/// <summary>
///  For a specified cell, applies DFS to the eight adjacent cells in a 2D matrix 
/// </summary>
/// <param name="unsArray"> Unsorted array </param>
/// <param name="n"> Size of array </param>
/// </return> Returns 0 or 1 depending if DFS should be applied to a specified negihbor cell
int includeDFS(int matrix[][colNumber], bool visitedMatrix[][colNumber], int neighborRow, int neighborCol)
{
    // Checks if neighbor cells is within 2D matrix boundaries and if it has a 1 value and has not been visited
    return (neighborRow >= 0) && (neighborRow < rowNumber) && (neighborCol >= 0) && (neighborCol < colNumber) && (matrix[neighborRow][neighborCol] && !visitedMatrix[neighborRow][neighborCol]);
}

/// <summary>
///  For a specified cell, applies DFS to the eight adjacent cells in a 2D matrix 
/// </summary>
/// <param name="unsArray"> Unsorted array </param>
/// <param name="n"> Size of array </param>
void DFS(int matrix[][colNumber], bool visitedMatrix[][colNumber], int cellRow, int cellCol)
{
    // Auxliary array to define the eight neiboring positions
    static int neighborRow[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
    static int neighborCol[] = { -1, 0, 1, -1, 1, -1, 0, 1 };

    // Cell has now been visited
    visitedMatrix[cellRow][cellCol] = true;

    // Apply DFS recursively for all eight neighbors
    for (int n = 0; n < 8; ++n) {
        // Checks if DFS needs to applied to the neighboring cell
        if (includeDFS(matrix, visitedMatrix, cellRow + neighborRow[n], cellCol + neighborCol[n]))
            // Apply DFS recursively
            DFS(matrix, visitedMatrix, cellRow + neighborRow[n], cellCol + neighborCol[n]);
    }
}

/// <summary>
///  Given a 2D binary matrix, identifies the number of islands
/// </summary>
void islandNumber() {

    //Input data
    int matrix[][colNumber] = {  { 0, 1, 0, 0, 0, 1, 0, 1 },
                                 { 1, 0, 0, 0, 0, 0, 0, 1 },
                                 { 0, 0, 1, 1, 1, 0, 0, 0 }, 
                                 { 1, 0, 0, 0, 0, 0, 1, 1 },
                                 { 1, 1, 1, 0, 1, 0, 0 ,1 },
                                 { 0, 0, 1, 1, 1, 0, 0, 0 },
                                 { 1, 0, 0, 0, 0, 0, 1, 1 },
                                 { 1, 1, 1, 0, 1, 0, 0 ,1 },};

    // Display used 2D matrix
    cout << "The used 2D binary matrix is as follows:" << endl;
    for (int i = 0; i < rowNumber; ++i) {
        for (int j = 0; j < colNumber; ++j) {
            cout << matrix[i][j] << " ";
        } cout << endl;
    }

    // Auxiliary matrix to track visited cells
    bool visitedMatrix[rowNumber][colNumber];
    
    // Initial fill as unvisited cells with only 0s
    memset(visitedMatrix, 0, sizeof(visitedMatrix));

    // Island counter
    int islandNumber = 0;

    // Loop through every cell in 2D matrix
    for (int i = 0; i < rowNumber; ++i) {
        for (int j = 0; j < colNumber; ++j) {
            // If matrix cell has a value of 1 and has not been visited yet
            if (matrix[i][j] && !visitedMatrix[i][j]) {
                // Call depth fisrt search
                DFS(matrix, visitedMatrix, i, j);
                ++islandNumber;
            }
        }
    }
    cout << "Identified number of islands: " << islandNumber << endl;

    cout << "\nPress [Enter] to continue..." << endl;
    cin.get();    // used to stop console from closing
}

#pragma endregion

int main() {

	AuthorDetails();	// calls AuthorDetails method/function 

	cout << endl;
	cout << "------------------ DFS Examples ---------------------------" << endl;
	cout << " Just uncomment the function in Main for the desired problem:" << endl;
    cout << endl;

    cout << " - Graph DFS from a given node" << endl;
    graphDFS(0);
    cout << endl;

    cout << " - Print all paths for a given source and destination" << endl;
    printPathSourceDestination(4,3);
    cout << endl;

    cout << " - Mother vertices" << endl;
    motherVertex();
    cout << endl;

    cout << " - Identify the longest path in a graph" << endl;
    longestPath();
    cout << endl;

    cout << " - Detect cycle in an directed graph" << endl;
    detectCycleDirectedGraph();
    cout << endl;

    cout << " - Transitive closure for a directed graph (adjacency matrix)" << endl;
    transitiveClosure();
    cout << endl;

	cout << " - Idenify the number of islands (1's) in a binary 2D matrix" << endl;
    islandNumber();
	cout << endl;

    return 0;
}







