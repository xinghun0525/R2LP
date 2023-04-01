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

using namespace std;
using namespace tsl;

bool maxScoreCmp(const pair<int, double>& a, const pair<int, double>& b){
    return a.second > b.second;
}

int main(int argc, char **argv){
	string filename = argv[1];
	double eps = atof(argv[2]);
	string seps = to_string(eps);

	double threshold = atof(argv[3]);
	cout << "filename = " << filename << ", eps = " << eps << ", threshold = " << threshold << endl;
	string gtpath = "/home/ssd/liuyu/rbs_gq/SimRankRelease-master/Local-Push/ground_truth/" + filename;
	string graphpath = "/home/ssd/liuyu/rbs_gq/dataset/" + filename + ".txt";
	string anspath = "/home/ssd/liuyu/rbs_gq/SimRankRelease-master/Local-Push/result/" + filename + ".ans" + seps;
	
	cout << gtpath << endl << graphpath << endl << anspath << endl;
	ifstream gtin;
	gtin.open(graphpath.data());
	assert(gtin.is_open());
	int n = 0;
	gtin >> n;
	cout << "node num = " << n << endl;
	gtin.close();
	
	robin_set<int> ground_idx;
	for(int i = 0; i < n; ++i)
		ground_idx.insert(i);
	cout << "ground_idx.size = " << ground_idx.size() << endl;

	robin_map<int, robin_map<int, double> > gt_sim;
	gtin.open(gtpath.data());
	assert(gtin.is_open());
	int a,b; double sim;
	while(gtin >> a >> b >> sim){
		if(sim < threshold || a == b)
			continue;
		if(a > b)
			gt_sim[a][b] = sim;
		else
			gt_sim[b][a] = sim;
	}
	gtin.close();

	robin_map<int, robin_map<int, double> > my_sim;
	gtin.open(anspath.data());
	assert(gtin.is_open());
	while(gtin >> a >> b >> sim){
		if(a > b) 
			my_sim[a][b] = sim;
		else if(a < b)
			my_sim[b][a] = sim;
	}
	gtin.close(); 
	// for(int i = 0; i < n; ++i)
	// 	my_sim[i][i] = 1.0;

	
	double avg_error = 0.0;
	int tnum = 0; int mynum = 0; int tpnum = 0; 
	for(auto &i : gt_sim){
		a = i.first;
		for(auto &j : i.second){
			++tnum;
			b = j.first;
			double gts = j.second;
			if(my_sim[a].find(b) == my_sim[a].end()){
				avg_error += gts;
			}
			else{
				avg_error += abs(gts - my_sim[a][b]);
				if(my_sim[a][b] >= threshold){
					++tpnum;
				}
			}
		}
	}
	avg_error /= tpnum;
	for(auto &i : my_sim){
		for(auto &j: i.second){
			if(j.second >= threshold)
				++mynum;
		}
	}
	double precision = (tpnum+0.0) / tnum;
	double recall = (tpnum+0.0) / mynum;
	double f1score = (2*recall*precision)/(recall + precision);
	cout << "ground truth pairs num = " << tnum << endl;
	cout << "my pairs num = " << mynum << endl;
	cout << "match pairs num = " << tpnum << endl;
	cout << "precision = " << precision << endl;
	cout << "recall = " << recall << endl;
	cout << "f1 score = " << f1score << endl;
	cout << "avg_error = " << avg_error << endl;
    return 0;
}


