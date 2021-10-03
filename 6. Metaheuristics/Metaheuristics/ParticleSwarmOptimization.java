package Metaheuristics;

import java.util.ArrayList;
import java.util.Collections;
import java.util.*;

public class ParticleSwarmOptimization {

	// Distance matrix between locations
    private final CreateData matrix;

    // Particle list
    public ArrayList<PSOParticle> particlesList = new ArrayList<PSOParticle>();
    
    // Stores best location
    public PSOLocation getBestLocation = new PSOLocation(new ArrayList<Double>());
    
    // Best path identified
    int[] bestPath;
    
    // Minimum distance identified
    private double minimumDistance;
    
    int iterationNumber;    // Number of iterations
    int locationNumber;     // Number of locations in problem
    int numberParticles; 	// Number of particles
    //PSO parameter
    public double c1 = 2;        // cognitive  acceleration  coefficient
    public double c2 = 2;        // collective  acceleration  coefficient
    
    // c1 and c2 are used to balance the  effect  of  self knowledge  and  social knowledge 
    // when particles move to the target. Usually are assigned with a value of 2.
        
    // Auxiliary list to randomize initial path
    List<Integer> auxiliaryList = new ArrayList<Integer>();
    
    // Construct
	public ParticleSwarmOptimization(CreateData matrix, int iterations, int maximumParticles) {
		
    	this.matrix = matrix;
    	minimumDistance = Double.POSITIVE_INFINITY;
    	
    	iterationNumber = iterations;
        locationNumber = matrix.getLocationNumber();        
        numberParticles = maximumParticles;
        
        
        
        System.out.println("-------PARTICLE SWARM OPTIMIZATION------");        
        System.out.println("Number of iterations: " + iterationNumber);
        System.out.println("Number of particles: " + numberParticles);
        
        // Initialize particle list, velocities, and locations
        initializeParticleList();
        initilizeVelocites();
        initilizeLocations();
        
        // Weight to regulate the global search. 
        // Adjust dynamically from max to min during the optimization to stimulate a local search behavior
        // Suggested range 0.2 <= w <= 0.4
        double w;
        
        // Loop for iterations
        for(int iter = 1; iter <= iterations ; iter++)
        {
        	
            for(int i = 0 ; i < this.particlesList.size()-1 ; i ++)
                this.particlesList.get(i).getBestPath();
                
            sortParticles();
            if(matrix.calculateDistance(this.particlesList.get(0).bestPath) < minimumDistance)
            {
            	bestPath = this.particlesList.get(0).bestPath;
            	minimumDistance = matrix.calculateDistance(this.particlesList.get(0).bestPath);
            }
  
            // Dynamically update w
            w = 0.4 - (0.2 * (((double) iter) / iterationNumber) );
            
            // Update particles
            for(int i = 0 ; i < this.particlesList.size() - 1 ; i++)
            {
                for(int j = 0 ; j< locationNumber - 1 ; j++)
                {
                    
                	// Assign two random doubles to calculate new velocity
                	Random rand = new Random();
                    double rand1 = rand.nextDouble();
                    double rand2 = rand.nextDouble();
                    
                    // Retrieve velocity and location 
                    double velocity = this.particlesList.get(i).velocity.getVelocity().get(j);                                
                    double location = this.particlesList.get(i).location.getLocations().get(j);
                    
                    // Retrieve best location from particle
                    double particleBestLocation = this.particlesList.get(i).locationPBest.getLocations().get(j);
                    
                    // Retrieve best overall location
                    double bestLocation = this.getBestLocation.getLocations().get(j);
                    
                    // Calculate new velocity
                    double newVelocity = (w * velocity) + (rand1 * c1) * (particleBestLocation - location) + (rand2 * c2) * (bestLocation - location);
                    
                    // Set new velocity
                    this.particlesList.get(i).velocity.getVelocity().set(j,newVelocity);
                    
                    // Calculate new location
                    double newLocation = location + newVelocity;
                    
                    // Set and swap with new location
                    this.particlesList.get(i).location.locations.set(j,newLocation);
                    this.particlesList.get(i).swapWithLocation((int)Math.abs(location - newLocation));
                }
            }
        }
        
        System.out.println("Minimum distance identified = " + minimumDistance);
        System.out.println("Best path identified: ");
        HelperFunctions.printBestPath(bestPath);   
	}
	
	// Initialize particle list
    private void initializeParticleList() 
    {    	
    	// Initiate particle list
    	int[][] particlePath = new int[numberParticles][locationNumber + 1];
    	
    	// Add locations to the list auxiliary list
        for (int i = 1; i < locationNumber; i++) 
        	auxiliaryList.add(i);
        
        // Add locations for each particle
        for (int i = 0; i < numberParticles; i++) 
        {
        	
        	// Shuffle the locations for each particle
        	Collections.shuffle(auxiliaryList);
        	
	        // Save shuffled list for each particle
        	
        	particlePath[i][0] = 0; // Initial location
	        for (int j = 1; j < locationNumber; j++) 
	        	particlePath[i][j] = auxiliaryList.get(j-1);
	        
	        // Final destination  for each particle (return to initial)
	        particlePath[i][locationNumber] = 0; 
	        
	        this.particlesList.add(new PSOParticle(matrix, particlePath[i]));
        }
    }
    
    // Sorts particles based on their best distance
    public void sortParticles() 
    {
        for (int i = 0; i < numberParticles-1; i++)
            for (int j = 0; j < numberParticles-i-1; j++)
                if (matrix.calculateDistance(this.particlesList.get(j).bestPath) > matrix.calculateDistance(this.particlesList.get(j+1).bestPath))
                {
                    PSOParticle auxiliar = this.particlesList.get(j);
                    this.particlesList.set(j,this.particlesList.get(j+1));
                    this.particlesList.set(j+1,auxiliar);
                }
    }
    
    // Initialize velocities
    public void initilizeVelocites()
    {
        for(PSOParticle particle : this.particlesList)
            for(int i = 0 ; i <locationNumber - 1 ; i++)
            {
            	Random rand = new Random();
            	particle.velocity.velocityDimensions.add(rand.nextDouble() * 2.0 - 1.0);
            }
    }

    // Initialize locations
    public void initilizeLocations()
    {
        for(PSOParticle particle : this.particlesList)
            for(int i = 0 ; i < locationNumber - 1 ; i++)
            {
            	particle.location.locations.add((double)i);
            	particle.locationPBest.locations.add((double)i);
            }
        for (int i = 0 ; i < locationNumber - 1 ; i++)
            getBestLocation.locations.add((double)i);
    }
    
}


