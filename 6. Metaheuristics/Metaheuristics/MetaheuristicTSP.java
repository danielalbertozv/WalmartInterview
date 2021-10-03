// -------------------------------------------------------------- -*- C++ -*-
// File: MetaheuristicTSP.java
// --------------------------------------------------------------------------
// Developed by Daniel Zuniga
// Date: 09/26/2021
// Version 1.0.0
//
// Java Version: Metaheuristic Examples - Traveling Salesman Problem
// --------------------------------------------------------------------------

package Metaheuristics;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.concurrent.ThreadLocalRandom;

public class MetaheuristicTSP {
	
	static long startTime;
	static long finalTime;
	
	// Creates AuthorDetials method, (void: no value returned)
	static void authorDetails() 
	{
		System.out.println( "--------------------------------------------------------");
		System.out.println( "File: MetaheuristicTSP.java");
		System.out.println( "--------------------------------------------------------");
		System.out.println( "Developed by Daniel Zuniga");
		System.out.println( "Date: 09/26/2021");
		System.out.println( "Version 1.0.0\n");
		System.out.println( "Java Version: Metaheuristic Examples - Traveling Salesman Problem");
		System.out.println( "--------------------------------------------------------\n");
	} // End AuthorDetails

	public static void main(String[] args) 
	{
      		
		authorDetails();
			
		// Uncomment for Random Matrix
		// Create random location size between 10 to 15
		int locationNumber = ThreadLocalRandom.current().nextInt(10, 15);
		//int locationNumber = 17;6
		
		// Create random distance matrix
        CreateData matrix = new CreateData(locationNumber);
        
              
        //------------------ TABU SEARCH --------------------
        int iterationsTabuSearch = 20000;
        startTime = System.currentTimeMillis();
        @SuppressWarnings("unused")
        TabuSearch tabuSearch = new TabuSearch(matrix, iterationsTabuSearch);      
        finalTime = System.currentTimeMillis() - startTime;
        System.out.println("Time (miliseconds): " + finalTime);

		System.out.println("Press Enter key to continue...");
        try{System.in.read();}  
        	catch(Exception e){} 

        
        //------------- SIMULATED ANNEALING ----------------
        //Set initial temperature
        double temperature = 300000;

        //Cooling rate (1 - alpha)
        double coolingRate = 0.03;
        
        // Call Simulated Annealing and save time
        startTime = System.currentTimeMillis();
        @SuppressWarnings("unused")
        SimulatedAnnealing simulatedAnnealing = new SimulatedAnnealing(matrix, temperature, coolingRate);      
        finalTime = System.currentTimeMillis() - startTime;
        System.out.println("Time (miliseconds): " + finalTime);

        try{System.in.read();}  
    	catch(Exception e){} 
		System.out.println("Press Enter key to continue...");
        try{System.in.read();}  
        	catch(Exception e){} 
        
        
        //------------- PARTICLE SWARM OPTIMZIATION ----------------
        //Set iterations
        int psoIterations = 50;

        //Set number of particles
        int numberParticles = 25;
        
        // Call Particle Swarm Optimization and save time
        startTime = System.currentTimeMillis();
        @SuppressWarnings("unused")
        ParticleSwarmOptimization particleSwarmOptimization = new ParticleSwarmOptimization(matrix, psoIterations, numberParticles);      
        finalTime = System.currentTimeMillis() - startTime;
        System.out.println("Time (miliseconds): " + finalTime);

        try{System.in.read();}  
    	catch(Exception e){} 
		System.out.println("Press Enter key to continue...");
        try{System.in.read();}  
        	catch(Exception e){} 
                
        
       // ************************************************
       //------------- ACCESSING DATABASE ----------------
       // ************************************************
        
        System.out.println("************ DATABASE ***************");
        System.out.println("Accessing database walmart_example");
        
		//Accessing driver from the JAR file
		try {
			Class.forName("com.mysql.cj.jdbc.Driver");
		} catch (ClassNotFoundException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
		
		// Creating a variable for the connection called "con"
		// jdbc:mysql://localhost:3306/walmart_example --> This is the data base
		// root is the database user
		// daniel123 is the password
		Connection con = null;
		try {
			con = DriverManager.getConnection("jdbc:mysql://localhost:3306/walmart_example","root","daniel123");
		} catch (SQLException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
				
		// Here we create the query
		PreparedStatement statement = null;
		try {
			statement = con.prepareStatement("select distance from distances");
		} catch (SQLException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
		
		// Creating a variable to execute query
		ResultSet result = null;
		try {
			result = statement.executeQuery();
		} catch (SQLException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
		
		// Print distance matrix from database and assigned it to a matrix
		System.out.println("Extratced distances matrix from database:");
		int locationsDB = 5;
		int i = 0, j = 0;
		int[][] matrixDB = new int[locationsDB][locationsDB];
		try {
			while(result.next()){
				System.out.print(result.getInt(1) + "   ");
				// getString returns the data
				//1 is the first field in the table
				matrixDB[i][j] = result.getInt(1);
				j++;
				if(j==5)
				{
					System.out.println("");
					j=0;
					i++;
				}
				
			}System.out.println("");
		} catch (SQLException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
		
		CreateData matrixDatabase = new CreateData(matrixDB);
		
        //-------------run PARTICLE SWARM OPTIMZIATION with distance matrix from DATABASE----------------
		System.out.println("Run PSO with distance matrix from Database");
        // Call Particle Swarm Optimization and save time
        startTime = System.currentTimeMillis();
        @SuppressWarnings("unused")
        ParticleSwarmOptimization particleSwarmOptimizationDB = new ParticleSwarmOptimization(matrixDatabase, psoIterations, numberParticles);      
        finalTime = System.currentTimeMillis() - startTime;
        System.out.println("Time (miliseconds): " + finalTime);

        try{System.in.read();}  
    	catch(Exception e){} 
		System.out.println("Press Enter key to continue...");
        try{System.in.read();}  
        	catch(Exception e){} 
    }	
}
