//
// Created by student on 18.11.2018.
//
#include <iostream>
#include <vector>
#include <stack>

#ifndef PROJECT3_GRAPH_H
#define PROJECT3_GRAPH_H

using namespace std;

class Graph {
public:
    int vertexNum;
    vector<int> mainGraph[100002];
    stack<int> s;
    bool visited[100002];


    Graph();
    Graph(int _vertexNum);

//    Graph(const Graph& other);
//
//    Graph &operator=(const Graph& other);
//
//    Graph(Graph&& other);
//
//    Graph &operator=(Graph&& other);
//
//    ~Graph();

    void addEdge(int from, int to);
    Graph transpose();
    void printGraph();
    vector<int> printSCC();
    void fillOrder(int v);
    void DFSUtil(int v, int scc[], int sccId);
};

#endif //PROJECT3_GRAPH_H
