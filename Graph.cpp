#include "Graph.h"

void Graph::readGraph(const char *filename)
{
    freopen(filename,"r",stdin);
    char line[200];
    while(gets(line)) {
        if (!isdigit(line[0])) continue;
        int u,v;
        sscanf(line, "%d%d", &u,&v);
      //  prt(u);prt(v);
        add_edge(u,v);
    }
    prt(filename);
    prt(number_of_edges());
}
set<int> set_intersect(set<int> a, set<int> b)
{
    set<int> si;
    set_intersection(a.begin(),a.end(), b.begin(),b.end(), inserter(si,si.begin()));
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
            if (result.size() >= maxTruss) break;
            ++it1;
            ++it2;
        }
    }
    return result;
}
Graph::degree(int u)
{
    return adj_list[u].size();
}
neighbor_set_t Graph::commonNeighborSet(int u, int v)
{
    return intersect(adj_list[u], adj_list[v]); //set_intersect(adj_list[u], adj_list[v]);
}
int Graph::commonNeighbor(int u, int v)
{
    return commonNeighborSet(u,v).size();
}
neighbor_set_t Graph::commonNeighborSet(PII pii)
{
    return commonNeighborSet(pii.first, pii.second);
}
int Graph::commonNeighbor(PII pii)
{
    return commonNeighbor(pii.first, pii.second);
}
void Graph::update(Pair p, int u)
{
    int sup = cns[p].size();
    assert(cns[p].count(u)==1);
    assert(sup>0);
    sup2edge[sup].erase(p);
    sup2edge[sup-1].insert(p);
    cns[p].erase(u);
}
void Graph::print_edges()
{
    for (Pair e: edge_set) {
        Prt(e);
    }
}
int Graph::TrussDecomposition()
{
    int en = number_of_edges();
    for (PII pii: edge_set) {
        int u=pii.first,v=pii.second;
        cns[pii] = commonNeighborSet(pii);
        int cn = cns[pii].size();
        support[pii] = cn;
//        Prt(pii); cout<<cn<<endl;
        sup2edge[min(cn,maxTruss)].insert(pii);
    }
    auto _cns = cns;
    int k = 2;
    int curMin = 0;
    int i=0;
    while (k<maxTruss && i<number_of_edges()) {
        while (i<number_of_edges()) {
            while (curMin<=k-2 && sup2edge[curMin].empty())
                curMin++;
            if (curMin > k-2) break;
            set<PII>::const_iterator it = sup2edge[curMin].begin();
            PAIR e = *it;
            int u=e.first,v=e.second;
            sup2edge[curMin].erase(*it);
            trussness[e] = k;
            auto cn = cns[e];
            for (int w: cn) {
                update(MP(u,w),v);
                update(MP(v,w),u);
                assert (edge_set.size() == en);
            }
            i++;
            if (curMin>0 && !sup2edge[curMin-1].empty()) curMin--;
        }
        k++;
    }
    cns = _cns;
    prt(k);
    return k;
}
void Graph::writeTruss(const char *ofn)
{
    freopen(ofn, "w", stdout);
    prt(number_of_edges());
    for (Pair e: edge_set) {
        printf("%d\t%d\t%d\n", e.first, e.second, trussness[e]);
    }
}
/*
miss (3,9)
u = 7
v = 9
w = 3
(1, 2)    (1, 3)    (1, 4)    (1, 5)    (1, 10)    (2, 3)    (2, 4)
 (3, 4)    (3, 7)    (4, 5)    (4, 6)    (4, 7)    (5, 6)    (5, 7)    (6, 7)    (6, 8)
 (6, 11)    (7, 8)    (7, 9)    (7, 10)    (7, 11)    (8, 9)    (8, 10)    (8, 11)    (9, 10)    (9, 11)    (10, 11)
 */
