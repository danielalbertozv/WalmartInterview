// -------------------------------------------------------------- -*- C++ -*-
// File: FacilityLocation.java
// --------------------------------------------------------------------------
// Developed by Daniel Zuniga
// Date: 09/19/2021
// Version 1.0.0
//
// Java Version: Facility Location
// --------------------------------------------------------------------------

import ilog.concert.*;
import ilog.cplex.*;
import java.io.*;

public class FacilityLocation {

	// Creates AuthorDetials method, (void: no value returned)
	static void authorDetails() {
		System.out.println( "--------------------------------------------------------");
		System.out.println( "File: FacilityLocation.java");
		System.out.println( "--------------------------------------------------------");
		System.out.println( "Developed by Daniel Zuniga");
		System.out.println( "Date: 09/19/2021");
		System.out.println( "Version 1.0.0\n");
		System.out.println( "Java Version: Facility Location");
		System.out.println( "--------------------------------------------------------\n");
	} // End AuthorDetails
	
   static void capacitatedFacilityLocation() {
	   
	    System.out.println("\n-----------Capacitated Facility Location-------------");
	   
	    //Define parameters
		int i, j;		// Indexes
		int I = 3;		// Set of facilities, cardinality
		int J = 6;		// Set of customers, cardinality
		double[][] c = { { 3, 6, 9, 12, 15, 3},
						   { 12, 5, 4, 5, 12, 2},
						   { 1, 15, 5, 8, 11, 7} }; // Cost of facility i to supply to customer j.
		double[] f = { 3000, 4000, 5000 };	// Cost of adding facility i.
		double[] d = { 50, 25, 30, 28, 15, 18 }; // Demand of customer j.
		double[] u = { 80, 100, 90 }; // Capacity of facility i.
		   
		 // Create the modeler/solver.
		 try (IloCplex cplex = new IloCplex()) {

			 // Create variables. 
			 IloNumVar[] x = cplex.boolVarArray(I);      // x[i]: Binary variables that is 1 if facility i is assigned and 0 otherwise.
			 
			 IloNumVar[][] y = new IloNumVar[I][];      // y[i][j]: Fraction of demand supplied from facility i to customer j.
			 for (i = 0; i < I; i++)
			    y[i] = cplex.numVarArray(J, 0.0, 1.0);
			
			 // Objective
			 IloLinearNumExpr obj = cplex.linearNumExpr();
			 for(i = 0; i < I; i++) {
				 obj.addTerm(f[i], x[i]);
			     for (j = 0; j < J; j++) {
			    	 obj.addTerm(c[i][j]*d[j], y[i][j]);
			     }
			 }             
			 cplex.addMinimize(obj);
			 obj.clear();
			 
			// ***********************************
			// Constraint 2.1a - Satisfied fraction of demand
			 for (j = 0; j < J; j++) {
			     IloLinearNumExpr CSTR_2_1a = cplex.linearNumExpr();
			     for (i = 0; i < I; i++)
			    	 CSTR_2_1a.addTerm(1, y[i][j]);
			     cplex.addEq(CSTR_2_1a, 1);
			     CSTR_2_1a.clear();
			  }
	        
	         
	         // Constraint 2.1b - Facility capacity      
	         for (i = 0; i < I; i++) {
	            IloLinearNumExpr CSTR_2_1b = cplex.linearNumExpr();
	            for (j = 0; j < J; j++) {
	            	CSTR_2_1b.addTerm(d[j], y[i][j]);
	            }          	
	            CSTR_2_1b.addTerm(-u[i], x[i]);
	            cplex.addLe(CSTR_2_1b, 0);
	            CSTR_2_1b.clear();
	         }
	         
	     	//**********************************************************************
	     	// Export the Cplex model
	       
	         cplex.exportModel("CapacitatedFacilityLocation_JAVA.lp");
	         cplex.exportModel("CapacitatedFacilityLocation_JAVA.mps");
	         
	         System.out.println( "------------------------------------------" );
	         System.out.println( "----------- SOLVING A MODEL  -------------" );
	         System.out.println( "------------------------------------------" );
	         
	         cplex.setParam(IloCplex.Param.MIP.Tolerances.MIPGap, 0.01);       
	         cplex.setParam(IloCplex.Param.TimeLimit, 600);
	         
	         double starttime;		// Auxiliary term to calculate computational time
	         boolean feasible;
	         
	         starttime = cplex.getCplexTime();
	 		 feasible = cplex.solve();                    // solves model 
	 		 starttime = cplex.getCplexTime() - starttime;
	         
	         // Solve and display solution.
	         if (feasible) {
	        	System.out.println("Solution status: " + cplex.getStatus());
	        	System.out.printf("Objective value: %.2f \n", cplex.getObjValue());
	        	System.out.printf("Computational Time: %.3f \n", starttime);
	        	System.out.println("x[i]: ");
	    		for (i = 0; i < I; i++) {
	    			System.out.printf("%.0f, ",cplex.getValue(x[i]));
	    		}System.out.println(" ");
	    		System.out.println("y[i][j]: ");
	    		for (i = 0; i < I; i++) {
	    			for (j = 0; j < J; j++) {
	    				System.out.printf("%.1f, ",cplex.getValue(y[i][j]));
	    			}
	    			System.out.println(" ");
	    		}
	    	 }
	    	 else
	    		 System.out.println("\nProblem infeasible.");
 
	   }
	   catch (IloException exc) {
	     System.err.println("Concert exception '" + exc + "' caught");
	     System.exit(-1);
	   }
		 
		System.out.println("Press Enter key to continue...");
        try{System.in.read();}  
        	catch(Exception e){} 
    }
 
   static void uncapacitatedFacilityLocation() {
	   
	    System.out.println("\n-----------Uncapacitated Facility Location-------------");
	   
	    //Define parameters
		int i, j;		// Indexes
		int I = 3;		// Set of facilities, cardinality
		int J = 6;		// Set of customers, cardinality
		double[][] c = { { 3, 6, 9, 12, 15, 3},
						   { 12, 5, 4, 5, 12, 2},
						   { 1, 15, 5, 8, 11, 7} }; // Cost of facility i to supply to customer j.
		double[] f = { 3000, 4000, 5000 };	// Cost of adding facility i.
		double[] d = { 50, 25, 30, 28, 15, 18 }; // Demand of customer j.
		double M = 500; // uncapacitated of facility.
		   
		 // Create the modeler/solver.
		 try (IloCplex cplex = new IloCplex()) {

			 // Create variables. 
			 IloNumVar[] x = cplex.boolVarArray(I);      // x[i]: Binary variables that is 1 if facility i is assigned and 0 otherwise.
			 
			 IloNumVar[][] y = new IloNumVar[I][];      // y[i][j]: Fraction of demand supplied from facility i to customer j.
			 for (i = 0; i < I; i++)
			    y[i] = cplex.numVarArray(J, 0.0, 1.0);
			
			 // Objective
			 IloLinearNumExpr obj = cplex.linearNumExpr();
			 for(i = 0; i < I; i++) {
				 obj.addTerm(f[i], x[i]);
			     for (j = 0; j < J; j++) {
			    	 obj.addTerm(c[i][j]*d[j], y[i][j]);
			     }
			 }             
			 cplex.addMinimize(obj);
			 obj.clear();
			 
			// ***********************************
			// Constraint 2.1a - Satisfied fraction of demand
			 for (j = 0; j < J; j++) {
			     IloLinearNumExpr CSTR_2_1a = cplex.linearNumExpr();
			     for (i = 0; i < I; i++)
			    	 CSTR_2_1a.addTerm(1, y[i][j]);
			     cplex.addEq(CSTR_2_1a, 1);
			     CSTR_2_1a.clear();
			  }
	        
	         
	         // Constraint 2.1b - Facility capacity      
	         for (i = 0; i < I; i++) {
	            IloLinearNumExpr CSTR_2_1b = cplex.linearNumExpr();
	            for (j = 0; j < J; j++) {
	            	CSTR_2_1b.addTerm(d[j], y[i][j]);
	            }          	
	            CSTR_2_1b.addTerm(-M, x[i]);
	            cplex.addLe(CSTR_2_1b, 0);
	            CSTR_2_1b.clear();
	         }
	         
	     	//**********************************************************************
	     	// Export the Cplex model
	       
	         cplex.exportModel("CapacitatedFacilityLocation_JAVA.lp");
	         cplex.exportModel("CapacitatedFacilityLocation_JAVA.mps");
	         
	         System.out.println( "------------------------------------------" );
	         System.out.println( "----------- SOLVING A MODEL  -------------" );
	         System.out.println( "------------------------------------------" );
	         
	         cplex.setParam(IloCplex.Param.MIP.Tolerances.MIPGap, 0.01);       
	         cplex.setParam(IloCplex.Param.TimeLimit, 600);
	         
	         double starttime;		// Auxiliary term to calculate computational time
	         boolean feasible;
	         
	         starttime = cplex.getCplexTime();
	 		 feasible = cplex.solve();                    // solves model 
	 		 starttime = cplex.getCplexTime() - starttime;
	         
	         // Solve and display solution.
	         if (feasible) {
	        	System.out.println("Solution status: " + cplex.getStatus());
	        	System.out.printf("Objective value: %.2f \n", cplex.getObjValue());
	        	System.out.printf("Computational Time: %.3f \n", starttime);
	        	System.out.println("x[i]: ");
	    		for (i = 0; i < I; i++) {
	    			System.out.printf("%.0f, ",cplex.getValue(x[i]));
	    		}System.out.println(" ");
	    		System.out.println("y[i][j]: ");
	    		for (i = 0; i < I; i++) {
	    			for (j = 0; j < J; j++) {
	    				System.out.printf("%.1f, ",cplex.getValue(y[i][j]));
	    			}
	    			System.out.println(" ");
	    		}
	    	 }
	    	 else
	    		 System.out.println("\nProblem infeasible.");

	   }
	   catch (IloException exc) {
	     System.err.println("Concert exception '" + exc + "' caught");
	     System.exit(-1);
	   }
		 
		System.out.println("Press Enter key to continue...");
	    try{System.in.read();}  
	    	catch(Exception e){} 
   }
   
   public static void main(String[] args) {
	   
	   authorDetails();
	   capacitatedFacilityLocation();
	   uncapacitatedFacilityLocation();
   }
   
}
