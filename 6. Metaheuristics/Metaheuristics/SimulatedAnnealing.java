package Metaheuristics;

import java.util.*;

public class SimulatedAnnealing {

    private final CreateData matrix;
    
    int[] currentPath;
    int locationNumber;
    
    private int[] minimumDistancePath;
    private int minimumDistance;
    
    // Auxiliary list to randomize initial path
    List<Integer> auxiliaryList = new ArrayList<Integer>();
    
    // Constructor
	public SimulatedAnnealing(CreateData matrix, double temperature, double coolingRate)
	{		
        this.matrix = matrix;
        locationNumber = matrix.getLocationNumber();
        
		System.out.println("-------SIMULATED ANNEALING------");        
        System.out.println("Initial temperature: " + temperature);
        System.out.println("Cooling rate (1 - alpha): " + coolingRate);
        
        // Initialize path
        initializeCurrentPath();
        initializeBestPath();
        
        // Stopping criteria (e.g., temperature, no. iterations)
        // In this case is temperature lower limit
        while (temperature > 1) 
        {

            // Get random locations from a symmetrical distribution (e.g., uniform)
            int locationA = HelperFunctions.randomLocation(1 , currentPath.length-1);
            int locationB = HelperFunctions.randomLocation(1 , currentPath.length-1);
            
            // Ensure both locations are different
    		while(locationA == locationB) {locationB = HelperFunctions.randomLocation(1 , currentPath.length-1);}
    		
    		// Noise is added through the swapping of two locations
            // x prime = x + noise
            swap(locationA, locationB);
            
            // calculate new solution (new distance)
            int currentDistance = matrix.calculateDistance(currentPath);
            
            // Calculate probability p = exp(delta h) / T)
            // Accept of reject candidate solution with probability p
            // u follows U(0,1) accept x = x prime
            if (Math.random() < acceptanceProbability(minimumDistance, currentDistance, temperature)) 
            {
            	minimumDistancePath = currentPath;
            	minimumDistance = currentDistance;
            }
            
            // Update Temperature
            // T = alpha * T, 0 < alpha < 1,  (alpha = 1 - coolingRate)
            //System.out.println("Temperature: " + temperature);
            temperature *= 1 - coolingRate;
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
        minimumDistance = matrix.calculateDistance(minimumDistancePath);     // Saves minimumDistance
    }
    
    // Swap current paths positions
    private void swap(int i, int k) 
    {
        int auxiliar = currentPath[i];
        currentPath[i] = currentPath[k];
        currentPath[k] = auxiliar;
    }
    
    // Calculate probability p = exp(delta h) / T)
    // where delta h is the difference between the best identified solution and current one
	public static double acceptanceProbability(int minimumDistance, int newDistance, double temperature) 
	{
		// If the new distance is better, return probability of 1
		if (newDistance < minimumDistance)
			return 1.0;

		// Otherwise, calculate an acceptance probability
		return Math.exp((minimumDistance - newDistance) / temperature);
	}
	    
}
