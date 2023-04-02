#ifndef KSIMJOIN_EPS_H
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>

#include "Graph.h"
#include "robin_map.h"
#include "robin_set.h"
#include "Random.h"

using namespace std;
using namespace tsl;
typedef long long ll;
struct Sim_pair
{
	int a;
	int b;
	double sim;
	Sim_pair(int _a, int _b, double _sim) : a(_a), b(_b), sim(_sim){};

	bool operator < (Sim_pair b) const{
		return this->sim > b.sim;
	}
};


class Ksimjoin
{
public:
	Graph g;
	int vert_num;
	int edge_num;
	double cvalue;
	double sqrtc;
	double eps;  
	vector<robin_map<int, robin_map<int, double>>> NP_l;				// l-<v, x, pv> v->x
	vector<robin_map<int, robin_map<int, double>>> RNP_l;				// l-<x, u, pu> u->x
	vector<robin_map<int, robin_map<int, robin_map<int, double>>>> M_l; // l-<u, v, x, px> u->x = l, v->x = l
	vector<robin_map<int, double>> second_prob;							// l-<u, second_prob>
	robin_map<int, robin_map<int, double>> Sim;

	Random mR;
	int maxlength = 3;
	int topk = 50;
	int times = 1000;
	double sec_p_cost = 0.0;

	void print_num(){
		int idx = 0;
		for(auto i : NP_l){
			ll tmp_num = 0;
			for(auto j : i){			
				tmp_num += j.second.size();
			}
			cout << "NP_l[" << idx << "] = " << tmp_num << endl;
			++idx;
		}
		cout << endl;
		idx = 0;
		for(auto i : RNP_l){
			ll tmp_num = 0;
			for(auto j : i){			
				tmp_num += j.second.size();
			}
			cout << "RNP_l[" << idx << "] = " << tmp_num << endl;
			++idx;
		}
		idx = 0;
		for(auto i : M_l){
			ll tmp_num = 0;
			for(auto j : i){
				for(auto k: j.second){
					
					tmp_num += k.second.size();
					
				}
			}
			cout << "M_l[" << idx << "] = " << tmp_num << endl;
			++idx;
		}
	}

	Ksimjoin(string file_name, int _maxlength = 3, int _topk = 50, int _times=100000, double _eps = 0.01, double _c = 0.6)
	{
		g.inputGraph(file_name);
		g.sort_node();
		vert_num = g.n;
		edge_num = g.m;
		maxlength = _maxlength;
		cvalue = _c;
		sqrtc = sqrt(cvalue);
		topk = _topk;
		second_prob.resize(maxlength);
		eps = _eps;
		times = _times;

		srand(unsigned(time(0)));
		mR = Random(unsigned(rand()));
		M_l.resize(maxlength + 1);

	}

	double get_second_possibility(int a, int l)
	{
		// (a,a) -> singleton node pair with exact l length.
		if (second_prob[l].find(a) != second_prob[l].end())
			return second_prob[l][a];

		clock_t t1=clock();
		int g_times = times;
		// int meet_times = 0;
		vector<int> meet_times(maxlength, 0);
		
		while (g_times--)
		{
			int ta = a, tb = a;
			int tmp_l = 0;
			while (mR.drand() < cvalue)
			{
				if(g.getInSize(ta) == 0 || g.getInSize(tb) == 0)
					break;
				++tmp_l;
				if(tmp_l >= maxlength) break;
				int idxa = mR.drand() * g.getInSize(ta);
				ta = g.getInVert(ta, idxa);
				int idxb = mR.drand() * g.getInSize(tb);
				tb = g.getInVert(tb, idxb);
				if (ta == tb)
				{
					++meet_times[tmp_l];
					// ++meet_times;
					break;
				}
			}
		}
		for(int i = 0; i < maxlength; ++i){
			second_prob[i][a] = meet_times[i]/(times+0.0);
		}
		// double prob = meet_times / (times + 0.0);
		// second_prob[l][a] = prob;
		double prob = second_prob[l][a];
		sec_p_cost += (clock()-t1)/(double)CLOCKS_PER_SEC;
		// cout << "compute_use time = "
		return prob;
	}

	void IncrementalSimRank(robin_set<int> &R, int l)
	{
		robin_map<int, robin_set<int>> Pi_l;
		// Pi_l[v] = {u} exists 2-way paths start from u & v meet at l hops

		assert(NP_l.size() >= l);
		for (const auto &v : R)
		{
			for (const auto &npi : NP_l[l][v])
			{
				int x = npi.first;
				double pv = npi.second;
				for (const auto &rnpi : RNP_l[l][x])
				{
					int u = rnpi.first;
					double pu = rnpi.second;
					double px = pv * pu;
					Pi_l[v].insert(u);

					Sim[u][v] += px;
					M_l[l][v][u][x] = px;
				}
			}
			for (const auto &u : Pi_l[v])
			{
				if (v <= u) continue;
				int j = 1;
				while (j < l)
				{
					for (const auto &ele : M_l[j][v][u])
					{
						Sim[v][u] -= get_second_possibility(ele.first, l - j) * ele.second;
					}
					++j;
				}
			}
		}
		return;
	}

	void Init_NP(int max_l)
	{
		robin_map<int, robin_map<int, double>> NP0;
		for (int i = 0; i < vert_num; ++i)
		{
			NP0[i][i] = 1.0;
		}
		NP_l.push_back(NP0);
		RNP_l.push_back(NP0);
		for (int tl = 1; tl <= max_l; ++tl)
		{
			robin_map<int, robin_map<int, double>> &lstNP = NP_l[tl - 1];
			robin_map<int, robin_map<int, double>> &lstRNP = RNP_l[tl - 1];
			robin_map<int, robin_map<int, double>> tmpNP;
			robin_map<int, robin_map<int, double>> tmpRNP;

			for (const auto &i : lstNP)
			{
				int v = i.first;
				for (const auto &j : i.second)
				{
					int x = j.first;
					double tmp_prob = j.second;
					int din = g.getInSize(x);
					for (int k = 0; k < din; ++k)
					{
						int tx = g.getInVert(x, k);
						tmpNP[v][tx] += tmp_prob * sqrtc / din;
					}
				}
			}
			NP_l.push_back(tmpNP);

			for (const auto &i : tmpNP)
			{
				int v = i.first;
				for (const auto &j : i.second)
				{
					tmpRNP[j.first][v] = j.second;
				}
			}
			RNP_l.push_back(tmpRNP);
		}
	}


	priority_queue<Sim_pair> IterativePruneTopk(int k, int max_length)
	{
		clock_t t1=clock();
		Init_NP(max_length);
		cout << "Init_NP finished. cost " << (clock()-t1)/(double)CLOCKS_PER_SEC << " s, NP_l size is " << NP_l.size() << endl;

		int l = 1;


		robin_set<int> R;
		for (int i = 0; i < vert_num; i++)
			R.insert(i);

		while (l < max_length)
		{
			cout << "compute IncrementalSimRank(R, " << l << "), get_second_possibility() already cost " << sec_p_cost << " s" << endl;
			IncrementalSimRank(R, l);
			++l;
		}
		cout << "compute IncrementalSimRank(R, " << l << "), get_second_possibility() already cost " << sec_p_cost << " s" << endl;
		IncrementalSimRank(R, l);
		cout << "Compute SimRank finished. total cost " << (clock()-t1)/(double)CLOCKS_PER_SEC << " s" << endl;
		cout << "get_second_possibility() total cost " << sec_p_cost << " s" << endl;
		// 使用优先队列K来保存最终的结果，并返回
		print_num();
		priority_queue<Sim_pair> K;
		for(const auto & si : Sim){
			int u = si.first;
			for(const auto & v_sim : si.second){
				int v = v_sim.first;
				
				if(v >= u) continue;
				double sim = v_sim.second;
				
				K.push(Sim_pair(u, v, Sim[u][v]));

			}
		}

		return K;
	}
};

#endif
