# -*- coding: UTF-8 -*-
# include "graph.h"
# include "fheap.h"
# include <stdlib.h>
# define Nmax 10000
from heapq import heappush, heappop
import codecs
import re
import os


class Graph():
    """

    """

    def __init__(self):
        digraph = False
        edge_weighted = False
        node_weighted = False

        self.n_nodes = 0
        self.n_edges = 0
        self.n_weight = 0.0
        self.e_weight = 0.0
        self.neighbors = []
        self.nodemap = {}
        self.r_nodemap = {}
        self.edgemap = {}
        self.r_edgemap = {}

    def setGraph(self, g):
        self.n_nodes = g.n_nodes
        self.n_edges = g.n_edges
        self.n_weight = g.n_weight
        self.e_weight = g.e_weight
        self.neighbors = g.neighbors
        self.nodemap = g.nodemap
        self.r_nodemap = g.r_nodemap
        self.edgemap = g.edgemap
        self.r_edgemap = g.r_edgemap
        return self

    def getNodeCount(self):
        return self.n_nodes

    def getEdgeCount(self):
        return self.n_edges

    def getEdgeMap(self):
        return self.edgemap

    def getREdgeMap(self):
        return self.r_edgemap

    def getRNodeMap(self):
        return self.r_nodemap

    def id2edge(self, eid):
        return self.r_edgemap[eid]

    def tokenize(self, string, tokens):
        tokens = []
        string = string.strip()
        tokens = string.split("\t")
        #print tokens
        #print len(tokens)
        return  tokens


    def vecorFind(self, key, vec):
        if key in vec:
            return True
        else:
            return False

    def stringVectorFind(self, key, vec):
        if key in vec:
            return True
        else:
            return False

    # return 0 for success, -1 failure
    def readEdgeFile(self, edge_file, edge_weighted, directed):
        tokens = []
        nodemap_it = {}
        edgemap_it = {}
        cur_node_num = 0
        cur_edge_num = 0
        edge = {}
        cur_weight = 1.0
        fileHandle = open(edge_file, 'r')
        s = fileHandle.readlines()
        #print "ssssssssssssssssssss"
        #print s

        try:
            file_handle = open(edge_file, 'r')
        except Exception as e:
            return -1
            raise Exception("can not open the file: %s" % edge_file)

        for line in s:
            tokens = self.tokenize(line, tokens)
            #print line
            for i in range(2):  # add the node
                if tokens[i] not in self.nodemap.keys():
                    self.nodemap[tokens[i]] = cur_node_num
                    self.r_nodemap[cur_node_num] = tokens[i]
                    if i == 0:
                        n1 = cur_node_num
                    else:
                        n2 = cur_node_num
                    self.neighbors.append([])
                    cur_node_num += 1
                else:
                    if i == 0:
                        #print i
                        n1 = self.nodemap[tokens[i]]
                        #print self.nodemap[tokens[i]]
                    else:
                        #print i
                        n2 = self.nodemap[tokens[i]]
                        #print self.nodemap[tokens[i]]
            edge = self.edgePair(n1, n2)
            #print edge
            edge = edge.items()
            #print edge
            edge = str(edge[0][0])+','+str(edge[0][1])
            #print edge

            if edge in self.edgemap.keys():
                print "reading edge file: duplicated edge"
            else:
                self.edgemap[edge] = cur_edge_num
                self.r_edgemap[cur_edge_num] = edge
                self.neighbors[n1].append(n2)
                if directed == False:
                    self.neighbors[n2].append(n1)
                    cur_edge_num += 1
        fileHandle.close()
        self.n_nodes = len(self.nodemap)
        self.n_edges = len(self.edgemap)
        return 0

    def readGraph(self, files, e_weighted, n_weighted, directed):
        edge_weighted = e_weighted;
        node_weighted = n_weighted;
        if len(files) == 0:
            return -1;
        if (self.readEdgeFile(files[0], edge_weighted, directed) == -1):
            return -1;
        if (node_weighted):
            if (len(files) < 2):
                print "Error: no node weight file"
                return -1
        return 0

    def degree(self, node):
        if node > self.n_nodes:
            print "no such node exists in the graph"
            return -1
        return len(self.neighbors[node])

    def edge2id(self, a, b):
        edge = self.edgePair(a, b)
        if edge in self.edgemap:
            return self.edgemap[edge]
        else:
            return -1

    def node2id(self, node):
        if node in self.nodemap:
            return self.nodemap[node]
        else:
            return -1

    def id2node(self, nid):
        if nid in self.r_nodemap:
            return self.r_nodemap[nid]
        else:
            return None

    def edgePair(self, n1, n2):
        tempv = {}
        temp = 0
        if n1 > n2:
            temp = n1
            n1 = n2
            n2 = temp
        tempv[n1] = n2
        return tempv

    def isNeighbor(self, n1, n2):
        if n2 in self.neighbors[n1]:
            return True
        else:
            return False

    def betweennesscentrality(self, res):
#        if self.edge_weighted == False:
            return self.betweennesscentralityunweighted(res)
 #       else:
 #           return -1

    def betweennesscentralityunweighted(self, res):
        res = [0] * self.n_nodes
        for i in range(self.n_nodes):
            #print "bc:%d"%i
            s = []  # 栈
            #l = []
            vli = [[] for xy in range(self.n_nodes)]
            #vli = [l] * self.n_nodes  # 双向链表
            sigma = [0] * self.n_nodes
            d = [-1] * self.n_nodes
            tri = [0.0] * self.n_nodes
            sigma[i] = 1
            d[i] = 0
            q = []  # 队列

            # 计算i点到每一节点的最短路径数，广度优先算法
            q.append(i)
            #os.system('pause')
            #print q
            while len(q) != 0:
                v = q[0]
                q.pop(0)
                s.append(v)
                #print v
                for ii in range(len(self.neighbors[v]) ):
                    #os.system('pause')
                    w = self.neighbors[v][ii]
                    #print 'self.neighbors'
                    #print self.neighbors[v]
                    #print self.neighbors[v][ii]
                    #print 'w'
                    #print w
                    if d[w] < 0:
                        q.append(w)
                        d[w] = d[v] + 1
                    if d[w] == d[v] + 1:
                        sigma[w] += sigma[v]
                        tri[w] = 1.0 * 1 / sigma[w]
                        vli[w].append(v)
                       # print v
                    #print 'w=%d'%w
                    #print tri[w]
                    #print v
                    #print vli[w]
            #print s

            delta = [0.0] * self.n_nodes
            while len(s) != 0:
                w = s[len(s)-1]
                #print w
                s.pop(len(s)-1)
                for jj in range(len(vli[w])):
                    v = vli[w][jj]
                    #print "w%d"%w
                    #print v
                    tri[v] += tri[w]
                    delta[v] = tri[v] * sigma[v] - 1
                    #print delta[v]
                if w != i:
                    res[w] += delta[w]
                #print 'res[w]=%d'%res[w]

#        if self.diagraph == False:
        for i in range(self.n_nodes):
            res[i] = res[i] / 2.0
            #print "%d=%f"%(i,res[i])

        return res


