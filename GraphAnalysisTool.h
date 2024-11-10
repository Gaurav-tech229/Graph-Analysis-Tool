// File: include/GraphAnalysisTool.h
#pragma once

#include <iostream>
#include <set>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <tuple>
#include "PerformanceMetrics.h"
#include "Timer.h"
#include "UnionFind.h"

class GraphAnalysisTool {
private:
    std::unordered_map<int, std::vector<std::pair<int, int>>> adjacencyList;
    std::unordered_set<int> nodes;
    int numNodes;
    int numEdges;
    std::unordered_map<std::string, PerformanceMetrics> metrics;

    void dfsUtil(int node, std::set<int>& visited, std::vector<int>& component);

public:
    GraphAnalysisTool();

    // Graph structure operations
    void addEdge(int from, int to, int weight = 1);
    std::string printGraphStructure() const;

    // Path finding operations
    std::string findShortestPath(int start, int end);
    std::pair<std::vector<int>, int> shortestPath(int start, int end);

    // Community detection
    std::string findCommunitiesOutput();
    std::vector<std::vector<int>> findCommunities();

    // Maximum flow
    std::string findMaxFlowOutput(int source, int sink);
    std::pair<int, std::vector<std::pair<std::vector<int>, int>>> maxFlow(int source, int sink);

    // Minimum spanning tree
    std::vector<std::tuple<int, int, int>> findMST();
    std::string getMSTOutput();

    // Performance and output
    std::string getPerformanceMetrics() const;
    std::string generateFullAnalysis(int startNode, int endNode);
    void exportToDOT(const std::string& filename);

    friend class Timer;

    void addNode(int node);
    std::vector<int> getIsolatedNodes() const;
    bool isNodeIsolated(int node) const;

    const std::unordered_map<int, std::vector<std::pair<int, int>>>& getAdjacencyList() const {
        return adjacencyList;
    }

    const std::unordered_set<int>& getNodes() const {
        return nodes;
    }

    int getNumNodes() const {
        return numNodes;
    }

    int getNumEdges() const {
        return numEdges;
    }
};