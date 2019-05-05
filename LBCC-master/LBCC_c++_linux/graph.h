#ifndef __GRAPH_H__
#define __GRAPH_H__

// Graph class
#include <vector>
#include <queue>
#include <stack>
#include <set>
#include <list>
#include <map>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <sstream>
#include <cfloat>
#include "matrix.h"
#include <limits>
using namespace std;

#define DEBUG 

const float INF = numeric_limits<float>::infinity();
const float EPS=1.0e-6; 

// nodes are integers (0, 1, ...) and may have labels
// edges are represented indirectly:
//   each node has a set of its neighbors
// there is no constructor; must read graph from a file before using it
// the caller of read() decides how many nodes there will be
// then edges are read by read() from the edgeFile
// undirected for now
class Graph {
  bool digraph; // for now this is "false" 
  int n_nodes;
  int n_edges;
  vector<float> n_weight;  // node weight (id->value)
  vector<float> e_weight;   // edge weight (id->value)
  map<string, int> nodemap;  //node name --> node id
  map<int, string> r_nodemap;  // node id --> node name
  map<pair<int,int>, int> edgemap;  /* edge pair -> edge_id */
  map<int, pair<int, int> > r_edgemap; /* edge_id -> pair */
  int betweennesscentralityunweighted(vector<float>& res);
  int betweennesscentralityweighted(vector<float>& res);
  int shortestPathsBFS(matrix<float> &d, const vector<int> &from);
  int shortestPathsDijkstra(matrix<float> &d, const vector<int> &from);
  int shortestPathsBellmanFord(matrix<float> &d, const vector<int> &from);
  int getShortestPathsBFS(vector<vector<int> >& p,  int from, const vector<int> &to);
  int getShortestPathsDijkstra(vector<vector<int> >& p,  int from, const vector<int> &to);
  int eccentricityUnweighted(vector<float>& res);
  int closenessUnweighted(vector<float>& res);
  int stressUnweighted(vector<int> &res);
public:
  bool edge_weighted;
  bool node_weighted;
  vector<vector<int> > neighbors; // for each vertex a set of neighbors
  /* overloading assignment operator */
  Graph();
  Graph& operator=(const Graph& g);
  int getNodeCount() const;
  int getEdgeCount() const;
  bool directed() { return digraph;} 
  float getEdgeWeight(int n1, int n2); /* return the weight of edge connecting node n1 and n2 */
  float getEdgeWeight(int e) const; /* return weight by edge id */
  float getNodeWeight(int n) const;
  map<pair<int,int>,int>& getEdgeMap();
  map<int, pair<int,int> >& getREdgeMap();
  map<int, string>& getRNodeMap();
  bool isNeighbor(int n1, int n2);
  pair<int,int> edgePair(int n1, int n2); /* return pair of (n1,n2) (n1<n2) */
  int edge2id(int,int);
  int node2id(string node);
  string id2node(int nid);
  pair<int, int> id2edge(int eid);
  bool checkEdgeWeight();
//int readGraph(string ppi_file, string file_name, fileType ft, weightType wt);
  int readGraph(vector<string> files, bool edgeWeighted, bool nodeWeighted, bool directed);
  int readNodeFile(string file_name);
  int readEdgeFile(string file_name, bool edge_weighted, bool directed);
  int degree(int node);
  void printDegreeSeq(); /* print degree sequence */
  int shortestPaths(matrix<float> &d, const vector<int> &from);
  int getShortestPaths(vector<vector<int> >& p,  int from, const vector<int> &to);
  int betweennesscentrality(vector<float>& res);
  int eccentricity(vector<float>& res);
  int stress(vector<int>& res);
  int diameter_approximation_sample_s(float sample_s, int n_nodes);
  int shortestpath(vector<float> &d,int m);
};

#endif
