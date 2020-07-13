//
// Created by student on 18.11.2018.
//

#include "Graph.h"
#include <set>

using namespace std;

Graph::Graph() {
    this->vertexNum = 0;
}

Graph::Graph(int _vertexNum) {
    this->vertexNum = _vertexNum;
}

//Graph::Graph(const Graph &other) {
//
//}
//
//Graph::Graph(Graph &&other) {
//
//}
//
//Graph& Graph::operator=(const Graph &other) {
//
//}
//
//Graph& Graph::operator=(Graph &&other) {
//
//}
//
//Graph::~Graph() {
//
//}

void Graph::addEdge(int from, int to) {
    this->mainGraph[from].push_back(to);
}

Graph Graph::transpose() {
    Graph g(this->vertexNum);

    for (int i = 1; i <= this->vertexNum; ++i) {
        for (const int& j : this->mainGraph[i]) {
            g.mainGraph[j].push_back(i);
        }
    }

    return g;
}

void Graph::printGraph() {
    cout << "Graph" << endl;
    for (int i = 1; i <= this->vertexNum ; ++i) {
        cout << i << " : ";
        for (const int& j : this->mainGraph[i]) {
            cout << j << " ";
        }
        cout << endl;
    }
}

vector<int> Graph::printSCC() {

    fill_n(this->visited,this->vertexNum+1,false);

    for (int i = 1; i <= this->vertexNum; ++i) {
        if(!this->visited[i]){
            fillOrder(i);
        }
    }

    Graph g = this->transpose();

    fill_n(this->visited,this->vertexNum+1,false);

    int sccTable[this->vertexNum+1];

    fill_n(sccTable,this->vertexNum+1,-1);

    int sccCount = 0;

    while (!this->s.empty()){

        int v = this->s.top();
        this->s.pop();

        if(!this->visited[v]){
            sccCount++;
            g.DFSUtil(v,sccTable,sccCount);
        }

    }

    Graph sccGraph(sccCount);

    for (int i = 1; i <= this->vertexNum ; ++i) {
        for (const int& j : this->mainGraph[i]) {
            if(sccTable[i] != sccTable[j]){
                sccGraph.addEdge(sccTable[i],sccTable[j]);
            }
        }
    }

    bool indegreeTable[sccCount+1];

    fill_n(indegreeTable,sccCount,true);

    for (int i = 1; i <= sccGraph.vertexNum ; ++i) {
        for (const int& j : sccGraph.mainGraph[i]) {
            if(i != j){
                indegreeTable[j] = false;
            }
        }
    }

    vector<int> result;

    for (int i = 1; i <= sccCount; ++i) {
        if(indegreeTable[i]){
            for (int j = 1; j <= this->vertexNum+1 ; ++j) {
                if(sccTable[j] == i){
                    result.push_back(j);
                    break;
                }
            }
        }
    }

    return result;
}

void Graph::fillOrder(int v) {
    this->visited[v] = true;

    for (const int& i : this->mainGraph[v]) {
        if(!this->visited[i]){
            fillOrder(i);
        }
    }

    this->s.push(v);
}

void Graph::DFSUtil(int v, int scc[], int sccId) {
    this->visited[v] = true;
    scc[v] = sccId;

    for (const int& i : this->mainGraph[v]) {
        if(!this->visited[i]){
            DFSUtil(i,scc,sccId);
        }
    }
}