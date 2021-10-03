// -------------------------------------------------------------- -*- C++ -*-
// File: CreateTabuList.java
// --------------------------------------------------------------------------
// Developed by Daniel Zuniga
// Date: 09/26/2021
// Version 1.0.0
//
// Java Version: Metaheuristic Examples - Traveling Salesman Problem
// --------------------------------------------------------------------------

package Metaheuristics;

public class CreateTabuList {

    int [][] tabuList ;

    // Creates a tabu list of 0s
    public CreateTabuList(int locationNumber)
    {
        tabuList = new int[locationNumber][locationNumber];
    }

    // Makes the tabu swap and add x to the tabu list 
    // TODO: sensitivity analysis of the tabu increase can be performed
    // to determine performance
    public void incrementTabu(int locationA, int locationB)
    { 
        tabuList[locationA][locationB]+= 5;
        tabuList[locationB][locationA]+= 5;
    }

    // Decrease the tabu list 
    public void decrementTabu()
    {
        for(int i = 0; i<tabuList.length; i++)
            for(int j = 0; j<tabuList.length; j++)
                tabuList[i][j]-=tabuList[i][j]<=0?0:1;        
    }	
}
