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

bool maxScoreCmp(const pair<int, double> &a, const pair<int, double> &b)
{
	return a.second > b.second;
}

struct Sim_pair
{
	int a;
	int b;
	double sim;
};


bool max_comp_Sim_pair(const Sim_pair &sp1, const Sim_pair &sp2)
{
	if (sp1.sim == sp2.sim && sp1.a == sp2.a)
		return sp1.b < sp2.b;
	if (sp1.sim == sp2.sim)
		return sp1.a < sp2.a;
	return sp1.sim > sp2.sim;
}

int main(int argc, char **argv)
{
	string filename = argv[1];
	double eps = atof(argv[2]);
	string seps = argv[2];
	string algo_name = argv[3];
	string stop_rea = argv[4];
	if (algo_name == "UISim"){
		eps = atoi(argv[2]);
		seps = to_string(int(eps));
	}
	string stopk = argv[5];
	int topk = stoi(stopk);
	string seta = argv[6];
	int eta = stoi(seta);

	time_t now = time(nullptr);
	char *curr_time = ctime(&now);
	cout << curr_time << endl;
	string base_path = "";

	cout << "filename = " << filename << ", eps = " << eps << ", stop_rea = " << stop_rea << ", topk=" << topk << ", eta=" << eta << endl;

	unordered_map<int, unordered_map<int, double>> gt_sim;		 
	unordered_map<int, unordered_map<int, double>> gt_sim_all_k; 

	string groundtruth_path = "/data/zhaoYingHao/metrics_exp/gt/" + filename + "_topk" + to_string(topk) + ".gt";
	string gt_all_k_path = "/data/zhaoYingHao/metrics_exp/gt/" + filename + "_all.gt";
	string gtpath = groundtruth_path;

	ifstream fin;
	fin.open(gtpath.c_str());
	assert(fin.is_open());
	int a;
	int b;
	double sim;
	long gtnum = 0;
	while (fin >> a >> b >> sim)
	{
		if (a == b)
			continue;
		if (a > b)
			gt_sim[a][b] = sim;
		else
			gt_sim[b][a] = sim;
		++gtnum;
	}
	fin.close();


	fin.open(gt_all_k_path);
	while (fin >> a >> b >> sim)
	{
		if (a == b)
			continue;
		if (a > b)
			gt_sim_all_k[a][b] = sim;
		else
			gt_sim_all_k[b][a] = sim;
	}
	fin.close();
	cout << "gtnum = " << gtnum << endl;


	string anspath;
	// string mfilename = "_stopRea" + stop_rea;
	if (algo_name == "rbs")
		anspath = base_path + "../rbs-sim/result/" + filename + "/topk_" + stopk;
	else if (algo_name == "optlp")
		anspath = base_path + "../SimRankRelease-master/Local-Push/eps_result/" + filename + "_topk" + stopk;
	else if (algo_name == "UISim")
	{
		anspath = "/data/zhaoYingHao/UISim2020-main/result/" + filename + "/eta" + seta + "_stopRea" + stop_rea + "_topk5000";
	}
	else if (algo_name == "KSimJoin")
		anspath = "/data/zhaoYingHao/KSimJoin/result/" + filename + "/" + "topk_times"+stop_rea+"_length"+ seta+"_eps"+ seps +".txt";

	unordered_map<int, unordered_map<int, double>> my_sim;
	vector<Sim_pair> vec_my_sim;
	cout << "anspath = " << anspath << endl;
	fin.open(anspath.data());
	assert(fin.is_open());
	if (algo_name == "UISim")
	{
		string st_time;
		fin >> st_time;
	}
	while (fin >> a >> b >> sim)
	{
		assert(a != b);
		if(sim <= 0) continue;
		if (a < b){
			int c = a;
			a = b;
			b = c;
		}
		if (gt_sim_all_k.find(a) == gt_sim_all_k.end() || gt_sim_all_k[a].find(b) == gt_sim_all_k[a].end()){
			continue;
		}

		Sim_pair sp;
		sp.a = a;
		sp.b = b;
		sp.sim = sim;
		vec_my_sim.emplace_back(sp);
	}
	fin.close();

	my_sim.clear();

	int start_vec = vec_my_sim.size();
	int my_sim_size = 0;
	cout << "vec_my_sim.size = " << start_vec << endl;
	int l_b = start_vec - topk;
	if (l_b < 0) l_b = 0;
	for (int i = start_vec - 1; i >= l_b; i--)
	{
		Sim_pair item = vec_my_sim[i];
		my_sim[item.a][item.b] = item.sim;
		my_sim_size += 1;
	}
	for (int i = l_b - 1; i >= 0; i--)
	{
		Sim_pair item = vec_my_sim[i];
		if (item.sim == vec_my_sim[l_b].sim)
		{
			my_sim[item.a][item.b] = item.sim;
			my_sim_size += 1;
		}
	}
	cout << "my_sim size is " << my_sim_size << endl;


	int true_num = 0;

	for (auto i : my_sim)
	{
		a = i.first;
		for (auto j : i.second)
		{
			b = j.first;
			double gts = j.second;
			if (gt_sim[a].find(b) != gt_sim[a].end())
			{
				++true_num;
			}
		}
	}

	double acc = (true_num + 0.0) / topk;
	cout << "accu = " << acc << endl;
	return 0;
}
