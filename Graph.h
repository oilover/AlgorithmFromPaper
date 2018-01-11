
#ifndef GRAPH_H
#define GRAPH_H
#include <bits/stdc++.h>
using namespace std;
#define prt(k) cerr<<#k" = "<<k<<endl
typedef long long LL;
//void Read(int &re) { scanf("%d", &re); }
const int maxTruss = 2018;
typedef pair<int,int> PII;
typedef PII PAIR;
#define Pair PAIR
#define PP PII
typedef int vertex_t;
typedef set<vertex_t> neighbor_set_t;

inline Pair MP(int u,int v) { // make_pair
    if (u>v) swap(u,v);
    return Pair(u,v);
}
inline void Prt(PII p) {
    fprintf(stderr,"(%d, %d)    ",p.first,p.second);
}
//template <class T>
set<int> set_intersect(set<int> a, set<int> b);
neighbor_set_t intersect( const neighbor_set_t& set1, const neighbor_set_t& set2 );
struct Edge
{
    int u,v,cn;
    bool operator<(Edge rhs) const {
        return cn > rhs.cn;
    }
};
class Graph
{
private:
    int N,M;
    set<int> nodes;
    vector<set<int> > adj_list;
    map<PII,int> trussness, support;
    set<PII> edge_set;
    set<PAIR> sup2edge[maxTruss+5];
    map<PII,set<int> > cns;
  //  const static int SIZE = 100;
public:
    Graph(int SIZE=100) {
        srand(time(NULL));
        nodes.clear(); N=M=0; adj_list.resize(SIZE);
        edge_set.clear();
        for(set<int> &u:adj_list)u.clear();
    }
    void readGraph(const char *filename);
    int number_of_nodes() {
        return nodes.size();
    }
    int number_of_edges() const{
        return edge_set.size();
    }
    set<int> get_nodes()
    {
        return nodes;
    }
    void add_edge(int u, int v)
    {
        if (u>v) swap(u,v);
        int uu = max(u,v);
        if (adj_list.size()<=uu) adj_list.resize(uu+20);
    //    if (adj_list[u].count(v)) return;
        nodes.insert(u); nodes.insert(v);
        if (edge_set.count(PP(u,v)) ) {
            cerr<<"Multiple edge\n";  Prt(PP(u,v));
            exit(0);
        }
        edge_set.insert(PII(u,v));
        adj_list[u].insert(v);
        adj_list[v].insert(u);
    }
    bool has_edge(int u, int v)
    {
        return adj_list[u].count(v);
    }
    neighbor_set_t commonNeighborSet(int u, int v);
    int commonNeighbor(int u, int v);
    neighbor_set_t commonNeighborSet(PAIR);
    int commonNeighbor(PII pii);
    void update(Pair,int);
    int TrussDecomposition();
    int degree(int);
    void print_edges();
    void writeTruss(const char *);
};


#endif // GRAPH_H


