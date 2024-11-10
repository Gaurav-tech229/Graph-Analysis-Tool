/*#include "GraphAnalysisTool.h"
#include "GraphGenerator.h"
#include "GraphVisualizer.h"
#include <iostream>
#include <string>

// Helper function to print separator line
void printSeparator() {
    std::cout << "\n" << std::string(50, '=') << "\n\n";
}

int main() {
    // Test different types of graphs
    GraphGenerator::GraphOptions genOptions;
    GraphVisualizer::VisualizationOptions visOptions;

    // 1. Random Graph
    std::cout << "Generating Random Graph...\n";
    genOptions.numNodes = 10;
    genOptions.density = 0.3;
    genOptions.weightRange = { 1, 10 };
    genOptions.directed = true;

    auto randomGraph = GraphGenerator::generateGraph(
        GraphGenerator::GraphType::RANDOM,
        genOptions
    );

    // Visualize random graph
    visOptions.layout = GraphVisualizer::Layout::FORCE;
    visOptions.colorScheme = GraphVisualizer::ColorScheme::WEIGHT;
    visOptions.showWeights = true;
    visOptions.showLabels = true;
    visOptions.directed = true;

    GraphVisualizer::exportToDOT(randomGraph, "random_graph.dot", visOptions);
    GraphVisualizer::exportToHTML(randomGraph, "random_graph.html", visOptions);

    std::cout << "Random Graph Analysis:\n";
    std::cout << randomGraph.generateFullAnalysis(0, 5);
    printSeparator();

    // 2. Scale-Free Network
    std::cout << "Generating Scale-Free Network...\n";
    genOptions.numNodes = 15;
    auto scaleFreeGraph = GraphGenerator::generateGraph(
        GraphGenerator::GraphType::SCALE_FREE,
        genOptions
    );

    visOptions.layout = GraphVisualizer::Layout::CIRCULAR;
    GraphVisualizer::exportToDOT(scaleFreeGraph, "scale_free_graph.dot", visOptions);
    GraphVisualizer::exportToHTML(scaleFreeGraph, "scale_free_graph.html", visOptions);

    std::cout << "Scale-Free Graph Analysis:\n";
    std::cout << scaleFreeGraph.generateFullAnalysis(0, 5);
    printSeparator();

    // 3. Grid Graph
    std::cout << "Generating Grid Graph...\n";
    genOptions.gridRows = 4;
    genOptions.gridCols = 4;
    auto gridGraph = GraphGenerator::generateGraph(
        GraphGenerator::GraphType::GRID,
        genOptions
    );

    visOptions.layout = GraphVisualizer::Layout::GRID;
    GraphVisualizer::exportToDOT(gridGraph, "grid_graph.dot", visOptions);
    GraphVisualizer::exportToHTML(gridGraph, "grid_graph.html", visOptions);

    std::cout << "Grid Graph Analysis:\n";
    std::cout << gridGraph.generateFullAnalysis(0, 15);
    printSeparator();

    // 4. Tree Graph
    std::cout << "Generating Tree Graph...\n";
    genOptions.numNodes = 15;
    genOptions.branchingFactor = 3;
    auto treeGraph = GraphGenerator::generateGraph(
        GraphGenerator::GraphType::TREE,
        genOptions
    );

    visOptions.layout = GraphVisualizer::Layout::HIERARCHICAL;
    GraphVisualizer::exportToDOT(treeGraph, "tree_graph.dot", visOptions);
    GraphVisualizer::exportToHTML(treeGraph, "tree_graph.html", visOptions);

    std::cout << "Tree Graph Analysis:\n";
    std::cout << treeGraph.generateFullAnalysis(0, 7);
    printSeparator();

    // 5. Small World Graph
    std::cout << "Generating Small World Graph...\n";
    genOptions.numNodes = 20;
    genOptions.rewireProbability = 0.2;
    auto smallWorldGraph = GraphGenerator::generateGraph(
        GraphGenerator::GraphType::SMALL_WORLD,
        genOptions
    );

    visOptions.layout = GraphVisualizer::Layout::FORCE;
    visOptions.colorScheme = GraphVisualizer::ColorScheme::COMMUNITY;
    GraphVisualizer::exportToDOT(smallWorldGraph, "small_world_graph.dot", visOptions);
    GraphVisualizer::exportToHTML(smallWorldGraph, "small_world_graph.html", visOptions);

    std::cout << "Small World Graph Analysis:\n";
    std::cout << smallWorldGraph.generateFullAnalysis(0, 10);
    printSeparator();

    // 6. Custom Graph with Isolated Nodes
    std::cout << "Creating Custom Graph with Isolated Nodes...\n";
    GraphAnalysisTool customGraph;

    // Add main component
    customGraph.addEdge(0, 1, 4);
    customGraph.addEdge(0, 2, 3);
    customGraph.addEdge(1, 2, 1);
    customGraph.addEdge(1, 3, 5);

    // Add isolated nodes and components
    customGraph.addNode(5);  // Single isolated node
    customGraph.addEdge(6, 7, 2);  // Isolated pair
    customGraph.addEdge(8, 8, 1);  // Self-loop

    visOptions.layout = GraphVisualizer::Layout::FORCE;
    visOptions.colorScheme = GraphVisualizer::ColorScheme::DEFAULT;
    GraphVisualizer::exportToDOT(customGraph, "custom_graph.dot", visOptions);
    GraphVisualizer::exportToHTML(customGraph, "custom_graph.html", visOptions);

    std::cout << "Custom Graph Analysis:\n";
    std::cout << customGraph.generateFullAnalysis(0, 3);
    printSeparator();

    std::cout << "\nAll visualizations have been exported to DOT and HTML files.\n";
    std::cout << "To view DOT files, use Graphviz tools (e.g., 'dot -Tpng file.dot -o file.png')\n";
    std::cout << "To view HTML files, open them in a web browser.\n";

    return 0;
}
*/


/*#include "GraphAnalysisTool.h"
#include "GraphGenerator.h"
#include "GraphVisualizer.h"
#include <iostream>
#include <string>
#include <cstdlib>

// Helper function to run Graphviz command
void generateImage(const std::string& dotFile, const std::string& outputFile,
    const std::string& layout = "dot") {
    std::string command = layout + " -Tpng " + dotFile + " -o " + outputFile;
    int result = std::system(command.c_str());
    if (result == 0) {
        std::cout << "Successfully generated " << outputFile << std::endl;
    }
    else {
        std::cout << "Failed to generate " << outputFile << std::endl;
    }
}

int main() {
    // Create output directory if it doesn't exist
#ifdef _WIN32
    std::system("if not exist output mkdir output");
#else
    std::system("mkdir -p output");
#endif

    // Test different types of graphs
    GraphGenerator::GraphOptions genOptions;
    GraphVisualizer::VisualizationOptions visOptions;

    // 1. Random Graph
    std::cout << "\nGenerating Random Graph...\n";
    genOptions.numNodes = 10;
    genOptions.density = 0.3;
    auto randomGraph = GraphGenerator::generateGraph(
        GraphGenerator::GraphType::RANDOM,
        genOptions
    );

    // Visualize with different layouts
    visOptions.showWeights = true;
    visOptions.showLabels = true;

    // Hierarchical layout
    visOptions.layout = GraphVisualizer::Layout::HIERARCHICAL;
    GraphVisualizer::exportToDOT(randomGraph, "output/random_hierarchical.dot", visOptions);
    generateImage("output/random_hierarchical.dot", "output/random_hierarchical.png", "dot");

    // Force-directed layout
    visOptions.layout = GraphVisualizer::Layout::FORCE;
    GraphVisualizer::exportToDOT(randomGraph, "output/random_force.dot", visOptions);
    generateImage("output/random_force.dot", "output/random_force.png", "fdp");

    // Circular layout
    visOptions.layout = GraphVisualizer::Layout::CIRCULAR;
    GraphVisualizer::exportToDOT(randomGraph, "output/random_circular.dot", visOptions);
    generateImage("output/random_circular.dot", "output/random_circular.png", "circo");

    // 2. Tree Graph
    std::cout << "\nGenerating Tree Graph...\n";
    genOptions.numNodes = 15;
    genOptions.branchingFactor = 3;
    auto treeGraph = GraphGenerator::generateGraph(
        GraphGenerator::GraphType::TREE,
        genOptions
    );

    visOptions.layout = GraphVisualizer::Layout::HIERARCHICAL;
    GraphVisualizer::exportToDOT(treeGraph, "output/tree.dot", visOptions);
    generateImage("output/tree.dot", "output/tree.png", "dot");

    // 3. Grid Graph
    std::cout << "\nGenerating Grid Graph...\n";
    genOptions.gridRows = 4;
    genOptions.gridCols = 4;
    auto gridGraph = GraphGenerator::generateGraph(
        GraphGenerator::GraphType::GRID,
        genOptions
    );

    visOptions.layout = GraphVisualizer::Layout::GRID;
    GraphVisualizer::exportToDOT(gridGraph, "output/grid.dot", visOptions);
    generateImage("output/grid.dot", "output/grid.png", "neato");

    // Print viewing instructions
    std::cout << "\nGraphviz visualizations have been generated in the 'output' directory:\n";
    std::cout << "- random_hierarchical.png (dot layout)\n";
    std::cout << "- random_force.png (fdp layout)\n";
    std::cout << "- random_circular.png (circo layout)\n";
    std::cout << "- tree.png (dot layout)\n";
    std::cout << "- grid.png (neato layout)\n";

    return 0;
}
*/

/*#include "GraphAnalysisTool.h"
#include "GraphGenerator.h"
#include "GraphVisualizer.h"
#include <iostream>
#include <string>
#include <cstdlib>

// Helper function to print separator line
void printSeparator() {
    std::cout << "\n" << std::string(50, '=') << "\n";
}

// Helper function to ensure output directory exists
void createOutputDirectory() {
#ifdef _WIN32
    std::system("if not exist visualizations mkdir visualizations");
#else
    std::system("mkdir -p visualizations");
#endif
}

int main() {
    createOutputDirectory();
    GraphGenerator::GraphOptions genOptions;
    GraphVisualizer::VisualizationOptions visOptions;

    // Base visualization options
    visOptions.showWeights = true;
    visOptions.showLabels = true;
    visOptions.nodeSize = 20;
    visOptions.edgeThickness = 2.0;
    visOptions.edgeColor = "#999999";
    visOptions.nodeColor = "#ff7700";
    visOptions.backgroundColor = "#ffffff";

    // 1. Simple Connected Graph
    std::cout << "Generating Simple Connected Graph...";
    GraphAnalysisTool simpleGraph;
    simpleGraph.addEdge(0, 1, 4);
    simpleGraph.addEdge(0, 2, 3);
    simpleGraph.addEdge(1, 2, 1);
    simpleGraph.addEdge(1, 3, 5);
    simpleGraph.addEdge(2, 3, 2);

    visOptions.layout = GraphVisualizer::Layout::FORCE;
    visOptions.colorScheme = GraphVisualizer::ColorScheme::DEFAULT;
    visOptions.nodeColor = "#1f77b4";  // Blue nodes
    visOptions.backgroundColor = "#ffffff";

    GraphVisualizer::exportToHTML(simpleGraph, "visualizations/simple_graph.html", visOptions);
    printSeparator();



    // 2. Random Dense Graph
    std::cout << "Generating Random Dense Graph...";
    genOptions.numNodes = 15;
    genOptions.density = 0.4;
    genOptions.weightRange = { 1, 10 };

    auto randomGraph = GraphGenerator::generateGraph(
        GraphGenerator::GraphType::RANDOM,
        genOptions
    );

    visOptions.colorScheme = GraphVisualizer::ColorScheme::WEIGHT;
    visOptions.nodeColor = "#2ca02c";  // Green nodes

    GraphVisualizer::exportToHTML(randomGraph, "visualizations/random_graph.html", visOptions);
    printSeparator();



    // 3. Tree Structure
    std::cout << "Generating Tree Structure...";
    genOptions.numNodes = 10;
    genOptions.branchingFactor = 2;

    auto treeGraph = GraphGenerator::generateGraph(
        GraphGenerator::GraphType::TREE,
        genOptions
    );

    visOptions.layout = GraphVisualizer::Layout::HIERARCHICAL;
    visOptions.colorScheme = GraphVisualizer::ColorScheme::DEFAULT;
    visOptions.nodeColor = "#ff7f0e";  // Orange nodes

    GraphVisualizer::exportToHTML(treeGraph, "visualizations/tree_graph.html", visOptions);
    printSeparator();




    // 4. Scale-Free Network
    std::cout << "Generating Scale-Free Network...";
    genOptions.numNodes = 20;

    auto scaleFreeGraph = GraphGenerator::generateGraph(
        GraphGenerator::GraphType::SCALE_FREE,
        genOptions
    );

    visOptions.layout = GraphVisualizer::Layout::FORCE;
    visOptions.colorScheme = GraphVisualizer::ColorScheme::DEGREE;
    visOptions.nodeColor = "#d62728";  // Red nodes

    GraphVisualizer::exportToHTML(scaleFreeGraph, "visualizations/scale_free_graph.html", visOptions);
    printSeparator();




    // 5. Grid Graph
    std::cout << "Generating Grid Graph...";
    genOptions.gridRows = 4;
    genOptions.gridCols = 4;

    auto gridGraph = GraphGenerator::generateGraph(
        GraphGenerator::GraphType::GRID,
        genOptions
    );

    visOptions.layout = GraphVisualizer::Layout::GRID;
    visOptions.colorScheme = GraphVisualizer::ColorScheme::DEFAULT;
    visOptions.nodeColor = "#9467bd";  // Purple nodes

    GraphVisualizer::exportToHTML(gridGraph, "visualizations/grid_graph.html", visOptions);
    printSeparator();




    // 6. Graph with Communities
    std::cout << "Generating Graph with Communities...";
    GraphAnalysisTool communityGraph;

    // Community 1
    for (int i = 0; i < 5; i++) {
        for (int j = i + 1; j < 5; j++) {
            communityGraph.addEdge(i, j, 1);
        }
    }

    // Community 2
    for (int i = 5; i < 10; i++) {
        for (int j = i + 1; j < 10; j++) {
            communityGraph.addEdge(i, j, 1);
        }
    }

    // Bridge edges
    communityGraph.addEdge(2, 7, 5);
    communityGraph.addEdge(4, 8, 5);

    visOptions.layout = GraphVisualizer::Layout::FORCE;
    visOptions.colorScheme = GraphVisualizer::ColorScheme::COMMUNITY;

    GraphVisualizer::exportToHTML(communityGraph, "visualizations/community_graph.html", visOptions);
    printSeparator();




    // 7. Graph with Isolated Components
    std::cout << "Generating Graph with Isolated Components...";
    GraphAnalysisTool isolatedGraph;

    // Main component
    isolatedGraph.addEdge(0, 1, 2);
    isolatedGraph.addEdge(1, 2, 3);
    isolatedGraph.addEdge(2, 0, 4);

    // Isolated pair
    isolatedGraph.addEdge(4, 5, 1);

    // Isolated node
    isolatedGraph.addNode(6);

    // Self-loop
    isolatedGraph.addEdge(7, 7, 1);

    visOptions.layout = GraphVisualizer::Layout::FORCE;
    visOptions.colorScheme = GraphVisualizer::ColorScheme::DEFAULT;
    visOptions.nodeColor = "#17becf";  // Cyan nodes

    GraphVisualizer::exportToHTML(isolatedGraph, "visualizations/isolated_graph.html", visOptions);
    printSeparator();

    std::cout << "\nHTML visualizations have been generated in the 'visualizations' directory:\n";
    std::cout << "1. simple_graph.html - Basic connected graph\n";
    std::cout << "2. random_graph.html - Random dense graph with weight-based coloring\n";
    std::cout << "3. tree_graph.html - Hierarchical tree structure\n";
    std::cout << "4. scale_free_graph.html - Scale-free network with degree-based coloring\n";
    std::cout << "5. grid_graph.html - Regular grid structure\n";
    std::cout << "6. community_graph.html - Graph with distinct communities\n";
    std::cout << "7. isolated_graph.html - Graph with isolated components\n";
    std::cout << "\nOpen these files in a web browser to view the interactive visualizations.\n";
    std::cout << "Features available in the visualization:\n";
    std::cout << "- Drag nodes to rearrange the layout\n";
    std::cout << "- Zoom in/out using mouse wheel\n";
    std::cout << "- Hover over nodes and edges to see details\n";
    std::cout << "- Click and drag the background to pan\n";

    return 0;
}*/


/*#include "GraphAnalysisTool.h"
#include "GraphGenerator.h"
#include "GraphVisualizer.h"
#include <iostream>

int main() {
    // Create test graphs
    GraphGenerator::GraphOptions genOptions;
    GraphVisualizer::VisualizationOptions visOptions;

    // Basic visualization settings
    visOptions.showWeights = true;
    visOptions.showLabels = true;
    visOptions.nodeSize = 20;
    visOptions.edgeThickness = 2.0;

    // 1. Tree Layout Example
    std::cout << "Generating Tree Layout...\n";
    genOptions.numNodes = 10;
    genOptions.branchingFactor = 2;
    auto treeGraph = GraphGenerator::generateGraph(
        GraphGenerator::GraphType::TREE,
        genOptions
    );

    visOptions.layout = GraphVisualizer::Layout::TREE;
    GraphVisualizer::exportToHTML(treeGraph, "visualizations/tree_graph.html", visOptions);

    // 2. Circular Layout Example
    std::cout << "Generating Circular Layout...\n";
    genOptions.numNodes = 8;
    genOptions.density = 0.3;
    auto circularGraph = GraphGenerator::generateGraph(
        GraphGenerator::GraphType::RANDOM,
        genOptions
    );

    visOptions.layout = GraphVisualizer::Layout::CIRCULAR;
    GraphVisualizer::exportToHTML(circularGraph, "visualizations/circular_graph.html", visOptions);

    // 3. Hierarchical Layout Example
    std::cout << "Generating Hierarchical Layout...\n";
    visOptions.layout = GraphVisualizer::Layout::HIERARCHICAL;
    GraphVisualizer::exportToHTML(treeGraph, "visualizations/hierarchical_graph.html", visOptions);

    // 4. Grid Layout Example
    std::cout << "Generating Grid Layout...\n";
    genOptions.gridRows = 3;
    genOptions.gridCols = 3;
    auto gridGraph = GraphGenerator::generateGraph(
        GraphGenerator::GraphType::GRID,
        genOptions
    );

    visOptions.layout = GraphVisualizer::Layout::GRID;
    GraphVisualizer::exportToHTML(gridGraph, "visualizations/grid_graph.html", visOptions);

    // 5. Force Layout Example
    std::cout << "Generating Force Layout...\n";
    visOptions.layout = GraphVisualizer::Layout::FORCE;
    GraphVisualizer::exportToHTML(circularGraph, "visualizations/force_graph.html", visOptions);

    std::cout << "\nAll visualizations have been generated in the 'visualizations' directory.\n";
    std::cout << "Open the HTML files in a web browser to view the graphs.\n";

    return 0;
}
*/