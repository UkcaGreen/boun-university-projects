#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <regex>
#include "Graph.h"

int main(int argc, char* argv[]) {

    ios_base::sync_with_stdio(false);

    // below reads the input file
    // in your next projects, you will implement that part as well
    if (argc != 3) {
        cout << "Run the code with the following command: ./project3 [input_file] [output_file]" << endl;
        return 1;
    }

    // here, perform the input operation. in other words,
    // read the file named <argv[1]>
    ifstream infile(argv[1]);

    int N;

    infile >> N;

    Graph MyGraph(N);

    for (int i = 1; i <= N ; ++i) {

        int numEdges;

        infile >> numEdges;

        for (int j = 0; j < numEdges ; ++j) {

            int temp;

            infile >> temp;

            MyGraph.addEdge(i,temp);

        }

    }

    // here, perform the output operation. in other words,
    // print your results into the file named <argv[2]>
    ofstream myfile;
    myfile.open (argv[2]);

    vector<int> v = MyGraph.printSCC();

//    reverse(v.begin(), v.end());

    myfile << v.size() << " ";

    for (const int& i : v) {
        myfile << i << " ";
    }

    myfile.close();

    return 0;
}