#include<stdio.h>
#include<unistd.h>
#include<cstdlib>
#include<string.h>
#include<cmath>
//#include<Windows.h>
#include"graph.h"
#include <time.h>

#include <iostream>
#include <fstream>


#define M 100//TOP N
using namespace std;

void print_usage(void){	//交互页面
  cout<<"Options: \n";
  cout<<"  -t (LBCC)\n";//DEN_1,DEN_2,BC结合IDC(即complex值)

}

int main(int argc, char** argv){
  char c;

  next://设置重新执行的点

  string edge_file="input.txt";//设置输入和输出文件
  string output_f="output.txt";
  string output_f1="cpu_output.txt";
  Graph g;
  //初始化
  bool directed=false; 
  bool t=false;   	//这里决定是算bc,cc,ec还是sc

  clock_t start,finish;//记录时间用的东西
  double totaltime;
  
  print_usage();//给予选择页面

   cin>>c;
	switch(c){
	  case 't':
		  t =true;
		  break;
	   default:
       cout<<"输入了未知选择\n";
	   exit(1);
    }

  if(edge_file.empty()){	//输入文件为空时
    cout<<"无输入文件!\n";
  }

  vector<string> vs;	//设置容器vs
  cout<<"reading the filen";
  vs.push_back(edge_file);	//向容器内添加输入文件
  if(g.readGraph(vs, false, false, directed)==-1){	//调用graph.cpp
	cout<<"读入图失败\n";
  };
  cout<<"success\n";
  ofstream ofile;
  //打开文件输出流
  ofile.open(output_f.c_str(), ios::out);
  cout<<"open the file\n";
  
  int n_count=g.getNodeCount();
  cout<<"the number of nodes:"<<n_count<<"\n";
  int e_count=g.getEdgeCount();
  cout<<"the number of edges:"<<e_count<<"\n";

    if(t){
	  vector<float> result;//1-邻域密度
	  vector<float> result2;//2-邻域密度
	  vector<int> sum;
	  vector<int> summ;
	  vector<float> bc;
	  vector<int> imp;//判断是否关键
	  vector<double> multi;//（度*邻域密度）的值
	  int sum1=0;//1-邻域中边的个数
	  int sum2=0;//2-邻域中边的个数
	  vector<vector<int> > pneighbors;//存放1-邻域中的顶点
	  vector<vector<int> > p2neighbors;//存放2-邻域中的顶点
         vector<int> pneighbor;
	  float m1;//1-邻域密度
	  float m2;//2-领域密度
	  int deg;
	  int sumimp=0;
	  

          cout<<"caculate BC..."<<endl; 
	  g.betweennesscentrality(bc);
	  
           cout<<"caculate DEN_1,DEN_2..."<<endl;
	   //初始化1-邻域，2-邻域边的和，以及密度与度的乘积。
	  for(int a=0;a<n_count;a++){
		  result.push_back(0.0);
		  result2.push_back(0.0);
		  sum.push_back(0);
		  summ.push_back(0);
		  multi.push_back(0.0);
		  imp.push_back(0);
	  }

	  //标记关键蛋白
	//读取关键蛋白文件
	string ess_file="deg.txt";
	vector<string> ve;
	ifstream essfile;
	string line1;
	essfile.open(ess_file.c_str());
	while(getline(essfile, line1)){
		ve.push_back(line1);
	}
	essfile.close();
	
	//去除ve中的空格等之类的无效字符 \r\n\t
	for(int i=0;i<ve.size();i++){
		size_t n = ve[i].find_last_not_of( " \r\n\t" );
		if( n != string::npos )
		{
		    ve[i].erase( n + 1 , ve[i].size() - n );
		}
		 
		n = ve[i].find_first_not_of ( " \r\n\t" );
		if( n != string::npos )
		{
		    ve[i].erase( 0 , n );
		}

		//cout<<ve[i]<<"\t"<<"lala"<<endl;

	}


	 //标记关键蛋白
      vector<string>::iterator vit;
	  for(int i=0; i<n_count; i++){
	  	for(vit=ve.begin(); vit!=ve.end(); vit++){
	  		if(*vit==g.id2node(i)){
				imp[i]=1;
				break;
	  		}
			else 
	  			imp[i]=0;
	  		}
		//ofile<<imp[i]<<end;
	  }
	

	//读入complex复合物
	 ifstream file1;
	  file1.open("complex.txt");
	  vector<string> complex;
	  char buf1[20000];
	  if(!file1){
	     cout<<"文件不存在"<<endl;
     }
     while(file1.good()){
	     file1.getline(buf1,20000);
	     complex.push_back(buf1);
		// ofile<<buf1<<endl;
      }

	 vector<vector<string> > complex1;//存放分开后的complex
	 string s;
	 vector<string> tempp;

	 //将每一个complex分解
	 for(int i=0; i<complex.size(); i++){
		 s = complex[i];
		 int start =0;
		 for(int j=0; j<complex[i].size(); j++){  
			   if(s[j] == ' ')  
			   {  
				tempp.push_back(s.substr(start,j - start));  
				start = j+1;  
			   }  
			   else if(j == complex[i].size()-2)//到达尾部  
			   {  
				   //cout<<s.substr(start,j - start+1)<<endl;
				tempp.push_back(s.substr(start,j+1 - start));  
			   }  
			}  
		
		complex1.push_back(tempp);
		tempp.clear();
	 }


	
	  //求1-邻域邻居节点，存储在pneighbors中
	   for(int j=0;j<n_count;j++){
	    	pneighbor.clear();
			pneighbor.push_back(j);
	    	for(int r=0;r<n_count;r++){
		    	if(g.isNeighbor(j,r)){
					pneighbor.push_back(r);
			    }
		   }
		pneighbors.push_back(pneighbor);
	  }

	   //将1-邻域邻接顶点中的重复顶点删除
	   vector<int>::iterator pos1;
	   for(int j=0;j<n_count;j++){
		   sort(pneighbors[j].begin(), pneighbors[j].end());
		   pos1 = unique(pneighbors[j].begin(), pneighbors[j].end());
		   pneighbors[j].erase(pos1, pneighbors[j].end());     
	   }

	   //求2-邻域邻居节点，存储在p2neighbors中
	   for(int j=0;j<n_count;j++){
	    	pneighbor.clear();
			pneighbor.push_back(j);
	    	for(int r=0;r<n_count;r++){
		    	if(g.isNeighbor(j,r)){
					pneighbor.push_back(r);
					for(int a=0;a<n_count;a++){
						if(g.isNeighbor(r,a)){
							pneighbor.push_back(a);
						}
					}
			    }
		   }
		p2neighbors.push_back(pneighbor);
	  }


	   //将2-邻域邻接顶点中的重复顶点删除
	   vector<int>::iterator pos;
	   for(int j=0;j<n_count;j++){
		   sort(p2neighbors[j].begin(), p2neighbors[j].end());
		   pos = unique(p2neighbors[j].begin(), p2neighbors[j].end());
		   p2neighbors[j].erase(pos, p2neighbors[j].end());     
	   }

	  

	  //计算1-邻域中（包括顶点本身）边的个数
	  //计算结果为边个数两倍
	  for(int i=0;i<n_count;i++){
		  sum1 = 0;
		  vector<int>::iterator it,it1;
		  for(it=pneighbors[i].begin(); it!=pneighbors[i].end(); it++){
			int v=*it; 
			for(it1=pneighbors[i].begin(); it1!=pneighbors[i].end(); it1++){
			    int w=*it1;
				if(v==w)
					continue;
				if(g.isNeighbor(v,w))
				sum1++;
			}
		}
			sum[i]=sum1;
	  }


	   //计算2-邻域中（包括顶点本身）边的个数
	  //计算结果为边个数两倍
	  for(int i=0;i<n_count;i++){
		  sum2 = 0;
		  vector<int>::iterator it,it1;
		  for(it=p2neighbors[i].begin(); it!=p2neighbors[i].end(); it++){
			int v=*it; 
			for(it1=p2neighbors[i].begin(); it1!=p2neighbors[i].end(); it1++){
			    int w=*it1;
				if(v==w)
					continue;
				if(g.isNeighbor(v,w))
				sum2++;
			}
		}
			summ[i]=sum2;
	  }



	  //计算1-邻域，2-邻域密度
	  //如果为单独的点，密度定义为0
	  for(int i=0;i<n_count;i++){
		  if(pneighbors[i].size()==1)
			  m1=0;
		  else
		  m1 = sum[i]*1.0/(pneighbors[i].size()*(pneighbors[i].size()-1));

		  if(p2neighbors[i].size()==1)
			  m2=0;
		  else
		  m2 = summ[i]*1.0/(p2neighbors[i].size()*(p2neighbors[i].size()-1));

		  //deg = g.degree(i);
		  result[i] =m1;
		  result2[i] =m2;
	  }

           cout<<"caculate IDC..."<<endl;
	   //计算IDC的complex值
	   vector<float> IDC;
	   int temp=0;
	   vector<string>::iterator it33;
	   vector<string>::iterator it44;
	   for(int i = 0; i < n_count; i++){
		   for(int j = 0;j < complex1.size(); j++){
			   for(it33=complex1[j].begin(); it33!=complex1[j].end(); it33++){
					   if(g.id2node(i)==*it33){
							for(it44=complex1[j].begin(); it44!=complex1[j].end(); it44++){
								if(g.id2node(i)==*it44)
									continue;
								if(g.isNeighbor(i,g.node2id(*it44))){
									temp++;
								}							
							}				   
						}   
				 }
			}
		   if(temp==0){
			   IDC.push_back(0.001);
		   }
		   else
		   IDC.push_back(temp);
		   temp=0;
	   }

/* 
**   caculate LBCC
**	 set  i=1, j=4, ii=3, jj=1 
*/
	   
//	   ofile<< "den_1" << "\t" << "den_2"<< "\t" << "idc" <<"\t" << "bc" << "\t" <<"sum" << endl;

	  //负责输出结果
	  //输出格式为：参数i：参数j：总共的关键蛋白数
                cout<<"caculate LBCC..."<<endl;
		for(float jj=1; 1-jj>=0; jj+=1){
		  for(float i=1; 1-i>=0; i+=1){
			  for(float j=4; 4-j>=0; j+=1){
				  for(float ii=3; 3-ii>=0; ii+=1){
					  sumimp = 0;
						  for(int k=0;k<n_count;k++){
							  if(result[k]==0 ||result2[k]==0)
								  multi[k]=0;
							  else
                                                                  multi[k]=pow(result[k],i)*pow(result2[k],j)*pow(IDC[k],ii)*pow(bc[k],jj);  
								  //multi[k]= 1.0*log10(result[k])*i + 1.0*log10(result2[k])*j + 1.0*log10(IDC[k])*ii + 1.0*log10(bc[k])*jj;
						  }

						  multimap<double,int> node_map;
							for(int i=0; i<n_count; i++){
								node_map.insert(make_pair(multi[i],i));
							}
							int s=0;
							multimap<double,int>::reverse_iterator r_it;
							 for( r_it=node_map.rbegin() ; r_it != node_map.rend(); r_it++ ){
								 sumimp=sumimp+imp[(*r_it).second];	
								// ofile<<(*r_it).first<<"\t"<<(*r_it).second<<endl;
								 s++;
								 if(s==M)
									 break;
							 }
						 // ofile<< i << "\t" << j<< "\t" << ii <<"\t" << jj << "\t" <<sumimp << endl;
						 }
			  }		
		  }
		}

/*
** output the value of DC, DEN_1, DEN_2, BC, IDC, LBCC
*/
		ofile<< "name" << "\t" << "DEN_1" << "\t" <<"DEN_2"<<"\t"<<"BC"<<"\t"<<"IDC"<<"\t"<<"LBCC"<<"\t"<<"ESS"<<endl;
		for(int i=0;i<n_count;i++){
			
			ofile<< g.id2node(i) <<"\t"<<result[i] <<"\t"<<result2[i]<<"\t"<<bc[i]<<"\t"<<IDC[i]<<"\t"<<multi[i]<<"\t"<<imp[i]<<endl;
		} 


  }

 

  ofile.close();//关闭文件输入流
 // ofile1.close();//关闭文件输入流
  cout<<"close the file\n";
  cout<<"\n";
  cout<<"\n";
  cout<<"\n";
  cout<<"\n";

  goto next;//从新选择和计算
  


  system("pause");//加入
  return 0;
  system("pause");//加入
}
