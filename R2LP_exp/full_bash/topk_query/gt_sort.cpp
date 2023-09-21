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
// 从大到小排序
bool max_comp_Sim_pair(const Sim_pair & sp1, const Sim_pair & sp2) {
	if (sp1.sim == sp2.sim && sp1.a == sp2.a)
		return sp1.b < sp2.b;
	if (sp1.sim == sp2.sim)
		return sp1.a < sp2.a;
	return sp1.sim > sp2.sim;
}


// 该函数的功能是读取每个图的ground-truth，之后排序后将其前top-k个是数据保存到：
int main(int argc, char **argv) {
	string filename = argv[1];

	time_t now = time(nullptr);
	char* curr_time = ctime(&now); cout << curr_time << endl;
	string base_path = "";

	cout << "filename = " << filename << endl;

	vector<int> nodeid_list;
	unordered_set<int> ground_idx;
	ifstream fin;
	string idx_path = "../ground_idx/" + filename + "/idx1";
	cout << "idx_path = " << idx_path << endl;
	fin.open(idx_path.data());
	assert(fin.is_open());
	int x;
	while (fin >> x) {
		ground_idx.insert(x);
		nodeid_list.emplace_back(x);
	}
	cout << "ground_idx.size = " << ground_idx.size() << ", 开始读取数据" << endl;

	fin.close();
	unordered_map<int, unordered_map<int, double> > gt_sim;
	vector<Sim_pair> vec_gt_sim;

	string groundtruth_path = "../SimRankRelease-master/Local-Push/ground_truth/" + filename;

	string gtpath = groundtruth_path;
	fin.open(gtpath.c_str());
	assert(fin.is_open());
	int a; int b; double sim;
	long long gtnum = 0;

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

	ofstream gfout;
	gfout.open("./gt/" + filename + "_all.gt");
	for (Sim_pair ite : vec_gt_sim) {
		gfout << ite.a << " " << ite.b << " " << ite.sim << endl;
	}
	gfout.close();

	return 0;
}
