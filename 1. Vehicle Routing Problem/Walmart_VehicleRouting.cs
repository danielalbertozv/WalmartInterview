// -------------------------------------------------------------- -*- C++ -*-
// File: Walmart_VechicleRouting.cpp
// --------------------------------------------------------------------------
// Developed by Daniel Zuniga
// Date: 10/02/2021
// Version 1.0.0
//
// C# Version: Vehicle Routing
// --------------------------------------------------------------------------

// Libraries / Headers
using System;                       // Contains fundamental classes and base classes 
using ILOG.Concert;                 // CPLEX library   
using ILOG.CPLEX;                   // CPLEX library   

// Creates namespace Walmart (namespace: to keep one set of names separated from another)
namespace Walmart
{
    // Creates class Walmart_VehicleRouting (class: blueprint of specified object)
    class Walmart_VehicleRouting
    {

        // Creates method Main (entry point to C#) static: cannot be instantiated, void: no value returned, 
        static void Main()
        {
            AuthorDetails();            // calls class AuthorDetails(), displays author details 
            twoIndexVehicleRouting();                 // calls class GetCplex(), define function getcplex();
            Console.ReadLine();         // used to stop console from closing

        } // End Main

        // Creates AuthorDetails() method. pulic: to be accessed by Main, static: cannot be instantiated, void: no value returned, 
        public static void AuthorDetails()
        {
            Console.WriteLine("--------------------------------------------------------");
            Console.WriteLine("File: Walmart_VechicleRouting.cs");
            Console.WriteLine("--------------------------------------------------------");
            Console.WriteLine("Developed by Daniel Zuniga");
            Console.WriteLine("Date: 10/02/2021");
            Console.WriteLine("Version 1.0.0\n");
            Console.WriteLine("C# VVersion: Vehicle Routing");
            Console.WriteLine("--------------------------------------------------------\n");
        } // End AuthorDetails

        #region Two Index Vehicle Routing

        // Creates GetCplex() method. public: to be accessed by Main, static: cannot be instantiated, void: no value returned, 
        public static void twoIndexVehicleRouting()
        {
            Console.WriteLine("-----------Two Index Vehicle Routing-------------" );

            //Define parameters
            const int D = 17;   // Set of destinations, cardinality

            int[,] c = new int [D,D] {   // Vehicle routing cost from vertex i to vertex j.
                 { 0,     658,	931,	835,	698,	329,	602,	233,	370,	233,	643,	602,	466,	425,	562,	931,	794 },
                 { 658,   0,	    821,	370,	233,	602,	876,	425,	835,	890,	1301,	713,	576,	809,	1219,	1042,	1452},
                 { 931,	821,	0,	    1190,	1054,	602,	329,	972,	562,	890,	480,	1534,	1397,	1356,	946,	1862,	905 },
                 { 835,	370,	1190,	0,	    137,	780,	1054,	602,	1013,	1068,	1478,	617,	754,	986,	1397,	672,	1630},
                 { 698,	233,	1054,	137,	0,	    643,	917,	466,	876,	931,	1342,	480,	617,	850,	1260,	809,	1493},
                 { 329,	602,	602,	780,	643,	0,	    274,	370,	233,	288,	698,	931,	794,	754,	617,	1260,	850 },
                 { 602,	876,	329,	1054,	917,	274,	0,	    643,	233,	562,	425,	1205,	1068,	1027,	617,	1534,	576 },
                 { 233,	425,	972,	602,	466,	370,	643,	0,	    410,	466,	876,	562,	425,	384,	794,	890,	1027},
                 { 370,	835,	562,	1013,	876,	233,	233,	410,	0,	    329,	466,	972,	835,	794,	384,	1301,	617 },
                 { 233,	890,	890,	1068,	931,	288,	562,	466,	329,	0,	    410,	643,	506,	466,	329,	972,	562 },
                 { 643,	1301,	480,	1478,	1342,	698,	425,	876,	466,	410,	0,	    1054,	917,	876,	466,	1382,	425 },
                 { 602,	713,	1534,	617,	480,	931,	1205,	562,	972,	643,	1054,	0,	    137,	370,	780,	329,	1013},
                 { 466,	576,	1397,	754,	617,	794,	1068,	425,	835,	506,	917,	137,	0,	    233,	643,	466,	876 },
                 { 425,	809,	1356,	986,	850,	754,	1027,	384,	794,	466,	876,	370,	233,	0,	    410,	506,	643 },
                 { 562,	1219,	946,	1397,	1260,	617,	617,	794,	384,	329,	466,	780,	643,	410,	0,	    917,	233 },
                 { 931,	1042,	1862,	672,	809,	1260,	1534,	890,	1301,	972,	1382,	329,	466,	506,	917,	0,	    958 },
                 { 794,	1452,	905,	1630,	1493,	850,	576,	1027,	617,	562,	425,	1013,	876,	643,	233,	958,	0   }
            };

            int K = 5;   // Number of vehicles
            int MAX = 6; // Maximum number of destinations a vehicle can be routed to

            Cplex mycplex = new Cplex();            // creates object class cplex


            // Define variables names in exported model .lp
            string[][] x_name = new string[D][];
            string[] y_name = new string[D];
            for (int i = 0; i < D; i++)
            {
                y_name[i] = "y_i" + i;
                x_name[i] = new string[D];
                for (int j = 0; j < D; j++)
                {
                    x_name[i][j] = "x_ij" + i + j;
                }
            }

            // Variable declaration
            // x[i][j]: Binary variables that is 1 if a vehicle is routed from destination i to destination j and 0 otherwise.
            INumVar[][] x = new INumVar[D][];
            for (int i = 0; i < D; i++)
            {
                x[i] = mycplex.NumVarArray(D, 0.0, 1.0, NumVarType.Bool,x_name[i]);
            }

            // y[i]: Integer variable that denotes the destination i position in the vehicle routing.
            INumVar[] y = mycplex.NumVarArray(D, 0, System.Int32.MaxValue, NumVarType.Int, y_name);


            //Objective function
            ILinearNumExpr OBJ = mycplex.LinearNumExpr();
            for (int i = 0; i < D; i++)
            {
                for (int j = 0; j < D; j++)
                {
                    OBJ.AddTerm(c[i,j],x[i][j]);
                }
            }
            mycplex.AddMinimize(OBJ);   // add exprssion of objective
            OBJ.Clear();    // Clears Expression


            // ***********************************
            // Constraint 1.1a - Only one vehicle can enter a destination
            for (int j = 1; j < D; j++)
            {
                ILinearNumExpr CSTR_1_1a = mycplex.LinearNumExpr();
                for (int i = 0; i < D; i++)
                {
                    if (i != j)
                        CSTR_1_1a.AddTerm(1,x[i][j]);
                }
                mycplex.AddEq(CSTR_1_1a , 1);   // Add constraint 1.1a
                CSTR_1_1a.Clear();              // Releases memory from Expr
            }

            // Constraint 1.1b - Only one vehicle can leave a destination
            for (int i = 1; i < D; i++)
            {
                ILinearNumExpr CSTR_1_1b = mycplex.LinearNumExpr();
                for (int j = 0; j < D; j++)
                {
                    if (i != j)
                        CSTR_1_1b.AddTerm(1, x[i][j]);
                }
                mycplex.AddEq(CSTR_1_1b , 1);   // Add constraint 1.1b
                CSTR_1_1b.Clear();              // Releases memory from Expr
            }

            // Constraint 1.1c - Number of vehicles leaving the depot at destination 0
            ILinearNumExpr CSTR_1_1c = mycplex.LinearNumExpr();
            for (int i = 1; i < D; i++)
            {
                CSTR_1_1c.AddTerm(1, x[i][0]);
            }
            mycplex.AddEq(CSTR_1_1c , K);  // Add constraint 1.1c
            CSTR_1_1c.Clear();              // Releases memory from Expr

            // Constraint 1.1d - Number of vehicles entering the depot at destination 0
            ILinearNumExpr CSTR_1_1d = mycplex.LinearNumExpr();
            for (int j = 1; j < D; j++)
            {
                CSTR_1_1d.AddTerm(1, x[0][j]);
            }
            mycplex.AddEq(CSTR_1_1d , K);  // Add constraint 1.1d
            CSTR_1_1d.Clear();              // Releases memory from Expr

            // Constraint 1.1e - Subtour elimination (Miller-Tucker-Zemlin)    
            for (int i = 1; i < D; i++)
            {
                for (int j = 1; j < D; j++)
                {
                    if (i != j)
                        mycplex.AddLe(mycplex.Sum( mycplex.Sum(y[i] , mycplex.Prod(-1, y[j])), mycplex.Prod(MAX , x[i][j])) , MAX - 1);  // Add constraint 1.1e
                }
            }

            //**********************************************************************
            // Create the Cplex model
            mycplex.ExportModel("TwoIndexVehicleRouting_CS.lp");   // Export model to lp format
            mycplex.ExportModel("TwoIndexVehicleRouting_CS.mps");  // Export model to mps format

            //set gap parameters
            mycplex.SetParam(Cplex.Param.MIP.Tolerances.MIPGap, 0.001);  // optimization gap
            mycplex.SetParam(Cplex.Param.TimeLimit, 3600);   //limits time in seconds and returns best solution so far

            double starttime;		// Auxiliary term to calculate computational time
            starttime = mycplex.GetCplexTime();   // Gets time before running solver
            try
            {
                mycplex.Solve();   // solves model
            }
            catch (ILOG.Concert.Exception e)
            {
                Console.WriteLine(e.Message);
            }
            starttime = mycplex.GetCplexTime() - starttime;  // Gets time after running solver and substract the difference

            Console.WriteLine("---------------------- PRINTING SOLUTIONS ------------------------------ ");
            //double finaltime = mycplex.GetCplexTime() - starttime;
        
            // Printing the solution
            Console.WriteLine("Problem status: " + mycplex.GetStatus());
            Console.WriteLine("The optimal objective is: " + mycplex.GetObjValue());
            Console.WriteLine("CPU time is: " + starttime);

            Console.WriteLine("x[i][j]: " );
            for (int i = 0; i < D; i++)
            {
                for (int j = 0; j < D; j++)
                {
                    if (i != j)
                    {
                        Console.Write(string.Format("{0} ", mycplex.GetValue(x[i][j])) );
                    }
                    else
                    {
                        Console.Write("0 ");
                    }
                }
                Console.Write(Environment.NewLine);
            }
            Console.Write(Environment.NewLine);

        } // End twoIndexVehicleRouting

        #endregion

    } // End class Walmart_VehicleRouting

} // End namespace Walmart
