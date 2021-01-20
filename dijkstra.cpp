#include <iostream>
#include "digraph.h"
#include "wdigraph.h"
#include "dijkstra.h"
#include "heap.h"

using namespace std;

void dijkstra(const WDigraph& graph, int startVertex, unordered_map<int, PIL>& tree) {
    /* This function uses dijkstra algorithm to find the shortest path.
    Args:
        graph: an instance of the weighted graph class;
        startVertex: given vertex that serves as the root of the search tree;
        tree: a search tree used to construct the least cost path to some vertex;
    Returns:
        none;
    */
    BinaryHeap<PIL, int> fires;
    fires.insert(PIL(-1, startVertex), 0);
    
    while (fires.size() > 0) {     
        auto earliestFire = fires.min();  
        int v = earliestFire.item.second, u = earliestFire.item.first, d = earliestFire.key;
        fires.popMin();

        if (tree.find(v) != tree.end()) {
            continue;
        }

        tree[v] = PIL(u, d);

        for (auto iter = graph.neighbours(v); iter != graph.endIterator(v); iter++) {
            int nbr = *iter;
            int burn = d + graph.getCost(v, nbr);
            fires.insert(PIL(v, nbr), burn);
        }
    }
}