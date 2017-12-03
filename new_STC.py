import networkx as nx
import numpy as np
import matplotlib.pyplot as plt
import copy
import time
import os.path
import pickle
import sys


def get_rID(sets):
    # preprocess
    rID = dict()
    for ID, team in sets.items():
        for user in team:
            if not user in rID: rID[user] = set()
            rID[user].add(ID)
    return rID
def hascf(sets):
    for k in sets.keys():
        for k2 in sets.keys():
            if k!=k2 and sets[k].issubset(sets[k2]):
                k,k2 = ID,ID2
                print ID, 'is subcommunity of', ID2
                return True
    return False                

def write_processed_sets(G, sets, name):
    pickle.dump((G,sets), open(os.path.join('UnderlyingNetwork', name +'.dat'),'wb') )

def quchong(sets):
    for k in sets.keys(): sets[k] = set(sets[k])
    team_list = sorted(sets.items(), key=lambda x:len(x[-1]))
    for it in range(len(team_list)):
        ID, team = team_list[it]
        for jt in range(it+1, len(team_list)):
            ID2, team2 = team_list[jt]
            if team.issubset(team2):
                del sets[ID]  # sets.pop(ID)
                print ID, 'is subcommunity of', ID2
                break    
    return sets

# get Gteams: Graph for every team
def getDataGraph(sets, G):  # sets: teams
    Gteams = {}
    for k in sets.keys():
        g = G.subgraph(sets[k])
        if g.number_of_nodes() >= 2:
            if nx.is_connected(g):
                 Gteams[k] = copy.deepcopy(g)
                 sets[k] = set(copy.deepcopy(g.nodes()))
            else:
                flag = False
                sets[k] = set()
                for Gc in nx.connected_component_subgraphs(g):
                    if Gc.number_of_nodes() >= 2: # max(2, len(sets[k])):
                        flag = True
                        Gteams[k] = copy.deepcopy(Gc)
                        sets[k] = set(copy.deepcopy(Gc.nodes()))
                if not flag: # if not exist 2 connected nodes
                    del sets[k]
        else:
            del sets[k]
    sets = quchong(sets)
    return sets, Gteams  

def getAllOpenTriangles(G, sets):   #  open[(u,v)]:  third edge of open triangles (u,v) in
    opens = {}          #   reverse:  node --> teamID
    index = {}
    # close = {}
    for v in nx.nodes_iter(G):
        s1 = set(G[v])
        for w in s1:
            s2 = set(G[w])
            pair = (v,w) if v < w else (w,v)
            
            if pair not in opens:
                opens[pair] = set()
                # close[pair] = set()
            else:   # add following two lines
                continue
            opens[pair] |= set([(i,v) if i < v else (v,i) for i in (s1 - s2 - set([w]))])
            opens[pair] |= set([(i,w) if i < w else (w,i) for i in (s2 - s1 - set([v]))])
            # close[pair] = s1 & s2
            for teamID, nodes in sets.iteritems():
                if v in nodes and w in nodes:
                    if pair not in index:
                        index[pair] = set()
                    index[pair].add(teamID)  # which teams edge(v,w) belongs to
    for pair in opens: 
        if opens[pair].empty(): del opens[pair]        
    return opens, index


def hasCycles(Gteams, index, e):  # index: teamIDs_of_edge
    out = True     # whether remove e would result in some team disconnected
    
    if e in index:
        for team in index[e]: # for every team e belong to -
            g = Gteams[team]
            g.remove_edge(*e)
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
    

def main(Gteams, sets):
    opens, index = getAllOpenTriangles(G, sets)
    print 'initial number of open (violated) triangles:', sum([len(i) for i in opens.values()])/2  
    backup_opens = opens
    Strong = set([(i,j) if i < j else (j,i) for (i,j) in G.edges()])
    Weak = set()
    
    unremovable = []
    
    tic = time.time()
    edge_list = opens.items()
    edge_list = sorted(edge_list, key=lambda x:x[-1], reverse=True)
    edge_list = [x[0] for x in edge_list]
    for e in edge_list:
        violScore = len(opens[e])
        to_remove = e
        if violScore<=0: break
        canDelete = hasCycles(Gteams, index, to_remove)        
        if canDelete:
            if to_remove in index:
                for gID in index[to_remove]: #Gteams.values():              
                    g = Gteams[gID]
                    if g.has_edge(*to_remove):
                        g.remove_edge(*to_remove)
                    
            opens = updateOpens(opens, to_remove)
            Strong.remove(to_remove)
            Weak.add(to_remove)

    print 'running time:', time.time() - tic

    print 'resulting number of violations: ', (sum([len(i) for i in opens.values()]))
    print 'number of strong and weak edges:'
    print len(Strong), len(Weak)
    pickle.dump((Gteams, sets, Strong, Weak), open('out' + name + '.p0', "wb"))
    # TODO: preprocess node->community, edge->community(subgraph)

if __name__ == '__main__':

    #name = sys.argv[1]
    name = 'lastFM_tags'                

    G, init_sets = pickle.load(open(os.path.join('UnderlyingNetwork', name +'.pkl'),'rb'))
    print 'Data: ', name
    print 'number of nodes and edges in the graph:'
    print G.number_of_nodes(), G.number_of_edges()
    sets = init_sets
    sets, Gteams = getDataGraph(sets, G)
    print 'number of sets:', len(sets)
    
    main(Gteams, sets)

    # pickle.dump((Gteams, sets, opens, Strong, Weak), open('out/'+name + '.open', "wb"))
