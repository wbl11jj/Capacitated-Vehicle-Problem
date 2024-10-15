#include <bits/stdc++.h>
#include <climits>
#include <chrono>
#include <iomanip>
   
#define ll long long
#define pb push_back
#define F first
#define S second
#define nl '\n'
#define all(x) x.begin(), x.end()
   
const int N = 1e5 + 7;
const int modd = 1e9 + 7;
const int INF = 1e9 + 7;
const double pi = 3.141592653589793238462643383279502884197;
using namespace std;

int n, m, k;
ll d[1007][1007];
vector<pair<int, int>> v[N];
ll a[N];
ll ans;
ll t_routes;
ll t_stops;

// Function to simulate the delivery process
void f(int i, int u, ll c){
    while(a[i] > 0){
        // cout << u << " " << i << nl;
        ans += d[u][i];
        int x = min(c, a[i]);
        a[i] -= x;
        c -= x;
        if(c > 0){
            bool ok = 0;
            for(int j = i + 1; j <= n; j++){
                if(d[i][j] <= d[i][1] && a[j] > 0){
                    f(j, i, c);
                    ok = 1;
                }
            }
            if(ok == 0){ 
                ans += d[i][1];
                t_routes++;
                // cout << i << " " << 1 << nl;
            }
        }
        else{
            ans += d[i][1];
            c = k;
            t_routes++;
            // cout << i << " " << 1 << nl;
        }
    }
}

// Solve function that processes the problem logic
void solve(){
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
    auto start = std::chrono::high_resolution_clock::now();

    for(int i = 1; i <= 10000; i++){
        n = individual.size();  // Number of customers
        k = vehicle_capacity;   // Vehicle capacity

        // Copy the demand to the global array a
        for(int i = 1; i <= n; i++){
            a[i] = demand[i];
        }

        // Initialize distance matrix
        for(int i = 1; i <= n; i++){
            for(int j = 1; j <= n; j++){
                d[i][j] = distances[i][j];
            }
        }

        ans = 0;
        t_routes = 0;

        // Process deliveries
        for(int i = 1; i <= n; i++){
            f(i, 0, k);  // Start delivery from the depot (node 0)
        }
    }
    auto stop = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = stop - start;
    // Output results
    cout << "Total distance traveled: " << ans << nl;
    cout << "Number of routes: " << t_routes << nl;
    std::cout << "Time taken: " << duration.count() << " seconds\n";
}
   
int main(){
    ios_base :: sync_with_stdio(0);
    cin.tie(0), cout.tie(0);
    
    int t = 1;
    while(t--){
        solve();
    }
    return 0;
}