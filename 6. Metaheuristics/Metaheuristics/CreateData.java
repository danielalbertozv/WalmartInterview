// -------------------------------------------------------------- -*- C++ -*-
// File: CreateData.java
// --------------------------------------------------------------------------
// Developed by Daniel Zuniga
// Date: 09/26/2021
// Version 1.0.0
//
// Java Version: Metaheuristic Examples - Traveling Salesman Problem
// --------------------------------------------------------------------------

package Metaheuristics;

import java.util.concurrent.ThreadLocalRandom;


public class CreateData {

    private int[][] distanceMatrix;
    private int locations;

    // Constructor
    // Given a number of locations (integer), creates a random distance matrix
    public CreateData(int locationNumber) 
    {
       locations = locationNumber;
       distanceMatrix = new int[locationNumber][locationNumber];
       createDistanceMatrix(locationNumber);
       
       /*
       int[][] distanceMatrixFixed = {
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
               {794,	1452,	905,	1630,	1493,	850,	576,	1027,	617,	562,	425,	1013,	876,	643,	233,	958,	0	},
           };
       
       distanceMatrix = distanceMatrixFixed;
       */
       
       printMatrix();
    }
    
    // Constructor
    public CreateData (int[][] matrix) {
    	locations = matrix.length;
    	distanceMatrix = matrix;
    }
  
    // Creates random distance matrix
    private void createDistanceMatrix(int locationNumber) 
    {
        for (int row = 0; row < locationNumber; row++) 
            for (int col = 0; col < locationNumber; col++) 
            	
            	// Avoid adding distance to the matrix diagonal
                if (row != col) 
                {
                	
                	// Saves random distance number 
                    int randomDistance = ThreadLocalRandom.current().nextInt(10, 99);
                    distanceMatrix[row][col] = randomDistance;
                    distanceMatrix[col][row] = randomDistance;
                }
            
        
    }
    
    // Get Edge between two location
    public int getEdge(int i, int j) 
    {
        return distanceMatrix[i][j];
    }
    
    // Get the number of locations
    public int getLocationNumber() 
    {
        return locations;
    }

    // Calculate the distance from the path
    public int calculateDistance(int currentPath[]) 
    {
        int cost = 0;
        for (int i = 0; i < currentPath.length - 1; i++) 
            cost += distanceMatrix[currentPath[i]][currentPath[i + 1]];

        return cost;
    }
    
    // Print distance matrix
    public void printMatrix() 
    {    	
    	System.out.println("The random distance matrix created:");
        for (int i = 0; i < locations; i++) 
        {
            for (int j = 0; j < locations; j++) 
                System.out.print(distanceMatrix[i][j] + "  ");

            System.out.println("\n ");
        }
    }
}
