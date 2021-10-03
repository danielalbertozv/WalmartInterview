// -------------------------------------------------------------- -*- C++ -*-
// File: Walmart_FacilityLocation.cpp
// --------------------------------------------------------------------------
// Developed by Daniel Zuniga
// Date: 10/02/2021
// Version 1.0.0
//
// C# Version: Facility Location
// --------------------------------------------------------------------------

// Libraries / Headers
using System;                       // Contains fundamental classes and base classes 
using System.Collections;   
using System.Linq;
using System.Text;
using System.Threading.Tasks;       

using ILOG.Concert;        // CPLEX library      
using ILOG.CPLEX;          // CPLEX library  

// Creates namespace Walmart (namespace: to keep one set of names separated from another)
namespace Walmart
{
    // Creates class (class: blueprint of specified object)
    class Walmart_FacilityLocation
    {
        // Creates method Main (entry point to C#) static: cannot be instantiated, void: no value returned, 
        static void Main()
        {
            AuthorDetails();                  // calls class AuthorDetails(), displays author details 
            capacitatedFacilityLocation();
            uncapacitatedFacilityLocation();
            Console.ReadLine();               // used to stop console from closing

        } // End Main

        // pulic: to be accessed by Main, static: cannot be instantiated, void: no value returned, 
        public static void AuthorDetails()
        {
            Console.WriteLine("--------------------------------------------------------");
            Console.WriteLine("File: Walmart_FacilityLocation.cs");
            Console.WriteLine("--------------------------------------------------------");
            Console.WriteLine("Developed by Daniel Zuniga");
            Console.WriteLine("Date: 05/15/2021");
            Console.WriteLine("Version 1.0.0\n");
            Console.WriteLine("C# Version: Facility Location");
            Console.WriteLine("--------------------------------------------------------\n");
        } //End AuthorDetails

        // public: pulic access, static: cannot be instantiated, void: no value returned, 
        public static void capacitatedFacilityLocation()
        {

            Console.WriteLine("-----------Capacitated Facility Location-------------");

            Cplex mycplex = new Cplex();            // creates object class cplex

            //Define parameters
            int i, j;       // Indexes
            const int I = 3;        // Set of facilities, cardinality
            const int J = 6;        // Set of customers, cardinality
            int [,] c = new int [I,J] {
                    { 3, 6, 9, 12, 15, 3},
					{ 12, 5, 4, 5, 12, 2},
					{ 1, 15, 5, 8, 11, 7}
            }; // Cost of facility i to supply to customer j.
            double [] f = { 3000, 4000, 5000 }; // Cost of adding facility i.
            double [] d = { 50, 25, 30, 28, 15, 18 }; // Demand of customer j.
            double [] u = { 80, 100, 90 }; // Capacity of facility i.

            // Define variables names in exported model .lp
            string[] x_name = new string[I];
            string[][] y_name = new string[I][];
            for (i = 0; i < I; i++)
            {
                x_name[i] = "x_i" + i;
                y_name[i] = new string[J];
                for (j = 0; j < J; j++)
                {
                    y_name[i][j] = "y_ij" + i + j;
                }
            }

            // Variable declaration
            // x[i]: Binary variables that is 1 if facility i is assigned and 0 otherwise.
            INumVar[] x = mycplex.NumVarArray(I, 0.0, 1.0, NumVarType.Bool, x_name);

            // y[i][j]: Fraction of demand supplied from facility i to customer j.
            INumVar[][] y = new INumVar[I][];
            for (i = 0; i < I; i++)
            {
                y[i] = mycplex.NumVarArray(J, 0.0, 1.0, y_name[i]);
            }

            //Objective function
            ILinearNumExpr OBJ = mycplex.LinearNumExpr();
            for (i = 0; i < I; i++)
            {
                OBJ.AddTerm(f[i] , x[i]);
                for (j = 0; j < J; j++)
                {
                    OBJ.AddTerm(c[i,j] * d[j] , y[i][j]);
                }
            }
            mycplex.AddMinimize(OBJ);    // Add objective function
            OBJ.Clear();

            // ***********************************
            // Constraint 2.1a - Satisfied fraction of demand
            for (j = 0; j < J; j++)
            {
                ILinearNumExpr CSTR_2_1a = mycplex.LinearNumExpr();
                for (i = 0; i < I; i++)
                {
                    CSTR_2_1a.AddTerm(1 , y[i][j]);
                }
                mycplex.AddEq(CSTR_2_1a , 1);  // Add constraint 2.1a
                CSTR_2_1a.Clear();              // Releases memory from Expr
            }

            // Constraint 2.1b - Facility capacity
            for (i = 0; i < I; i++)
            {
                ILinearNumExpr CSTR_2_1b = mycplex.LinearNumExpr();
                for (j = 0; j < J; j++)
                {
                    CSTR_2_1b.AddTerm(d[j] , y[i][j]);
                }
                CSTR_2_1b.AddTerm( - u[i] , x[i]);
                mycplex.AddLe(CSTR_2_1b , 0);  // Add constraint 2.1b
                CSTR_2_1b.Clear();              // Releases memory from Expr
            }

            //**********************************************************************
            // Create the Cplex model
            mycplex.ExportModel("CapacitatedFacilityLocation_CPP.lp");   // Export model to lp format
            mycplex.ExportModel("CapacitatedFacilityLocation_CPP.mps");  // Export model to mps format

            //set gap parameters
            mycplex.SetParam(Cplex.Param.MIP.Tolerances.MIPGap, 0.001);  // optimization gap
            mycplex.SetParam(Cplex.Param.TimeLimit, 3600);   //limits time in seconds and returns best solution so far

            double starttime;       // Auxiliary term to calculate computational time
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
                                                                // Printing the solution
            Console.WriteLine("Problem status: " + mycplex.GetStatus());
            Console.WriteLine("The optimal objective is: " + mycplex.GetObjValue());
            Console.WriteLine("CPU time is: " + starttime);

            Console.WriteLine("x[i]: ");
            for (i = 0; i < I; i++)
            {
                Console.Write(string.Format("{0} ", mycplex.GetValue(x[i]) ));
            }
            Console.Write(Environment.NewLine);

            Console.WriteLine("y[i][j]: ");
            for (i = 0; i < I; i++)
            {
                for (j = 0; j < J; j++)
                {
                    Console.Write(string.Format("{0} ", mycplex.GetValue(y[i][j]) ));
                }
                Console.Write(Environment.NewLine);
                Console.Write(Environment.NewLine);
            }
            Console.Write(Environment.NewLine);

        } // End capacitatedFacilityLocation

        // public: pulic access, static: cannot be instantiated, void: no value returned, 
        public static void uncapacitatedFacilityLocation()
        {

            Console.WriteLine("-----------Capacitated Facility Location-------------");

            Cplex mycplex = new Cplex();            // creates object class cplex

            //Define parameters
            int i, j;       // Indexes
            const int I = 3;        // Set of facilities, cardinality
            const int J = 6;        // Set of customers, cardinality
            int[,] c = new int[I, J] {
                    { 3, 6, 9, 12, 15, 3},
                    { 12, 5, 4, 5, 12, 2},
                    { 1, 15, 5, 8, 11, 7}
            }; // Cost of facility i to supply to customer j.
            double[] f = { 3000, 4000, 5000 }; // Cost of adding facility i.
            double[] d = { 50, 25, 30, 28, 15, 18 }; // Demand of customer j.
            double M = 500; // uncapacitated of facility.

            // Define variables names in exported model .lp
            string[] x_name = new string[I];
            string[][] y_name = new string[I][];
            for (i = 0; i < I; i++)
            {
                x_name[i] = "x_i" + i;
                y_name[i] = new string[J];
                for (j = 0; j < J; j++)
                {
                    y_name[i][j] = "y_ij" + i + j;
                }
            }

            // Variable declaration
            // x[i]: Binary variables that is 1 if facility i is assigned and 0 otherwise.
            INumVar[] x = mycplex.NumVarArray(I, 0.0, 1.0, NumVarType.Bool, x_name);

            // y[i][j]: Fraction of demand supplied from facility i to customer j.
            INumVar[][] y = new INumVar[I][];
            for (i = 0; i < I; i++)
            {
                y[i] = mycplex.NumVarArray(J, 0.0, 1.0, y_name[i]);
            }

            //Objective function
            ILinearNumExpr OBJ = mycplex.LinearNumExpr();
            for (i = 0; i < I; i++)
            {
                OBJ.AddTerm(f[i], x[i]);
                for (j = 0; j < J; j++)
                {
                    OBJ.AddTerm(c[i, j] * d[j], y[i][j]);
                }
            }
            mycplex.AddMinimize(OBJ);    // Add objective function
            OBJ.Clear();

            // ***********************************
            // Constraint 2.1a - Satisfied fraction of demand
            for (j = 0; j < J; j++)
            {
                ILinearNumExpr CSTR_2_1a = mycplex.LinearNumExpr();
                for (i = 0; i < I; i++)
                {
                    CSTR_2_1a.AddTerm(1, y[i][j]);
                }
                mycplex.AddEq(CSTR_2_1a, 1);  // Add constraint 2.1a
                CSTR_2_1a.Clear();              // Releases memory from Expr
            }

            // Constraint 2.1b - Facility capacity
            for (i = 0; i < I; i++)
            {
                ILinearNumExpr CSTR_2_1b = mycplex.LinearNumExpr();
                for (j = 0; j < J; j++)
                {
                    CSTR_2_1b.AddTerm(d[j], y[i][j]);
                }
                CSTR_2_1b.AddTerm(-M, x[i]);
                mycplex.AddLe(CSTR_2_1b, 0);  // Add constraint 2.1b
                CSTR_2_1b.Clear();              // Releases memory from Expr
            }

            //**********************************************************************
            // Create the Cplex model
            mycplex.ExportModel("UncapacitatedFacilityLocation_CS.lp");   // Export model to lp format
            mycplex.ExportModel("UncapacitatedFacilityLocation_CS.mps");  // Export model to mps format

            //set gap parameters
            mycplex.SetParam(Cplex.Param.MIP.Tolerances.MIPGap, 0.001);  // optimization gap
            mycplex.SetParam(Cplex.Param.TimeLimit, 3600);   //limits time in seconds and returns best solution so far

            double starttime;       // Auxiliary term to calculate computational time
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
                                                             // Printing the solution
            Console.WriteLine("Problem status: " + mycplex.GetStatus());
            Console.WriteLine("The optimal objective is: " + mycplex.GetObjValue());
            Console.WriteLine("CPU time is: " + starttime);

            Console.WriteLine("x[i]: ");
            for (i = 0; i < I; i++)
            {
                Console.Write(string.Format("{0} ", mycplex.GetValue(x[i])));
            }
            Console.Write(Environment.NewLine);

            Console.WriteLine("y[i][j]: ");
            for (i = 0; i < I; i++)
            {
                for (j = 0; j < J; j++)
                {
                    Console.Write(string.Format("{0} ", mycplex.GetValue(y[i][j])));
                }
                Console.Write(Environment.NewLine);
                Console.Write(Environment.NewLine);
            }
            Console.Write(Environment.NewLine);

        } // End uncapacitatedFacilityLocation



    } // End Class Walmart_FacilityLocation

} // End namespace Walmart
