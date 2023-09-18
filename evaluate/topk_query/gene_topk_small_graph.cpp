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
	if(sp1.sim == sp2.sim && sp1.a == sp2.a)
		return sp1.b < sp2.b;
	if(sp1.sim == sp2.sim)
		return sp1.a < sp2.a;
	return sp1.sim > sp2.sim;
}


// The function reads the ground truth for each graph, sorts it, and saves the top-k entries into the `./gt` directory.
int main(int argc, char **argv){
	string filename = argv[1];

	time_t now = time(nullptr); 
	char* curr_time = ctime(&now); cout << curr_time <<endl;
	string base_path = "";
    
	cout << "filename = " << filename << endl;

	vector<int> nodeid_list;
	unordered_set<int> ground_idx;
    ifstream fin;
    string idx_path = "../../ground_idx/" + filename + "/idx1";
	cout << "idx_path = " << idx_path << endl;
    fin.open(idx_path.data());
    assert(fin.is_open());
    int x;
    while(fin >> x){
        ground_idx.insert(x);
		nodeid_list.emplace_back(x);
    }
    cout << "ground_idx.size = " << ground_idx.size() << endl;

	fin.close();
    unordered_map<int, unordered_map<int, double> > gt_sim;
	
    string groundtruth_path = "../../SimRankRelease-master_exp/Local-Push/ground_truth/" + filename;

	string gtpath = groundtruth_path;
	fin.open(gtpath.c_str());
	assert(fin.is_open());
	int a; int b; double sim;
	long long gtnum = 0;
	
	while(fin >> a >> b >> sim){
		if(a == b)
			continue;
		if(a > b)
			gt_sim[a][b] = sim;
		else
			gt_sim[b][a] = sim;
		++gtnum;
	}
	fin.close();
	cout << "gtnum = " << gtnum << ", random chiose 1000*1000 pair-node" << endl;

	vector<Sim_pair> vec_gt_sim;  
	unordered_map<int, unordered_map<int, double> > set_gt_sim;
	vector<int>fromnodelist;
	vector<int>tonodelist;

	while(true){
		random_shuffle(nodeid_list.begin(), nodeid_list.end());
		int max_node_num = 1000;
		for(int i = 0; i < max_node_num; i++){
			if(gt_sim.find(nodeid_list[i]) == gt_sim.end() && max_node_num < nodeid_list.size()-1){
				max_node_num += 1;
				continue;
			}
			fromnodelist.emplace_back(nodeid_list[i]);
		}

		random_shuffle(nodeid_list.begin(), nodeid_list.end());
		max_node_num = 1000;
		for(int i = 0; i < max_node_num; i++){
			if(gt_sim.find(nodeid_list[i]) == gt_sim.end() && max_node_num < nodeid_list.size()-1){
				max_node_num += 1;
				continue;
			}
			tonodelist.emplace_back(nodeid_list[i]);
		}

		for(int i = 0; i < 1000; i++){
			int fromnode = fromnodelist[i];
			for(int j = 0; j < 1000; j++){
				int tonode = tonodelist[j];

				if(tonode == fromnode) continue;
				if(fromnode < tonode && gt_sim[tonode][fromnode]!=0){
					set_gt_sim[tonode][fromnode] = gt_sim[tonode][fromnode];
				}
				else if(fromnode > tonode && gt_sim[fromnode][tonode] != 0){
					set_gt_sim[fromnode][tonode] = gt_sim[fromnode][tonode];
				}
			}	
		}

		int vaild_num = 0;
		for(auto i: set_gt_sim){
			vaild_num += i.second.size();
		}
		if(vaild_num >= 10000) break;
		fromnodelist.clear();
		tonodelist.clear();
	}

	for(auto i: set_gt_sim){
		for(auto j: i.second){
			Sim_pair sp; sp.a = i.first; sp.b = j.first; sp.sim = j.second;
			vec_gt_sim.push_back(sp);
		}
	}

	cout << "vec_gt_sim.size(): " << vec_gt_sim.size()<< endl;
	sort(vec_gt_sim.begin(), vec_gt_sim.end(), max_comp_Sim_pair);

	cout << "save 1000*1000 pairs ground-truth..." << endl;
	ofstream gfout;
	gfout.open("./gt/"+filename+"_all.gt");
	for(Sim_pair ite : vec_gt_sim){
		gfout << ite.a << " " << ite.b << " " << ite.sim << endl;
	}
	gfout.close();

	int topk_list[4] = {500,1000,5000,10000};
	for(int i = 0; i < 4; i++){
		int topk = topk_list[i];
		if(vec_gt_sim.size() < topk){
			break;
		}

		vector<Sim_pair> vec_gt_sim_topk(vec_gt_sim.begin(), vec_gt_sim.begin() + topk);
		double topk_val = vec_gt_sim[topk-1].sim;

		for(int i = topk; i < vec_gt_sim.size(); ++i){
			if(topk_val !=0 && vec_gt_sim[i].sim == topk_val)
				vec_gt_sim_topk.push_back(vec_gt_sim[i]);
			else
				break;
		}
		gt_sim.clear();
		for(auto i : vec_gt_sim_topk){
			gt_sim[i.a][i.b] = i.sim;
		}

		string respath = "./gt/";

		ofstream fout;
		fout.open((respath + filename + "_topk" + to_string(topk) + ".gt").c_str());
		for(const auto & i: vec_gt_sim_topk){
			fout << i.a << " " << i.b << " " << i.sim << endl;
		}

		fout.close();
	}
    return 0;
}
