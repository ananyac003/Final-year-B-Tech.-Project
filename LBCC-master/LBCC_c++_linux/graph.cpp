#include "graph.h"
#include "fheap.h"
#include <stdlib.h>
#define Nmax 10000

// default constructor
Graph::Graph(){
  digraph=false;
  edge_weighted=false;
  node_weighted=false;
}

Graph& Graph::operator=(const Graph& g){
  if(this!=&g){
    n_nodes=g.n_nodes;
    n_edges=g.n_edges;
    n_weight=g.n_weight;
    e_weight=g.e_weight;
    neighbors=g.neighbors;
    nodemap=g.nodemap;
    r_nodemap=g.r_nodemap;
    edgemap=g.edgemap;
    r_edgemap=g.r_edgemap;
  }
  return *this;       //在类的非静态成员函数中返回类对象本身的时候使用
}

int Graph::getNodeCount() const{
  return n_nodes;
}

int Graph::getEdgeCount() const{
  return n_edges;
}

map<pair<int,int>,int>& Graph::getEdgeMap(){
  map<pair<int,int>,int>& em=edgemap;
  return em;
}

map<int, pair<int,int> >& Graph::getREdgeMap(){
  map<int, pair<int,int> >& em=r_edgemap;
  return em;
}

map<int, string>& Graph::getRNodeMap(){
  map<int, string>& em=r_nodemap;
  return em;
}

pair<int, int> Graph::id2edge(int eid){
  return r_edgemap[eid]; 
}

/* string tokenzier */
void Tokenize(const string& str, vector<string>& tokens,
   const string& delimiters = " "){
  string::size_type lastPos=str.find_first_not_of(delimiters, 0);
  string::size_type pos=str.find_first_of(delimiters, lastPos);

  while (string::npos != pos || string::npos != lastPos)
  {
    tokens.push_back(str.substr(lastPos, pos - lastPos));
    lastPos = str.find_first_not_of(delimiters, pos);
    pos = str.find_first_of(delimiters, lastPos);
  }
}

/* find an item in a vector */
inline bool vectorFind(const int key, const vector<int>& vec) {
  for (unsigned int i = 0; i < vec.size(); i++)
    if (vec[i] == key)
      return true;
  return false;
}

/* find an item in a vector */
inline bool stringVectorFind(const string key, const vector<string>& vec) {
  for (unsigned int i = 0; i < vec.size(); i++)
    if (key.compare(vec[i]) == 0)
      return true;
  return false;
}

/* return 0 for success, -1 failure 
 * edge file  format:
   nodename1 nodename2 [weight]
   nodename3 nodename4 [weight]
   ...
  */
int Graph::readEdgeFile(string edge_file, bool edge_weighted, bool directed){
  string line;
  vector<string> tokens;
  int i;
  ifstream edgefile;
  map<string, int>::iterator nodemap_it;
  map< pair<int,int>, int>::iterator edgemap_it;
  int cur_node_num=0, cur_edge_num=0;
  int n1,n2;
  pair<int,int> edge;
  float cur_weight=1.0; // default value

  edgefile.open(edge_file.c_str());
  if(!edgefile){
    cout<<"cannot open file: "<<edge_file<<endl;
    return -1;
  }

 // string::iterator it;

  while(getline(edgefile, line)){
//line[line.size()-1]='\0';
   //line=line.Substring(0,s.Length-1)
   //printf<<line.size();
   //line.erase(line.size()-1);
//    for (it =line.begin(); it != line.end(); ++it)
 //   {
 //       if ( *it == '\n' || *it == '\r')
   //     {
     //       line.erase(it);
       // }
    //}
    tokens.clear();
    Tokenize(line, tokens,"\t");
    if(tokens.size() < 2){
      cout<<"reading edge file: this is not an edge, continue..."<<endl;
      continue;
    }
    else if(tokens.size()==3 && edge_weighted){
      stringstream ss(tokens[2]);
      ss>>cur_weight;
    }
    for(i=0; i<2; i++){
      nodemap_it=nodemap.find(tokens[i]);
      if(nodemap_it == nodemap.end()){ /*not found */
        nodemap.insert(make_pair(tokens[i],cur_node_num));
        r_nodemap.insert(make_pair(cur_node_num, tokens[i]));
        /* intialize node weight to INF */
        n_weight.push_back(INF);
        if(i==0)
          n1=cur_node_num;
        else
          n2=cur_node_num;
        /* initialize the neighbor vector for the new node */
        neighbors.push_back(vector<int>());
        cur_node_num++;
      }       
      else{ /* the node is already added */
        if(i==0)
          n1=nodemap_it->second; 
        else
          n2=nodemap_it->second; 
      }
    }
    /* now adding the edge */
    edge=edgePair(n1,n2);
    /* the edge should be added previously, otherwise a duplicated edge */
    edgemap_it=edgemap.find(edge);
    if(edgemap_it != edgemap.end()){
      cout<<"reading edge file: duplicated edge"<<endl;
    }
    else{
      edgemap.insert(make_pair(edge,cur_edge_num));
      r_edgemap.insert(make_pair(cur_edge_num, edge));
      e_weight.push_back(cur_weight);
      /* update the neighbors */
      neighbors[n1].push_back(n2);
      if(!directed)
        neighbors[n2].push_back(n1);
      cur_edge_num++;
    }
  }
  edgefile.close();
  n_nodes=nodemap.size();
  n_edges=edgemap.size();
#ifdef DEBUG2
  cout<<"n_nodes="<<n_nodes<<endl;
  cout<<"n_edges="<<n_edges<<endl;
#endif
  return 0;
}

/* node file:
 * node_name weight
 * node_name weight
 */
int Graph::readNodeFile(string file_name){
  string line;
  vector<string> tokens;
  ifstream node_file;

  /* initially all node weight is INF */
  node_file.open(file_name.c_str());
  if(!node_file){
    cout<<"cannot open file "<<file_name<<endl;
    return -1;
  }
  while(getline(node_file, line)){
    tokens.clear();
    Tokenize(line, tokens,"\t");     
    if((int)tokens.size()<2){
      cout<<"Node weight not set."<<endl;
      return -1;
    }
    /* if the gene is not found in the PPI
        just ignore */
    if(nodemap.find(tokens[0])==nodemap.end())
      continue;
    /* the first token is the node name */
    stringstream ss(tokens[1]);
    float weight;
    ss>>weight;
    n_weight[node2id(tokens[0])]=weight;
#ifdef DEBUG2
    cout<<tokens[0]<<endl;
#endif
  } 
  node_file.close();
  return 0;
}

 /** return 0 for success, -1 failure 
  * vector<string> files: contains the name(s) of edge list file (required), node weight file (optional)
 */
int Graph::readGraph(vector<string> files, bool e_weighted, bool n_weighted, bool directed){
  edge_weighted=e_weighted;
  node_weighted=n_weighted;
  if(files.size()==0)
    return -1;
  if(readEdgeFile(files[0], edge_weighted, directed)==-1)
    return -1;
  if(node_weighted){
    if(files.size()<2){
      cout<<"Error: no node weight file"<<endl;
      return -1;
    }
    else
      return readNodeFile(files[1]);
  }
  return 0;
}

/* print the degree sequence (increasing order)
 * to the stdout
 */
void Graph::printDegreeSeq(){
  vector<int> dseq;
  int n_nodes;
   
  n_nodes=getNodeCount();
  for(int i=0; i<n_nodes; i++){
    dseq.push_back(degree(i));
  }

  sort(dseq.begin(), dseq.end());
  
  cout<<"Degree sequence: ";
  for(vector<int>::iterator it=dseq.begin(); it!=dseq.end();
      it++){
    cout<<*it<<" ";
  }
  cout<<endl;

}

/* return the degree of a node */
int Graph::degree(int node){
  if(node>=n_nodes){
    cout<<"no such node exists in the graph"<<endl;
    return -1;
  }
  return neighbors[node].size();
}

/* get edge id from edgemap */
int Graph::edge2id(int a, int b){
   map<pair<int,int>,int>::iterator id;
   pair<int, int> edge;
   
   edge=edgePair(a,b);
   id=edgemap.find(edge);
   if(id !=edgemap.end()){
     return id->second;
   }
   else{
     return -1;
   }
}

/* convert node name to id */
int Graph::node2id(string node){
  map<string,int>::iterator id;

  id=nodemap.find(node);
  if(id!=nodemap.end()){
    return id->second;
  }
  else{
   return -1;
  }
}

/* convert node id to name */
string Graph::id2node(int nid){
  map<int,string>::iterator it;

  it=r_nodemap.find(nid);
  if(it!=r_nodemap.end()){
    return it->second;
  }
  else
   return NULL;
}

pair<int, int> Graph::edgePair(int n1, int n2) {
  int temp;
  if(n1>n2){
    temp=n1;
    n1=n2;
    n2=temp;
  }
#ifdef DEBUG2
  cout<<"edge: "<<edge<<endl;
#endif
  return make_pair(n1,n2);
}

/* return the weight of edge connecting node n1 and n2 */   
float Graph::getEdgeWeight(int n1, int n2){

  if(!isNeighbor(n1, n2))
    return INF;
  return e_weight[edge2id(n1,n2)];
}

float Graph::getEdgeWeight(int e) const{
  return e_weight[e];
}

float Graph::getNodeWeight(int n) const{
  if(n>n_nodes){
    cerr<<"node id > graph size, exiting..."<<endl;
    exit(1);
  }
  return n_weight[n];
}

bool Graph::isNeighbor(int n1, int n2){
  if(vectorFind(n2,  neighbors[n1]))
    return true;
  else
    return false;
}

/* check if the edge weight contains negative number
 * return:
 *   true - there is negative edge weight
 *   false - otherwise
 */
bool Graph::checkEdgeWeight(){
  if(*min_element(e_weight.begin(), e_weight.end())>=0)
    return true;
  else
    return false;
}

/* Calculate the length of shortest paths between vertices
 * input:
     from:  a list of source vertice node IDs
   output:
     d:   a matrix of distances with size nxm
         n - the size of "from"
         m - the size of graph (the number of nodes)
     d[i,j] - the shortest distance from node i to j, if unreachable, set to INF
   return:
      0 - success
      -1 - failure
 */
int Graph::shortestPaths(matrix<float> &d, const vector<int> &from){
  if(!edge_weighted){
    cout<<"edge unweighted"<<endl;
    return shortestPathsBFS(d, from);
  }
  else{
    bool noNegativeEdgeWeight=checkEdgeWeight();
    if(noNegativeEdgeWeight){    
      return shortestPathsDijkstra(d, from);
    }
    else 
      return shortestPathsBellmanFord(d, from);
   }
/* TODO: 
 * Johnson's algorithm 
 */
}


/* Using BFS to solve shortest path problem
 * See: shortestPaths() for input/output/return parameters
 */
int Graph::shortestPathsBFS(matrix<float> &d, const vector<int> &from){
  vector<int>::const_iterator vit;
  queue<int> q;
  queue<float> qdist;  //距离
  vector<int> already_counted(n_nodes);
  int i;
  for(vit=from.begin(),i=0; vit!=from.end(); vit++, i++){  
    int reached=1;  //计数
    q.push(*vit);
    qdist.push(0);
    already_counted[*vit]=i+1;

    while(!q.empty()){
        int act=q.front();
      q.pop();
      float actdist=qdist.front();
      qdist.pop();
      d(i,act)=actdist;
      vector<int>::iterator vit2;
      for(vit2=neighbors[act].begin(); vit2!=neighbors[act].end(); vit2++){
        int neighbor=(*vit2);
        if(already_counted[neighbor] == i+1) 
          continue; 
        already_counted[neighbor] = i+1;
        reached++;
        q.push(neighbor);
        qdist.push(actdist+1);
      }
    }
    int j=0;
    while(j<n_nodes){
      if(fabs(d(i,j))<EPS && j!=*vit){   //fabs(d(i,j))<EPS：判断d(i,j)是否为0；整句话的意思是搜索不与节点i相邻的节点，等于遍历整个图
        d(i,j)=INF;
        reached++;
      }
      j++;
    }
  }
  return 0;
}

/* shortest path: weighted graph using Dijkstra algorithm
 */
int Graph::shortestPathsDijkstra(matrix<float> &d, const vector<int> &from){
  FHeap h(n_nodes);
  float dist;
  vector<int>::const_iterator vit;
  int i,j;
  int v;

  /* initialize the distance matrix to all infinity */
  for(i=0; i<from.size(); i++){
    for(j=0; j<n_nodes; j++){
      d(i,j)=INF;
    }
  }

  for(vit=from.begin(),i=0; vit!=from.end(); vit++, i++){
   // bool s[n_nodes]; /* solution set */
    //bool f[n_nodes]; /* frontier */

	 bool s[Nmax]; /* solution set */
    bool f[Nmax]; /* frontier */



    for(j=0; j<n_nodes; j++){
      s[j]=f[j]=false;
    }
    d(i,*vit)=0;
    h.insert(*vit,0);
    f[*vit]=true;
    while(h.nItems()>0){
      v=h.deleteMin();
      s[v]=true;
      f[v]=false;
      vector<int>::iterator vit2;
      for(vit2=neighbors[v].begin(); vit2!=neighbors[v].end(); vit2++){
        if(s[*vit2]==false){
          dist=d(i,v)+getEdgeWeight(v,*vit2);
          if(dist<d(i,*vit2)){
            d(i,*vit2)=dist;
            if(f[*vit2])
              h.decreaseKey(*vit2,dist);
            else{
              h.insert(*vit2,dist);
              f[*vit2]=true;
            }
          }
        }
      }
    }
  }
  return 0;
}

int Graph::shortestPathsBellmanFord(matrix<float> &d, const vector<int> &from){
  return 0;
}

/* Calcuates the shortest paths between vertices,
 * if there is more than one shortest path between two vertices, this
 * function gives only one of them.
 * input:
     from:  a source vertice IDs
     to:    distinct destination vertice IDs
   output:
     p:   a vector of vectors
       the size of p is the size of vector "to"
       each element i of "p" p[i]  is a vector
       that represents a path from vertex "from" to 
       vertex to[i]
   return:
      0 - success
      -1 - failure
 */
int Graph::getShortestPaths(vector<vector<int> >& p,  int from, const vector<int> &to){
  if(!edge_weighted){
    cout<<"edge unweighted"<<endl;
    return getShortestPathsBFS(p, from, to);
  }
  else{
    bool noNegativeEdgeWeight=checkEdgeWeight();
    if(noNegativeEdgeWeight){    
      return getShortestPathsDijkstra(p, from, to);
    }
    else 
      return -1; /* for now negative weight is not supported */
   }
}


int Graph::getShortestPathsBFS(vector<vector<int> >& p,  int from, const vector<int> &to){    //得到最短路径
  int to_reach, reached;

  to_reach=to.size();
  reached=0;

  vector<int> pred;
  for(int i=0; i<n_nodes; i++)
    pred.push_back(0);
  vector<int>::const_iterator vit2;
  for(vit2=to.begin(); vit2!=to.end(); vit2++){
    pred[*vit2]=-1;
  }

#ifdef DEBUG2
  for(int i=0; i<n_nodes; i++)
    cout<<pred[i]<<" ";
  cout<<endl;
#endif

  queue<int> q;
  q.push(from+1);
  if(pred[from]<0)
    reached++; 
  pred[from]=from+1;
  vector<int>::iterator vit;
  while(!q.empty() && reached<to_reach){
    int act=q.front();
    q.pop();
    for(vit=neighbors[act-1].begin(); vit!=neighbors[act-1].end(); vit++){
      if(pred[*vit]>0)
        continue;
      else if(pred[*vit]<0)
        reached++;
      pred[*vit]=act;
      q.push((*vit)+1);
    }
  }

  if(reached < to_reach)
    cerr<<"some vertices are not reachable"<<endl;
 
#ifdef DEBUG2
  for(int i=0; i<n_nodes; i++)
    cout<<pred[i]<<" ";
  cout<<endl;
#endif

  int i;
  for(vit2=to.begin(),i=0; vit2!=to.end(); vit2++, i++){
    p[i].push_back(*vit2);
    if(pred[*vit2]>0){
      int act=*vit2;
      while(pred[act]!=act+1){
        p[i].push_back(pred[act]-1);
        act=pred[act]-1;
      }
      reverse(p[i].begin(), p[i].end());
    }
  }
  return 0;//添加
}

int Graph::getShortestPathsDijkstra(vector<vector<int> >& p,int from, const vector<int> &to){
  int to_reach, reached;
  float dist;
  FHeap h(n_nodes);
  int v;

  to_reach=to.size();
  reached=0;
  vector<int> pred;      
  for(int i=0; i<n_nodes; i++)
    pred.push_back(0);

  vector<float> d(n_nodes, INF);
  //bool s[n_nodes]; /* solution set */
  //bool f[n_nodes]; /* frontier */

  bool s[Nmax]; /* solution set */
  bool f[Nmax]; /* frontier */
  for(int j=0; j<n_nodes; j++){
    s[j]=f[j]=false;
  }
  d[from]=0;
  h.insert(from,0);
  f[from]=true;
  vector<int>::const_iterator vit;
  vit=find(to.begin(), to.end(), from);
  if(vit!=to.end()) /* if "from" is in "to" */
    reached++;
  pred[from]=from+1;
  while(h.nItems()>0 && reached < to_reach){
    v=h.deleteMin();
    s[v]=true;
    f[v]=false;
    vector<int>::iterator vit2;
    for(vit2=neighbors[v].begin(); vit2!=neighbors[v].end(); vit2++){
      if(s[*vit2]==false){
        dist=d[v]+getEdgeWeight(v,*vit2);
        if(dist<d[*vit2]){
          pred[*vit2]=v+1; 
          d[*vit2]=dist;
          if(f[*vit2])
            h.decreaseKey(*vit2,dist);
          else{
            h.insert(*vit2,dist);
            f[*vit2]=true;
          }
        }
      }
    }
  }
  int i;
  vector<int>::const_iterator vit2;
  for(vit2=to.begin(),i=0; vit2!=to.end(); vit2++, i++){
    p[i].push_back(*vit2);
    if(pred[*vit2]>0){
      int act=*vit2;
      while(pred[act]!=act+1){
        p[i].push_back(pred[act]-1);
        act=pred[act]-1;
      }
      reverse(p[i].begin(), p[i].end());
    }
  }
  return 0;
}

/* Use Brandes algorithms.
 * Ulrik Brandes, A Faster Algorithm for Betweenness Centrality. Journal of Mathematical Sociology 25(2):163-177, 2001. 
 * input: the graph (edge possibly weighted)
 * output:  res: a vector of size "n_nodes", each stores the value of vertex centrality
 * return: 
 *       -1: error
 *       0:  success
 */
int Graph::betweennesscentrality(vector<float>& res){//引用
  if(!edge_weighted){
    return betweennesscentralityunweighted(res);
  }
  else{
    bool noNegativeEdgeWeight=checkEdgeWeight();
    if(noNegativeEdgeWeight){    
      return betweennesscentralityweighted(res);
    }
    else 
      return -1; /* for now negative weight is not supported */
   }
}


int Graph::betweennesscentralityunweighted(vector<float>& res){

  for(int i=0; i<n_nodes; i++)
    res.push_back(0); 
  
  /* for each vertex */
  for(int i=0; i<n_nodes; i++){
    stack<int> s;
    /* a vector of lists */ 
    list<int> l;
    vector<list<int> > vli(n_nodes, l);  //为每一个节点分配一个双向链表,记录每个节点的前向节点
    vector<int> sigma(n_nodes, 0);
    vector<int> d(n_nodes,-1);
	vector<float> tri(n_nodes,0);
	sigma[i]=1; //i点相当于公式中的s点，所以初始化i点的sigma为1，d为0
    d[i]=0;    
    queue<int> q;

	//计算i点到每一个节点的最短路径数  广度优先算法
    q.push(i); //将首节点压入队列
    while(!q.empty()){
      int v=q.front();
      q.pop();
      s.push(v);//将v压入栈，随后搜索v的相连节点
      vector<int>::iterator vit;   //搜索v的相连节点
      for(vit=neighbors[v].begin(); vit!=neighbors[v].end(); vit++){
        int w=*vit;
        if(d[w]<0){ //w没有被搜索过
          q.push(w);
          d[w]=d[v]+1;
        }
         if(d[w]==d[v]+1){ 
          sigma[w]+=sigma[v];  //计算出每个节点的sigma
		  tri[w]=1.0*1/sigma[w];
          vli[w].push_back(v); //v为w的前向节点
        }
      }
    }
	
    vector<float> delta(n_nodes,0);  //每个节点的delta初始化为0
    while(!s.empty()){
      int w=s.top();
      s.pop();  
      list<int>::iterator lit;
      for(lit=vli[w].begin(); lit!=vli[w].end(); lit++){
        int v=*lit;//v是w的前向节点
		tri[v]+=tri[w];
		delta[v]=tri[v]*sigma[v]-1;
	  }
	  if(w!=i)//不等于两边端点 因为v的值还不确定，所以算的是w的值
	   res[w]+=delta[w];
	  //res[w]+=tri[w]*sigma[w]-1;
	}
  }

  if(!digraph){
    for(int i=0; i<n_nodes; i++)
      res[i]/=2.0;
  }
  return 0; 
}

 

 /*
 ** 计算点node的k-邻域密度。
 ** 返回密度值
 ** 要求图中没有环
 
 float Graph::desity(int node,int k){
	 int sum=0;
	 float x;

	if(k==1){ 
		 vector<int>::iterator it,it1;
		 for(it=neighbors[node].begin(); it!=neighbors[node].end(); it++){
			 int v=*it; 
			 for(it1=neighbors[node].begin(); it1!=neighbors[node].end(); it1++){
				 int w=*it1;
				 if(isNeighbor(v,w))
					 sum++;
			 }
		 }
	}

	 if(neighbors[node].size()==1)
		 x=0;
	 else
	 x = float(sum/(neighbors[node].size()*(neighbors[node].size()-1)));
	 return x;
}

*/




int Graph::betweennesscentralityweighted(vector<float>& res){
  for(int i=0; i<n_nodes; i++)
    res.push_back(0);

  /* for each vertex */
  for(int s=0; s<n_nodes; s++){
    stack<int> stk;
    //float seen[n_nodes]; /* settled  */

	float seen[Nmax]; /* settled  */ //Nmax=10000（节点数）
    for(int j=0; j<n_nodes; j++){
      seen[j]=INF; //INF = numeric_limits<float>::infinity()  数值极限::无限类型  //seen[j]:每个节点的最终值 dist：每个节点每次的计算值
    }
    /* a vector of lists */ 
    list<int> l;
    vector<list<int> > pred(n_nodes, l);
    vector<int> sigma(n_nodes, 0);
    vector<float> d(n_nodes,INF);
	vector<float> tri(n_nodes,0);
    sigma[s]=1;
    FHeap h(n_nodes);//分配堆
    h.insert(s,0);//将点s插入堆中 
    d[s]=0;

    while(h.nItems()>0){//判断堆中是否还有元素？
      int v=h.deleteMin();//删除最小元
      stk.push(v);
      vector<int>::iterator vit;
      for(vit=neighbors[v].begin(); vit!=neighbors[v].end(); vit++){
        int w=*vit;
        float dist=d[v]+getEdgeWeight(v,w);
        if(fabs(d[w]-INF)<=EPS && fabs(seen[w]-INF)<=EPS){//fabs：求绝对值 EPS=1.0e-6
          seen[w]=dist;
          h.insert(w,dist);
          sigma[w]=sigma[v];
		  tri[w]=1.0*1/sigma[w];
          pred[w].push_back(v);
        }
        else if (fabs(d[w]-INF)<=EPS && dist<seen[w]){
          seen[w]=dist;
          h.decreaseKey(w,dist);//降低w点的键值 使其从原本的seen[w]将为现在的dist
          sigma[w]=sigma[v];//如果seen[w]的值是从v点传递的，那么两点最短路径的条数相等
		  tri[w]=1.0*1/sigma[w];
          pred[w].clear();//清空w点以前的点，将v点压入栈
          pred[w].push_back(v);
        }
        else if(fabs(seen[w]-dist)<=EPS){
          sigma[w]+=sigma[v];//如果seen[w]的值等于v点传递的值并且等于别的点传递的，那么w点的最短路径的条数就等于w点的加上别的点传递的
		  tri[w]=1.0*1/sigma[w];
          pred[w].push_back(v);
        }
      }
    }

    vector<float> delta(n_nodes,0);
    while(!stk.empty()){
      int w=stk.top();
      stk.pop();
      list<int>::iterator lit;
      for(lit=pred[w].begin(); lit!=pred[w].end(); lit++){
        int v=*lit;
		tri[v]+=tri[w];
      }
      if(w!=s)
        res[w]+=tri[w]*sigma[w]-1;
    }
  }
  if(!digraph){
    for(int i=0; i<n_nodes; i++)
      res[i]/=2.0;
  }
  return 0;
}


int Graph::diameter_approximation_sample_s(float sample_s, int n_nodes){
	 float dia=0;
	 float component_dia=0;
	/* vector<int> rea(n_nodes);    
     list<int> l;
     vector<list<int>> vli(n_nodes,l);

    for(int i=0; i<n_nodes; i++){
    queue<int> q;
    vector<int>d(n_nodes,-1);
    int reached=1;
    q.push(i);
    d[i]=0;
    int k=0;
  
    while(!q.empty()){
      int w=q.front();
      q.pop();
	  vli[i].push_back(w);
      vector<int>::iterator vit2;
      for(vit2=neighbors[w].begin(); vit2!=neighbors[w].end(); vit2++){
        int v=(*vit2);
        if(d[v] <0) { 
        q.push(v);
		d[v]=0;
      }
		if(d[v]==0){
			reached+=1;
		}
    }
	rea[i]=reached;
   }
  }*/

//	bool do_computation=1;

   /*  for(int j=0; j<rea.size(); j++){
		int s=rea.at(j);
		int max_s=0;
		if(max_s<s-1){
			max_s=s;
		}*/
  //   if(do_computation){
		srand((unsigned)time(NULL));
        int s_v=rand()%n_nodes;
		vector<float> d(n_nodes);
		shortestpath(d, s_v);
		int longest=0;
		int s_longest=0;
		for(int n=0; n<n_nodes; n++){
			if(d[n]>longest){
				longest=d[n];
				s_longest=longest;
			}
		}
        component_dia=longest+s_longest;
		if(component_dia>dia)
			dia=component_dia;
	 float delta=0.1, epsilon=0.1, r,sample_s_contant=0.5;
	// float sample_s=0;
	 if(delta>0&&delta<1&&epsilon>0&&epsilon<1){  
		sample_s=ceil((sample_s_contant/epsilon*epsilon))*(floor(log(dia-1))+1-log(delta));
	}
	// }
//	}
	  return 0;
}

int Graph::shortestpath(vector<float> &d, int m){
    queue<int> q;
    queue<float> qdist;  //距离
    vector<int> already_counted(n_nodes);//标记节点是否被搜索过

	int i=0;
    q.push(m);
    qdist.push(0);
    already_counted[m]=i+1;

    while(!q.empty()){
        int act=q.front();
      q.pop();
      float actdist=qdist.front();
      qdist.pop();
      d[act]=actdist;
      vector<int>::iterator vit;
      for(vit=neighbors[act].begin(); vit!=neighbors[act].end(); vit++){
        int neighbor=(*vit);
        if(already_counted[neighbor] == i+1) 
          continue; 
        already_counted[neighbor] = i+1;
        q.push(neighbor);
        qdist.push(actdist+1);
      }
    }
/*    int j=0;
    while(j<n_nodes){
      if(fabs(d[j])<EPS && j!=i){   //fabs(d(i,j))<EPS：判断d(i,j)是否为0；整句话的意思是搜索不与节点i相邻的节点，等于遍历整个图
        d[j]=INF;
      }
      j++;
    }*/
	 return 0;
  }
 
