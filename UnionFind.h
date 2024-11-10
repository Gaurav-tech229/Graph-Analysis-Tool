// File: include/UnionFind.h
#pragma once

#include <vector>

class UnionFind {
private:
    std::vector<int> parent;
    std::vector<int> rank;

public:
    explicit UnionFind(int n);
    int find(int x);
    void unite(int x, int y);
};