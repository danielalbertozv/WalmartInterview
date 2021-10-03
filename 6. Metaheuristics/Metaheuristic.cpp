// -------------------------------------------------------------- -*- C++ -*-
// File: Example_Walmart_Metaheuristics.cpp
// --------------------------------------------------------------------------
// Developed by Daniel Zuniga
// Date: 09/19/2021
// Version 1.0.0
//
// C++ Version: Heuristics - Tabus Search and Simulated Annealing
// --------------------------------------------------------------------------

// Libraries
#include <algorithm>	// For range of elements, subsets of elements
#include <assert.h>		// Diagnostic messages and verify program assumptions
#include <cmath>		// Perform math operations, sqrt(), log(), ...
#include <cstdlib>		// General purpose function, abs(), ...
#include <cstdint>
#include <ctime>		// Converts time to character string
#include <fstream>		// Open file for writing
#include <iostream>		// To read and write
#include <time.h>		// C library for ctime 
#include <stdio.h>	    // File input and output
#include <stdlib.h>		// Memory allocation, process control, conversion, ...
#include <string>		// String manipulation
#include <vector>

// Google OR-Tools imported libraries
#include "ortools/constraint_solver/routing.h"
#include "ortools/constraint_solver/routing_enums.pb.h"
#include "ortools/constraint_solver/routing_index_manager.h"
#include "ortools/constraint_solver/routing_parameters.h"


// Defines Google namespace, i.e., declarative region for identifiers (types, functions, variables, etc.)
namespace operations_research {

    // Creates structure DistParam (struct: can store multiple data-types)
    struct DistParam {
        // Creates two-dimensional vector distances (Matrix) (const: constrant, prevent programmer to modify it)
        const std::vector<std::vector<int64_t>> distances{
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

        const int machineryNo = 4;       // Assignes number of machinery  (const: constrant, prevent programmer to modify it)

        // Creates NodeIndex location, i.e., uses variables indices through its API,   (const: constrant, prevent programmer to modify it)
        const RoutingIndexManager::NodeIndex location{ 0 };
    };

    // Creates methos DisplaySolution wih 4 arguments (void: does not return value)
    void DisplaySolution(const DistParam& data, const RoutingIndexManager& manager, const RoutingModel& routing, const Assignment& solution) {

        // Print the objective value
        std::cout << "Objective: " << solution.ObjectiveValue() << std::endl;

        // Creates a 64 bit integer to store the maximum route distance
        int64_t max_route_distance{ 0 };

        // Loop for the machinery number
        for (int i = 0; i < data.machineryNo; ++i) {

            // Creates a 64 bit integer to store routing index of a particular machinery
            int64_t index = routing.Start(i);

            // Print machinery number
            std::cout << "Route of machinery  " << i << ":" << std::endl;

            // Creates a 64 bit integer to compare routing distance with maximum distance so far
            int64_t route_distance = { 0 };

            // Creates stringstream to read (print) as a stream
            std::stringstream route;

            // While loop to print the route for a machinery
            while (routing.IsEnd(index) == false) {
                route << manager.IndexToNode(index).value() << " -> ";

                // Auxiliary 64 bit integer to store previous index
                int64_t previous_index = index;

                // Updates index with next index value
                index = solution.Value(routing.NextVar(index));

                // Sums the distances from the route for a particular machinery
                route_distance += routing.GetArcCostForVehicle(previous_index, index, int64_t{ i });
            }

            // Prints route distance for each machinery
            std::cout << route.str() << manager.IndexToNode(index).value() << std::endl;
            std::cout << "Distance of the route: " << route_distance << "m" << std::endl;

            // Compares and updates the maximum distance
            max_route_distance = std::max(route_distance, max_route_distance);
        }

        //Print maximum distance and wall time
        std::cout << "Maximum of the route distances: " << max_route_distance << "m" << std::endl;
        std::cout << "Problem solved in " << routing.solver()->wall_time() << "ms" << std::endl;
        std::cout << "" << std::endl;
    }

    // Creates method HeuristicSearch (void: does not return value)
    void HeuristicSearch(int heuristicMethod) {

        DistParam mydata;    // Instantiate my data as struc Distparam     

        // Creates instance of RoutingIndexManager, manager (int number_of_nodes, int number_machinery, NodeIndex)
        RoutingIndexManager manager(mydata.distances.size(), mydata.machineryNo, mydata.location);

        // Creates multimachinery routing model, mymultiroute ( RoutingIndexManager )
        RoutingModel mymultiroute(manager);

        // Create and register a transit callback. 
        // (& operator: is reference, so changes will affect the variable or structure)
        // (-> operatior: allows aces to elements in struct, in this case my data) 
        const int transitIndex = mymultiroute.RegisterTransitCallback([&mydata, &manager](int64_t from_index, int64_t to_index) -> int64_t {

            // Convert from routing variable Index to distance matrix NodeIndex.
            auto from_vertex = manager.IndexToNode(from_index).value();
            auto to_vertex = manager.IndexToNode(to_index).value();

            // returns distances between locations
            return mydata.distances[from_vertex][to_vertex];
        }); //End transitIndex

        // Define cost of each edge or arc.
        mymultiroute.SetArcCostEvaluatorOfAllVehicles(transitIndex);

        // Add Distance constraint through AddDimension method(), e.i.,
        // computes cumulative distance traveled by each machinery along its route
        // 
        // Argument: int evaluator_index,    
        //               int64 slack_max,   upper bund of slack variable
        //                int64 capacity,   upper bound of cumulative variable
        //  bool fix_start_cumul_to_zero,   start cumul to zero
        //       const std::string& name    is the name used to reference the dimension
        // 
        mymultiroute.AddDimension(transitIndex, 0, 3000, true, "Distance");

        //  Set maximum distance for the routes, i.e.,
        //  Sets a cost proportional to the global dimension span, that is the
        //  difference between the largest value of route end cumul variables and
        //  the smallest value of route start cumul variables.
        mymultiroute.GetMutableDimension("Distance")->SetGlobalSpanCostCoefficient(100);

        // Sets the parameters for the search
        RoutingSearchParameters searchParameters = DefaultRoutingSearchParameters();

        // Assign metaheuristic local search option (AUTOMATIC, PATH_CHEAPEST_ARC, SAVINGS, SWEEP, among others)
        searchParameters.set_first_solution_strategy(FirstSolutionStrategy::PATH_CHEAPEST_ARC);

        // Set the limit number of solutions before stoppoing search
        searchParameters.set_solution_limit(30);

        // Set a time limit before stoping search (seconds)
        searchParameters.mutable_time_limit()->set_seconds(150);

        // switch to select the heuristic methos to escape local optimum
        switch (heuristicMethod) {
        case 0:
            std::cout << "----- TABU SEARCH -----" << std::endl;
            searchParameters.set_local_search_metaheuristic(LocalSearchMetaheuristic::TABU_SEARCH);
            break;
        case 1:
            std::cout << "----- SIMULATED ANNEALING -----" << std::endl;
            searchParameters.set_local_search_metaheuristic(LocalSearchMetaheuristic::SIMULATED_ANNEALING);
        } // End switch

        // Solve the problem.
        const Assignment* solution = mymultiroute.SolveWithParameters(searchParameters);

        // Checks if solutions exists and prints it to command window
        if (solution != nullptr) {
            // Calls method DisplaySolution
            DisplaySolution(mydata, manager, mymultiroute, *solution);
        }
        else {
            std::cout << "No solution identified" << std::endl;
        }
    } //End HeuristicSearch method

}  // namespace operations_research


// Creates AuthorDetials method, (void: no value returned)
void AuthorDetails() {
    std::cout << "--------------------------------------------------------" << std::endl;
    std::cout << "File: Example_Walmart_Metaheuristic.cpp" << std::endl;
    std::cout << "--------------------------------------------------------" << std::endl;
    std::cout << "Developed by Daniel Zuniga" << std::endl;
    std::cout << "Date: 09/19/2021" << std::endl;
    std::cout << "Version 1.0.0\n" << std::endl;
    std::cout << "C++ Version: Heuristics - Tabus Search and Simulated Annealing" << std::endl;
    std::cout << "--------------------------------------------------------\n" << std::endl;
} // End AuthorDetails

// Creates Main method, entry point of C++ (int: returns integer)
int main() {

    AuthorDetails();	// calls AuthorDetails method/function 

    // Calls method HeuristicSearch() from Google operations_research namespace to escape local minima as follows:
    // i=0, TABU_SEARCH
    // i=1, SIMULATED_ANNEALING
    for (int i = 0; i < 2; i++)
        operations_research::HeuristicSearch(i);

    return EXIT_SUCCESS; // Exit and abort, EXIT_SUCCESS 0

}// End Main
