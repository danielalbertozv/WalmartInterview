// -------------------------------------------------------------- -*- C++ -*-
// File: TabuSearch.java
// --------------------------------------------------------------------------
// Developed by Daniel Zuniga
// Date: 09/26/2021
// Version 1.0.0
//
// Java Version: Metaheuristic Examples - Traveling Salesman Problem
// --------------------------------------------------------------------------

package Metaheuristics;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class TabuSearch {
	
    private CreateTabuList tabuList;
    private final CreateData matrix;

    int[] currentPath;
    int iterationNumber;
    int locationNumber;

    private int[] minimumDistancePath;
    private int minimumDistance;

    // Auxiliary list to randomize initial path
    List<Integer> auxiliaryList = new ArrayList<Integer>();
    
    // Constructor
    public TabuSearch(CreateData matrix, int iterations) 
    {   	
    	this.matrix = matrix;
        locationNumber = matrix.getLocationNumber();
        
        // Creates a tabu list of zeros [locationNumber][locationNumber]
        tabuList = new CreateTabuList(locationNumber);
        iterationNumber = iterations;
        
        System.out.println("-------TABU SEARCH------");        
        System.out.println("Number of iterations: " + iterationNumber);
        
        // Initialize path (x_o)
        initializeCurrentPath();
        
        // Initialize solution for x_o
        initializeBestPath();
        
        // Stopping criteria (iteration number)
        for (int i = 0; i < iterationNumber; i++) 
        {
            int locationA = 0;
            int locationB = 0;

            // Generate neighborhood of x_o
            for (int j = 1; j < currentPath.length - 1; j++) 
                for (int k = 2; k < currentPath.length - 1; k++) 
                    if (j != k) 
                    {                    	
                    	// Generate x in neighborhood of x_o by swapping
                        swap(j, k);
                        
                        // Determine solution for x in neighborhood
                        int currDistance = matrix.calculateDistance(currentPath);
                        
                        // If solution x in neighborhood is better and it is not in the tabu list,
                        // i.e., tabu list[j][k] is 0
                        if ((currDistance < minimumDistance) && tabuList.tabuList[j][k] == 0) 
                        {
                        	locationA = j;
                        	locationB = k;
                            System.arraycopy(currentPath, 0, minimumDistancePath, 0, minimumDistancePath.length);
                            minimumDistance = currDistance;
                        }
                    }
          
            // If locationA != 0, a new best solution has been identified.
            // Update tabu list 
            if (locationA != 0) 
            {
            	// Decrease current tabu
                tabuList.decrementTabu();
                
                // Add new x to the tabu list
                tabuList.incrementTabu(locationA, locationB);
            }
        }

       System.out.println("Minimum distance identified = " + minimumDistance);
       System.out.println("Best path identified: ");
       HelperFunctions.printBestPath(minimumDistancePath);
    }

    // Initialize random path
    private void initializeCurrentPath() 
    {
    	// Initiate path
    	currentPath = new int[locationNumber + 1];
    	
    	// Add locations to the list and shuffle it
        for (int i = 1; i < locationNumber; i++)
        	auxiliaryList.add(i);

        Collections.shuffle(auxiliaryList);
        
        // Save shuffled list in currentPath
        currentPath[0] = 0; // Initial location
        for (int i = 1; i < locationNumber; i++)
        	currentPath[i] = auxiliaryList.get(i-1);

        currentPath[locationNumber] = 0; // Final destination (return to initial)
    }
    
    // Assigns initial path as best path (just as starting point)
    private void initializeBestPath() 
    {
    	minimumDistancePath = new int[locationNumber + 1];
        System.arraycopy(currentPath, 0, minimumDistancePath, 0, minimumDistancePath.length);
        minimumDistance = matrix.calculateDistance(minimumDistancePath);
    }
    
    // Swap current paths
    private void swap(int i, int k) 
    {
        int auxiliar = currentPath[i];
        currentPath[i] = currentPath[k];
        currentPath[k] = auxiliar;
    }
    
}