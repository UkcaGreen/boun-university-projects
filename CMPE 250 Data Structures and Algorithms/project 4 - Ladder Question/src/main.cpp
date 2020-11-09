#include <iostream>
#include <queue>
#include <sstream>
#include <fstream>
#include <vector>
#include <regex>
#include <cstring>

using namespace std;

int rows, cols;
pair<short,short> group[1001][1001];

bool visited[1001][1001];
pair<short,short> parent[1001][1001][21];
int high[1001][1001][21];
int depth[1001][1001];

struct subset
{
    pair<short,short> parent;
    int rank;
};

struct Edge
{
    int weight;
    pair<short,short> src, dest;

    bool operator<(const Edge &other) const {
        return this->weight < other.weight;
    }

    Edge(){
        this->src.first = 0;
        this->src.second = 0;
        this->dest.first = 0;
        this->dest.second = 0;
        this->weight = 0;
    }

    Edge(int srcR, int srcC, int destR, int destC, int _weight){
        this->src.first = srcR;
        this->src.second = srcC;
        this->dest.first = destR;
        this->dest.second = destC;
        this->weight = _weight;
    }
};

vector<Edge> MST[1005][1005];
vector<Edge> myGraph[1005][1005];
subset subsets[1001][1001];

void DFS(pair<short,short> root, int d){
    visited[root.first][root.second] = true;
    depth[root.first][root.second] = d;

    for (Edge& e : MST[root.first][root.second]) {
        if(!visited[e.dest.first][e.dest.second]){
            parent[e.dest.first][e.dest.second][0] = root;
            high[e.dest.first][e.dest.second][0] = e.weight;
            DFS(e.dest, d+1);
        }
    }
}

int Solver(int fromR, int fromC, int toR, int toC){
    pair<short,short> a(fromR,fromC),b(toR,toC);
    int result = 0;

    if(depth[a.first][a.second] != depth[b.first][b.second]){
        int fark = abs(depth[a.first][a.second] - depth[b.first][b.second]);

        if(depth[a.first][a.second] > depth[b.first][b.second]){
            swap(a,b);
        }

        for (int i = 20; i >= 0; --i) {
            if((1<<i) <= fark){
                result = max(result,high[b.first][b.second][i]);
                b = parent[b.first][b.second][i];
                fark = fark - (1<<i);
            }
        }
    }

    if(a == b){
        return result;
    }

    for (int i = 20; i >= 0; --i) {
        if(parent[a.first][a.second][i] != parent[b.first][b.second][i]){
            result = max(result,high[a.first][a.second][i]);
            result = max(result,high[b.first][b.second][i]);
            a = parent[a.first][a.second][i];
            b = parent[b.first][b.second][i];
        }
    }

    result = max(result,high[a.first][a.second][0]);
    result = max(result,high[b.first][b.second][0]);

    return result;
}

//-----------------------------------------------

pair<short,short> find(pair<short,short> root)
{
    if (subsets[root.first][root.second].parent != root)
        subsets[root.first][root.second].parent = find(subsets[root.first][root.second].parent);

    return subsets[root.first][root.second].parent;
}

void Union(pair<short,short> x, pair<short,short> y)
{
    pair<short,short> xroot = find(x);
    pair<short,short> yroot = find(y);

    if (subsets[xroot.first][xroot.second].rank < subsets[yroot.first][yroot.second].rank)
        subsets[xroot.first][xroot.second].parent = yroot;
    else if (subsets[xroot.first][xroot.second].rank > subsets[yroot.first][yroot.second].rank)
        subsets[yroot.first][yroot.second].parent = xroot;

    else
    {
        subsets[yroot.first][yroot.second].parent = xroot;
        subsets[xroot.first][xroot.second].rank++;
    }
}


//-----------------------------------------------

int dist[1005][1005];

class Compare
{
public:
    bool operator() (pair<int,int> a, pair<int,int>b)
    {
        return dist[a.first][a.second] > dist[b.first][b.second];
    }
};

//-----------------------------------------------

int main(int argc, char* argv[]) {

    ios_base::sync_with_stdio(false);

    if (argc != 3) {
        cout << "Run the code with the following command: ./project4 [input_file] [output_file]" << endl;
        return 1;
    }

    // here, perform the input operation. in other words,
    // read the file named <argv[1]>
    ifstream infile(argv[1]);

    infile >> rows >> cols;

    int graph[rows+1][cols+1];

    for (int i = 1; i <= rows; ++i) {
        for (int j = 1; j <= cols ; ++j) {
            int temp;
            infile >> temp;
            graph[i][j] = temp;
        }
    }

    int Q;

    infile >> Q;

//    if(Q == 0){
//
//        priority_queue<Edge> edges;
//
//        int fromR, fromC, toR, toC;
//
//        infile >> fromR >> fromC >> toR >> toC;
//
//        pair<int,int> src(fromR,fromC);
//        pair<int,int> dest(toR,toC);
//
//        priority_queue<pair<int,int>, vector<pair<int,int>>, Compare> vertexes;
//
//        for (int i = 1; i <= rows; ++i) {
//            for (int j = 1; j <= cols ; ++j) {
//                if(j < cols){
//                    myGraph[i][j].push_back(Edge(i,j,i,j+1,abs(graph[i][j] - graph[i][j+1])));
//                    myGraph[i][j+1].push_back(Edge(i,j+1,i,j,abs(graph[i][j] - graph[i][j+1])));
//                }
//                if(i < rows){
//                    myGraph[i][j].push_back(Edge(i,j,i+1,j,abs(graph[i][j] - graph[i+1][j])));
//                    myGraph[i+1][j].push_back(Edge(i+1,j,i,j,abs(graph[i][j] - graph[i+1][j])));
//                }
//            }
//        }
//
//        memset(dist, 63, sizeof dist);
//        dist[src.first][src.second] = 0;
//
//        vertexes.push(src);
//
//        while(!vertexes.empty()){
//            pair<int,int> curr = vertexes.top();
//            vertexes.pop();
//
//            for(Edge& e: myGraph[curr.first][curr.second]){
//                if(max(e.weight,dist[curr.first][curr.second])<dist[e.dest.first][e.dest.second]){
//                    dist[e.dest.first][e.dest.second] = max(e.weight,dist[curr.first][curr.second]);
//                    vertexes.push(e.dest);
//                }
//            }
//        }
//
//        cout << dist[dest.first][dest.second] << endl;
//
//    }else{

        vector<Edge> edges;

        for (int i = 1; i <= rows; ++i) {
            for (int j = 1; j <= cols ; ++j) {
                if(j < cols){
                    edges.push_back(Edge(i,j,i,j+1,abs(graph[i][j] - graph[i][j+1])));
                }
                if(i < rows){
                    edges.push_back(Edge(i,j,i+1,j,abs(graph[i][j] - graph[i+1][j])));
                }
            }
        }

        for (int m = 1; m <= rows; ++m) {
            for (int i = 1; i <= cols; ++i) {
                subsets[m][i].parent = pair<short,short>(m,i);
                subsets[m][i].rank = 0;
            }
        }

        sort(edges.begin(),edges.end());

        int k = 0;
        int i = 0;

        while (k < rows*cols-1 )
        {
            Edge e = edges[i++];

            pair<short,short> x = find(e.src);
            pair<short,short> y = find(e.dest);

            if (x != y)
            {
                Union(x, y);
                k++;
                MST[e.src.first][e.src.second].push_back(e);
                swap(e.src,e.dest);
                MST[e.src.first][e.src.second].push_back(e);
            }
        }

        DFS(pair<short,short>(1,1), 0);

        for (int k = 1; k < 20; ++k) {
            for (int i = 1; i <= rows; ++i) {
                for (int j = 1; j <= cols ; ++j) {
                    parent[i][j][k] = parent[parent[i][j][k-1].first][parent[i][j][k-1].second][k-1];
                    high[i][j][k] = max(high[i][j][k-1],high[parent[i][j][k-1].first][parent[i][j][k-1].second][k-1]);
                }
            }
        }

        vector<int> answer;

        for (int k = 0; k < Q; ++k) {

            int fromR, fromC, toR, toC;

            infile >> fromR >> fromC >> toR >> toC;

            int ans = Solver(fromR,fromC,toR,toC);

//            cout << ans << endl;

            answer.push_back(ans);
        }

        // here, perform the output operation. in other words,
        // print your results into the file named <argv[2]>
        ofstream myfile;
        myfile.open (argv[2]);

        for (int& i : answer) {
            myfile << i << endl;
        }

        myfile.close();
//    }

    return 0;
}
