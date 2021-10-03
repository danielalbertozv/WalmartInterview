package Metaheuristics;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class VariableNeighborDescent {

    private final CreateData matrix;

    int[] currentPath;
    int iterationNumber;
    int locationNumber;

    private int[] minimumDistancePath;
    private int minimumDistance;
    
    // Auxiliary list to randomize initial path
    List<Integer> auxiliaryList = new ArrayList<Integer>();
    
    // Neighbor enumerated type
    private enum ENeighborType {
        INVERSE, INSERT, SWAP
    }
    
	public VariableNeighborDescent(CreateData matrix, int iterations) {
		
    	this.matrix = matrix;
        locationNumber = matrix.getLocationNumber();
        
        iterationNumber = iterations;
        
        System.out.println("-------VARIABLE NEIGHBOR DESCENT------");        
        System.out.println("Number of iterations: " + iterationNumber);
        
        // Initialize path (x_o)
        initializeCurrentPath();
        
        // Initialize solution for x_o
        initializeBestPath();
        
        // Create neighborTypes
        ENeighborType[] neighborType;
        neighborType = new ENeighborType[]{ENeighborType.INVERSE, 
        		ENeighborType.INSERT, 
        		ENeighborType.SWAP};
        
        // Initialize neighbor type counter
        int typeCounter = 0;
        
        // While a neighbor type provides a better solution
		while (typeCounter < neighborType.length) {
			
			// Call hillCimbing with specified neighbor type
			hillClimbing(neighborType[typeCounter]);
			
			// Calculate new distance
			int currentDistance = matrix.calculateDistance(currentPath);

			// Check if distance has improved
			if (currentDistance < minimumDistance) {
				minimumDistancePath = currentPath;
				minimumDistance = currentDistance; 
				typeCounter = 0; // Restart neighbor type counter
			} else {
				typeCounter++; // update neighbor type counter
			}
		}
        
        
        System.out.println("Minimum distance identified = " + minimumDistance);
        System.out.println("Best path identified: ");
        HelperFunctions.printBestPath(minimumDistancePath);
	}
	
    // Initialize random path
    private void initializeCurrentPath() {
    	// Initiate path
    	currentPath = new int[locationNumber + 1];
    	
    	// Add locations to the list and shuffle it
        for (int i = 1; i < locationNumber; i++) {
        	auxiliaryList.add(i);
        }
        Collections.shuffle(auxiliaryList);
        
        // Save shuffled list in currentPath
        currentPath[0] = 0; // Initial location
        for (int i = 1; i < locationNumber; i++) {
        	currentPath[i] = auxiliaryList.get(i-1);
        }
        currentPath[locationNumber] = 0; // Final destination (return to initial)
    }
    
    // Assigns initial path as best path (just as starting point)
    private void initializeBestPath() {
    	minimumDistancePath = new int[locationNumber + 1];
        System.arraycopy(currentPath, 0, minimumDistancePath, 0, minimumDistancePath.length);
        minimumDistance = matrix.calculateDistance(minimumDistancePath);
    }
    
    // Hill Climbing
    private void hillClimbing(ENeighborType neighborType) {
    	
    	boolean improved = true;
		
    	// Loop while an improvement is identified
    	while (improved) {
			
    		improved = false;
			
    		// Initial location (Source)
    		int initialLocation = 0;
			
    		// Loop for all 
    		for (int i = 0; i < currentPath.length - 2 && !improved; i++) {
				
    			int locationA = currentPath[i+1];				
    			int locationB = currentPath[i+2];
				
    			int j = i+2;   // Auxiliary counter
    			while (locationB != initialLocation && !improved) {
					
    				// Based on the neighbor type, 
    				if (neighborType == ENeighborType.INVERSE) {
					    improved = findInverse(locationA, locationB, improved);
					} else if (neighborType == ENeighborType.INSERT) {
						improved = findInsert(locationA, locationB, improved);
					} else if (neighborType == ENeighborType.SWAP) {
						improved = findSwap(locationA, locationB, improved);
					}
    				
					j++; // Update auxiliary counter
					locationB = currentPath[j]; // Update locationB
				}
			}
		}
    }
    
    private boolean findInverse(int i, int j, boolean improved){
    	
    	double distanceAB = matrix.getEdge(i,j);
    	double distanceNANB = matrix.getEdge(currentPath[i+1],currentPath[j+1]);
    	double distanceANA = matrix.getEdge(i,currentPath[i+1]);
    	double distanceBNB = matrix.getEdge(j,currentPath[j+1]);
    	
    	double invertDistance = distanceAB + distanceNANB - (distanceANA + distanceBNB);
    	
		if (invertDistance < 0 ) {
			

		}
    	
    	return improved;
    }
    
    private boolean findInsert(int ci, int cj, boolean improved){
    	
    	return improved;
    }
    
    private boolean findSwap(int ci, int cj, boolean improved){
    
    	return improved;
    }
}
