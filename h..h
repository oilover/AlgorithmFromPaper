
#ifndef GRAPH_H
#define GRAPH_H
#include <bits/stdc++.h>
using namespace std;
#define prt(k) cout<<#k" = "<<k<<endl
typedef long long LL;
void read(int &re) { scanf("%d", &re); }
const int maxTruss = 2018;
typedef pair<int,int> PII;
typedef PII PAIR;
#define Pair PAIR
typedef int vertex_t;
typedef set<vertex_t> neighbor_set_t;
template <class T>
inline Pair MP(int u,int v) {
    if (u>v) swap(u,v);
    return Pair(u,v);
}
set<T> set_intersect(set<T> a, set<T> b)
{
    set<T> si;
    set_intersection(a.begin(),a.end(), b.begin(),b.end(), inserter(si));
    return si;
}
neighbor_set_t intersect( const neighbor_set_t& set1, const neighbor_set_t& set2 ) {
    neighbor_set_t::const_iterator it1 = set1.begin();
    neighbor_set_t::const_iterator it2 = set2.begin();

    neighbor_set_t result = neighbor_set_t();
    while( it1 != set1.end() && it2 != set2.end() ) {
        vertex_t v1 = *it1;
        vertex_t v2 = *it2;
        if( v1 < v2 ) {
            ++it1;
        } else if( v1 > v2 ) {
            ++it2;
        } else {
            result.insert( *it1 );
            ++it1;
            ++it2;
        }
    }
    return result;
}
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
  //  const static int SIZE = 100;
public:
    Graph(int SIZE=100) {
        srand(time(NULL));
        nodes.clear(); N=M=0; adj_list.resize(SIZE);
        for(auto u:adj_list)u.clear();
    }
    int number_of_nodes() {
        return nodes.size();
    }
    int number_of_edges() {
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
        if (adj_list.size()<=uu) adj_list.resize(uu+1);
        if (adj_list[u].count(v)) return;
        M++;

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
    void update(Pair p);
    void TrussDecomposition();
};


#endif // GRAPH_H


