#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <cstring>
#include <unordered_set>
#include <unordered_map>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "robin_map.h"
#include "robin_set.h"
#include <unistd.h>
#include "Random.h"
#include "Graph.h"

using namespace std;
using namespace tsl;

bool mkdir_p(string folderPath){
    string command = "mkdir -p " + folderPath;  
    return system(command.c_str());
}

bool maxScoreCmp(const pair<int, double>& a, const pair<int, double>& b){
    return a.second > b.second;
}

int main(int argc, char **argv){
	Random mR;
	string filename = argv[1];
	int idxnum1 = atoi(argv[2]);
	int idxnum2 = atoi(argv[3]);


	cout << "filename = " << filename << ", idxnum1 = " << idxnum1 << ", idxnum2 = " << idxnum2 << endl;
	string graphpath = "../../dataset/" + filename + ".txt";
	Graph g;
	g.inputGraph(graphpath);
	int n = g.n;
	cout << "n = " << n << endl;
	unordered_set<int> result; 
	for(int i = 0; i < idxnum1 + idxnum2; ++i){
		while(true){
			double rd = mR.drand();
			int x = rd*n;
			if(result.find(x) == result.end() && g.getInSize(x) > 0){
				result.insert(x);
				break;
			}
		}
	}
	vector<int> ve_result;
	for(auto x: result){
		ve_result.push_back(x);
	}
    mkdir_p("../../ground_idx/" + filename);
	ofstream fidx1, fidx2;
	string path1 = "../../ground_idx/" + filename + "/idx1";
	string path2 = "../../ground_idx/" + filename + "/idx2";
	fidx1.open(path1.data());
	fidx2.open(path2.data());
	for(int i = 0; i < idxnum1; ++i){
		fidx1 << ve_result[i] << endl;
	}
	for(int i = idxnum1; i < idxnum1 + idxnum2; ++i){
		fidx2 << ve_result[i] << endl;
	}

	fidx1.close();
	fidx2.close();
	
    return 0;
}


// example ./getidx IT 100 1000