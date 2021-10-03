// -------------------------------------------------------------- -*- C++ -*-
// File: VechicleRouting.cpp
// --------------------------------------------------------------------------
// Developed by Daniel Zuniga
// Date: 09/19/2021
// Version 1.0.0
//
// C++ Version: Vehicle Routing
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
	cout << "File: VehicleRouting.cpp" << endl;
	cout << "--------------------------------------------------------" << endl;
	cout << "Developed by Daniel Zuniga" << endl;
	cout << "Date: 09/19/2021" << endl;
	cout << "Version 1.0.0\n" << endl;
	cout << "C++ Version: Vehicle Routing" << endl;
	cout << "--------------------------------------------------------\n" << endl;
} // End AuthorDetails

#pragma region Two Index Vehicle Routing

void twoIndexVehicleRouting() {

    cout << "-----------Two Index Vehicle Routing-------------" << endl;

    //Define parameters
    int i, j;		    // Indexes
    const int D = 17;   // Set of destinations, cardinality

    int c[D][D] ={   // Vehicle routing cost from vertex i to vertex j.
     {0,     658,	931,	835,	698,	329,	602,	233,	370,	233,	643,	602,	466,	425,	562,	931,	794 },
     {658,   0,	    821,	370,	233,	602,	876,	425,	835,	890,	1301,	713,	576,	809,	1219,	1042,	1452},
     {931,	821,	0,	    1190,	1054,	602,	329,	972,	562,	890,	480,	1534,	1397,	1356,	946,	1862,	905 },
     {835,	370,	1190,	0,	    137,	780,	1054,	602,	1013,	1068,	1478,	617,	754,	986,	1397,	672,	1630},
     {698,	233,	1054,	137,	0,	    643,	917,	466,	876,	931,	1342,	480,	617,	850,	1260,	809,	1493},
     {329,	602,	602,	780,	643,	0,	    274,	370,	233,	288,	698,	931,	794,	754,	617,	1260,	850 },
     {602,	876,	329,	1054,	917,	274,	0,	    643,	233,	562,	425,	1205,	1068,	1027,	617,	1534,	576 },
     {233,	425,	972,	602,	466,	370,	643,	0,	    410,	466,	876,	562,	425,	384,	794,	890,	1027},
     {370,	835,	562,	1013,	876,	233,	233,	410,	0,	    329,	466,	972,	835,	794,	384,	1301,	617 },
     {233,	890,	890,	1068,	931,	288,	562,	466,	329,	0,	    410,	643,	506,	466,	329,	972,	562 },
     {643,	1301,	480,	1478,	1342,	698,	425,	876,	466,	410,	0,	    1054,	917,	876,	466,	1382,	425 },
     {602,	713,	1534,	617,	480,	931,	1205,	562,	972,	643,	1054,	0,	    137,	370,	780,	329,	1013},
     {466,	576,	1397,	754,	617,	794,	1068,	425,	835,	506,	917,	137,	0,	    233,	643,	466,	876	},
     {425,	809,	1356,	986,	850,	754,	1027,	384,	794,	466,	876,	370,	233,	0,	    410,	506,	643	},
     {562,	1219,	946,	1397,	1260,	617,	617,	794,	384,	329,	466,	780,	643,	410,	0,	    917,	233	},
     {931,	1042,	1862,	672,	809,	1260,	1534,	890,	1301,	972,	1382,	329,	466,	506,	917,	0,	    958	},
     {794,	1452,	905,	1630,	1493,	850,	576,	1027,	617,	562,	425,	1013,	876,	643,	233,	958,	0	}
    };

    int K = 5;   // Number of vehicles
    int MAX = 6; // Maximum number of destinations a vehicle can be routed to

    // Model Definition
    IloEnv myenv; // environment object
    IloModel mycplex(myenv, "TwoIndexVehicleRouting"); // model object
    mycplex.setName("TwoIndexVehicleRouting");
    
    //Create the CPLEX variable
    typedef IloArray<IloNumVarArray> IloNumVarArray2;
    typedef IloArray<IloNumVarArray2> IloNumVarArray3;

    typedef IloArray<IloBoolVarArray> IloBoolVarArray2;
    typedef IloArray<IloBoolVarArray2> IloBoolVarArray3;

    typedef IloArray<IloIntVarArray> IloIntVarArray2;

    // Variable declaration
    IloBoolVarArray2 x(myenv, D);			  // x[i][j]: Binary variables that is 1 if a vehicle is routed from destination i to destination j and 0 otherwise.
    for (i = 0; i < D; i++) {
        x[i] = IloBoolVarArray(myenv, D);
    }

    // Added 1000 as upper bound to avoid using IloInfinity
    IloIntVarArray y(myenv, D, 0, 1000);     // y[i]: Integer variable that denotes the destination i position in the vehicle routing.

    //Objective function
    IloExpr OBJ(myenv);
    for (i = 0; i < D; i++) {
        for (j = 0; j < D; j++) {
            OBJ += c[i][j] * x[i][j];
        }
    }
    mycplex.add(IloMinimize(myenv, OBJ));    // Add objective function
    OBJ.end();

    // ***********************************
    // Constraint 1.1a - Only one vehicle can enter a destination
    for (j = 1; j < D; j++) {
        IloExpr CSTR_1_1a(myenv);
        for (i = 0; i < D; i++) {
            if (i != j)
                CSTR_1_1a += x[i][j];
        }
        mycplex.add(CSTR_1_1a == 1);  // Add constraint 1.1a
        CSTR_1_1a.end();              // Releases memory from Expr
    }

    // Constraint 1.1b - Only one vehicle can leave a destination
    for (i = 1; i < D; i++) {
        IloExpr CSTR_1_1b(myenv);
        for (j = 0; j < D; j++) {
            if (i != j)
                CSTR_1_1b += x[i][j];
        }
        mycplex.add(CSTR_1_1b == 1);  // Add constraint 1.1b
        CSTR_1_1b.end();              // Releases memory from Expr
    }

    // Constraint 1.1c - Number of vehicles leaving the depot at destination 0
    IloExpr CSTR_1_1c(myenv);
    for (i = 1; i < D; i++) {
        CSTR_1_1c += x[i][0];
    }
    mycplex.add(CSTR_1_1c == K);  // Add constraint 1.1c
    CSTR_1_1c.end();              // Releases memory from Expr

    // Constraint 1.1d - Number of vehicles entering the depot at destination 0
    IloExpr CSTR_1_1d(myenv);
    for (j = 1; j < D; j++) {
        CSTR_1_1d += x[0][j];
    }
    mycplex.add(CSTR_1_1d == K);  // Add constraint 1.1d
    CSTR_1_1d.end();              // Releases memory from Expr

    // Constraint 1.1e - Subtour elimination (Miller-Tucker-Zemlin)    
    for (i = 1; i < D; i++) {
        for (j = 1; j < D; j++) {
            if(i!=j)
                mycplex.add(y[i] - y[j] + (MAX * x[i][j]) <= MAX - 1);  // Add constraint 1.1e
        }
    }

    //**********************************************************************
    // Create the Cplex model

    float starttime;		// Auxiliary term to calculate computational time

    IloCplex cplexModel(mycplex);					  // Create Cplex model based on IloModel
    IloBool feasible = false;						  // Auxiliary term to check feasibility
    cplexModel.exportModel("TwoIndexVehicleRouting_CPP.lp");       // Export model to lp format
    cplexModel.exportModel("TwoIndexVehicleRouting_CPP.mps");       // Export model to mps format

    //***********************************************************************
    //***********************************************************************
    cout << "------------------------------------------" << endl;
    cout << "----------- SOLVING A MODEL  -------------" << endl;
    cout << "------------------------------------------" << endl;

    //set gap parameters
    cplexModel.setParam(IloCplex::EpGap, 0.01);		  // optimization gap 
    cplexModel.setParam(IloCplex::TiLim, 600);      //limits time in seconds and returns best solution so far

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
 
        cout << "x[i][j]: " << endl;
        for (i = 0; i < D; i++) {
            for (j = 0; j < D; j++) {
                if (i != j) {
                    cout << cplexModel.getValue(x[i][j]) << ",";
                }
                else {
                    cout << 0 << ",";
                }
            }
            cout << endl;
        }cout << endl;
    }
    else
        cout << "\nProblem infeasible." << endl;
    //Save optimal solution to an external file (calls ofstream method from std namespace)
    std::ofstream oFile("TwoIndexVehicleRouting_CPP.csv");
   
    if (feasible == IloTrue)
    {
        oFile << "\nProblem feasible." << endl;
        oFile << "The solution status: " << cplexModel.getStatus() << endl;
        oFile << "The objective value is: " << cplexModel.getObjValue() << endl;
        oFile << "CPU time is: " << starttime << endl;
        oFile << "x[i][j]: " << endl;
        for (i = 0; i < D; i++) {
            for (j = 0; j < D; j++) {
                if (i != j) {
                    oFile << cplexModel.getValue(x[i][j]) << ",";
                }
                else {
                    oFile << 0 << ",";
                }
            }
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

	twoIndexVehicleRouting();

	return 0;
}
