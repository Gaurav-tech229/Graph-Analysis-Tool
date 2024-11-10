// File: src/GraphGenerator.cpp
#include "GraphGenerator.h"
#include <queue>
#include <algorithm>
#include <cmath>

GraphAnalysisTool GraphGenerator::generateGraph(GraphType type, const GraphOptions& options) {
    switch (type) {
    case GraphType::RANDOM:
        return generateRandomGraph(options);
    case GraphType::COMPLETE:
        return generateCompleteGraph(options);
    case GraphType::BIPARTITE:
        return generateBipartiteGraph(options);
    case GraphType::TREE:
        return generateTreeGraph(options);
    case GraphType::SCALE_FREE:
        return generateScaleFreeGraph(options);
    case GraphType::SMALL_WORLD:
        return generateSmallWorldGraph(options);
    case GraphType::GRID:
        return generateGridGraph(options);
    default:
        return GraphAnalysisTool();
    }
}

GraphAnalysisTool GraphGenerator::generateRandomGraph(const GraphOptions& options) {
    GraphAnalysisTool graph;
    auto rng = getRNG();
    std::uniform_real_distribution<> edgeDist(0.0, 1.0);
    std::uniform_int_distribution<> weightDist(options.weightRange.first, options.weightRange.second);

    // Add all nodes first
    for (int i = 0; i < options.numNodes; ++i) {
        graph.addNode(i);
    }

    // Add edges based on density
    for (int i = 0; i < options.numNodes; ++i) {
        for (int j = 0; j < options.numNodes; ++j) {
            if (i != j && edgeDist(rng) < options.density) {
                graph.addEdge(i, j, weightDist(rng));
                if (!options.directed && i < j) { // Add reverse edge for undirected graphs
                    graph.addEdge(j, i, weightDist(rng));
                }
            }
        }
    }
    return graph;
}

GraphAnalysisTool GraphGenerator::generateCompleteGraph(const GraphOptions& options) {
    GraphAnalysisTool graph;
    auto rng = getRNG();
    std::uniform_int_distribution<> weightDist(options.weightRange.first, options.weightRange.second);

    for (int i = 0; i < options.numNodes; ++i) {
        graph.addNode(i);
        for (int j = 0; j < i; ++j) {
            int weight = weightDist(rng);
            graph.addEdge(i, j, weight);
            if (!options.directed) {
                graph.addEdge(j, i, weight);
            }
        }
    }
    return graph;
}

GraphAnalysisTool GraphGenerator::generateBipartiteGraph(const GraphOptions& options) {
    GraphAnalysisTool graph;
    auto rng = getRNG();
    std::uniform_int_distribution<> weightDist(options.weightRange.first, options.weightRange.second);

    int setSize1 = options.numNodes / 2;
    int setSize2 = options.numNodes - setSize1;

    // Add nodes for both sets
    for (int i = 0; i < options.numNodes; ++i) {
        graph.addNode(i);
    }

    // Connect nodes between sets
    for (int i = 0; i < setSize1; ++i) {
        for (int j = 0; j < setSize2; ++j) {
            if (std::uniform_real_distribution<>(0, 1)(rng) < options.density) {
                int weight = weightDist(rng);
                graph.addEdge(i, setSize1 + j, weight);
                if (!options.directed) {
                    graph.addEdge(setSize1 + j, i, weight);
                }
            }
        }
    }
    return graph;
}

GraphAnalysisTool GraphGenerator::generateTreeGraph(const GraphOptions& options) {
    GraphAnalysisTool graph;
    auto rng = getRNG();
    std::uniform_int_distribution<> weightDist(options.weightRange.first, options.weightRange.second);

    // Add root node
    graph.addNode(0);

    std::queue<int> nodeQueue;
    nodeQueue.push(0);
    int nextNodeId = 1;

    while (!nodeQueue.empty() && nextNodeId < options.numNodes) {
        int currentNode = nodeQueue.front();
        nodeQueue.pop();

        // Add children
        for (int i = 0; i < options.branchingFactor && nextNodeId < options.numNodes; ++i) {
            graph.addNode(nextNodeId);
            int weight = weightDist(rng);
            graph.addEdge(currentNode, nextNodeId, weight);
            if (!options.directed) {
                graph.addEdge(nextNodeId, currentNode, weight);
            }
            nodeQueue.push(nextNodeId);
            nextNodeId++;
        }
    }
    return graph;
}

GraphAnalysisTool GraphGenerator::generateGridGraph(const GraphOptions& options) {
    GraphAnalysisTool graph;
    auto rng = getRNG();
    std::uniform_int_distribution<> weightDist(options.weightRange.first, options.weightRange.second);

    // Create grid nodes
    for (int i = 0; i < options.gridRows; ++i) {
        for (int j = 0; j < options.gridCols; ++j) {
            int nodeId = i * options.gridCols + j;
            graph.addNode(nodeId);

            // Connect to left neighbor
            if (j > 0) {
                int weight = weightDist(rng);
                graph.addEdge(nodeId, nodeId - 1, weight);
                if (!options.directed) {
                    graph.addEdge(nodeId - 1, nodeId, weight);
                }
            }

            // Connect to upper neighbor
            if (i > 0) {
                int weight = weightDist(rng);
                graph.addEdge(nodeId, nodeId - options.gridCols, weight);
                if (!options.directed) {
                    graph.addEdge(nodeId - options.gridCols, nodeId, weight);
                }
            }
        }
    }
    return graph;
}

GraphAnalysisTool GraphGenerator::generateScaleFreeGraph(const GraphOptions& options) {
    GraphAnalysisTool graph;
    auto rng = getRNG();
    std::uniform_int_distribution<> weightDist(options.weightRange.first, options.weightRange.second);

    // Start with a small complete graph
    int initialNodes = 3;
    for (int i = 0; i < initialNodes; ++i) {
        graph.addNode(i);
        for (int j = 0; j < i; ++j) {
            int weight = weightDist(rng);
            graph.addEdge(i, j, weight);
            if (!options.directed) {
                graph.addEdge(j, i, weight);
            }
        }
    }

    // Add remaining nodes with preferential attachment
    std::vector<int> degreeCount(options.numNodes, 0);
    for (int i = 0; i < initialNodes; ++i) {
        degreeCount[i] = initialNodes - 1;
    }

    for (int i = initialNodes; i < options.numNodes; ++i) {
        graph.addNode(i);

        // Calculate total degree for probability distribution
        int totalDegree = 0;
        for (int j = 0; j < i; ++j) {
            totalDegree += degreeCount[j];
        }

        // Add edges based on preferential attachment
        for (int j = 0; j < std::min(3, i); ++j) {
            int target = 0;
            int accumDegree = 0;
            int randomValue = std::uniform_int_distribution<>(0, totalDegree - 1)(rng);

            for (int k = 0; k < i; ++k) {
                accumDegree += degreeCount[k];
                if (accumDegree > randomValue) {
                    target = k;
                    break;
                }
            }

            int weight = weightDist(rng);
            graph.addEdge(i, target, weight);
            if (!options.directed) {
                graph.addEdge(target, i, weight);
            }
            degreeCount[i]++;
            degreeCount[target]++;
        }
    }
    return graph;
}

GraphAnalysisTool GraphGenerator::generateSmallWorldGraph(const GraphOptions& options) {
    GraphAnalysisTool graph;
    auto rng = getRNG();
    std::uniform_int_distribution<> weightDist(options.weightRange.first, options.weightRange.second);
    std::uniform_real_distribution<> rewireDist(0.0, 1.0);

    // Initially create a ring lattice
    for (int i = 0; i < options.numNodes; ++i) {
        graph.addNode(i);
        // Connect to K nearest neighbors
        for (int j = 1; j <= 2; ++j) {
            int target = (i + j) % options.numNodes;
            int weight = weightDist(rng);
            graph.addEdge(i, target, weight);
            if (!options.directed) {
                graph.addEdge(target, i, weight);
            }
        }
    }

    // Rewire edges
    for (int i = 0; i < options.numNodes; ++i) {
        for (int j = 1; j <= 2; ++j) {
            if (rewireDist(rng) < options.rewireProbability) {
                int oldTarget = (i + j) % options.numNodes;
                int newTarget;
                do {
                    newTarget = std::uniform_int_distribution<>(0, options.numNodes - 1)(rng);
                } while (newTarget == i || newTarget == oldTarget);

                // Remove old edge and add new one
                graph.addEdge(i, newTarget, weightDist(rng));
                if (!options.directed) {
                    graph.addEdge(newTarget, i, weightDist(rng));
                }
            }
        }
    }
    return graph;
}

