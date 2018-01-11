#include "Graph.h"
string fn = "toy.txt";
string ofn = fn + ".truss0";
int main(int argc, char **argv)
{
    prt(CLOCKS_PER_SEC);
    Graph G = Graph();
    double t0 = clock();
    G.readGraph(fn.c_str());
    double t1 = clock();
    cerr << "Read time: " << (t1-t0)/CLOCKS_PER_SEC<<endl;
    G.TrussDecomposition();
    double  t2 = clock();
    cerr << "Truss time: " << (t2-t1)/CLOCKS_PER_SEC << endl;
    G.writeTruss(ofn.c_str());

    return 0;
}
// intersect(): 13 35  53s
//              38 60  77s
/*
  Truss time: 60
number_of_edges() = 925872


Process returned 0 (0x0)   execution time : 77.216 s
Press any key to continue.
-------
filename = com-amazon.ungraph.txt
number_of_edges() = 925872
Read time: 7
Truss time: 13

Process returned 0 (0x0)   execution time : 26.002 s
Press any key to continue.
--------
filename = com-amazon.ungraph.txt
number_of_edges() = 925872
Read time: 7
Truss time: 38

Process returned 0 (0x0)   execution time : 51.182 s
Press any key to continue.
----
filename = facebook_combined.txt
number_of_edges() = 88234
Read time: 486
k = 98
Truss time: 57371
number_of_edges() = 88234

Process returned 0 (0x0)   execution time : 61.621 s
Press any key to continue.


*/
m
