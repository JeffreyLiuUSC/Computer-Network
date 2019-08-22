/**
 * Reference: https://www.geeksforgeeks.org/bellman-ford-algorithm-dp-23/
 * Line 7~13, 21~25, 34~43, 51~57 use part of the code in reference. 
 */
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

struct Edge {
    // source node value
    int src;
    // destination node value
    int dest;
    // weight of the edge
    int weight;
    Edge(int a, int b, int c) {
        src = a;
        dest = b;
        weight = c;
    }
};

struct Graph {
    // number of nodes
    int v;
    // number of edges
    int e;
    // array of edges
    vector<Edge> edges;
    Graph(int v) {
        this->v = v;
        e = 0;
    }
};

void bellman_ford(Graph& graph) {
    int v = graph.v;
    int e = graph.e;
    // The array distance store the distance from source node to destination node
    int distance[v];
    // Initialize the distance array
    distance[0] = 0;
    for(int i = 1; i < v; i++) {
        distance[i] = INT_MAX;
    }
    // The array parent store the parent of each node
    int parent[v];
    // Initialize the parent array
    for(int i = 0; i < v; i++) {
        parent[i] = 0;
    }
    // We need to do the loop (v - 1)times
    for(int i = 0; i < v - 2; i++) {
        for(int j = 0; j < e; j++) {
            int src = graph.edges[j].src;
            int dest = graph.edges[j].dest;
            int weight = graph.edges[j].weight;
            if(distance[src] != INT_MAX && distance[src] + weight < distance[dest]) {
                distance[dest] = distance[src] + weight;
                parent[dest] = src;
            }
        }
    }
    // Write the result to txt file.
    FILE* output;
    output = fopen("output2.txt", "w");
    // Write the distance from node 0 to all other nodes.
    for(int i = 0; i < v - 1; i++) {
        fprintf(output, "%d", distance[i]);
        fprintf(output, "%s", ",");
    }
    fprintf(output, "%d\n", distance[v - 1]);
    // Detect negative loop. Do the loop one more time. If we got a shorter path, there is negative loop.
    for(int i = 0; i < e; i++) {
        int src = graph.edges[i].src;
        int dest = graph.edges[i].dest;
        int weight = graph.edges[i].weight;
        if(distance[src] != INT_MAX && distance[src] + weight < distance[dest]) {
            cout << "Negative Loop Detected." << endl;
        }
    }
    // Print the path of each node from node0.
    vector<int> path;
    fprintf(output, "%d\n", 0);
    for(int i = 1; i < v; i++) {
        int j = i;
        while(parent[j] != 0) {
            path.push_back(parent[j]);
            j = parent[j];
        }
        fprintf(output, "%d", 0);
        fprintf(output, "%s", "->");
        while(!path.empty()) {
            fprintf(output, "%d", path.back());
            fprintf(output, "%s", "->");
            path.pop_back();
        }
        fprintf(output, "%d\n", i);
    }
}

int main() {
    ifstream in("input2.txt");
    string filename;
    string my_line;
    vector<string> buffer;
    while(getline(in, my_line)) {
        buffer.push_back(my_line);
    }
    Graph graph(buffer.size());
    for(int i = 0; i < buffer.size(); i++) {
        string line = buffer[i];
        if(line[line.size() - 1] == '\r') {
            line.pop_back();
        }
        // The destination_node
        int des_node = 0;
        while(line.find(",") != line.npos) {
            int comma_pos = line.find(",");
        // if *, no edge
            if(line.substr(0, comma_pos) == "*") {
                des_node++;
                line = line.substr(comma_pos + 1, line.size() - comma_pos - 1);
                continue;
            }else {
                int dest = stoi(line.substr(0, comma_pos));
                line = line.substr(comma_pos + 1, line.size() - comma_pos - 1);
                // If dest != 0, this is a valid edge
                if(dest != 0) {
                    graph.edges.push_back(Edge(i, des_node, dest));
                    des_node++;
                }else {
                    des_node++;
                }
            }

        }
        // Add the last edge of this line
        if(line != "*" && line != "0") {
            graph.edges.push_back(Edge(i, des_node, stoi(line)));
        }
    }
    graph.e = graph.edges.size();
    bellman_ford(graph);
}