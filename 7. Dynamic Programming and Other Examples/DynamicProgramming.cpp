// -------------------------------------------------------------- -*- C++ -*-
// File: Example_Walmart_DynamicProgramming.cpp
// --------------------------------------------------------------------------
// Developed by Daniel Zuniga
// Date: 09/19/2021
// Version 1.0.0
//
// C++ Version: Dynamic Programming and Other Examples
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

ILOSTLBEGIN // IBM macro to allow run the application in c++ with STL (Standard Template Library)

// Creates AuthorDetials method, (void: no value returned)
void AuthorDetails() {
	cout << "--------------------------------------------------------" << endl;
	cout << "File: Example_Walmart_DynamicProgramming.cpp" << endl;
	cout << "--------------------------------------------------------" << endl;
	cout << "Developed by Daniel Zuniga" << endl;
	cout << "Date: 09/19/2021" << endl;
	cout << "Version 1.0.0\n" << endl;
	cout << "C++ Version: Dynamic Programming and Other Examples" << endl;
	cout << "--------------------------------------------------------\n" << endl;

} // End AuthorDetails

// Creates MinimumDominatingSet method, (void: no value returned)
void MinimumDominatingSet() {

	int i, j, k;
	const int num_node = 14;           //number of nodes
	int num_node_AUX, num_edge;        //number of edges


	//--------------------------------------------------------------
	//--------------------read input data file ---------------------
	std::ifstream iData("MinimumDominatingSet_Data.txt");

	assert(iData);

	iData >> num_node_AUX;  //read the number of nodes
	iData >> num_edge; //read the number of edges

	//construct a adjacent matrix A=(a_ij)_{num_node * num_node}
	//a_ii=1, a_ij=1 if a line exists and a_ji=a_ij
	int a_ij[num_node + 1][num_node + 1];
	for (i = 1; i <= num_node; i++) {
		for (j = 1; j <= num_node; j++) {
			a_ij[i][j] = 0;
		}
	}

	while (!iData.eof()) //read till end of data
	{
		iData >> i;
		iData >> j;
		a_ij[i][j] = 1;
		a_ij[j][i] = 1;
	}
	iData.close();

	cout << "------------------------------- CHECK INPUT DATA  ---------------------------------------" << endl;
	cout << "Check the input of data:" << endl;
	cout << "The number of nodes: " << num_node << endl;
	cout << "The number edges: " << num_edge << endl;
	cout << "The adjancency matrix: " << endl;
	for (i = 1; i <= num_node; i++) {
		for (j = 1; j <= num_node; j++) {
			cout << " " << a_ij[i][j];
		}cout << endl;
	}

	cout << "\nLoading CPLEX solver...\n" << endl;

	//create the CPLEX variable
	typedef IloArray<IloBoolVarArray> IloBoolVarArray2;

	//Create the CPLEX environement 	
	IloEnv myenv;

	//create the model for MDS
	IloModel mycplex(myenv); //model for dominating set problem

	IloBoolVarArray x_i(myenv, num_node + 1); // decision variable x_i[i]

	IloExpr OBJ(myenv);
	for (i = 1; i <= num_node; i++) {
		OBJ += x_i[i];
	}
	mycplex.add(IloMinimize(myenv, OBJ)); //add the objective function to the model
	OBJ.end();

	for (i = 1; i <= num_node; i++) {
		IloExpr CSTR_1(myenv);
		for (j = 1; j <= num_node; j++) {
			CSTR_1 += a_ij[i][j] * x_i[j];
		}
		CSTR_1 += x_i[i];
		mycplex.add(CSTR_1 >= 1); //add the constraint 1 for vertex i
		CSTR_1.end();
	}

	IloCplex cplexModel(mycplex);					  // Create Cplex model based on IloModel

	cplexModel.exportModel("MinimumDominatingSet_CPP.lp");       // Export model to lp format
	cplexModel.exportModel("MinimumDominatingSet_CPP.mps");       // Export model to mps format

	IloBool success = false;
	IloAlgorithm::Status status;

	try {

		//set gap parameters
		cplexModel.setParam(IloCplex::EpGap, 0.01);		//gap for model
		cplexModel.setParam(IloCplex::TiLim, 3600);		//limits time in seconds and returns best solution so far


		float starttime;			// Auxiliary term to calculate computational time

		starttime = cplexModel.getTime();
		success = cplexModel.solve();		//solve MDS
		starttime = cplexModel.getTime() - starttime;

		if (success) {
			cout << "------------------------- PRINTING SOLUTIONS ---------------------------------" << endl;
			cout << "Problem Status: " << cplexModel.getStatus() << endl;;
			cout << "The min obj for MDS:   " << cplexModel.getObjValue() << endl;
			cout << "The CPU seconds for MDS:   " << starttime << endl;
			cout << "Minimum Dominating Set includes the vertices: ";
			for (i = 1; i <= num_node; i++) {
				if (cplexModel.getValue(x_i[i]) > 0.5)
					cout << i << " ";
			}
			cout << endl;

			status = cplexModel.getStatus();
		}

		else
		{
			cout << "Problem Status: " << cplexModel.getStatus() << endl;
		}

	}//end of try

	catch (IloException& e) {}

} // End MinimumDominatingSet

// Creates OptimumJobAssignment method, (void: no value returned)
void OptimumJobAssignment() {


	const int I = 5;   // Assignes Set of Factories
	const int J = 5;   // Assignes Set of Products 

	int i, j;		// indexes for factories and products respectively

	//--------------------------------------------------------------
	//--------------------read input data file ---------------------
	std::ifstream iData("OptimalJobAssignment_Data.txt");

	assert(iData);

	int I_AUX, J_AUX; // Auxiliary variables to store sets
	iData >> I_AUX;  //read the number of nodes
	iData >> J_AUX; //read the number of edges

	// a_ij[i][j]: Production rate of product j at factory i (product/time)
	int a_ij[I][J];
	for (i = 0; i < I; i++) {
		for (j = 0; j < J; j++) {
			iData >> a_ij[i][j];
		}
	}

	iData.close();

	cout << "------------------------------- CHECK INPUT DATA  ---------------------------------------" << endl;
	cout << "Number of factories: " << I << endl;
	cout << "Number of products: " << J << endl;
	cout << "Production rate of factory i for product j: " << endl;
	for (i = 0; i < I; i++) {
		for (j = 0; j < J; j++) {
			cout << " " << a_ij[i][j];
		}cout << endl;
	}

	cout << "\nLoading CPLEX solver...\n" << endl;

	//create the CPLEX variable
	typedef IloArray<IloBoolVarArray> IloBoolVarArray2;
	typedef IloArray<IloBoolVarArray2> IloBoolVarArray3;

	//Create the CPLEX environement 	
	IloEnv myenv;

	//create the model for MDS
	IloModel mycplex(myenv); //model for dominating set problem

	IloBoolVarArray2 x_ij(myenv, I);			  // x_ij[i][j]: binary variable array for product j assignment to factory i
	for (i = 0; i < I; i++) {
		x_ij[i] = IloBoolVarArray(myenv, J);
	}

	// Declare objective function
	IloExpr OBJ(myenv);
	for (i = 0; i < I; i++)
		for (j = 0; j < J; j++)
			OBJ += a_ij[i][j] * x_ij[i][j];

	mycplex.add(IloMaximize(myenv, OBJ)); //add the objective function to the model
	OBJ.end(); // Clears Expression

	// **********************************************************
	// Constraint (2b): each factory i can only produce one product j 
	for (i = 0; i < I; i++)
	{
		IloExpr CSTR_2B(myenv);
		for (j = 0; j < J; j++)
			CSTR_2B += x_ij[i][j];
		mycplex.add(CSTR_2B == 1);
		CSTR_2B.end();    // Clears Expression
	}


	// Constraint (2c): each factory i can only produce one product j 
	for (j = 0; j < J; j++)
	{
		IloExpr CSTR_2C(myenv);
		for (i = 0; i < I; i++)
			CSTR_2C += x_ij[i][j];
		mycplex.add(CSTR_2C == 1);
		CSTR_2C.end();    // Clears Expression
	}

	IloCplex cplexModel(mycplex);					  // Create Cplex model based on IloModel

	cplexModel.exportModel("OptimumJobAssignment_CPP.lp");       // Export model to lp format
	cplexModel.exportModel("OptimumJobAssignment_CPP.mps");       // Export model to mps format

	IloBool success = false;
	IloAlgorithm::Status status;

	try {

		//set cplex parameters
		cplexModel.setParam(IloCplex::EpGap, 0.01);		//gap for model
		cplexModel.setParam(IloCplex::TiLim, 3600);		//limits time in seconds and returns best solution so far

		float starttime;			// Auxiliary term to calculate computational time

		starttime = cplexModel.getTime();
		success = cplexModel.solve();		//solve model
		starttime = cplexModel.getTime() - starttime;

		if (success) {
			cout << "------------------------- PRINTING SOLUTIONS ---------------------------------" << endl;
			cout << "Problem Status: " << cplexModel.getStatus() << endl;;
			cout << "The optimal objective is: " << cplexModel.getObjValue() << endl;
			cout << "CPU time in seconds is: " << starttime << endl;
			cout << "\nFactory / Product assignment is as follows: " << endl;
			for (i = 0; i < I; i++)
			{
				for (j = 0; j < J; j++)
					cout << cplexModel.getValue(x_ij[i][j]) << " ";
				cout << endl;
			}
			cout << endl;

		}

		else
		{
			cout << "Problem Status: " << cplexModel.getStatus() << endl;
		}

	}//end of try

	catch (IloException& e) {}

} // End OptimumJobAssignment

// Creates KnapsackProblem method, (void: no value returned)
void KnapsackProblem() {

	const int I = 5;   // Assignes Set of Products

	int i;		// indexes for products

	//--------------------------------------------------------------
	//--------------------read input data file ---------------------
	std::ifstream iData("01KnapsackProblem_Data.txt");

	assert(iData);

	int I_AUX; // Auxiliary variables to store sets
	iData >> I_AUX;  //read the number of nodes

	int W; // Bag's weight limit (kg)
	iData >> W;  //read the number of nodes

	int w_i[I]; // w_i[i]: Weight of product i (kg)
	int v_i[I]; // v_i[i]: Value of product i ($)

	for (i = 0; i < I; i++) {
		iData >> w_i[i];		// Read w_i[i]: Weight of product i (kg)
	}

	for (i = 0; i < I; i++) {
		iData >> v_i[i];		// v_i[i]: Value of product i ($)
	}

	iData.close();

	cout << "------------------------------- CHECK INPUT DATA  ---------------------------------------" << endl;
	cout << "Number of products: " << I << endl;
	cout << "Bag weight limit: " << W << endl;
	cout << "Weight of product i: " << endl;
	for (i = 0; i < I; i++) {
		cout << w_i[i] << ",";
	}cout << endl;

	cout << "Value of product i: " << endl;
	for (i = 0; i < I; i++) {
		cout << v_i[i] << ",";
	}cout << endl;

	cout << "\nLoading CPLEX solver...\n" << endl;

	//create the CPLEX variable
	typedef IloArray<IloBoolVarArray> IloBoolVarArray2;

	//Create the CPLEX environement 	
	IloEnv myenv;

	//create the model for MDS
	IloModel mycplex(myenv); //model for dominating set problem

	IloBoolVarArray x_i(myenv, I);		 // x_i[i]: binary variable array product selection

	// Declare objective function
	IloExpr OBJ(myenv);
	for (i = 0; i < I; i++)
		OBJ += v_i[i] * x_i[i];

	mycplex.add(IloMaximize(myenv, OBJ)); //add the objective function to the model
	OBJ.end(); // Clears Expression

	// **********************************************************
	// Constraint (3b): weight limit
	IloExpr CSTR_3B(myenv);
	for (i = 0; i < I; i++)
	{
		CSTR_3B += w_i[i] * x_i[i];
	}
	mycplex.add(CSTR_3B <= W);
	CSTR_3B.end();    // Clears Expression


	IloCplex cplexModel(mycplex);					  // Create Cplex model based on IloModel

	cplexModel.exportModel("01KnapsackPoblem_CPP.lp");       // Export model to lp format
	cplexModel.exportModel("01KnapsackPoblem_CPP.mps");       // Export model to mps format

	IloBool success = false;
	IloAlgorithm::Status status;

	try {

		//set cplex parameters
		cplexModel.setParam(IloCplex::EpGap, 0.01);		//gap for model
		cplexModel.setParam(IloCplex::TiLim, 3600);		//limits time in seconds and returns best solution so far

		float starttime;			// Auxiliary term to calculate computational time

		starttime = cplexModel.getTime();
		success = cplexModel.solve();		//solve model
		starttime = cplexModel.getTime() - starttime;

		if (success) {
			cout << "------------------------- PRINTING SOLUTIONS ---------------------------------" << endl;
			cout << "Problem Status: " << cplexModel.getStatus() << endl;;
			cout << "The optimal objective is: " << cplexModel.getObjValue() << endl;
			cout << "CPU time in seconds is: " << starttime << endl;
			cout << "\nProduct selection is as follows: " << endl;
			for (i = 0; i < I; i++)
				cout << cplexModel.getValue(x_i[i]) << ",";

		}

		else
		{
			cout << "Problem Status: " << cplexModel.getStatus() << endl;
		}

	}//end of try

	catch (IloException& e) {}

} // End KnapsackProblem

// Creates KnapsackProblemDynamicProgramming method, (void: no value returned)
void KnapsackProblemDynamicProgramming() {

	const int I = 4;   // Assignes Set of Products
	const int W = 5;   // Assignes Set of Products

	int i, w;		// indexes for products and auxiliary index for weights

	//--------------------------------------------------------------
	//--------------------read input data file ---------------------
	std::ifstream iData("01KnapsackProblemDynamicProgramming_Data.txt");

	assert(iData);

	int I_AUX; // Auxiliary variables to store sets
	iData >> I_AUX;  //read the number of nodes

	int W_AUX; // Bag's weight limit (kg)    NOT USED
	iData >> W_AUX;  //read Bag's weight limit (kg)  NOT USED

	int w_i[I + 1]; // w_i[i]: Weight of product i (kg)
	int v_i[I + 1]; // v_i[i]: Value of product i ($)

	for (i = 0; i <= I; i++) {
		iData >> w_i[i];		// Read w_i[i]: Weight of product i (kg)
	}

	for (i = 0; i <= I; i++) {
		iData >> v_i[i];		// v_i[i]: Value of product i ($)
	}

	iData.close();

	cout << "------------------------------- CHECK INPUT DATA  ---------------------------------------" << endl;
	cout << "Number of products: " << I << endl;
	cout << "Bag weight limit: " << W << endl;
	cout << "Weight of product i: " << endl;
	for (i = 0; i <= I; i++) {
		cout << w_i[i] << ",";
	}cout << endl;

	cout << "Value of product i: " << endl;
	for (i = 0; i <= I; i++) {
		cout << v_i[i] << ",";
	}cout << endl;


	// Array to store solution values for dynamic programming initiated with 0
	int CostArray[I + 1][W + 1] = { 0 };

	// Loop through the solution array
	for (i = 1; i <= I; i++) {
		for (w = 1; w <= W; w++) {
			if (w_i[i] > w)  CostArray[i][w] = CostArray[i - 1][w];  //if w_i > w, update using F(i-1, w)
			else  //else w_i <= w
			{
				if (v_i[i] + CostArray[i - 1][w - w_i[i]] > CostArray[i - 1][w]) // Determines max {F(i-1,w),(F(i-1,w_wi)+v_i)}
					CostArray[i][w] = v_i[i] + CostArray[i - 1][w - w_i[i]];  // Updates using F(i-1,w_wi)+v_i
				else
					CostArray[i][w] = CostArray[i - 1][w]; // updates using F(i-1,w)
			}
		}
	}

	cout << endl;
	cout << "---------------------- PRINTING SOLUTIONS ------------------------------ " << endl;
	cout << "Optimal objective: " << CostArray[I][W] << endl;
	cout << "Printing cost matrix for product i and weight i: " << endl;
	for (i = 0; i <= I; i++)
	{
		for (w = 0; w <= W; w++)
			cout << CostArray[i][w] << ", ";
		cout << endl;
	}

	cout << "Products i selected: " << endl;;
	i = I;
	w = W;
	while (i > 0 && w > 0)
	{
		if (CostArray[i][w] != CostArray[i - 1][w])
		{
			cout << i << ", ";
			w = w - w_i[i];
			i = i - 1;
		}
		else
		{
			i = i - 1;
		}
	}

} // End KnapsackProblemDynamicProgramming

// Creates RopeCuttingDynamicProgramming method, (void: no value returned)
void RopeCuttingDynamicProgramming() {

	const int C = 5;   // Assignes Set of lengths
	const int N = 5;   // Assignes Set of lengths

	int c, n;		// indexes for cut and auxiliary index for lengths

	//--------------------------------------------------------------
	//--------------------read input data file ---------------------
	std::ifstream iData("RopeCuttingDynamicProgramming_Data.txt");

	assert(iData);

	int C_AUX; // Auxiliary variables to Set of cuts
	iData >> C_AUX;  //read the Set of cuts

	int N_AUX; // Auxiliary variables to Set of lengths
	iData >> N_AUX;  //read the Set of lengths

	int l_c[C + 1]; // l_c[c]: Length of cut (m)
	int p_c[C + 1]; // p_c[c]: Sale price of cut ($)

	for (c = 0; c <= C; c++) {
		iData >> l_c[c];		// Read l_c[c]: Length of cut c (m)
	}

	for (c = 0; c <= C; c++) {
		iData >> p_c[c];		// Read p_c[c]: Price of cut c ($)
	}

	iData.close();

	cout << "------------------------------- CHECK INPUT DATA  ---------------------------------------" << endl;
	cout << "Number of cuts: " << C << endl;
	cout << "Length of rope: " << N << endl;
	cout << "Length of cut l_c / Price of cut p_c: " << endl;
	for (c = 0; c <= C; c++) {
		cout << l_c[c] << ",";
	}cout << endl;
	for (c = 0; c <= C; c++) {
		cout << p_c[c] << ",";
	}cout << endl;

	// Array to store values for dynamic programming
	static int INF = 100000;
	static int r_n[N + 1];

	// Initialize the r_n array with 0 and a sufficient low number for the rest of the array -INF
	r_n[0] = 0;
	for (n = 1; n <= N; n++)
		r_n[n] = -1 * INF;

	// Create a structure to have a method defined within a method
	// and recursively call it
	struct DP {
	public: static int topDownRopeCutting(int p_c[], int N) {

		// Since r_n is initalized as -INF, here we check if the solution is already stored
		if (r_n[N] >= 0) return r_n[N];

		int maximumRevenue = -1 * INF;    // Initialize maximum solution -INF
		int n;						      // Auxiliary counter (needs to be declared within the local method)

		// Recursive loop to compare current maximumRevenue with updated one, note it is called recursively 
		for (n = 1; n <= N; n++) {
			if (maximumRevenue < p_c[n] + topDownRopeCutting(p_c, N - n))   // recursive calling of method
				maximumRevenue = p_c[n] + topDownRopeCutting(p_c, N - n);
		}

		r_n[N] = maximumRevenue;    // Assigns maximum revenue identified
		return r_n[N];              //returns maximum revenue
	}
	};

	// Calls method for dynamic programing
	DP::topDownRopeCutting(p_c, N);

	cout << endl;
	cout << "---------------------- PRINTING SOLUTIONS ------------------------------ " << endl;
	cout << "Optimal objective: " << r_n[N] << endl;
	cout << "Printing revenue array r_n: " << endl;
	for (n = 0; n <= N; n++) {
		cout << r_n[n] << ", ";
	}cout << endl;

} // End RopeCuttingDynamicProgramming

// Creates CoinChangeDynamicProgramming method, (void: no value returned)
void CoinChangeDynamicProgramming() {

	const int C = 4;   // Assignes the Set of coins
	const int A = 38;  // Assignes the Value to sum with coins
	const int N = A + 1;	// Assignes Set of remaining sum to be achieved

	int c, n;		// indexes for cut and auxiliary index for lengths

	//--------------------------------------------------------------
	//--------------------read input data file ---------------------
	std::ifstream iData("CoinChangeDynamicProgramming_Data.txt");

	assert(iData);

	int C_AUX; // Auxiliary variables to Set of coins
	iData >> C_AUX;  //read the Set of coins

	int A_AUX; // Auxiliary variables for the Value to sum with coins
	iData >> A_AUX;  //read the Value to sum with coins



	int v_c[C + 1];  // v_c[c]: value of coin c ($)

	for (c = 0; c <= C; c++) {
		iData >> v_c[c];		// Read l_c[c]: Length of cut c (m)
	}

	iData.close();

	cout << "------------------------------- CHECK INPUT DATA  ---------------------------------------" << endl;
	cout << "Number of coins: " << C << endl;
	cout << "Number to sum with coins: " << A << endl;
	cout << "Value of coin v_c: " << endl;
	for (c = 0; c <= C; c++) {
		cout << v_c[c] << ",";
	}cout << endl;

	// Array to store values for dynamic programming
	static int INF = 100000;
	static int r_n[N + 1];

	// Array to store the maximum coin able to be included in the remaining sum
	int M_n[N];
	M_n[0] = 0;

	// Array to store the selected coin
	int S_n[N];
	S_n[0] = 0;

	// Loop through the remaining amount to be sumed
	for (n = 1; n < N; n++) {

		// Set minimum to -INF and coin to 0
		int minimum = INF;
		int coin = 0;

		// Loop to identify the greatest value coin identified to be sumed without exceeding the sum Limit A
		for (c = 1; c <= C; c++) {

			// if remaining sum is greater than the value coin
			if (n >= v_c[c]) {

				//if a greater value coin is identified without exceeding the remaining sum 
				if (minimum > 1 + M_n[n - v_c[c]]) {
					minimum = 1 + M_n[n - v_c[c]];    // update greatest value coin
					coin = c;                         // update coin number
				}
			}
		}

		// Saves greatest value coin and coin number
		M_n[n] = minimum;
		S_n[n] = coin;
	}

	cout << endl;
	cout << "---------------------- PRINTING SOLUTIONS ------------------------------ " << endl;
	cout << "Optimal objective (minimum number of coins): " << M_n[A] << endl;
	cout << "Printing coin array M_n: " << endl;
	for (n = 0; n < N; n++) {
		cout << M_n[n] << ",";
	}cout << endl;

	cout << "Printing selected coins: " << endl;
	// Printing sum goes backwards, i.e., last element of array will always be in the solution,
	// and finds next equal or greater coin value to print
	int Aux = v_c[S_n[A]];  // Auxiliary term
	for (n = A; n >= 0; n--) {
		if (v_c[S_n[n]] >= Aux) {
			cout << v_c[S_n[n]] << ",";
			Aux = v_c[S_n[n]];
		}
	}

} // End CoinChangeDynamicProgramming


// Creates ShortestPathDijkstra method, (void: no value returned)
void ShortestPathDijkstra() {

	const int N = 10;   // Assignes the Set of nodes
	const int src = 0;  // Assignes inital node (source)

	int n, i, j;		// indexes for nodes

	//--------------------------------------------------------------
	//--------------------read input data file ---------------------
	std::ifstream iData("ShortestPathDijkstraDP_Data.txt");

	assert(iData);

	int N_AUX; // Auxiliary variables to Set of nodes
	iData >> N_AUX;  //read the Set of coins

	int src_AUX; // Auxiliary variables to inital node
	iData >> src_AUX;  //read the initial node

	int d_ij[N][N];  // d_ij[i][j]: distance between node i and j (m)

	for (i = 0; i < N; i++)
		for (j = 0; j < N; j++)
			iData >> d_ij[i][j];		// Read d_ij[i][j]: distance between node i and j (m)

	iData.close();

	cout << "------------------------------- CHECK INPUT DATA  ---------------------------------------" << endl;
	cout << "Number of nodes: " << N << endl;
	cout << "Distance between node i and j: " << endl;
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++)
			cout << d_ij[i][j] << ", ";
		cout << endl;
	}

	cout << endl;

	// Stores the minimum distance from the inital node i to node j array 
	int dist[N];

	// Bool array set to 1 if node i is in the shortest path
	bool NodeSel[N];

	// Initialize all distances as INF and selection of node as false
	for (int j = 0; j < N; j++)
		dist[j] = INT_MAX, NodeSel[j] = false;

	// Initial node distance (source)
	dist[src] = 0;

	// Loop for all other remaining nodes (N-1)
	for (int n = 0; n < N - 1; n++) {
		// Pick the minimum distance vertex from the set of vertices not
		// yet processed. i is always equal to src in the first iteration.

		// Create auxiliary variables to store minimum distance and its node 
		int min_dist = INT_MAX, min_node;

		// Loop for all nodes j
		for (int j = 0; j < N; j++)
			if (NodeSel[j] == false && dist[j] <= min_dist)  // Checks if there is an unassigned node with minimum distance
				min_dist = dist[j], min_node = j; // Update bool status and minimum distance node

		int i = min_node; // For ease of notation, reassignes minimum distance node to counter i

		// Marks picked node as true
		NodeSel[i] = true;

		// Update the distance value for nodes adjacent to the selected one
		for (int j = 0; j < N; j++)
			// Update dist[j] only if :
			// - is not selected in NodeSel, 
			// - there is an edge from node i to node j, 
			// - total distance from to node j through i is smaller than current dist[j]

			if (!NodeSel[j] && d_ij[i][j] && dist[i] != INT_MAX && dist[i] + d_ij[i][j] < dist[j])
				dist[j] = dist[i] + d_ij[i][j];
	}

	cout << endl;
	cout << "---------------------- PRINTING SOLUTIONS ------------------------------ " << endl;
	cout << "Optimal objective (minimum distance): " << dist[N - 1] << endl;
	cout << endl;
	cout << "Minum distance to node no.: " << endl;
	cout << "      Node   Distance " << endl;
	for (n = 0; n < N; n++) {
		cout << "\t" << n << "\t" << dist[n] << endl;
	}cout << endl;

} // End ShortestPathDijkstra

// Creates Main method, entry point of C++ (int: returns integer)
int main() {

	AuthorDetails();	// calls AuthorDetails method/function 

	cout << endl;
	cout << "------------------ Dynamic Programming and Other Examples ---------------------------" << endl;
	cout << " - 0-1 Knapsack Problem" << endl;
	cout << " - 0-1 Knapsack Problem - Dynamic Programming" << endl;
	cout << " - Rope Cutting - Dynamic Programming" << endl;
	cout << " - Coin Change - Dynamic Programming" << endl;
	cout << " - Shortest Path - Dijkstra's - Dynamic Programming " << endl;
	cout << " - Minimum Dominating Set" << endl;
	cout << " - Optimum Assignment Problem" << endl;

	cout << endl;

	cout << "Press ENTER to see 0-1 Knapsack Problem" << endl;
	cin.get();
	KnapsackProblem();

	cout << endl;
	cout << "Press ENTER to see 0-1 Knapsack Problem - Dynamic Programming" << endl;
	cin.get();
	KnapsackProblemDynamicProgramming();

	cout << endl;
	cout << "Press ENTER to see Rope Cutting - Dynamic Programming" << endl;
	cin.get();
	RopeCuttingDynamicProgramming();

	cout << endl;
	cout << "Press ENTER to see Coin Change - Dynamic Programming" << endl;
	cin.get();
	CoinChangeDynamicProgramming();

	cout << endl;
	cout << "Press ENTER to see Shortest Path - Dijkstra's - Dynamic Programming " << endl;
	cin.get();
	ShortestPathDijkstra();

	cout << endl;
	cout << "Press ENTER to see Minimum Dominating Set" << endl;
	cin.get();
	MinimumDominatingSet();

	cout << endl;
	cout << "Press ENTER to see Optimum Assignment Problem" << endl;
	cin.get();
	OptimumJobAssignment();
}

