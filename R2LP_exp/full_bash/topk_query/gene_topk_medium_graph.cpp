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
	string idx_path = "../../../ground_idx/" + filename + "/idx1";
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

	string groundtruth_path = "../../../SimRankRelease-master/Local-Push/ground_truth/" + filename;

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
	cout << "gtnum = " << gtnum << ", 随机选1000*1000个点对" << endl;

	// 这里开始随机选1000 * 1000个点对
	vector<Sim_pair> vec_gt_sim;  // 最终的存放
	unordered_map<int, unordered_map<int, double> > set_gt_sim;  // 用来去重
	vector<int>fromnodelist;
	vector<int>tonodelist;
	// 先筛选from_node
	random_shuffle(nodeid_list.begin(), nodeid_list.end());
	int max_node_num = 1000;
	for (int i = 0; i < max_node_num; i++) {
		// if(gt_sim.find(nodeid_list[i]) == gt_sim.end()){
		// 	max_node_num += 1;
		// 	continue;
		// }
		fromnodelist.emplace_back(nodeid_list[i]);
	}
	// 再筛选1000个tonode
	random_shuffle(nodeid_list.begin(), nodeid_list.end());
	max_node_num = 1000;
	for (int i = 0; i < max_node_num; i++) {
		// if(gt_sim.find(nodeid_list[i]) == gt_sim.end()){
		// 	max_node_num += 1;
		// 	continue;
		// }
		tonodelist.emplace_back(nodeid_list[i]);
	}
	// 先对这1000*1000个节点进行去重
	for (int i = 0; i < 1000; i++) {
		int fromnode = fromnodelist[i];
		for (int j = 0; j < 1000; j++) {
			int tonode = tonodelist[j];
			// if(tonode == fromnode || gt_sim[fromnode].find(tonode) == gt_sim[fromnode].end()) continue;
			if (tonode == fromnode) continue;
			if (fromnode < tonode) {
				set_gt_sim[tonode][fromnode] = gt_sim[tonode][fromnode];
			}
			else {
				set_gt_sim[fromnode][tonode] = gt_sim[fromnode][tonode];
			}
		}
	}
	// 将去重后的结果放到最终容器中
	for (auto i : set_gt_sim) {
		for (auto j : i.second) {
			Sim_pair sp; sp.a = i.first; sp.b = j.first; sp.sim = j.second;
			vec_gt_sim.push_back(sp);
		}
	}

	cout << "保存集合A和B..." << endl;
	ofstream fout;
	fout.open("./gt/" + filename);
	for (auto i : fromnodelist) {
		fout << i << endl;
	}
	for (auto i : tonodelist) {
		fout << i << endl;
	}
	fout.close();

	cout << "最终选择的点对一共有" << vec_gt_sim.size() << "个，开始排序..." << endl;
	sort(vec_gt_sim.begin(), vec_gt_sim.end(), max_comp_Sim_pair);

	cout << "保存1000*1000的ground-truth..." << endl;
	ofstream gfout;
	gfout.open("./gt/" + filename + "_all.gt");
	for (Sim_pair ite : vec_gt_sim) {
		gfout << ite.a << " " << ite.b << " " << ite.sim << endl;
	}
	gfout.close();

	// 这里开始循环不同的top-k
	int topk_list[4] = { 500,1000,5000,10000 };
	for (int i = 0; i < 4; i++) {
		int topk = topk_list[i];
		cout << "开始top-k " << topk << " 的处理" << endl;
		vector<Sim_pair> vec_gt_sim_topk(vec_gt_sim.begin(), vec_gt_sim.begin() + topk);
		double topk_val = vec_gt_sim[topk - 1].sim;
		// 这里是把相同相似度的都加进来，因此vec_gt_sim_topk的大小可能大于k
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

		string respath = "./gt/";
		cout << "开始保存文件, 路径为 " << respath + filename + "_topk" + to_string(topk) + ".gt" << endl;

		ofstream fout;
		fout.open((respath + filename + "_topk" + to_string(topk) + ".gt").c_str());
		for (const auto & i : vec_gt_sim_topk) {
			fout << i.a << " " << i.b << " " << i.sim << endl;
		}

		fout.close();
		cout << "保存完毕, 保存的数据数为" << vec_gt_sim_topk.size() << "条" << endl;
	}
	return 0;
}
