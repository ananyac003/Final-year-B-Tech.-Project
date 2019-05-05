#!usr/bin/python
# -*- coding: UTF-8 -*-

# include<stdio.h>
# include<unistd.h>
# include<cstdlib>
# include<string.h>
# include<cmath>
# include<Windows.h>
# include"graph.h"
# include <time.h>

# include <iostream>
# include <fstream>
from Graph import Graph

M = 5



def main():
    c = ''

    edge_file = 'input.txt'  
    output_f = 'output.txt'
    g = Graph()
    directed = False;
    t = False


    fileHandle = open(edge_file, 'r')
    print "reading the file\n"
    fileHandle.seek(0)
    vs = fileHandle.readlines()
    if g.readEdgeFile(edge_file, False, False) == -1:
        print "fail\n"
    print "success\n"

    fileHandle2 = open(output_f, 'w')
    print "open the file\n"

    n_count = g.getNodeCount()
    print "the number of nodes:%d" % n_count
    e_count = g.getEdgeCount();
    print "the number of edges:%d" % e_count

    sum1 = 0
    sum2 = 0
    sumimp = 0
    bc = []
    imp = [0]*n_count
    multi = [0]*n_count
    map = {}

    print "caculating BC..."
    bc=g.betweennesscentrality(bc)

    # 标记关键蛋白
    ess_file = 'deg.txt'
    fileHandle3 = open(ess_file, 'r')


    fileHandle3.seek(0)

    ve = fileHandle3.readlines()
    for i in range(len(ve)):
        ve[i] = ve[i].strip()

    for i in range(n_count):
        for j in range(len(ve)):
            if ve[j] == g.id2node(i):
                #print g.id2node(i)
                imp[i]=1
                break
            else:
                imp[i]=0
    fileHandle3.close()

    # 读入complex复合物
    complex_file = 'complex.txt'
    fileHandle4 = open(complex_file, 'r')
    fileHandle4.seek(0)
    complex = fileHandle4.readlines()


    for i in range(len(complex)):
        complex[i] = complex[i].strip()
        complex[i] = complex[i].split(' ') #complex 以空格分割

    print "caculating DEN_1,DEN_2..."
    # 将1-邻域邻居节点，存储在pneighbors中
    pneighbors = []
    pneighbor = []
    for j in range(n_count):
        pneighbor = []
        pneighbor.append(j)
        for r in range(n_count):
            if g.isNeighbor(j, r):
                pneighbor.append(r)
        pneighbors.append(pneighbor)
        pneighbors[j] = list(set(pneighbors[j]))  # 将邻居节点中重复顶点删除

    # 将2-邻域邻居节点，存储在p2neighbors中
    p2neighbors = []
    for j in range(n_count):
        pneighbor = []
        pneighbor.append(j)
        for r in range(n_count):
            if g.isNeighbor(j, r):
                pneighbor.append(r)
                for a in range(n_count):
                    if g.isNeighbor(a, r):
                        pneighbor.append(a)
        p2neighbors.append(pneighbor)
        p2neighbors[j] = list(set(p2neighbors[j]))  # 将邻居节点中重复顶点删除

    # 计算1-邻域中（包括顶点本身）边的个数，计算结果为边的个数的两倍
    sum = [0] * n_count
    for i in range(n_count):
        sum1 = 0
        #print len(pneighbors[i])
        for j in range(len(pneighbors[i])):
            for l in range(len(pneighbors[i])):
                if pneighbors[i][j] == pneighbors[i][l]:
                    continue
                if g.isNeighbor(pneighbors[i][j], pneighbors[i][l]):
                    sum1 = sum1 + 1
        sum[i] = sum1

    # 计算2 - 邻域中（包括顶点本身）边的个数,计算结果为边个数两倍
    summ = [0] * n_count
    for i in range(n_count):
        sum2 = 0
        for j in range(len(p2neighbors[i])):
            for l in range(len(p2neighbors[i])):
                if p2neighbors[i][j] == p2neighbors[i][l]:
                    continue
                if g.isNeighbor(p2neighbors[i][j], p2neighbors[i][l]):
                    sum2 = sum2 + 1
        summ[i] = sum2

    # //计算1-邻域，2-邻域密度,如果为单独的点，密度定义为0
    result = []
    result2 = []
    for i in range(n_count):
        if len(pneighbors[i]) == 1:
            m1 = 0
        else:
            m1 = sum[i] * 1.0 / (len(pneighbors[i]) * (len(pneighbors[i]) - 1))
        if len(p2neighbors[i]) == 1:
            m2 = 0
        else:
            m2 = summ[i] * 1.0 / (len(p2neighbors[i]) * (len(p2neighbors[i]) - 1))
        result.append(m1)
        result2.append(m2)

    print "caculating IDC..."
    # 计算 IDC的 complex 值
    IDC = []
    temp = 0
    for i in range(n_count):
        for j in range(len(complex)):
            for l in range(len(complex[j])):
                if g.id2node(i) == complex[j][l]:
                    for k in range(len(complex[j])):
                        if (g.id2node(i) == complex[j][k]):
                            continue
                        if (g.isNeighbor(i, g.node2id(complex[j][k]))):
                            temp = temp + 1
        if temp == 0:
            IDC.append(0.001)
        else:
            IDC.append(temp)
        temp = 0

    # 计算 LBCC
    # set i =1, j=4, ii=3, jj=1
    print "caculating LBCC..."
    for jj in range(1, 2):
        for i in range(1, 2):
            for j in range(4, 5):
                for ii in range(3, 4):
                    sumimp = 0
                    for k in range(n_count):
                        if result[k] == 0 or result2[k] == 0:
                            multi[k] = 0
                        else:
                            multi[k] = pow(result[k], i) * pow(result2[k], j) * pow(IDC[k], ii) * pow(bc[k], jj)
                    #print multi
                    for i1 in range(n_count):
                        map[i1] = multi[i1]

                    s = 0
                    map = sorted(map.iteritems(), key=lambda asd: asd[1], reverse=True)
                    for i2 in range(M):
                        sumimp = sumimp + map[i2][1]

    fileHandle5 = open(output_f, 'w')
    fileHandle5.write("name\tDEN_1\tDEN_2\tBC\tIDC\tLBCC\tESS\n")
    for i in range(n_count):
        fileHandle5.write("%s\t%.4f\t%.4f\t%.4f\t%.4f\t%.4f\t%d\n" %(g.id2node(i), result[i], result2[i], bc[i], IDC[i], multi[i],imp[i]))

    fileHandle.close()
    fileHandle2.close()
    fileHandle4.close()
    fileHandle5.close()

    return 0


if __name__ == '__main__':
    main()
