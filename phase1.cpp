#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <map>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <ctype.h>
#include <stack>

using namespace std;

void Insert_node(int n, vector<int> &loops, stack<int> &stk);
int findBB(map<int, int>* BB, map<string, int>* L2BB, ifstream& ifp);
vector<pair<int, int> >* findEdges(map<int, int>* BB, map<string, int>* L2BB, ifstream& ifp);
vector<int>* findPredesessors(int NumOfBlock, vector<pair<int, int> >* edges);
vector<int>* findDominators(int NumOfBlock, vector<int>* pred);
void findLoops(vector<int>* D, vector<int>* pred, vector<pair<int, int> >* edges);

int main(int argc, char** argv)
{
  ifstream ifp(argv[1]);
  map<int, int> *BB = new map<int, int>;
  map<string, int> *L2BB = new map<string, int>;

  if(argc != 2){
    cout << "Usage: ./phase1 inputfile" << endl;
    return -1;
  }

  if(ifp.fail()){
    cout << "Open file failed!" << endl;
    return -2;
  }

  int blockNum = findBB(BB, L2BB, ifp);
  ifp.clear();
  ifp.seekg(0, ios::beg);
  vector<pair<int, int> >* edges = findEdges(BB, L2BB, ifp);
  ifp.close();
  vector<int>* pred = findPredesessors(blockNum, edges);
  vector<int>* dom = findDominators(blockNum, pred);
  findLoops(dom, pred, edges);

  delete BB;
  delete L2BB;
  delete edges;
  delete[] pred;
  delete[] dom;
  return 0;
}

//------------------------------------------- Find blocks -------------------------------------//
int findBB(map<int, int> *BB, map<string, int> *L2BB, ifstream& ifp)
{
  int NumOfBlock = 1;
  string line;
  cout << "#======== Blocks ========" << endl << endl << endl;
  bool bbInserted = false;
  int lineNum = 0;
  while(getline(ifp, line)){
    // Lable statement
	lineNum++;
	istringstream words(line);
	string op;
	words >> op;
    if(op == ":"){
      string label;
      words >> label;
      if (!bbInserted) {
     	  (*L2BB)[label] = NumOfBlock;
     	  (*BB)[lineNum] = NumOfBlock;
    	  cout << "BB" << NumOfBlock++ << endl;
      	}
      else
     	  (*L2BB)[label] = NumOfBlock-1;
      }
    // :=label OR ?:=label
    else if((op == ":=") || (op == "?:=") ){
      cout << line << endl;
   	  (*BB)[lineNum] = NumOfBlock;
      cout << "BB" << NumOfBlock++ << endl;
      bbInserted = true;
      continue;
    }
    else
    	bbInserted = false;

    if(NumOfBlock > 1)
    	cout << line << endl;
  }
  return NumOfBlock;
}

//------------------------------------------- Find edges -------------------------------------//
vector<pair<int, int> >* findEdges(map<int, int> *BB, map<string, int> *L2BB, ifstream& ifp)
{
  cout << endl << endl << "#======== Edges ========" << endl << endl;
  
  vector<pair<int, int> > *edges = new vector<pair<int, int> >;
  pair<int, int> edge;
  int currentBB = 0;
  int pre_node = 0;
  int NumOfEdge = 0;
  int lineNum = 0;
  string line;
  string op;
  string label;


  lineNum = 0;
  while(getline(ifp, line)){
    lineNum++;
	istringstream words(line);
	words >> op;
    // beginning of BB
    if( BB->count(lineNum) ) {
      if ( (*BB)[lineNum] == 1 ){
	  continue;
      }


     currentBB = (*BB)[lineNum];
     //cout << bb << " --> " << currentBB << endl;

      //cout << currentBB << endl;
      //cout << pre_line << endl;
      
      // not branches
      if(op != ":=" && pre_node != currentBB){
	edges->push_back(make_pair(currentBB-1, currentBB));
	cout << "EDGE " << NumOfEdge++ << ": BB" << currentBB-1 << " -> BB" << currentBB << endl;
      }

		words >> label;
		if (label.at(label.size() - 1) == ',')
		  label.resize(label.size() - 1);
      // ?:=label
      if(op == "?:="){
		  edges->push_back(make_pair(currentBB-1, (*L2BB)[label]));
		  cout << "EDGE " << NumOfEdge++ << ": BB" << currentBB-1 << " -> BB" << (*L2BB)[label] << endl;
      }
      // :=label
      if(op == ":="){
    	  edges->push_back(make_pair(currentBB-1, (*L2BB)[label]));
    	  cout << "EDGE " << NumOfEdge++ << ": BB" << currentBB-1 << " -> BB" << (*L2BB)[label] << endl;
      }
    }
    
    pre_node = currentBB;
    //pre_line = line;
  }
  return edges;
}

//------------------------------------------- Find pred(m)s -------------------------------------//
vector<int>* findPredesessors(int NumOfBlock, vector<pair<int, int> >* edges)
{
  // pred information
  vector<int> *pred = new vector<int>[NumOfBlock];
  for(vector<pair<int, int> >::iterator it = edges->begin(); it != edges->end(); it++){
    //cout << "BB" << it->first << " -> BB" << it->second << endl;
    pred[it->second].push_back(it->first);
  }
  return pred;
}

//------------------------------------------- Find dominators -------------------------------------//
vector<int>* findDominators(int NumOfBlock, vector<int>* pred)
{
  // no use index-0
  vector<int> *D = new vector<int>[NumOfBlock];
  // node-1
  D[1].push_back(1);
  // other nodes init to all
  for(int j = 2; j < NumOfBlock; j++){
    for(int i = 1; i < NumOfBlock; i++){
      D[j].push_back(i);
    }
  }

  vector<int> Dp, Dd;
  for(int i = 1; i < NumOfBlock; i++)
      Dp.push_back(i);

  int n = 2;  // n != n0 (n0 = 1)
  bool change = true;
  
  // NumOfblock - 1
  int num = 0; // at least two loop not D changes
  while((change || num < 2) && NumOfBlock > 2){
    change = false;
    
    int len = D[n].size();
    for(vector<int>::iterator it = pred[n].begin(); it != pred[n].end(); it++){
      sort(Dp.begin(), Dp.end());
      sort(D[*it].begin(), D[*it].end());
      // intersection needs sorting 
      set_intersection(Dp.begin(), Dp.end(), D[*it].begin(), D[*it].end(), back_inserter(Dd));
      
      Dp = Dd;
      Dd.clear();
    }
    vector<int>::iterator itu = find(Dp.begin(), Dp.end(), n);
    if(itu == Dp.end())
      Dp.push_back(n);
    D[n] = Dp;
    
    /*
    // Dominator information
    cout << "Node " << n << endl;
    for(vector<int>::iterator it = D[n].begin(); it != D[n].end(); it++){
    cout << *it << '\t';
    }
    cout << endl;
    */
    
    Dp.clear();
    for(int i = 1; i < NumOfBlock; i++)
      Dp.push_back(i);
    
    
    if(len > D[n].size()){
      num = 0;
      change = true;
    }

      n++;
      if(n == NumOfBlock){
	n = 2;
	num++;
      }
  }
  return D;
}

void findLoops(vector<int>* D, vector<int>* pred, vector<pair<int, int> >* edges)
{
  //------------------------------------------- Find back edges -------------------------------------//
  vector<pair<int, int> > back_edge;
  for(vector<pair<int, int> >::iterator it = edges->begin(); it != edges->end(); it++){
    vector<int>::iterator itb = find(D[it->first].begin(), D[it->first].end(), it->second);
    if(itb != D[it->first].end()){
      //cout << "Find an back edge from " << it->first << " -> " << it->second << endl;
      back_edge.push_back(make_pair(it->first, it->second));
    }
  }

  //------------------------------------------- Find loops -------------------------------------//  
  cout << endl << "#======== Loops ========" << endl << endl;
  stack<int> stack_loop;
  int NumOfLoop = 0;
  vector<int> loops;
 
  for(vector<pair<int, int> >::iterator it = back_edge.begin(); it != back_edge.end(); it++){
    // Loop = head
    loops.push_back(it->second);
    // push tail
    //stack_loop.push(it->first);
    Insert_node(it->first, loops, stack_loop);
    while(!stack_loop.empty()){
      int m = stack_loop.top();
      stack_loop.pop();
      //cout << "pop " << m << endl;
      // each pred of m
      for(vector<int>::iterator it = pred[m].begin(); it != pred[m].end(); it++){      
	Insert_node(*it, loops, stack_loop);
      }
    }
    sort(loops.begin(), loops.end());
    if(!loops.empty()){
      cout << "LOOP " << NumOfLoop++ << ": ";
      for(vector<int>::iterator it = loops.begin(); it != loops.end(); it++){
    	  cout << "BB" << *it << ' ';
      }
      cout << endl;
    }  
    loops.clear();
  }
}

void Insert_node(int n, vector<int> &loops, stack<int> &stk)
{
  vector<int>::iterator it = find(loops.begin(), loops.end(), n);
  // not find n in loop
  if(it == loops.end()){
    loops.push_back(n);
    stk.push(n);
  }
}
    
				  
