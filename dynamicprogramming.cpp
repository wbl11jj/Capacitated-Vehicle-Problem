#include <iostream>
#include <vector>
#include <climits>
#include <chrono>
#include <algorithm> // Include this header for std::reverse
using namespace std;

int total_cost = 0;
// Function to calculate the route cost based on the distance matrix
int calculate_route_cost(const std::vector<int>& route, const std::vector<std::vector<int>>& distances) {
    int cost = 0;
    for (int i = 0; i < route.size() - 1; ++i) {
        cost += distances[route[i]][route[i + 1]]; // Add the distance between consecutive stops
    }
    return cost;
}

// Function to split the sequence of customers into routes using dynamic programming
std::vector<std::vector<int>> split_into_routes_dp(const std::vector<int>& demand, int vehicle_capacity, const std::vector<std::vector<int>>& distances) {
    int n = demand.size() - 1; // Number of customers (excluding the depot)
    std::vector<int> dp(n + 1, INT_MAX);  // dp[i] stores the minimum cost to service the first i customers
    std::vector<int> previous(n + 1, -1); // Stores the last customer for the optimal split
    dp[0] = 0; // Base case: no cost for servicing zero customers

    // Calculate the minimum cost for each subset of customers
    for (int i = 1; i <= n; ++i) {
        int load = 0;
        for (int j = i; j > 0; --j) {
            load += demand[j];
            if (load > vehicle_capacity) break; // Stop if the load exceeds the vehicle capacity
            
            // Calculate the route cost for the current segment (from j-1 to i)
            std::vector<int> route = {0}; // Route starts at depot
            for (int k = j; k <= i; ++k) {
                route.push_back(k); // Add customers to the route
            }
            route.push_back(0); // End route at depot
            
            int route_cost = calculate_route_cost(route, distances);
            if (dp[j - 1] + route_cost < dp[i]) { // Update the dp value if the cost is lower
                dp[i] = dp[j - 1] + route_cost;
                previous[i] = j - 1; // Store the split point
            }
        }
    }

    // Reconstruct the routes from the dp table
    std::vector<std::vector<int>> routes;
    int i = n;
    while (i > 0) {
        int j = previous[i];
        std::vector<int> route = {0}; // Start the route from the depot
        for (int k = j + 1; k <= i; ++k) {
            route.push_back(k); // Add customers to the route
        }
        route.push_back(0); // End the route at the depot
        routes.push_back(route);
        i = j;
    }
    std::reverse(routes.begin(), routes.end()); // Reverse to get the routes in correct order

    return routes;
}

// Function to calculate the total cost of all routes based on the distance matrix
int calculate_total_cost(const std::vector<std::vector<int>>& routes, const std::vector<std::vector<int>>& distances) {
    int total_cost = 0;
    for (const auto& route : routes) {
        total_cost += calculate_route_cost(route, distances);  // Sum up the costs of all routes
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

    for(int i = 1; i <= 1000; i++){
    // Calculate optimal routes using the dynamic programming approach
    std::vector<std::vector<int>> routes = split_into_routes_dp(demand, vehicle_capacity, distances);

    // Calculate the total cost
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