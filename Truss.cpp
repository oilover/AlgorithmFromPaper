#include "Graph.h"


neighbor_set_t Graph::commonNeighborSet(int u, int v)
{
    return set_intersect(adj_list[u], adj_list[v]);
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
void Graph::update(Pair p)
{
    int sup = support[p];
    sup2edge[sup].erase(p);
    sup2edge[sup-1].insert(p);
    support[p]--;
}
void Graph::TrussDecomposition()
{
    for (PII pii: edge_set) {
        int u=pii.first,v=pii.second;
        int cn = commonNeighbor(pii);
        support[pii] = cn;
        sup2edge[min(cn,maxTruss)].insert(pii);
//        edge_list.push_back((Edge){u,v,commonNeighbor(u,v)});
    }
    auto _support = support;
    int k = 2;
    int curMin = 0;
    int i=0;
    while (k<maxTruss && i<number_of_edges()) {
        while (i<number_of_edges()) {
            while (curMin<=k-2 && sup2edge[curMin].empty())
                curMin++;
            if (curMin > k-2) break;
            auto it = sup2edge[curMin].begin();
            PAIR e = *it;
            int u=e.first,v=e.second;
            sup2edge[curMin].erase(it);
            trussness[e] = k;
            auto cn = commonNeighborSet(e);
            for (int w: cn) {
                update(MP(u,w));
                update(MP(v,w));
            }
            i++;
            if (curMin>0 && !sup2edge[curMin-1].empty()) curMin--;
        }
        k++;
    }
    support = _support;
}
