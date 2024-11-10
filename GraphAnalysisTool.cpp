// File: src/GraphAnalysisTool.cpp
#include "GraphAnalysisTool.h"
#include <queue>
#include <set>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <limits>

GraphAnalysisTool::GraphAnalysisTool() : numNodes(0), numEdges(0) {}

void GraphAnalysisTool::addEdge(int from, int to, int weight) {
    // Add both nodes to ensure they're tracked
    addNode(from);
    addNode(to);

    adjacencyList[from].push_back({ to, weight });
    numEdges++;
}

std::string GraphAnalysisTool::printGraphStructure() const {
    std::stringstream ss;
    ss << "Graph has " << numNodes << " nodes and " << numEdges << " edges.\n";
    ss << "Adjacency List:\n";

    for (const auto& [node, neighbors] : adjacencyList) {
        ss << "Node " << node << " -> [";
        for (size_t i = 0; i < neighbors.size(); ++i) {
            const auto& [dest, weight] = neighbors[i];
            ss << "(Node " << dest << ", Weight " << weight << ")";
            if (i < neighbors.size() - 1) ss << ", ";
        }
        ss << "]\n";
    }
    return ss.str();
}

std::string GraphAnalysisTool::findShortestPath(int start, int end) {
    Timer timer("Shortest Path", *this);
    std::stringstream ss;

    auto [path, distance] = shortestPath(start, end);

    if (path.empty()) {
        ss << "No path exists between Node " << start << " and Node " << end << "\n";
        return ss.str();
    }

    ss << "Shortest path from Node " << start << " to Node " << end << ": ";
    for (size_t i = 0; i < path.size(); i++) {
        ss << path[i];
        if (i < path.size() - 1) ss << " -> ";
    }
    ss << "\nTotal Path Weight: " << distance << "\n";

    return ss.str();
}

std::pair<std::vector<int>, int> GraphAnalysisTool::shortestPath(int start, int end) {
    std::vector<int> distances(numNodes, std::numeric_limits<int>::max());
    std::vector<int> previous(numNodes, -1);
    std::priority_queue<std::pair<int, int>,
        std::vector<std::pair<int, int>>,
        std::greater<>> pq;

    distances[start] = 0;
    pq.push({ 0, start });
    metrics["Shortest Path"].nodesProcessed = 0;

    while (!pq.empty()) {
        int current = pq.top().second;
        int dist = pq.top().first;
        pq.pop();
        metrics["Shortest Path"].nodesProcessed++;

        if (dist > distances[current]) continue;

        for (const auto& [next, weight] : adjacencyList[current]) {
            if (distances[current] + weight < distances[next]) {
                distances[next] = distances[current] + weight;
                previous[next] = current;
                pq.push({ distances[next], next });
            }
        }
    }

    std::vector<int> path;
    if (distances[end] != std::numeric_limits<int>::max()) {
        for (int at = end; at != -1; at = previous[at]) {
            path.push_back(at);
        }
        std::reverse(path.begin(), path.end());
    }

    return { path, distances[end] };
}

void GraphAnalysisTool::dfsUtil(int node, std::set<int>& visited, std::vector<int>& component) {
    visited.insert(node);
    component.push_back(node);
    metrics["Community Detection"].nodesProcessed++;

    for (const auto& [next, _] : adjacencyList[node]) {
        if (visited.find(next) == visited.end()) {
            dfsUtil(next, visited, component);
        }
    }
}

std::string GraphAnalysisTool::findCommunitiesOutput() {
    Timer timer("Community Detection", *this);
    std::stringstream ss;

    auto communities = findCommunities();
    ss << "Communities found: " << communities.size() << "\n";

    // Track nodes in communities
    std::set<int> nodesInCommunities;

    // Output non-isolated communities
    int communityCount = 0;
    for (const auto& community : communities) {
        if (community.size() > 1 || !isNodeIsolated(community[0])) {
            communityCount++;
            ss << "Community " << communityCount << " [size=" << community.size() << "]: [";
            for (size_t i = 0; i < community.size(); i++) {
                ss << "Node " << community[i];
                nodesInCommunities.insert(community[i]);
                if (i < community.size() - 1) ss << ", ";
            }
            ss << "]\n";
        }
    }

    // Find and output isolated nodes
    auto isolated = getIsolatedNodes();
    if (!isolated.empty()) {
        ss << "\nIsolated Nodes [" << isolated.size() << "]: [";
        for (size_t i = 0; i < isolated.size(); i++) {
            ss << "Node " << isolated[i];
            if (i < isolated.size() - 1) ss << ", ";
        }
        ss << "]\n";
    }

    // Find and output self-loops
    ss << "\nSelf-loop Nodes: [";
    bool first = true;
    for (const auto& [node, edges] : adjacencyList) {
        for (const auto& [dest, _] : edges) {
            if (dest == node) {
                if (!first) ss << ", ";
                ss << "Node " << node;
                first = false;
                break;
            }
        }
    }
    ss << "]\n";

    return ss.str();
}

std::vector<std::vector<int>> GraphAnalysisTool::findCommunities() {
    std::vector<std::vector<int>> communities;
    std::set<int> visited;
    metrics["Community Detection"].nodesProcessed = 0;

    // First, find connected components
    for (const auto& node : nodes) {
        if (visited.find(node) == visited.end() && !isNodeIsolated(node)) {
            std::vector<int> component;
            dfsUtil(node, visited, component);
            if (!component.empty()) {
                communities.push_back(component);
            }
        }
    }

    // Then, add isolated nodes as single-node communities
    auto isolated = getIsolatedNodes();
    for (int node : isolated) {
        communities.push_back({ node });
    }

    return communities;
}

std::string GraphAnalysisTool::findMaxFlowOutput(int source, int sink) {
    Timer timer("Maximum Flow", *this);
    std::stringstream ss;

    auto [maxFlowValue, flowPaths] = maxFlow(source, sink);

    ss << "Maximum Flow from Node " << source << " to Node " << sink << ": "
        << maxFlowValue << "\n";
    ss << "Paths contributing to max flow:\n";

    for (const auto& [path, flow] : flowPaths) {
        for (size_t i = 0; i < path.size(); i++) {
            ss << path[i];
            if (i < path.size() - 1) ss << " -> ";
        }
        ss << " (Flow " << flow << ")\n";
    }

    return ss.str();
}

std::pair<int, std::vector<std::pair<std::vector<int>, int>>>
GraphAnalysisTool::maxFlow(int source, int sink) {
    std::unordered_map<int, std::unordered_map<int, int>> residualGraph;
    std::vector<std::pair<std::vector<int>, int>> flowPaths;

    // Initialize residual graph
    for (const auto& [node, edges] : adjacencyList) {
        for (const auto& [dest, capacity] : edges) {
            residualGraph[node][dest] = capacity;
        }
    }

    int maxFlowValue = 0;
    metrics["Maximum Flow"].nodesProcessed = 0;

    while (true) {
        std::vector<int> parent(numNodes, -1);
        std::queue<int> q;
        q.push(source);
        parent[source] = source;

        while (!q.empty() && parent[sink] == -1) {
            int current = q.front();
            q.pop();
            metrics["Maximum Flow"].nodesProcessed++;

            for (const auto& [next, capacity] : residualGraph[current]) {
                if (parent[next] == -1 && capacity > 0) {
                    parent[next] = current;
                    q.push(next);
                }
            }
        }

        if (parent[sink] == -1) break;

        int pathFlow = std::numeric_limits<int>::max();
        std::vector<int> path;
        for (int v = sink; v != source; v = parent[v]) {
            path.push_back(v);
            pathFlow = std::min(pathFlow, residualGraph[parent[v]][v]);
        }
        path.push_back(source);
        std::reverse(path.begin(), path.end());

        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            residualGraph[u][v] -= pathFlow;
            residualGraph[v][u] += pathFlow;
        }

        maxFlowValue += pathFlow;
        flowPaths.push_back({ path, pathFlow });
    }

    return { maxFlowValue, flowPaths };
}

std::vector<std::tuple<int, int, int>> GraphAnalysisTool::findMST() {
    Timer timer("Minimum Spanning Tree", *this);
    std::vector<std::tuple<int, int, int>> edges;

    // Collect all edges
    for (const auto& [from, neighbors] : adjacencyList) {
        for (const auto& [to, weight] : neighbors) {
            edges.emplace_back(from, to, weight);
        }
    }

    // Sort edges by weight
    std::sort(edges.begin(), edges.end(),
        [](const auto& a, const auto& b) {
            return std::get<2>(a) < std::get<2>(b);
        });

    UnionFind uf(numNodes);
    std::vector<std::tuple<int, int, int>> mst;

    for (const auto& [from, to, weight] : edges) {
        if (uf.find(from) != uf.find(to)) {
            uf.unite(from, to);
            mst.emplace_back(from, to, weight);
            metrics["Minimum Spanning Tree"].nodesProcessed++;
        }
    }

    return mst;
}

std::string GraphAnalysisTool::getMSTOutput() {
    std::stringstream ss;
    auto mst = findMST();

    ss << "Minimum Spanning Tree:\n";
    int totalWeight = 0;

    for (const auto& [from, to, weight] : mst) {
        ss << "  (Node " << from << ", Node " << to
            << ", Weight " << weight << ")\n";
        totalWeight += weight;
    }

    ss << "Total MST Weight: " << totalWeight << "\n";
    return ss.str();
}

std::string GraphAnalysisTool::getPerformanceMetrics() const {
    std::stringstream ss;
    ss << "Performance Metrics:\n";
    for (const auto& [operation, metric] : metrics) {
        ss << operation << " Performance:\n"
            << "  Execution Time: " << std::fixed << std::setprecision(3)
            << metric.executionTime.count() << " seconds\n"
            << "  Nodes Processed: " << metric.nodesProcessed << "\n"
            << "  Memory Used: " << std::fixed << std::setprecision(2)
            << (metric.memoryUsed / 1024.0) << " KB\n";
    }
    return ss.str();
}

std::string GraphAnalysisTool::generateFullAnalysis(int startNode, int endNode) {
    std::stringstream ss;
    ss << "--- Graph Analysis Summary ---\n\n";

    // Graph structure
    ss << "1. " << printGraphStructure() << "\n";

    // Shortest path
    ss << "2. " << findShortestPath(startNode, endNode) << "\n";

    // Communities
    ss << "3. " << findCommunitiesOutput() << "\n";

    // Maximum flow
    ss << "4. " << findMaxFlowOutput(startNode, endNode) << "\n";

    // Minimum Spanning Tree
    ss << "5. " << getMSTOutput() << "\n";

    // Performance metrics
    ss << "6. " << getPerformanceMetrics() << "\n";

    // Visualization info
    ss << "Graph data saved to 'graph.dot' for visualization.\n";

    return ss.str();
}

void GraphAnalysisTool::exportToDOT(const std::string& filename) {
    std::ofstream out(filename);
    out << "digraph G {\n";

    for (const auto& [node, edges] : adjacencyList) {
        for (const auto& [dest, weight] : edges) {
            out << "    " << node << " -> " << dest
                << " [label=\"" << weight << "\"];\n";
        }
    }

    out << "}\n";
    out.close();
}

void GraphAnalysisTool::addNode(int node) {
    nodes.insert(node);
    numNodes = std::max(numNodes, node + 1);
    // Ensure node exists in adjacency list even if it has no edges
    if (adjacencyList.find(node) == adjacencyList.end()) {
        adjacencyList[node] = std::vector<std::pair<int, int>>();
    }
}


std::vector<int> GraphAnalysisTool::getIsolatedNodes() const {
    std::vector<int> isolated;
    for (int node : nodes) {
        if (isNodeIsolated(node)) {
            isolated.push_back(node);
        }
    }
    return isolated;
}

bool GraphAnalysisTool::isNodeIsolated(int node) const {
    // Check if node has any incoming or outgoing edges
    if (adjacencyList.find(node) == adjacencyList.end() ||
        adjacencyList.at(node).empty()) {
        // Check for incoming edges
        for (const auto& [src, edges] : adjacencyList) {
            for (const auto& [dest, _] : edges) {
                if (dest == node) return false;
            }
        }
        return true;
    }
    return false;
}