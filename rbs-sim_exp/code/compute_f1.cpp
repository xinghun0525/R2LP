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
using namespace std;
using namespace tsl;

bool maxScoreCmp(const pair<int, double>& a, const pair<int, double>& b){
    return a.second > b.second;
}

int main(int argc, char **argv){
	string filename = argv[1];
	double eps = atof(argv[2]);

	robin_set<int> ground_idx;
	robin_map<int, robin_set<int> > ground_sim;

	robin_map<int, robin_set<int> > exp_sim;

	string idx_path = "./query/" + filename + ".idx";
	ifstream fidx;
	fidx.open(idx_path.data());
	assert(fidx.is_open());
	cout << "start reading ground_idx" << endl;
	int x;
	while(fidx >> x){
		ground_idx.insert(x);
	}
	cout << "finished reading idx, num = " << ground_idx.size() << endl;
	
	string gd_truth_path = "../ExactSim-master/results/" + filename + "/1e-06/";
	for(const auto& i : ground_idx){
		ifstream fgroundtruth;
		fgroundtruth.open(gd_truth_path + to_string(i) + ".txt");
		assert(fgroundtruth.is_open());
		int x; double sim;
		while(fgroundtruth >> x >> sim){			
			if(sim >= 0.1)
				ground_sim[i].insert(x);
		}
		cout << "ground_sim.size = " << ground_sim[i].size() << endl; 
	}
	cout << "finished ground_sim, num = " << ground_sim.size() << endl;

	string ansname = "0315wy_result/"+filename+"/" + filename + to_string(eps).substr(0,5)+"_ans/";
	// string ansname = "0315wy_result/" + filename + "0.01_ans/";
	for(const auto& i: ground_idx){
		ifstream fdata;
		string sdata = ansname + to_string(i);
		fdata.open(sdata.data());
		assert(fdata.is_open());
		int x; double sim;
		while(fdata >> x >> sim){
			if(sim >= 0.1)
				exp_sim[i].insert(x);
		}
		fdata.close();
	}
	cout << "finished reading exp sim, num = " << exp_sim.size() << endl;
	
	int tp = 0, tn = 0, fp = 0, fn = 0;
	int times = 0;
	int allx = 0, ally = 0, allxy = 0;
	for(const auto& i: ground_idx){
		++times;
		int x = 0;
		if(ground_sim.find(i) != ground_sim.end())
			x = ground_sim[i].size();
		int y = 0;
		if(exp_sim.find(i) != exp_sim.end())
			y = exp_sim[i].size();
		int xy = 0;
		if(x > 0 && y > 0){
			for(const auto &gds: ground_sim[i]){
				if(exp_sim[i].find(gds) != exp_sim[i].end())
					++xy;
			}
		}
		allx += x; ally += y; allxy += xy;
		cout << "*** " << times << " ***" << endl;
		cout << "i = " << i << ", x = " << x << ", y = " << y << ", xy = " << xy << endl;	
	}
	cout << "\n allx = " << allx << ", ally = " << ally << ", allxy = " << allxy << endl;

	double precision = (allxy + 0.0)/ally;
	double recall = (allxy + 0.0)/allx;
	cout << "precision = " << precision << ", recall = " << recall << endl;
	cout << "f1score = " << 2*precision*recall/(precision + recall) << endl; 
    return 0;
}
