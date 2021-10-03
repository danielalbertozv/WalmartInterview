// -------------------------------------------------------------- -*- C++ -*-
// File: FacilityLocation.cpp
// --------------------------------------------------------------------------
// Developed by Daniel Zuniga
// Date: 09/19/2021
// Version 1.0.0
//
// C++ Version: Facility Location
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

ILOSTLBEGIN  // IBM macro to allow run the application in c++ with STL (Standard Template Library)

// Creates AuthorDetials method, (void: no value returned)
void AuthorDetails() {
	cout << "--------------------------------------------------------" << endl;
	cout << "File: FacilityLocation.cpp" << endl;
	cout << "--------------------------------------------------------" << endl;
	cout << "Developed by Daniel Zuniga" << endl;
	cout << "Date: 09/19/2021" << endl;
	cout << "Version 1.0.0\n" << endl;
	cout << "C++ Version: Facility Location" << endl;
	cout << "--------------------------------------------------------\n" << endl;
} // End AuthorDetails

#pragma region Capacitated Facility Location

// Creates Main method, entry point of C++ (int: returns integer)
void capacitatedFacilityLocation()
{
	cout << "-----------Capacitated Facility Location-------------" << endl;

	//Define parameters
	int i, j;		// Indexes
	const int I = 3;		// Set of facilities, cardinality
	const int J = 6;		// Set of customers, cardinality
	double c[I][J] = { { 3, 6, 9, 12, 15, 3},
					   { 12, 5, 4, 5, 12, 2},
					   { 1, 15, 5, 8, 11, 7} }; // Cost of facility i to supply to customer j.
	double f[I] = { 3000, 4000, 5000 };	// Cost of adding facility i.
	double d[J] = { 50, 25, 30, 28, 15, 18 }; // Demand of customer j.
	double u[I] = { 80, 100, 90 }; // Capacity of facility i.

	// Model Definition
	IloEnv myenv; // environment object
	IloModel mycplex(myenv, "CapacitatedFacilityLocation_CPP"); // model object
	mycplex.setName("CapacitatedFacilityLocation_CPP");

	//Create the CPLEX variable
	typedef IloArray<IloNumVarArray> IloNumVarArray2;
	typedef IloArray<IloNumVarArray2> IloNumVarArray3;

	typedef IloArray<IloBoolVarArray> IloBoolVarArray2;
	typedef IloArray<IloBoolVarArray2> IloBoolVarArray3;

	// Variable declaration
	IloBoolVarArray x(myenv, I);			  // x[i]: Binary variables that is 1 if facility i is assigned and 0 otherwise.


	IloNumVarArray2 y(myenv, I);			  // y[i][j]: Fraction of demand supplied from facility i to customer j.
	for (i = 0; i < I; i++) {
		y[i] = IloNumVarArray(myenv, J, 0, IloInfinity, ILOFLOAT);
	}

	//Objective function
	IloExpr OBJ(myenv);
	for (int i = 0; i < I; i++) {
		OBJ += f[i] * x[i];
		for (int j = 0; j < J; j++) {
			OBJ += c[i][j] * d[j] * y[i][j];
		}
	}
	mycplex.add(IloMinimize(myenv, OBJ));    // Add objective function
	OBJ.end();


	// ***********************************
	// Constraint 2.1a - Satisfied fraction of demand
	for (j = 0; j < J; j++) {
		IloExpr CSTR_2_1a(myenv);
		for (i = 0; i < I; i++) {
			CSTR_2_1a += y[i][j];
		}
		mycplex.add(CSTR_2_1a == 1);  // Add constraint 2.1a
		CSTR_2_1a.end();              // Releases memory from Expr
	}

	// Constraint 2.1b - Facility capacity
	for (i = 0; i < I; i++) {
		IloExpr CSTR_2_1b(myenv);
		for (j = 0; j < J; j++) {
			CSTR_2_1b += d[j] * y[i][j];
		}
		CSTR_2_1b += -u[i] * x[i];
		mycplex.add(CSTR_2_1b <= 0);  // Add constraint 2.1b
		CSTR_2_1b.end();              // Releases memory from Expr
	}

	//**********************************************************************
	// Create the Cplex model

	float starttime;		// Auxiliary term to calculate computational time

	IloCplex cplexModel(mycplex);					  // Create Cplex model based on IloModel
	IloBool feasible = false;						  // Auxiliary term to check feasibility
	cplexModel.exportModel("CapacitatedFacilityLocation_CPP.lp");       // Export model to lp format
	cplexModel.exportModel("CapacitatedFacilityLocation_CPP.mps");       // Export model to mps format

	//***********************************************************************
	//***********************************************************************
	cout << "------------------------------------------" << endl;
	cout << "----------- SOLVING A MODEL  -------------" << endl;
	cout << "------------------------------------------" << endl;

	//set gap parameters
	cplexModel.setParam(IloCplex::EpGap, 0.01);		  // optimization gap 
	cplexModel.setParam(IloCplex::TiLim, 21600);      //limits time in seconds and returns best solution so far

	try {
		starttime = cplexModel.getTime();
		feasible = cplexModel.solve();                    // solves model and store true if feasible
		starttime = cplexModel.getTime() - starttime;
	}
	catch (IloException& e) {
		cout << e.getMessage() << endl;
	}


	// Printing the Solution
	if (feasible == IloTrue)
	{
		cout << "\nProblem feasible." << endl;
		cout << "The solution status: " << cplexModel.getStatus() << endl;
		cout << "The objective value is: " << cplexModel.getObjValue() << endl;
		cout << "CPU time is: " << starttime << endl;
		cout << "x[i]: " << endl;
		for (i = 0; i < I; i++) {
			cout << cplexModel.getValue(x[i]) << ",";
		}cout << endl;

		cout << "y[i][j]: " << endl;
		for (i = 0; i < I; i++) {
			for (j = 0; j < J; j++) {
				cout << cplexModel.getValue(y[i][j]) << ",";
			}
			cout << endl;
			cout << endl;
		}cout << endl;
	}
	else
		cout << "\nProblem infeasible." << endl;

	//Save optimal solution to an external file (calls ofstream method from std namespace)
	std::ofstream oFile("CapacitatedFacilityLocation_CPP.csv");

	if (feasible == IloTrue)
	{
		oFile << "\nProblem feasible." << endl;
		oFile << "The solution status: " << cplexModel.getStatus() << endl;
		oFile << "The objective value is: " << cplexModel.getObjValue() << endl;
		oFile << "CPU time is: " << starttime << endl;
		oFile << "x[i]: " << endl;
		for (i = 0; i < I; i++) {
			oFile << cplexModel.getValue(x[i]) << ",";
		}oFile << endl;

		oFile << "y[i][j]: " << endl;
		for (i = 0; i < I; i++) {
			for (j = 0; j < J; j++) {
				oFile << cplexModel.getValue(y[i][j]) << ",";
			}
			oFile << endl;
			oFile << endl;
		}oFile << endl;
	}
	else
		oFile << "\nProblem infeasible." << endl;

	oFile.close();        // Closes ouput file
	cplexModel.clear();   // Closing the Model
	myenv.end();          // end environment    

	cout << "\nPress [Enter] to continue..." << endl;
	cin.get();    // used to stop console from closing
}
#pragma endregion

#pragma region Uncapacitated Facility Location
// Creates Main method, entry point of C++ (int: returns integer)
void uncapacitatedFacilityLocation()
{
	cout << "-----------Uncapacitated Facility Location-------------" << endl;

	//Define parameters
	int i, j;		// Indexes
	const int I = 3;		// Set of facilities, cardinality
	const int J = 6;		// Set of customers, cardinality
	double c[I][J] = { { 3, 6, 9, 12, 15, 3},
					   { 12, 5, 4, 5, 12, 2},
					   { 1, 15, 5, 8, 11, 7} }; // Cost of facility i to supply to customer j.
	double f[I] = { 3000, 4000, 5000 };	// Cost of adding facility i.
	double d[J] = { 50, 25, 30, 28, 15, 18 }; // Demand of customer j.
	double M = 500; // uncapacitated of facility.

	// Model Definition
	IloEnv myenv; // environment object
	IloModel mycplex(myenv, "UncapacitatedFacilityLocation_CPP"); // model object
	mycplex.setName("UncapacitatedFacilityLocation_CPP");

	//Create the CPLEX variable
	typedef IloArray<IloNumVarArray> IloNumVarArray2;
	typedef IloArray<IloNumVarArray2> IloNumVarArray3;

	typedef IloArray<IloBoolVarArray> IloBoolVarArray2;
	typedef IloArray<IloBoolVarArray2> IloBoolVarArray3;

	// Variable declaration
	IloBoolVarArray x(myenv, I);			  // x[i]: Binary variables that is 1 if facility i is assigned and 0 otherwise.


	IloNumVarArray2 y(myenv, I);			  // y[i][j]: Fraction of demand supplied from facility i to customer j.
	for (i = 0; i < I; i++) {
		y[i] = IloNumVarArray(myenv, J, 0, IloInfinity, ILOFLOAT);
	}

	//Objective function
	IloExpr OBJ(myenv);
	for (int i = 0; i < I; i++) {
		OBJ += f[i] * x[i];
		for (int j = 0; j < J; j++) {
			OBJ += c[i][j] * d[j] * y[i][j];
		}
	}
	mycplex.add(IloMinimize(myenv, OBJ));    // Add objective function
	OBJ.end();


	// ***********************************
	// Constraint 2.1a - Satisfied fraction of demand
	for (j = 0; j < J; j++) {
		IloExpr CSTR_2_1a(myenv);
		for (i = 0; i < I; i++) {
			CSTR_2_1a += y[i][j];
		}
		mycplex.add(CSTR_2_1a == 1);  // Add constraint 2.1a
		CSTR_2_1a.end();              // Releases memory from Expr
	}

	// Constraint 2.ba - Facility capacity
	for (i = 0; i < I; i++) {
		IloExpr CSTR_2_1b(myenv);
		for (j = 0; j < J; j++) {
			CSTR_2_1b += d[j] * y[i][j];
		}
		CSTR_2_1b += -M * x[i];
		mycplex.add(CSTR_2_1b <= 0);  // Add constraint 2.1b
		CSTR_2_1b.end();              // Releases memory from Expr
	}

	//**********************************************************************
	// Create the Cplex model

	float starttime;		// Auxiliary term to calculate computational time

	IloCplex cplexModel(mycplex);					  // Create Cplex model based on IloModel
	IloBool feasible = false;						  // Auxiliary term to check feasibility
	cplexModel.exportModel("UncapacitatedFacilityLocation_CPP.lp");       // Export model to lp format
	cplexModel.exportModel("UncapacitatedFacilityLocation_CPP.mps");       // Export model to mps format

	//***********************************************************************
	//***********************************************************************
	cout << "------------------------------------------" << endl;
	cout << "----------- SOLVING A MODEL  -------------" << endl;
	cout << "------------------------------------------" << endl;

	//set gap parameters
	cplexModel.setParam(IloCplex::EpGap, 0.01);		  // optimization gap 
	cplexModel.setParam(IloCplex::TiLim, 21600);      //limits time in seconds and returns best solution so far

	try {
		starttime = cplexModel.getTime();
		feasible = cplexModel.solve();                    // solves model and store true if feasible
		starttime = cplexModel.getTime() - starttime;
	}
	catch (IloException& e) {
		cout << e.getMessage() << endl;
	}

	// Printing the Solution
	if (feasible == IloTrue)
	{
		cout << "\nProblem feasible." << endl;
		cout << "The solution status: " << cplexModel.getStatus() << endl;
		cout << "The objective value is: " << cplexModel.getObjValue() << endl;
		cout << "CPU time is: " << starttime << endl;
		cout << "x[i]: " << endl;
		for (i = 0; i < I; i++) {
			cout << cplexModel.getValue(x[i]) << ",";
		}cout << endl;

		cout << "y[i][j]: " << endl;
		for (i = 0; i < I; i++) {
			for (j = 0; j < J; j++) {
				cout << cplexModel.getValue(y[i][j]) << ",";
			}
			cout << endl;
			cout << endl;
		}cout << endl;
	}
	else
		cout << "\nProblem infeasible." << endl;

	//Save optimal solution to an external file (calls ofstream method from std namespace)
	std::ofstream oFile("UncapacitatedFacilityLocation_CPP.csv");

	if (feasible == IloTrue)
	{
		oFile << "\nProblem feasible." << endl;
		oFile << "The solution status: " << cplexModel.getStatus() << endl;
		oFile << "The objective value is: " << cplexModel.getObjValue() << endl;
		oFile << "CPU time is: " << starttime << endl;
		oFile << "x[i]: " << endl;
		for (i = 0; i < I; i++) {
			oFile << cplexModel.getValue(x[i]) << ",";
		}oFile << endl;

		oFile << "y[i][j]: " << endl;
		for (i = 0; i < I; i++) {
			for (j = 0; j < J; j++) {
				oFile << cplexModel.getValue(y[i][j]) << ",";
			}
			oFile << endl;
			oFile << endl;
		}oFile << endl;
	}
	else
		oFile << "\nProblem infeasible." << endl;

	oFile.close();        // Closes ouput file
	cplexModel.clear();   // Closing the Model
	myenv.end();          // end environment    

	cout << "\nPress [Enter] to continue..." << endl;
	cin.get();    // used to stop console from closing
}
#pragma endregion

// Creates Main method, entry point of C++ (int: returns integer)
int main()
{
	AuthorDetails();	// calls AuthorDetails method/function 

	capacitatedFacilityLocation(); 
	uncapacitatedFacilityLocation();

	return 0;
}
