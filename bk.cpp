#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <stdlib.h>
#include <vector>
#include <algorithm>

using namespace std;

int main(int argc, char** argv)
{
  ifstream ifp(argv[1]);
  ofstream ofp("Blocks.dat");
  string line, pre_line;
  int NumOfBlock = 1;
  map<char, int> L2BB;

  if(argc != 2){
    cout << "Usage: ./phase1 inputfile" << endl;
    return -1;
  }

  if(ifp.fail() || ofp.fail()){
    cout << "Open file failed!" << endl;
    return -2;
  }

  ofp << "#======== Blocks ========" << endl;
  
  while(getline(ifp, line)){
    // Lable statement
    if(line[0] == ':'){
      // not START
      if(line[3] != 'T'){
        L2BB[line[3]] = NumOfBlock;
      }
      ofp << "BB" << NumOfBlock++ << endl;
    }
    // :=label OR ?:=labe
    else if((line[2] == ':' && line[3] == '=') || (line[2] == '?') ){
      ofp << line << endl;
      ofp << "BB" << NumOfBlock++ << endl;
      continue;
    }

    ofp << line << endl;
  }

  ifp.close();
  ofp.close();

  ifp.open("Blocks.dat");
  ofp.open("Edges.dat");
  if(ifp.fail() || ofp.fail()){
    cout << "Open file failed!" << endl;
    return -2;
  }
  
  ofp << "#======== Edges ========" << endl;
  //for(map<char, int>::iterator it = L2BB.begin(); it != L2BB.end(); it++){
  //cout << "L" << it->first << " BB" << it->second << endl;
  //}

  vector<pair<int, int> > edges;
  pair<int, int> edge;
  int currentBB = 0;
  int pre_node = 0;
  int NumOfEdge = 0;

  while(getline(ifp, line)){
    // beginning of BB
    if(line[0] == 'B' && line[1] == 'B' && line[2] != '1'){
      currentBB = line[2] - '0';
      
      // not branches
      if(pre_line[1] != ':' && pre_node != currentBB){
        edges.push_back(make_pair(currentBB-1, currentBB));
        ofp << "EDGE " << NumOfEdge++ << ": BB" << currentBB-1 << " -> BB" << currentBB << endl;
      }
      // ?:=label
      if(pre_line[1] == '?'){
        edges.push_back(make_pair(currentBB-1, L2BB[line[6]]));
        ofp << "EDGE " << NumOfEdge++ << ": BB" << currentBB-1 << " -> BB" << L2BB[line[6]] << endl;
      }
      // :=label
      if(pre_line[1] == ':' && pre_line[2] == '='){
        edges.push_back(make_pair(currentBB-1, L2BB[line[5]]));
        ofp << "EDGE " << NumOfEdge++ << ": BB" << currentBB-1 << " -> BB" << L2BB[line[5]] << endl;
      }
    }
    
    pre_node = currentBB;
    pre_line = line;
  }

  ifp.close();
  ofp.close();

  
  ofp.open("Loops.dat");
  if(ofp.fail()){
    cout << "Open file failed!" << endl;
    return -2;
  }
  
  vector<int> loops;
  int entry;
  int outry;
  int NumOfPath = 0;

  ofp << "#======== Loops ========" << endl;
  //for(vector<pair<int, int> >::iterator it = edges.begin(); it != edges.end(); it++){
  //  cout << "BB" << it->first << " -> BB" << it->second << endl;
  //}
  
  for(vector<pair<int, int> >::reverse_iterator it = edges.rbegin(); it != edges.rend(); it++){
    if(it->first >= it->second){
      if(it->first == it->second){
        ofp << "LOOP " << NumOfPath++ << ": BB" << it->first << endl;  
      }
      else{
        loops.push_back(it->second);
        loops.push_back(it->first);
        outry = it->first;
        entry = it->second;

        // start to find loops
        for(vector<pair<int, int> >::iterator inside = edges.begin(); inside != edges.end(); inside++){
          vector<int>::iterator ii = find(loops.begin(), loops.end(), inside->first);
          if(ii != loops.end() && inside->first == *ii && inside->second < outry){
            loops.push_back(inside->second);
          }
        }
      }
    }

    if(!loops.empty())
      ofp << "LOOP " << NumOfPath++ << ": ";
    sort(loops.begin(), loops.end());
    int pre_n = 0;
    for(vector<int>::iterator node = loops.begin(); node != loops.end(); node++){
      if(*node != pre_n){
        ofp << "BB" << *node << " ";  
        pre_n = *node;
      }
    }
    if(!loops.empty())
      ofp << endl;
    loops.clear();
    pre_n = 0;
    
  }
  

  ofp.close();
  
  return 0;
}

