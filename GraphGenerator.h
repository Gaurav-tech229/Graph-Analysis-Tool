// File: include/GraphGenerator.h
#pragma once

#include "GraphAnalysisTool.h"
#include <random>
#include <chrono>

class GraphGenerator {
public:
    // Graph types
    enum class GraphType {
        RANDOM,         // Random graph with given density
        COMPLETE,       // Complete graph where every node connects to every other
        BIPARTITE,     // Bipartite graph with two sets of nodes
        TREE,          // Tree structure
        SCALE_FREE,    // Scale-free network (Barabási-Albert model)
        SMALL_WORLD,   // Small-world network (Watts-Strogatz model)
        GRID           // 2D grid network
    };

    // Graph generation options
    struct GraphOptions {
        int numNodes = 10;              // Number of nodes
        double density = 0.3;           // Edge density for random graphs
        std::pair<int, int> weightRange = { 1, 10 };  // Range for edge weights
        bool directed = true;           // Directed or undirected
        double rewireProbability = 0.1; // For small-world networks
        int gridRows = 3;              // For grid networks
        int gridCols = 3;              // For grid networks
        int branchingFactor = 2;       // For trees
    };

    static GraphAnalysisTool generateGraph(GraphType type, const GraphOptions& options);

private:
    static std::mt19937 getRNG() {
        return std::mt19937(std::chrono::system_clock::now().time_since_epoch().count());
    }

    static GraphAnalysisTool generateRandomGraph(const GraphOptions& options);
    static GraphAnalysisTool generateCompleteGraph(const GraphOptions& options);
    static GraphAnalysisTool generateBipartiteGraph(const GraphOptions& options);
    static GraphAnalysisTool generateTreeGraph(const GraphOptions& options);
    static GraphAnalysisTool generateScaleFreeGraph(const GraphOptions& options);
    static GraphAnalysisTool generateSmallWorldGraph(const GraphOptions& options);
    static GraphAnalysisTool generateGridGraph(const GraphOptions& options);
};