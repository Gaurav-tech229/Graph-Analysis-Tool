// File: src/GraphVisualizer.cpp
#include "GraphVisualizer.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <cmath>
#include <algorithm>

std::string GraphVisualizer::getLayoutEngine(Layout layout) {
    switch (layout) {
    case Layout::CIRCULAR: return "circo";
    case Layout::FORCE: return "fdp";
    case Layout::HIERARCHICAL: return "dot";
    case Layout::GRID: return "neato";
    case Layout::SPECTRAL: return "sfdp";
    default: return "fdp";
    }
}

std::string GraphVisualizer::colorForWeight(double weight, double maxWeight) {
    // Convert weight to color (blue to red gradient)
    double ratio = weight / maxWeight;
    int red = static_cast<int>(255 * ratio);
    int blue = static_cast<int>(255 * (1 - ratio));
    std::stringstream ss;
    ss << "#" << std::hex << std::setfill('0') << std::setw(2) << red
        << "00" << std::setw(2) << blue;
    return ss.str();
}

void GraphVisualizer::exportToDOT(const GraphAnalysisTool& graph,
    const std::string& filename,
    const VisualizationOptions& options) {

    std::ofstream out(filename);

    // Start DOT file
    out << (options.directed ? "digraph" : "graph") << " G {\n";

    // Graph attributes
    out << "    // Graph attributes\n";
    out << "    graph [\n";
    out << "        layout=" << getLayoutEngine(options.layout) << "\n";
    out << "        bgcolor=\"" << options.backgroundColor << "\"\n";
    out << "        splines=true\n";
    out << "        overlap=false\n";
    out << "    ];\n\n";

    // Node attributes
    out << "    // Default node attributes\n";
    out << "    node [\n";
    out << "        shape=circle\n";
    out << "        style=filled\n";
    out << "        fillcolor=\"" << options.nodeColor << "\"\n";
    out << "        fontcolor=\"" << options.labelColor << "\"\n";
    out << "        width=" << options.nodeSize / 72.0 << "\n";
    out << "    ];\n\n";

    // Edge attributes
    out << "    // Default edge attributes\n";
    out << "    edge [\n";
    out << "        penwidth=" << options.edgeThickness << "\n";
    out << "        color=\"" << options.edgeColor << "\"\n";
    out << "        fontcolor=\"" << options.labelColor << "\"\n";
    out << "    ];\n\n";

    // Get graph data
    const auto& adjacencyList = graph.getAdjacencyList();
    const auto& nodes = graph.getNodes();
    double maxWeight = 0;

    // First, declare all nodes (including isolated ones)
    for (int node : nodes) {
        out << "    " << node;
        if (options.showLabels) {
            out << " [label=\"" << node << "\"]";
        }
        out << ";\n";
    }

    // Find max weight for color scaling
    if (options.colorScheme == ColorScheme::WEIGHT) {
        for (const auto& [_, edges] : adjacencyList) {
            for (const auto& [__, weight] : edges) {
                maxWeight = std::max(maxWeight, static_cast<double>(weight));
            }
        }
    }

    // Add edges
    for (const auto& [from, edges] : adjacencyList) {
        for (const auto& [to, weight] : edges) {
            out << "    " << from << " "
                << (options.directed ? "->" : "--") << " "
                << to;

            // Edge attributes
            std::vector<std::string> attrs;

            if (options.showWeights) {
                attrs.push_back("label=\"" + std::to_string(weight) + "\"");
            }

            if (options.colorScheme == ColorScheme::WEIGHT) {
                attrs.push_back("color=\"" + colorForWeight(weight, maxWeight) + "\"");
            }

            if (!attrs.empty()) {
                out << " [";
                for (size_t i = 0; i < attrs.size(); ++i) {
                    if (i > 0) out << ", ";
                    out << attrs[i];
                }
                out << "]";
            }
            out << ";\n";
        }
    }

    out << "}\n";
    out.close();
}





void GraphVisualizer::exportToHTML(const GraphAnalysisTool& graph,
    const std::string& filename,
    const VisualizationOptions& options) {
    std::ofstream out(filename);

    // Write HTML header and styles
    out << R"(<!DOCTYPE html>
<html>
<head>
    <title>Graph Visualization</title>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/d3/7.8.5/d3.min.js"></script>
    <style>
        #graph-container {
            width: 100%;
            height: 800px;
            border: 1px solid #ccc;
            background-color: )" << options.backgroundColor << R"(;
        }
        .node {
            stroke: #fff;
            stroke-width: 1.5px;
        }
        .link {
            stroke: #999;
            stroke-opacity: 1;
            stroke-width: 2px;
        }
        .node text {
            pointer-events: none;
            font-size: 12px;
        }
        .link-label {
            font-size: 10px;
            fill: #666;
        }
    </style>
</head>
<body>
    <div id="graph-container"></div>
    <script>
)";

    // Convert graph data to JSON
    out << "const graphData = {\n";
    out << "    nodes: [\n";

    const auto& nodes = graph.getNodes();
    bool firstNode = true;
    for (int node : nodes) {
        if (!firstNode) out << ",\n";
        out << "        { id: " << node << " }";
        firstNode = false;
    }

    out << "\n    ],\n    links: [\n";

    const auto& adjacencyList = graph.getAdjacencyList();
    bool firstEdge = true;
    for (const auto& [from, edges] : adjacencyList) {
        for (const auto& [to, weight] : edges) {
            if (!firstEdge) out << ",\n";
            out << "        { source: " << from
                << ", target: " << to
                << ", weight: " << weight << " }";
            firstEdge = false;
        }
    }

    out << "\n    ]\n};\n";

    // Add specific layout algorithms
    switch (options.layout) {
    case Layout::TREE:
        out << R"(
    function treeLayout() {
        const width = document.getElementById('graph-container').clientWidth;
        const height = document.getElementById('graph-container').clientHeight;
        const hierarchy = d3.stratify()
            .id(d => d.id)
            .parentId(d => {
                const parentLink = graphData.links.find(l => l.target === d.id);
                return parentLink ? parentLink.source : null;
            })(graphData.nodes);

        const treeLayout = d3.tree()
            .size([width - 100, height - 100]);

        const root = treeLayout(hierarchy);
        
        root.descendants().forEach(d => {
            const node = graphData.nodes.find(n => n.id === Number(d.id));
            node.x = d.x + 50;
            node.y = d.y + 50;
            node.fx = node.x;
            node.fy = node.y;
        });
    }
    treeLayout();
)";
        break;

    case Layout::CIRCULAR:
        out << R"(
    function circularLayout() {
        const width = document.getElementById('graph-container').clientWidth;
        const height = document.getElementById('graph-container').clientHeight;
        const radius = Math.min(width, height) / 3;
        const centerX = width / 2;
        const centerY = height / 2;
        
        graphData.nodes.forEach((node, i) => {
            const angle = (i / graphData.nodes.length) * 2 * Math.PI;
            node.x = centerX + radius * Math.cos(angle);
            node.y = centerY + radius * Math.sin(angle);
            node.fx = node.x;
            node.fy = node.y;
        });
    }
    circularLayout();
)";
        break;

    case Layout::GRID:
        out << R"(
    function gridLayout() {
        const width = document.getElementById('graph-container').clientWidth;
        const height = document.getElementById('graph-container').clientHeight;
        const cols = Math.ceil(Math.sqrt(graphData.nodes.length));
        const rows = Math.ceil(graphData.nodes.length / cols);
        const cellWidth = width / (cols + 1);
        const cellHeight = height / (rows + 1);
        
        graphData.nodes.forEach((node, i) => {
            const col = i % cols;
            const row = Math.floor(i / cols);
            node.x = cellWidth * (col + 1);
            node.y = cellHeight * (row + 1);
            node.fx = node.x;
            node.fy = node.y;
        });
    }
    gridLayout();
)";
        break;

    case Layout::HIERARCHICAL:
        out << R"(
    function hierarchicalLayout() {
        const width = document.getElementById('graph-container').clientWidth;
        const height = document.getElementById('graph-container').clientHeight;
        
        // Find root nodes (nodes with no incoming edges)
        const hasIncoming = new Set(graphData.links.map(l => l.target));
        const roots = graphData.nodes.filter(n => !hasIncoming.has(n.id));
        
        // Assign levels through BFS
        const levels = new Map();
        const visited = new Set();
        
        function assignLevels() {
            let currentLevel = [roots[0].id];
            let level = 0;
            
            while (currentLevel.length > 0) {
                levels.set(level, currentLevel);
                const nextLevel = [];
                
                currentLevel.forEach(nodeId => {
                    visited.add(nodeId);
                    graphData.links
                        .filter(l => l.source === nodeId)
                        .forEach(l => {
                            if (!visited.has(l.target)) {
                                nextLevel.push(l.target);
                            }
                        });
                });
                
                currentLevel = nextLevel;
                level++;
            }
        }
        
        assignLevels();
        
        // Position nodes based on their level
        const levelHeight = height / (levels.size + 1);
        levels.forEach((nodes, level) => {
            const levelWidth = width / (nodes.length + 1);
            nodes.forEach((nodeId, index) => {
                const node = graphData.nodes.find(n => n.id === nodeId);
                node.x = levelWidth * (index + 1);
                node.y = levelHeight * (level + 1);
                node.fx = node.x;
                node.fy = node.y;
            });
        });
    }
    hierarchicalLayout();
)";
        break;

    case Layout::FORCE:
        out << R"(
    function forceLayout() {
        const width = document.getElementById('graph-container').clientWidth;
        const height = document.getElementById('graph-container').clientHeight;

        // Initialize force simulation
        const simulation = d3.forceSimulation(graphData.nodes)
            .force('link', d3.forceLink(graphData.links)
                .id(d => d.id)
                .distance(100))
            .force('charge', d3.forceManyBody()
                .strength(-300))
            .force('center', d3.forceCenter(width/2, height/2))
            .force('collision', d3.forceCollide().radius(30));

        // Draw edges
        const link = g.append('g')
            .selectAll('line')
            .data(graphData.links)
            .join('line')
            .attr('class', 'link')
            .style('stroke', ')" << options.edgeColor << R"(')
            .style('stroke-width', )" << options.edgeThickness << R"();

        // Draw nodes
        const node = g.append('g')
            .selectAll('circle')
            .data(graphData.nodes)
            .join('circle')
            .attr('class', 'node')
            .attr('r', )" << options.nodeSize << R"()
            .style('fill', ')" << options.nodeColor << R"(');

        // Add node labels if enabled
        let labels = null;
        if ()" << (options.showLabels ? "true" : "false") << R"() {
            labels = g.append('g')
                .selectAll('text')
                .data(graphData.nodes)
                .join('text')
                .text(d => d.id)
                .attr('font-size', '12px')
                .attr('dx', 15)
                .attr('dy', 4);
        }

        // Add edge weights if enabled
        let edgeLabels = null;
        if ()" << (options.showWeights ? "true" : "false") << R"() {
            edgeLabels = g.append('g')
                .selectAll('text')
                .data(graphData.links)
                .join('text')
                .text(d => d.weight)
                .attr('font-size', '10px')
                .attr('fill', '#666');
        }

        // Update positions on each tick
        simulation.on('tick', () => {
            // Keep nodes within bounds
            graphData.nodes.forEach(d => {
                d.x = Math.max(50, Math.min(width - 50, d.x));
                d.y = Math.max(50, Math.min(height - 50, d.y));
            });

            link
                .attr('x1', d => d.source.x)
                .attr('y1', d => d.source.y)
                .attr('x2', d => d.target.x)
                .attr('y2', d => d.target.y);

            node
                .attr('cx', d => d.x)
                .attr('cy', d => d.y);

            if (labels) {
                labels
                    .attr('x', d => d.x)
                    .attr('y', d => d.y);
            }

            if (edgeLabels) {
                edgeLabels
                    .attr('x', d => (d.source.x + d.target.x) / 2)
                    .attr('y', d => (d.source.y + d.target.y) / 2);
            }
        });

        // Add drag behavior
        node.call(d3.drag()
            .on('start', (event, d) => {
                if (!event.active) simulation.alphaTarget(0.3).restart();
                d.fx = d.x;
                d.fy = d.y;
            })
            .on('drag', (event, d) => {
                d.fx = event.x;
                d.fy = event.y;
            })
            .on('end', (event, d) => {
                if (!event.active) simulation.alphaTarget(0);
                d.fx = null;
                d.fy = null;
            }));
    }
    forceLayout();
)";

    default: // Force-directed layout with constraints
        out << R"(
    function forceLayout() {
        const simulation = d3.forceSimulation(graphData.nodes)
            .force('link', d3.forceLink(graphData.links).id(d => d.id))
            .force('charge', d3.forceManyBody().strength(-1000))
            .force('center', d3.forceCenter(width / 2, height / 2))
            .stop();

        // Run simulation manually
        for (let i = 0; i < 300; ++i) simulation.tick();
        
        // Fix final positions
        graphData.nodes.forEach(node => {
            node.fx = node.x;
            node.fy = node.y;
        });
    }
    forceLayout();
)";
        break;
    }

    // Add visualization rendering code
    out << R"(
    const width = document.getElementById('graph-container').clientWidth;
    const height = document.getElementById('graph-container').clientHeight;

    const svg = d3.select('#graph-container')
        .append('svg')
        .attr('width', width)
        .attr('height', height);

    // Create a group for the graph
    const g = svg.append('g');

    // Add zoom behavior
    svg.call(d3.zoom()
        .extent([[0, 0], [width, height]])
        .scaleExtent([0.1, 4])
        .on('zoom', (event) => {
            g.attr('transform', event.transform);
        }));

    // Draw edges
    const link = g.append('g')
        .selectAll('line')
        .data(graphData.links)
        .join('line')
        .attr('class', 'link')
        .attr('x1', d => graphData.nodes.find(n => n.id === d.source).x)
        .attr('y1', d => graphData.nodes.find(n => n.id === d.source).y)
        .attr('x2', d => graphData.nodes.find(n => n.id === d.target).x)
        .attr('y2', d => graphData.nodes.find(n => n.id === d.target).y);

    // Add edge weights
    if ()" << (options.showWeights ? "true" : "false") << R"() {
        g.append('g')
            .selectAll('text')
            .data(graphData.links)
            .join('text')
            .attr('class', 'link-label')
            .attr('x', d => (graphData.nodes.find(n => n.id === d.source).x + 
                           graphData.nodes.find(n => n.id === d.target).x) / 2)
            .attr('y', d => (graphData.nodes.find(n => n.id === d.source).y + 
                           graphData.nodes.find(n => n.id === d.target).y) / 2)
            .text(d => d.weight);
    }

    // Draw nodes
    const node = g.append('g')
        .selectAll('circle')
        .data(graphData.nodes)
        .join('circle')
        .attr('class', 'node')
        .attr('r', )" << options.nodeSize << R"()
        .attr('cx', d => d.x)
        .attr('cy', d => d.y)
        .style('fill', ')" << options.nodeColor << R"(');

    // Add node labels
    if ()" << (options.showLabels ? "true" : "false") << R"() {
        g.append('g')
            .selectAll('text')
            .data(graphData.nodes)
            .join('text')
            .attr('x', d => d.x + )" << options.nodeSize + 5 << R"()
            .attr('y', d => d.y + 5)
            .text(d => d.id);
    }
    </script>
</body>
</html>)";

    out.close();
}