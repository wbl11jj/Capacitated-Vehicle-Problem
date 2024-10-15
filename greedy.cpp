#include <iostream>
#include <vector>
#include <chrono>
using namespace std;

int total_cost = 0;
// Greedy solution
// Function to split a sequence of customers (an 'individual') into feasible routes
// based on their demands and the vehicle's capacity.
std::vector<std::vector<int>> split_into_routes(const std::vector<int>& individual, const std::vector<int>& demand, int vehicle_capacity) {
    std::vector<std::vector<int>> routes;  // Stores the list of all routes
    std::vector<int> route = {0};          // Start each route from the depot (represented by 0)
    int load = 0;                          // Current load of the vehicle

    // Iterate through each customer in the sequence
    for (int customer : individual) {
        // Check if adding this customer's demand would exceed the vehicle's capacity
        if (load + demand[customer] > vehicle_capacity) {
            // If yes, complete the current route by returning to the depot
            route.push_back(0);
            routes.push_back(route);       // Store the completed route
            route = {0, customer};         // Start a new route with the current customer
            load = demand[customer];       // Reset the load to the current customer's demand
        } else {
            // If no, add the customer to the current route
            route.push_back(customer);
            load += demand[customer];      // Update the current load of the vehicle
        }
    }
    // After the loop, complete the last route and add it to the list of routes
    route.push_back(0);
    routes.push_back(route);

    return routes;  // Return the list of routes
}

// Function to calculate the total cost based on the distances between nodes
int calculate_total_cost(const std::vector<std::vector<int>>& routes, const std::vector<std::vector<int>>& distances) {
    int total_cost = 0;
    for (const auto& route : routes) {
        for (size_t i = 1; i < route.size(); ++i) {
            total_cost += distances[route[i-1]][route[i]];  // Sum the distances between consecutive stops
        }
    }
    return total_cost;
}

int main() {
    // Example data: sequence of customers and their respective demands
    vector<int> individual = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
    vector<int> demand = {0, 1, 2, 3, 4, 5, 6, 3, 2, 1, 5, 3, 4, 6, 7, 8, 2, 3, 5, 4, 6};  // Demand for customers 1 to 10, with 0 as depot
    int vehicle_capacity = 20;  // Vehicle capacity constraint

    // Distance matrix (assuming n+1 nodes: depot + 10 customers)
    vector<vector<int>> distances = {
        {0, 4, 8, 6, 7, 9, 10, 3, 11, 12, 4, 5, 6, 7, 8, 9, 10, 2, 11, 13, 14},
        {4, 0, 2, 6, 8, 7, 5, 9, 12, 5, 8, 7, 10, 9, 4, 3, 7, 8, 6, 11, 9},
        {8, 2, 0, 3, 5, 8, 6, 7, 10, 4, 3, 6, 8, 5, 6, 7, 4, 8, 7, 9, 12},
        {6, 6, 3, 0, 4, 7, 5, 9, 11, 6, 3, 5, 8, 7, 6, 9, 5, 4, 6, 8, 11},
        {7, 8, 5, 4, 0, 2, 6, 8, 9, 5, 4, 3, 7, 6, 5, 8, 7, 6, 3, 10, 8},
        {9, 7, 8, 7, 2, 0, 4, 6, 7, 8, 5, 6, 9, 7, 4, 3, 6, 9, 5, 11, 7},
        {10, 5, 6, 5, 6, 4, 0, 3, 6, 7, 8, 4, 3, 7, 6, 8, 5, 7, 9, 10, 5},
        {3, 9, 7, 9, 8, 6, 3, 0, 4, 5, 6, 7, 5, 8, 9, 7, 6, 3, 2, 11, 9},
        {11, 12, 10, 11, 9, 7, 6, 4, 0, 3, 2, 5, 8, 9, 7, 6, 3, 4, 5, 8, 6},
        {12, 5, 4, 6, 5, 8, 7, 5, 3, 0, 4, 2, 3, 6, 8, 7, 5, 4, 3, 9, 7},
        {4, 8, 3, 3, 4, 5, 8, 6, 2, 4, 0, 2, 5, 4, 3, 5, 6, 7, 8, 9, 4},
        {5, 7, 6, 5, 3, 6, 4, 7, 5, 2, 2, 0, 3, 6, 5, 7, 8, 9, 4, 6, 3},
        {6, 10, 8, 8, 7, 9, 3, 5, 8, 3, 5, 3, 0, 4, 7, 8, 9, 6, 3, 4, 2},
        {7, 9, 5, 7, 6, 7, 7, 8, 9, 6, 4, 6, 4, 0, 3, 5, 7, 8, 6, 3, 4},
        {8, 4, 6, 6, 5, 4, 6, 9, 7, 8, 3, 5, 7, 3, 0, 4, 6, 5, 3, 8, 6},
        {9, 3, 7, 9, 8, 3, 8, 7, 6, 7, 5, 7, 8, 5, 4, 0, 3, 7, 8, 9, 7},
        {10, 7, 4, 5, 7, 6, 5, 6, 3, 5, 6, 8, 9, 7, 6, 3, 0, 8, 6, 5, 4},
        {2, 8, 8, 4, 6, 9, 7, 3, 4, 4, 7, 9, 6, 8, 5, 7, 8, 0, 6, 4, 3},
        {11, 6, 7, 6, 3, 5, 9, 2, 5, 3, 8, 4, 3, 6, 3, 8, 6, 6, 0, 7, 4},
        {13, 11, 9, 8, 10, 11, 10, 11, 8, 9, 9, 6, 4, 3, 8, 9, 5, 4, 7, 0, 6},
        {14, 9, 12, 11, 8, 7, 5, 9, 6, 7, 4, 3, 2, 4, 6, 7, 4, 3, 4, 6, 0}
    };

    // Start timing the execution
    auto start = std::chrono::high_resolution_clock::now();

    for(int i = 1; i <= 10000; i++){
        // Calculate feasible routes using the split_into_routes function
        std::vector<std::vector<int>> routes = split_into_routes(individual, demand, vehicle_capacity);

        // Calculate the total cost based on distances
        total_cost = calculate_total_cost(routes, distances);
    }
    // Stop timing the execution
    auto stop = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = stop - start;
    // Print the generated routes
    // std::cout << "Routes:\n";
    // for (const auto& route : routes) {
    //     for (int stop : route) {
    //         std::cout << stop << " ";  // Print each stop in the route
    //     }
    //     std::cout << "\n";
    // }

    // Print the total cost
    std::cout << "Total cost (based on distances): " << total_cost << "\n";

    // Print the time taken to run the code
    std::cout << "Time taken: " << duration.count() << " seconds\n";

    return 0;
}