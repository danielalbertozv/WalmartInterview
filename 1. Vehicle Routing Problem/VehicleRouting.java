// -------------------------------------------------------------- -*- C++ -*-
// File: VehicleRouting.java
// --------------------------------------------------------------------------
// Developed by Daniel Zuniga
// Date: 09/19/2021
// Version 1.0.0
//
// Java Version: Vehicle Routing
// --------------------------------------------------------------------------

import ilog.concert.*;
import ilog.cplex.*;
import java.io.*;

public class VehicleRouting {
	
	// Creates AuthorDetials method, (void: no value returned)
	static void authorDetails() {
		System.out.println( "--------------------------------------------------------");
		System.out.println( "File: VehicleRuting.java");
		System.out.println( "--------------------------------------------------------");
		System.out.println( "Developed by Daniel Zuniga");
		System.out.println( "Date: 09/19/2021");
		System.out.println( "Version 1.0.0\n");
		System.out.println( "Java Version: Vehicle Routing");
		System.out.println( "--------------------------------------------------------\n");
	} // End AuthorDetails
	 
   static void twoIndexVehicleRouting() {
	   
	    System.out.println("\n-----------Two Index Vehicle Routing-------------");
	   
	    //Define parameters
		int i, j;		// Indexes
		int D = 17;		// Set of facilities, cardinality
	    int[][] c ={   // Vehicle routing cost from vertex i to vertex j.
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
	    
		 // Create the modeler/solver.
		 try (IloCplex cplex = new IloCplex()) {

			 // Create variables. 			 
			 IloNumVar[][] x = new IloNumVar[D][];               // x[i][j]: Binary variables that is 1 if a vehicle is routed from destination i to destination j and 0 otherwise.
			 for (i = 0; i < D; i++)
			    x[i] = cplex.boolVarArray(D);
			 
			// Added 1000 as upper bound to avoid using IloInfinity
			 IloNumVar[] y = cplex.intVarArray(D, 0, 1000);      // y[i]: Integer variable that denotes the destination i position in the vehicle routing.
			 
			
			 // Objective
			 IloLinearNumExpr obj = cplex.linearNumExpr();
			 for(i = 0; i < D; i++) {
			     for (j = 0; j < D; j++) {
			    	 obj.addTerm(c[i][j], x[i][j]);
			     }
			 }             
			 cplex.addMinimize(obj);
			 obj.clear();
			 
			// ***********************************
			// Constraint 1.1a - Only one vehicle can enter a destination
			for (j = 1; j < D; j++) {
			    IloLinearNumExpr CSTR_1_1a = cplex.linearNumExpr();
			    for (i = 0; i < D; i++) {
			   	 if(i!=j)
			   		 CSTR_1_1a.addTerm(1, x[i][j]);
			    }
			    cplex.addEq(CSTR_1_1a, 1);
			    CSTR_1_1a.clear();
			 }
	        
	         
			// Constraint 1.1b - Only one vehicle can leave a destination 
	        for (i = 1; i < D; i++) {
	           IloLinearNumExpr CSTR_1_1b = cplex.linearNumExpr();
	           for (j = 0; j < D; j++) {
	           	if(i!=j)
	           		CSTR_1_1b.addTerm(1, x[i][j]);
	           }          	
	           cplex.addEq(CSTR_1_1b, 1);
	           CSTR_1_1b.clear();
	        }
	         
	        // Constraint 1.1c - Number of vehicles leaving the depot at destination 0
            IloLinearNumExpr CSTR_1_1c = cplex.linearNumExpr();
            for (i = 1; i < D; i++) {
            	CSTR_1_1c.addTerm(1, x[i][0]);
            }          	
            cplex.addEq(CSTR_1_1c, K);
            CSTR_1_1c.clear();
	        
	        // Constraint 1.1d - Number of vehicles leaving the depot at destination 0
            IloLinearNumExpr CSTR_1_1d = cplex.linearNumExpr();
            for (j = 1; j < D; j++) {
            	CSTR_1_1d.addTerm(1, x[0][j]);
            }          	
            cplex.addEq(CSTR_1_1d, K);
            CSTR_1_1d.clear();
            
            // Constraint 1.1e - Subtour elimination (Miller-Tucker-Zemlin)
	        for (i = 1; i < D; i++) {
		        for (j = 0; j < D; j++) {
		        	if(i!=j)
		        		 cplex.addLe(cplex.sum(cplex.sum(y[i], cplex.prod(-1,y[j])), cplex.prod(MAX, x[i][j])), MAX - 1);
		        }          	
		    }
            
            
	     	//**********************************************************************
	     	// Export the Cplex model
	       
	         cplex.exportModel("TwoIndexVehicleRouting_JAVA.lp");
	         cplex.exportModel("TwoIndexVehicleRouting_JAVA.mps");
	         
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
	    		System.out.println("x[i][j]: ");
	    		for (i = 0; i < D; i++) {
	    			for (j = 0; j < D; j++) {
	    				if(i!=j) {
	    					System.out.printf("%.1f, ",cplex.getValue(x[i][j]));
	    				}
	    				else {
	    					System.out.printf("0, ");
	    				}
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
	   twoIndexVehicleRouting();
   }
   
}
