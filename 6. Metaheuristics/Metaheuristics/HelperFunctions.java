package Metaheuristics;

import java.util.Random;

public class HelperFunctions {
	
    // Prints best path identified
    static void printBestPath(int[] bestPath) 
    {
        for (int i = 0; i < bestPath.length; i++) 
            System.out.print(bestPath[i] + " ");
        
        System.out.println();
    }

	// Determine random location based on a Uniform distribution
	public static int randomLocation(int min , int max) 
	{
		Random rand = new Random();
		double location = min + rand.nextDouble() * (max - min);
		return (int)location;
	}
}
