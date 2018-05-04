#include <iostream>
#include <climits>
#include <map>
#include <sstream>
#include <vector>

#include "graph.h"

using namespace std;

// global variable - need better method, but works for testing
// used by graphVisitor
ostringstream graphOut;

// global variable for Djikstra
map<string, int> weight;
map<string, string> previous;

// OK or ERR for tests
template <typename T>
string isOK(const T& got, const T& expected) {
    if (got == expected) {
        return "OK: ";
    } else {
        cout << "    Got   " << got << "\n expected " << expected << endl;
        return "ERR: ";
    }
}

// visitor function - print out the vertex label
void display(const string& anItem) {
    cout << anItem << " ";
}

// visitor function - add the string to global variable graphOut
void graphVisitor(const string& anItem) {
    graphOut << anItem << " ";
}

// add the path to get to this vertex to global variable global variable
// [A B]
// previous is a map of vertexLabel prevVertexLabel
// need to process it in reverse to get the path
void graphCostDisplayPath(string vertex) {
    vector<string> v;
    auto prevIt = previous.find(vertex);
    while (prevIt != previous.end()) {
        v.push_back(prevIt->second);
        prevIt = previous.find(prevIt->second);
    }
    // v now has the path
    // oh, avoiding the joy of subtracting from unsigned int!
    int size = static_cast<int>(v.size());
    // return if we did we not go through any other vertex
    if (size <= 1) {
        return;
    }
    // add space between entries
    graphOut << "via [";
    // all entries except the first and last one in reverse
    // v[size-1] is the starting vertex, skipping it
    for (int i {size - 2}; i > 0; --i)
        graphOut << v[i] << " ";
    // add the second to last entry without space after it
    // last entry is the destination vertex, not needed
    if (size > 0)
        graphOut << v[0];
    graphOut << "] ";
}

// unreachable nodes should not even be in weight
// but if they are,
// skip them as they should have weight of INT_MAX
// C(8) via [A B]
// getting to C has a cost of 8, we can get to C via A->B->C
void graphCostDisplay() {
    graphOut.str("");
    for (auto it: weight) {
        int cost = it.second;
        if (cost == INT_MAX)
            continue;
        string vertex = it.first;
        graphOut << vertex << "(" << cost << ") ";
        graphCostDisplayPath(vertex);
    }
}

void testGraph0() {
    cout << "testGraph0" << endl;
    Graph g;
    g.readFile("C:\\temp\\Resh\\CSS343\\BST_assign2\\graph0.txt");
    cout << "Checing ...."<<g.getNumVertices();
     cout << isOK(g.getNumVertices(), 3) << "3 vertices" << endl;
    cout << isOK(g.getNumEdges(), 3) << "3 edges" << endl;

    graphOut.str("");
    g.depthFirstTraversal("A", graphVisitor);

    cout << isOK(graphOut.str(), string("A B C ")) << "DFS*" << endl;

    graphOut.str("");
    g.breadthFirstTraversal("A", graphVisitor);
   cout << isOK(graphOut.str(), string("A B C ")) << "BFS" << endl;

   g.djikstraCostToAllVertices("A", weight, previous);
   graphCostDisplay();
    cout << isOK(graphOut.str(), string("B(1) C(4) via [B] "))
         << "Djisktra" << endl;

}

void testGraph1() {
    cout << "testGraph1" << endl;
    Graph g;
    g.readFile("graph1.txt");
    cout << isOK(g.getNumVertices(), 10) << "10 vertices" << endl;
    cout << isOK(g.getNumEdges(), 9) << "9 edges" << endl;

    graphOut.str("");
    g.depthFirstTraversal("A", graphVisitor);
    cout << isOK(graphOut.str(), string("A B C D E F G H ")) << "DFS" << endl;

    graphOut.str("");
    g.breadthFirstTraversal("A", graphVisitor);
    cout << isOK(graphOut.str(), string("A B H C G D E F ")) << "BFS" << endl;

    g.djikstraCostToAllVertices("A", weight, previous);
    graphCostDisplay();
    cout << isOK(graphOut.str(),
                 string("B(1) C(2) via [B] "s +
                 "D(3) via [B C] E(4) via [B C D] "s +
                 "F(5) via [B C D E] " +
                 "G(4) via [H] "s +
                 "H(3) "))
         << "Djisktra" << endl;

}


void testGraph2() {
    Graph g;

    g.readFile("graph2.txt");
    cout << isOK(g.getNumVertices(), 21) << "21 vertices" << endl;
    cout << isOK(g.getNumEdges(), 24) << "24 edges" << endl << endl;

    graphOut.str("");
    g.depthFirstTraversal("A", graphVisitor);
    cout << isOK(graphOut.str(), string("A B E F J C G K L D H M I N "))
         << "DFS from A" << endl;

    graphOut.str("");
    g.depthFirstTraversal("O", graphVisitor);
    cout << isOK(graphOut.str(), string("O P R S T U Q "))
         << "DFS from O" << endl;

    graphOut.str("");
    g.breadthFirstTraversal("A", graphVisitor);
    cout << isOK(graphOut.str(), string("A B C D E F G H I J K L M N "))
         << "BFS from A" << endl;

    graphOut.str("");
    g.breadthFirstTraversal("D", graphVisitor);
    cout << isOK(graphOut.str(), string("D H I M N "))
         << "BFS from D" << endl;

    graphOut.str("");
    g.depthFirstTraversal("U", graphVisitor);
    cout << isOK(graphOut.str(), string("U "))
         << "DFS from U" << endl;

    graphOut.str("");
    g.breadthFirstTraversal("U", graphVisitor);
    cout << isOK(graphOut.str(), string("U "))
         << "BFS from U" << endl;

    g.djikstraCostToAllVertices("O", weight, previous);
    graphCostDisplay();
    cout << isOK(graphOut.str(),
                 string("P(5) Q(2) R(3) via [Q] S(6) via [Q R] ") +
                 string("T(8) via [Q R S] U(9) via [Q R S] "))
         << "Djisktra O" << endl;
}

int main() {
	testGraph0();
	testGraph1();
	testGraph2();
    return 0;
}
