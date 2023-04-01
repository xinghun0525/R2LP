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
#include <unordered_set>
#include <unordered_map>
#include <unistd.h>
#include <ctime>
// #include <direct.h>

using namespace std;

bool maxScoreCmp(const pair<int, double>& a, const pair<int, double>& b){
    return a.second > b.second;
}

struct Sim_pair{
	int a; int b;
	double sim;
};

bool max_comp_Sim_pair(const Sim_pair & sp1, const Sim_pair & sp2){
	return sp1.sim > sp2.sim;
}

int main(int argc, char **argv){
	// string filename = argv[1];
	// string stopk = argv[2];
    // string algo_name = argv[3];
	string filename = "CG";
	string stopk = "5000";
    string algo_name = "argv[3]";

	int topk = stoi(stopk);

    time_t now = time(nullptr); 
	char* curr_time = ctime(&now); cout << curr_time <<endl;
	string base_path = "";
    

    cout << "filename = " << filename << ", topk = " << topk << endl;
    unordered_set<int> ground_idx;
    ifstream fin;
    string idx_path = base_path + "../ground_idx/" + filename + "/idx1";
	cout << "idx_path = " << idx_path << endl;
    fin.open(idx_path.data());
    assert(fin.is_open());
    int x;
    while(fin >> x){
        ground_idx.insert(x);
    }
    cout << "ground_idx.size = " << ground_idx.size() << endl;

	fin.close();
    unordered_map<int, unordered_map<int, double> > gt_sim;
	
	int gtnum = 0;
    string groundtruth_path = "../ExactSim-master/results/" + filename + "/1e-06/";
    for(auto a: ground_idx){
        string gtpath = groundtruth_path + to_string(a) + ".txt";
        // cout << gtpath << endl;
        fin.open(gtpath.data());
        assert(fin.is_open());
        int b; double sim;
        while(fin >> b >> sim){
            
        }
        fin.close();
    }
	ofstream fout;
	fout.open(("gt/" + filename + sthres + ".gt").c_str());
	for(const auto &a: gt_sim){
		for(const auto &b:a.second){
			fout << a.first << " " << b.first << " " << b.second << endl;
		}
	}
	fout.close();
	cout << "gtnum = " << gtnum << endl;
    string anspath;
    if(algo_name == "rbs")
    	anspath = base_path + "../rbs-sim/result/" + filename + "/eps" + seps + "t" + sthres;
    else if(algo_name == "optlp")
        anspath = base_path + "../SimRankRelease-master/Local-Push/eps_result/" + filename +".ans_t" + sthres + "_eps" + seps;
	unordered_map<int, unordered_map<int, double> > my_sim;
	cout << "anspath = " << anspath << endl;

	fin.open(anspath.data());
	assert(fin.is_open());
	int mynum11 = 0;
	int a,b; double sim;
	while(fin >> a >> b >> sim){
        if(sim < threshold || a == b) 
            continue;
		if(a > b) 
			my_sim[a][b] = sim;
		else if(a < b)
			my_sim[b][a] = sim;
		mynum11++;
	}
	fin.close(); 
	cout << "mynum = " << mynum11 << endl;
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
	double precision = (tpnum+0.0) / mynum;
	double recall = (tpnum+0.0) / tnum;
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
