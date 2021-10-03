package Metaheuristics;

import java.util.ArrayList;
import java.util.concurrent.ThreadLocalRandom;

public class PSOParticle {

	int[] bestPath;	                  // Best path identified
	private final CreateData matrix;

    public PSOLocation location;
    public PSOLocation locationPBest;
    
    public PSOVelocity velocity;  
    int locationNumber;              // Number of locations in problem
    public int neighborSearchIterations = 120;    // Iterations for neighbor search

    // Constructor
    public PSOParticle(CreateData matrix, int[] bestPath)
    {
        this.bestPath = bestPath;
        this.matrix = matrix;
        
        location = new PSOLocation(new ArrayList<Double>());
        locationPBest = new PSOLocation(new ArrayList<Double>());
        velocity = new PSOVelocity(new ArrayList<Double>());
        locationNumber = bestPath.length;
    }

    // Search for the best neighborhood path based on distance
    public void getBestPath()
    {
    	int[] neighborPath = bestPath;
        int i = 0 ;
        for(i = 0 ; i < neighborSearchIterations ; i++)
        {
        	neighborPath = getNeighborPath(bestPath);
            if(matrix.calculateDistance(neighborPath) < matrix.calculateDistance(bestPath))
            	bestPath = neighborPath;
        }
    }

    // Gets a random neighbor from a particular path
    public int[] getNeighborPath(int[] neighborPath)
    {
        int locationA = 0 ;
        int locationB = 0 ;
        while(locationA==locationB){
        	locationA = ThreadLocalRandom.current().nextInt(1, locationNumber-1);
        	locationB = ThreadLocalRandom.current().nextInt(1, locationNumber-1);
        }
        int auxiliar;
        auxiliar = neighborPath[locationA];
        neighborPath[locationA] = neighborPath[locationB];
        neighborPath[locationB] = auxiliar;
        return neighborPath;
    }
    
    // Swaps locations based on a coefficient determined from abs(location - newLocation)
    public void swapWithLocation(int coefficient)
    {
        for(int i = 0 ; i < coefficient ; i++)
        {
            int locationA = 0 ;
            int locationB = 0 ;
            while(locationA==locationB){
            	locationA = ThreadLocalRandom.current().nextInt(1, locationNumber-1);
            	locationB = ThreadLocalRandom.current().nextInt(1, locationNumber-1);
            }
            
            int auxiliar;
            auxiliar = bestPath[locationA];
            bestPath[locationA] = bestPath[locationB];
            bestPath[locationB] = auxiliar;
        }
    }
}