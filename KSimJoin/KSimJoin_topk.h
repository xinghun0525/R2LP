#ifndef KSIMJOIN_H
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
	vector<robin_map<int, robin_map<int, double>>> NP_l;
	vector<robin_map<int, robin_map<int, double>>> RNP_l;
	vector<robin_map<int, pair<int,int>>> VsMap;
	vector<robin_map<int, robin_map<int, robin_map<int, double>>>> M_l;
	vector<robin_map<int, double>> second_prob;
	robin_map<int, robin_map<int, double>> Sim;
	priority_queue<Sim_pair> K;
	robin_map<int, vector<double>> Ui_map;
	Random mR;
	int maxlength = 3;
	int topk = 10000;
	int times = 1000;
	double sec_p_cost = 0.0;
	double incre_cost = 0.0;

	Ksimjoin(string file_name, int _maxlength = 3, int _topk = 10000, int _times=1000, double _eps = 0.01, double _c = 0.6)
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
		if (second_prob[l].find(a) != second_prob[l].end())
			return second_prob[l][a];

		clock_t t1=clock();
		int g_times = times;

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
					break;
				}
			}
		}
		for(int i = 0; i < maxlength; ++i){
			second_prob[i][a] = meet_times[i]/(times+0.0);
		}

		double prob = second_prob[l][a];
		sec_p_cost += (clock()-t1)/(double)CLOCKS_PER_SEC;

		return prob;
	}

	void IncrementalSimRank(robin_set<int> &R, int l)
	{
		clock_t t1=clock();
		while (!K.empty()) K.pop();

		robin_map<int, robin_set<int>> Pi_l;

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

				K.push(Sim_pair(v, u, Sim[v][u]));

				while (K.size() > topk) K.pop();
			}
		}
		incre_cost += (clock()-t1)/(double)CLOCKS_PER_SEC;
		return;
	}

	void Init_NP(int max_l)
	{
		robin_map<int, pair<int,int>> VsItem0;
		VsMap.emplace_back(VsItem0);

		robin_map<int, robin_map<int, double>> NP0;
		for (int i = 0; i < vert_num; ++i)
		{
			NP0[i][i] = 1.0;
		}
		NP_l.push_back(NP0);
		RNP_l.push_back(NP0);
		for (int tl = 1; tl <= max_l; ++tl)
		{
			robin_map<int, pair<int,int>> tmpVsItem;
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

					if(tmpVsItem.count(j.first)){
						int Vs1 = tmpVsItem[j.first].first, Vs2 = tmpVsItem[j.first].second;
						if(j.second >= tmpRNP[j.first][Vs1]){
							tmpVsItem[j.first].first = v;
							tmpVsItem[j.first].second = Vs1;
						}
						else if(j.second >= tmpRNP[j.first][Vs2]){
							tmpVsItem[j.first].second = v;
						}
					}
					else{
						tmpVsItem[j.first] = pair<int,int>(v, v);
					}
				}
			}
			RNP_l.push_back(tmpRNP);
			VsMap.emplace_back(tmpVsItem);
		}
	}

	double UpperBound(int v, int l)
	{
		if(!Ui_map.count(v)){
			vector<double> Ui_list(maxlength+1, 0.0);
			Ui_map[v] = Ui_list;
		}

		double U = 0;
		if (l == 1)
		{
			for (int i = 1; i <= maxlength; ++i)
			{
				double Ui = 0;
				
				for (const auto &xp : NP_l[i][v])
				{
					int x = xp.first;
					int Vs1 = VsMap[i][x].first;
					if(Vs1 == v) Vs1 = VsMap[i][x].second;
					if(Vs1 == v){
						continue;
					}
					
					double P = xp.second;
					double PVs = RNP_l[i][x][Vs1];
					double Px = P * PVs;

					M_l[i][v][Vs1][x] = Px;
					Ui += Px;
				}
				Ui_map[v][i] = Ui;
			}
		}
		else
		{
			for(int i = l; i <= maxlength; i++){
				int newi = i-(l-1);
				for (const auto &xp : NP_l[newi][v]){
					int x = xp.first;
					int Vs1 = VsMap[newi][x].first;
					if(Vs1 == v){
						Vs1 = VsMap[newi][x].second;
					}
					if(Vs1 == v){
						continue;
					}
					double P = M_l[newi][v][Vs1][x];
					double sec_prob = get_second_possibility(x, l-1);
					double Py = P * sec_prob;
					Ui_map[v][i] -= Py;
				}
			}
		}

		for(int i = 1; i <= maxlength; i++) U += Ui_map[v][i];
		
		return U;
	}

	priority_queue<Sim_pair> IterativePruneTopk(int k, int max_length)
	{
		cout << "Init_NP" << endl;
		clock_t t1=clock();
		Init_NP(max_length);
		cout << "Init_NP finished. cost " << (clock()-t1)/(double)CLOCKS_PER_SEC << " s, NP_l size is " << NP_l.size() << endl;

		int l = 1;
		robin_set<int> R;
		for (int i = 0; i < vert_num; i++)
			R.insert(i);

		while (l < max_length)
		{
			IncrementalSimRank(R, l);

			Sim_pair sigma = K.top();

			double U_min = 100.0, U_max = -100.0;
			robin_set<int> newR;

			clock_t uclock = clock();
			for (const auto &v : R)
			{
				double U = UpperBound(v, l);

				if (U >= sigma.sim)
				{
					newR.insert(v);
				}
				U_min = min(U_min, U);
				U_max = max(U_max, U);
			}
			++l;
			R = newR;
		}
		IncrementalSimRank(R, l);
		cout << "\nIncrementalSimRank(R, " << l << ") already cost " << incre_cost <<"s, get_second_possibility() already cost " << sec_p_cost << " s" << endl;
		cout << "Compute SimRank finished. total cost " << (clock()-t1)/(double)CLOCKS_PER_SEC << " s. ";
		cout << "get_second_possibility() total cost " << sec_p_cost << " s" << endl;


		for(const auto & si : Sim){
			int u = si.first;
			for(const auto & v_sim : si.second){
				int v = v_sim.first;
				if(v >= u) continue;
				double sim = v_sim.second;
				if(sim < 9e-4) continue;
				else if(sim >=1) continue;

				K.push(Sim_pair(u, v, Sim[u][v]));
			}
		}

		return K;
	}
};

#endif
