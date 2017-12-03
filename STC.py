import networkx as nx
import numpy as np
import matplotlib.pyplot as plt
import copy
import time
import os.path
import pickle
import sys

   
def getDataGraph(sets, G):
    Gteams = {}
    for k in sets.keys():
        g = G.subgraph(sets[k])
        if g.number_of_nodes() >= 2:
            if nx.is_connected(g):
                 Gteams[k] = copy.deepcopy(g)
                 sets[k] = copy.deepcopy(g.nodes())
            else:
                flag = False
                for Gc in nx.connected_component_subgraphs(g):
                    if Gc.number_of_nodes() >= 2:
                        flag = True
                        Gteams[k] = copy.deepcopy(Gc)
                        sets[k] = copy.deepcopy(Gc.nodes())
                if not flag:
                    del sets[k]
        else:
            del sets[k]
 
    return sets, Gteams

def getAllOpenTriangles(G, sets):
    opens = {}
    index = {}
    
    for v in nx.nodes_iter(G):
        s1 = set(G[v])
        for w in s1:
            s2 = set(G[w])
            pair = (v,w) if v < w else (w,v)
            
            if pair not in opens:
                opens[pair] = []
            opens[pair].extend([(i,v) if i < v else (v,i) for i in (s1 - s2 - set([w]))])
            opens[pair].extend([(i,w) if i < w else (w,i) for i in (s2 - s1 - set([v]))])
            
            for teamID, nodes in sets.iteritems():
                if v in nodes and w in nodes:
                    if pair not in index:
                        index[pair] = set()
                    index[pair].add(teamID)
                    
    return opens, index
    
def hasCycles(Gteams, index, e):
    out = True
    
    if e in index:
        for k in index[e]:
            g = Gteams[k]
            if g.has_edge(*e): g.remove_edge(*e)
            out =  nx.has_path(g, e[0], e[1])
            g.add_edge(*e)            
            if out == False:
                return out               
    return out

def updateOpens(opens, e):
    for k in opens[e]:
        opens[k].remove(e)
    del opens[e]
    return opens
     

if __name__ == '__main__':

    #name = sys.argv[1]
    name = 'FB_circles' #'youtube_10000' #'BM_tags' #'lastFM_tags'                

    G, init_sets = pickle.load(open(os.path.join('UnderlyingNetwork', name +'.pkl'),'rb'))
    print 'Data: ', name
    print 'number of nodes and edges in the graph:'
    print G.number_of_nodes(), G.number_of_edges()
    sets, Gteams = getDataGraph(init_sets, G)
    print 'number of sets:', len(sets)
    
    opens, index = getAllOpenTriangles(G, sets)
    all_open = sum([len(i) for i in opens.values()])/2
    print 'initial number of open (violated) triangles:', all_open
        
    Strong = set([(i,j) if i < j else (j,i) for (i,j) in G.edges()])
    Weak = set()
    
    unremovable = set()
    
    tic = time.time()
    print 'Begin Greedy...'
    while True:
        violScore, to_remove = -1, 0
        for k,v in opens.iteritems():
            if k not in unremovable and len(v) > violScore:
                violScore = len(v)
                to_remove = k 
        if violScore <= 0:
            break                  
        canDelete = hasCycles(Gteams, index, to_remove)
        
        if canDelete:
            for g in Gteams.values():              
                if g.has_edge(*to_remove):
                    g.remove_edge(*to_remove)
                    
            opens = updateOpens(opens, to_remove)
            Strong.remove(to_remove)
            Weak.add(to_remove)
        else:
            unremovable.add(to_remove)

    print 'running time:', time.time() - tic
    res = sum([len(i) for i in opens.values()])/2
    print 'resulting number of violations: ', res
    print 'violation rate: (b in paper)', 1.0*res / all_open
    print 'number of strong and weak edges:'
    print len(Strong), len(Weak)
    print 'Strong edge ratio (s in paper): ', 1.0*len(Strong) / G.number_of_edges()
    pickle.dump((Gteams, sets, Strong, Weak), open('out/' + name + time.ctime(time.time()) + '.p2', "wb"))
