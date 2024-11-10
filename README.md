# Graph Analysis Tool

A high-performance C++ library for graph analysis, visualization, and network structure exploration. This tool combines efficient graph algorithms with powerful visualization capabilities, making it suitable for both academic research and practical applications in network analysis.

## 🎯 Overview

The Graph Analysis Tool is designed to handle various types of graph-based analyses with a focus on performance and visualization. It supports multiple graph representations, various analysis algorithms, and flexible visualization options through both static (DOT/Graphviz) and interactive (D3.js) rendering.

### Key Capabilities
- Comprehensive graph analysis algorithms
- Multiple graph generation patterns
- Dual visualization system
- Performance metrics tracking
- Memory optimization
- Extensible architecture

## 🌟 Features

### Core Analysis Components

#### 1. Graph Operations
- **Basic Operations**
  - Edge addition and removal
  - Node management
  - Graph property queries
  - Connectivity checking

- **Graph Traversal**
  - Depth-First Search (DFS)
  - Breadth-First Search (BFS)
  - Custom traversal patterns

#### 2. Path Finding
- **Algorithms**
  - Dijkstra's Algorithm
  - Shortest path finding
  - Path reconstruction
  - Multiple path detection

#### 3. Network Analysis
- **Community Detection**
  - Connected components
  - Subgraph identification
  - Community structure analysis
  - Isolated node detection

- **Flow Analysis**
  - Maximum flow calculation
  - Flow path tracking
  - Bottleneck identification
  - Network capacity analysis

### Graph Generation System

#### 1. Graph Types
- **Random Graphs**
  - Erdős-Rényi model
  - Customizable density
  - Weight distribution control

- **Scale-Free Networks**
  - Barabási-Albert model
  - Preferential attachment
  - Power-law degree distribution

- **Regular Structures**
  - Grid patterns
  - Ring lattices
  - Complete graphs
  - Tree structures

- **Small World Networks**
  - Watts-Strogatz model
  - Controllable rewiring probability
  - Clustering coefficient manipulation

#### 2. Generation Parameters
```cpp
struct GraphOptions {
    int numNodes;              // Number of nodes
    double density;            // Edge density
    std::pair<int, int> weightRange;  // Edge weight range
    bool directed;             // Directed/Undirected
    double rewireProbability;  // For small-world networks
    int gridRows;             // For grid networks
    int gridCols;             // For grid networks
    int branchingFactor;      // For trees
};
```

## 🎨 Visualization Systems

### 1. DOT (Graphviz) Visualization

#### Supported Layouts
```bash
# Hierarchical Layout (dot)
dot -Tpng input.dot -o hierarchical.png

# Spring Model Layout (neato)
neato -Tpng input.dot -o spring.png

# Circular Layout (circo)
circo -Tpng input.dot -o circular.png

# Force-Directed Layout (fdp)
fdp -Tpng input.dot -o force.png

# Radial Layout (twopi)
twopi -Tpng input.dot -o radial.png
```

#### Layout Customization
```dot
digraph G {
    // Graph attributes
    graph [
        rankdir=LR
        splines=ortho
        nodesep=0.5
        ranksep=0.5
    ];

    // Node attributes
    node [
        shape=circle
        style=filled
        fillcolor=lightblue
    ];

    // Edge attributes
    edge [
        color=gray
        penwidth=2
    ];
}
```

### 2. D3.js Interactive Visualization

#### Features
- **Interaction**
  - Node dragging
  - Zoom and pan
  - Hover effects
  - Dynamic layout adjustment

- **Customization**
  - Node colors and sizes
  - Edge weights and styles
  - Label positioning
  - Force parameters

#### Layout Options
```cpp
enum class Layout {
    FORCE,        // Force-directed layout
    CIRCULAR,     // Circular arrangement
    HIERARCHICAL, // Tree-like structure
    GRID,         // Grid pattern
    SPECTRAL      // Spectral layout
};
```

## 🔧 Technical Implementation

### 1. Data Structures
```cpp
// Adjacency List Implementation
class GraphAnalysisTool {
private:
    std::unordered_map<int, std::vector<std::pair<int, int>>> adjacencyList;
    std::unordered_set<int> nodes;
    int numNodes;
    int numEdges;
};
```

### 2. Algorithm Examples

#### Shortest Path Implementation
```cpp
std::pair<std::vector<int>, int> shortestPath(int start, int end) {
    std::vector<int> distances(numNodes, std::numeric_limits<int>::max());
    std::vector<int> previous(numNodes, -1);
    std::priority_queue<std::pair<int, int>, 
                      std::vector<std::pair<int, int>>, 
                      std::greater<>> pq;

    distances[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        int current = pq.top().second;
        int dist = pq.top().first;
        pq.pop();

        if (dist > distances[current]) continue;

        for (const auto& [next, weight] : adjacencyList[current]) {
            if (distances[current] + weight < distances[next]) {
                distances[next] = distances[current] + weight;
                previous[next] = current;
                pq.push({distances[next], next});
            }
        }
    }

    // Path reconstruction
    std::vector<int> path;
    if (distances[end] != std::numeric_limits<int>::max()) {
        for (int at = end; at != -1; at = previous[at]) {
            path.push_back(at);
        }
        std::reverse(path.begin(), path.end());
    }

    return {path, distances[end]};
}
```

### 3. Performance Metrics
```cpp
struct PerformanceMetrics {
    std::chrono::duration<double> executionTime;
    size_t nodesProcessed;
    size_t memoryUsed;
};
```

## 📊 Performance Analysis

### Time Complexity
| Algorithm | Average Case | Worst Case |
|-----------|--------------|------------|
| Shortest Path | O(E log V) | O(V²) |
| Community Detection | O(V + E) | O(V + E) |
| Maximum Flow | O(VE) | O(VE²) |
| Graph Generation | O(V + E) | O(V²) |

### Space Complexity
| Component | Space Usage |
|-----------|-------------|
| Adjacency List | O(V + E) |
| Path Storage | O(V) |
| Community Detection | O(V) |
| Visualization Data | O(V + E) |

### Memory Optimization
- Efficient data structure choices
- Smart pointer usage where appropriate
- Memory tracking and reporting
- Resource cleanup management

## 🚀 Advanced Usage Examples

### 1. Custom Graph Analysis
```cpp
// Create and configure graph
GraphAnalysisTool graph;
graph.addEdge(0, 1, 4);
graph.addEdge(0, 2, 3);
graph.addEdge(1, 2, 1);

// Analyze specific paths
auto [path, distance] = graph.shortestPath(0, 2);

// Find communities
auto communities = graph.findCommunities();

// Calculate maximum flow
int maxFlow = graph.maxFlow(0, 2);
```

### 2. Visualization Configuration
```cpp
GraphVisualizer::VisualizationOptions options;
options.layout = GraphVisualizer::Layout::HIERARCHICAL;
options.colorScheme = GraphVisualizer::ColorScheme::WEIGHT;
options.showWeights = true;
options.nodeSize = 20;
options.edgeThickness = 2.0;

// Export visualizations
GraphVisualizer::exportToDOT(graph, "analysis.dot", options);
GraphVisualizer::exportToHTML(graph, "analysis.html", options);
```
