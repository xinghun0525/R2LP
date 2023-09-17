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
using ll = long long;
bool maxScoreCmp(const pair<int, double>& a, const pair<int, double>& b){
    return a.second > b.second;
}

int main(int argc, char **argv){
	string filename = argv[1];
	double eps = atof(argv[2]);
	string seps = to_string(eps);
    string algo_name = argv[3];
	string stop_rea = argv[4];
	double threshold = (1e-3) - (1e-7);
	if(algo_name == "UISim"){
		eps = atoi(argv[2]);
		seps = to_string(int(eps));
	}
	int hubs = 0;
	string eta = argv[5];

    time_t now = time(nullptr); 
	char* curr_time = ctime(&now); cout << curr_time <<endl;
	string base_path = "";
    

    cout << "filename = " << filename << ", eps = " << eps << ", stop_rea = " << stop_rea << ", eta= " << eta << endl;
	unordered_set<int> ground_idx;
    ifstream fin;
    string idx_path = "/data/geQian/packing_gq/ground_idx/" + filename + "/idx1";
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
	
	ll gtnum = 0;
    string groundtruth_path = "/data/geQian/packing_gq/ExactSim-master/results/" + filename + "/1e-06/";
    for(auto a: ground_idx){
        string gtpath = groundtruth_path + to_string(a) + ".txt";
        // cout << gtpath << endl;
        fin.open(gtpath.data());
        assert(fin.is_open());
        int b; double sim;
        while(fin >> b >> sim){
            if(sim < threshold || a == b)
                continue;
            if(a > b)
                gt_sim[a][b] = sim;
            else
                gt_sim[b][a] = sim;
			++gtnum;
        }
        fin.close();
    }
	// ofstream fout;
	// fout.open(("gt/" + filename + sthres + ".gt").c_str());
	// for(const auto &a: gt_sim){
	// 	for(const auto &b:a.second){
	// 		fout << a.first << " " << b.first << " " << b.second << endl;
	// 	}
	// }
	// fout.close();
	cout << "gtnum = " << gtnum << endl;

	string mfilename = "_stopRea"+stop_rea;
    string anspath;
    if(algo_name == "rbs")
    	anspath = base_path + "/data/geQian/packing_gq/rbs-sim_exp/result_eps/" + filename + "/eps_" + seps;
    else if(algo_name == "optlp")
        anspath = base_path + "/data/geQian/packing_gq/SimRankRelease-master_exp/Local-Push/eps_result/" + filename + ".ans_eps" + to_string(eps);
	else if(algo_name == "flp")
        anspath = base_path + "/data/geQian/packing_gq/SimRankRelease-master_exp_flp_large_graph/Local-Push/eps_result_flp/" + filename + ".ans_eps" + to_string(eps);
	else if(algo_name == "UISim"){
		anspath = "/data/zhaoYingHao/UISim2020-main/result/" + filename + "/eta"+eta+"_stopRea" + stop_rea + "_topk5000";
	}
		

	unordered_map<int, unordered_map<int, double> > my_sim;
	cout << "anspath = " << anspath << endl;

	fin.open(anspath.data());
	assert(fin.is_open());
	ll mynum11 = 0;
	if(algo_name == "UISim"){
		string st_time;
		fin >> st_time;
		cout << st_time << endl;
	}
	int a, b; double sim;
	while(fin >> a >> b >> sim){
        if(sim < threshold || a == b) 
            continue;

		if(ground_idx.find(a) != ground_idx.end() || ground_idx.find(b) != ground_idx.end()){
			if(a > b) 
				my_sim[a][b] = sim;
			else if(a < b)
				my_sim[b][a] = sim;
			mynum11++;
		}
	}
	fin.close(); 
	cout << "mynum = " << mynum11 << endl;


	
	double avg_error = 0.0;
	ll tnum = 0; ll mynum = 0; ll tpnum = 0; 
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
	avg_error /= tnum;
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
