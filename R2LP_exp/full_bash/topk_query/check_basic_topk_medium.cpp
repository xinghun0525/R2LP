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

bool maxScoreCmp(const pair<int, double>& a, const pair<int, double>& b) {
	return a.second > b.second;
}

struct Sim_pair {
	int a; int b;
	double sim;
};

bool max_comp_Sim_pair(const Sim_pair & sp1, const Sim_pair & sp2) {
	if (sp1.sim == sp2.sim && sp1.a == sp2.a)
		return sp1.b < sp2.b;
	if (sp1.sim == sp2.sim)
		return sp1.a < sp2.a;
	return sp1.sim > sp2.sim;
}

int main(int argc, char **argv) {
	string filename = argv[1];
	string stopk = argv[2];
	string algo_name = argv[3];
	// string filename = "CG";
	// string stopk = "5000";
	// string algo_name = "rbs";

	int topk = stoi(stopk);

	time_t now = time(nullptr);
	char* curr_time = ctime(&now); cout << curr_time << endl;
	string base_path = "";


	cout << "filename = " << filename << ", topk = " << topk << endl;
	unordered_set<int> ground_idx;
	ifstream fin;
	string idx_path = "../ground_idx/" + filename + "/idx1";
	cout << "idx_path = " << idx_path << endl;
	fin.open(idx_path.data());
	assert(fin.is_open());
	int x;
	while (fin >> x) {
		ground_idx.insert(x);
	}
	cout << "ground_idx.size = " << ground_idx.size() << endl;

	fin.close();
	unordered_map<int, unordered_map<int, double> > gt_sim;


	string groundtruth_path = "./gt/" + filename+"_all.gt";

	string gtpath = groundtruth_path;
	// cout << gtpath << endl;
	fin.open(gtpath.c_str());
	assert(fin.is_open());
	int a; int b; double sim;
	int gtnum = 0;
	vector<Sim_pair> vec_gt_sim;
	while (fin >> a >> b >> sim) {
		if (a == b)
			continue;
		if (a > b)
			gt_sim[a][b] = sim;
		else
			gt_sim[b][a] = sim;
		++gtnum;
	}
	fin.close();
	for (auto i : gt_sim) {
		for (auto j : i.second) {
			Sim_pair sp; sp.a = i.first; sp.b = j.first; sp.sim = j.second;
			vec_gt_sim.push_back(sp);
		}
	}

	sort(vec_gt_sim.begin(), vec_gt_sim.end(), max_comp_Sim_pair);
	vector<Sim_pair> vec_gt_sim_topk(vec_gt_sim.begin(), vec_gt_sim.begin() + topk);
	double topk_val = vec_gt_sim[topk - 1].sim;
	for (int i = topk; i < vec_gt_sim.size(); ++i) {
		if (vec_gt_sim[i].sim == topk_val)
			vec_gt_sim_topk.push_back(vec_gt_sim[i]);
		else
			break;
	}
	gt_sim.clear();
	for (auto i : vec_gt_sim_topk) {
		gt_sim[i.a][i.b] = i.sim;
	}

	cout << "gtnum = " << gtnum << endl;


	string anspath;
	if (algo_name == "rbs")
		anspath = base_path + "../../../rbs-sim_exp/result/" + filename + "/topk_" + stopk;
	else if (algo_name == "optlp")
		anspath = base_path + "../../../SimRankRelease-master_exp/Local-Push/eps_result/" + filename + "_topk" + stopk;
	unordered_map<int, unordered_map<int, double> > my_sim;
	cout << "anspath = " << anspath << endl;

	fin.open(anspath.data());
	assert(fin.is_open());
	int mynum11 = 0;
	while (fin >> a >> b >> sim) {
		assert(a != b);
		if (a > b)
			my_sim[a][b] = sim;
		else if (a < b)
			my_sim[b][a] = sim;
		mynum11++;
	}
	fin.close();
	cout << "mynum = " << mynum11 << endl;
	vector<Sim_pair> vec_my_sim;
	for (auto i : my_sim) {
		for (auto j : i.second) {
			Sim_pair sp; sp.a = i.first; sp.b = j.first; sp.sim = j.second;
			vec_my_sim.push_back(sp);
		}
	}
	sort(vec_my_sim.begin(), vec_my_sim.end(), max_comp_Sim_pair);
	vector<Sim_pair> vec_my_sim_topk(vec_my_sim.begin(), vec_my_sim.begin() + topk);
	my_sim.clear();
	for (auto i : vec_my_sim_topk) {
		my_sim[i.a][i.b] = i.sim;
	}
	// for(int i = 0; i < n; ++i)
	// 	my_sim[i][i] = 1.0;


	int true_num = 0;

	for (auto i : my_sim) {
		a = i.first;
		for (auto j : i.second) {
			b = j.first;
			double gts = j.second;
			if (gt_sim[a].find(b) != gt_sim[a].end()) {
				++true_num;
			}

		}
	}

	double acc = (true_num + 0.0) / topk;
	cout << "ground truth pairs num = " << vec_gt_sim_topk.size() << endl;
	cout << "accu = " << acc << endl;
	return 0;
}
