// File: include/GraphVisualizer.h
#pragma once

#include "GraphAnalysisTool.h"
#include <string>
#include <map>
#include <vector>

class GraphVisualizer {
public:
    enum class Layout {
        TREE,           // Tree layout
        CIRCULAR,    // Nodes arranged in a circle
        FORCE,       // Force-directed layout
        HIERARCHICAL,// Tree-like hierarchical layout
        GRID,       // Grid-based layout
        SPECTRAL    // Spectral layout
    };

    enum class ColorScheme {
        DEFAULT,    // Standard colors
        WEIGHT,     // Color based on edge weights
        COMMUNITY,  // Color based on communities
        DEGREE,     // Color based on node degrees
        FLOW       // Color based on flow values
    };

    struct VisualizationOptions {
        Layout layout = Layout::FORCE;
        ColorScheme colorScheme = ColorScheme::DEFAULT;
        bool showWeights = true;
        bool showLabels = true;
        bool directed = true;
        double nodeSize = 20.0;
        double edgeThickness = 1.0;
        std::string backgroundColor = "#FFFFFF";
        std::string nodeColor = "#1f77b4";
        std::string edgeColor = "#666666";
        std::string labelColor = "#000000";
    };

    static void exportToDOT(const GraphAnalysisTool& graph,
        const std::string& filename,
        const VisualizationOptions& options = VisualizationOptions());

    static void exportToHTML(const GraphAnalysisTool& graph,
        const std::string& filename,
        const VisualizationOptions& options = VisualizationOptions());

private:
    static std::string generateDOTAttributes(const VisualizationOptions& options);
    static std::string colorForWeight(double weight, double maxWeight);
    static std::string getLayoutEngine(Layout layout);
};