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
typedef long long ll;
struct Sim_pair
{
	int a;
	int b;
	double sim;
	Sim_pair(int _a, int _b, double _sim) : a(_a), b(_b), sim(_sim){};

	bool operator < (Sim_pair b) const{  // 先pop小的
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
	// vector<robin_map<int, robin_map<int, int>>> VsMap;					// l-<v, x, Vs> super node Vs
	vector<robin_map<int, robin_map<int, robin_map<int, double>>>> M_l; // l-<u, v, x, px> u->x = l, v->x = l
	// vector<robin_map<int, robin_map<int, int>>> VsMap;					// l-<v, x, Vs> super node Vs
	// vector<robin_map<int, robin_map<int, double>>> UB_p; // upper bound prob. l-<u, x, px>
	vector<robin_map<int, double>> second_prob;							// l-<u, second_prob>
	robin_map<int, robin_map<int, double>> Sim;
	// vector<double> Ui_list;  // l, UpperBound中记录Ui

	Random mR;
	int maxlength = 3;
	int topk = 50;
	int times = 1000;
	double sec_p_cost = 0.0;  // 记录get_second_prob函数的总耗时

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

		// 初始化Ui_list
		// for(int i = 0; i <= maxlength; i++){
		// 	Ui_list.emplace_back(0.0);
		// }
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
		// while (!K.empty())
		// 	K.pop();
		// robin_map<int, robin_map<int, double> > S_wave;

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

					// if(l == 2 && v == 9 && u == 10){
					// 	cout << "Sim[10][9] is " << Sim[u][v] << " ,px is " << px;
					// 	cout << " , pv is " << pv << ", pu is " << pu << endl;   
					// }

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

						// if(l == 2 && v == 10 && u == 9){
						// 	cout << "ele.first is " << ele.first << ", second possib is " << get_second_possibility(ele.first, l - j);
						// 	cout << ", ele.second is " << ele.second << ", Sim[10][9] is " << Sim[v][u] << endl;
						// }
					}
					++j;
				}

				// if(l == 2 && v == 10 && u == 9){
				// 	cout << v << " " << u << " sim is " << Sim[v][u] << endl;
				// }

				// K.push(Sim_pair(v, u, Sim[v][u]));

				// while (K.size() > topk)
				// 	K.pop();
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


			// 输出当l=2时的NP
			// cout << "l=2, NP为：" << endl;
			// for(const auto &i : tmpNP){
			// 	int u = i.first;
			// 	for (const auto &j : i.second)
			// 	{
			// 		int v = j.first;
			// 		double tmp_prob = j.second;
			// 		cout << u << " " << v << " " << tmp_prob << endl;
			// 	}
			// }

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

	// void Cre_VsMap()
	// {
	// 	// 只保存 l-(v, x, Vs), PVs通过 RNP_l[l][x][Vs] 来获取
	// 	robin_map<int, robin_map<int, int>> VsMap_item0;
	// 	for (int v = 0; v < vert_num; v++)
	// 	{
	// 		VsMap_item0[v][v] = v;
	// 	}
	// 	VsMap.emplace_back(VsMap_item0);
	//
	// 	for (int i = 1; i <= maxlength; i++)
	// 	{
	// 		robin_map<int, robin_map<int, int>> VsMap_itemi;
	// 		vector<vector<int>> xRNP; // 储存每个x的第一大和第二大概率的节点(x, (v1, v2))
	// 		for (int x = 0; x < vert_num; x++)
	// 		{
	// 			vector<int> xRNP_item; // 储存到达x的第一大和第二大概率的节点
	// 			int min_index = 0;	   // 记录第二大概率节点的索引。{0, 1}
	// 			for (const auto &npi : RNP_l[i][x])
	// 			{
	// 				int v = npi.first;
	// 				double v2xP = npi.second;
	// 				if (xRNP_item.size() == 0)
	// 					xRNP_item.emplace_back(v);
	// 				else if (xRNP_item.size() == 1)
	// 				{
	// 					xRNP_item.emplace_back(v);
	// 					if (RNP_l[i][x][xRNP_item[0]] > v2xP)
	// 						min_index = 1;
	// 				}
	// 				else
	// 				{
	// 					if (RNP_l[i][x][xRNP_item[min_index]] < v2xP)
	// 					{
	// 						xRNP_item[min_index] = v; // 替换掉第二大概率
	// 						// 判断第一大概率是否仍为最大的
	// 						if (RNP_l[i][x][xRNP_item[1 - min_index]] < v2xP)
	// 							min_index = 1 - min_index;
	// 					}
	// 				}
	// 			}
	// 			xRNP.emplace_back(xRNP_item);
	// 		}
	// 		// 开始填充VsMap
	// 		for (int v = 0; v < vert_num; v++)
	// 		{
	// 			for (const auto &npi : NP_l[i][v])
	// 			{
	// 				int x = npi.first;
	// 				// 获取x节点的第一大概率，看其是否与v相同，若相同，则使用第二大概率。
	// 				vector<int> xRNP_item = xRNP[x];
	// 				int max_index = 0;
	// 				if (RNP_l[i][x][xRNP_item[0]] < RNP_l[i][x][xRNP_item[1]])
	// 					max_index = 1;
	// 				// 判断最大概率节点是否和v相同
	// 				if (xRNP_item[max_index] == v)
	// 					max_index = 1 - max_index;
	// 				// 更新VsMap(v,x,Vs)
	// 				VsMap_itemi[v][x] = xRNP_item[max_index];
	// 			}
	// 		}
	// 		VsMap.emplace_back(VsMap_itemi);
	// 	}
	// }

	// double UpperBound(int v, int l)
	// {
	// 	// to be finished !!
	// 	double U = 0;
	//
	// 	if (l == 1)
	// 	{
	// 		for (int i = 1; i <= maxlength; ++i)
	// 		{
	// 			double Ui = 0;
	// 			for (const auto &xp : NP_l[i][v])
	// 			{
	// 				int x = xp.first;
	// 				double P = xp.second;
	// 				int Vs = VsMap[i][v][x];
	// 				double PVs = RNP_l[i][x][Vs];
	// 				double Px = P * PVs;
	// 				// l-<u, v, x, px> u->x = l, v->x = l
	// 				M_l[i][v][Vs][x] = Px;
	// 				Ui += Px;
	// 			}
	// 			Ui_list[i] = Ui;
	// 		}
	// 	}
	// 	else
	// 	{
	// 		for(int i = l; i <= maxlength; i++){
	// 			// l-(v, x, Vs)，遍历Vs
	// 			for(const auto &vm : VsMap[i][v]){
	// 				int x = vm.first;
	// 				int Vs = vm.second;
	// 				double P = M_l[i-(l-1)][v][Vs][x];
	// 				double sec_prob = get_second_possibility(x, l-1);
	// 				double Py = P * sec_prob;
	// 				Ui_list[i] -= Py;
	// 			}
	// 		}
	// 	}
	// 	for(int i = 1; i <= maxlength; i++) U += Ui_list[i];
    //
	// 	return U;
	// }

	priority_queue<Sim_pair> IterativePruneTopk(int k, int max_length)
	{
		cout << "正在Init_NP" << endl;
		clock_t t1=clock();
		Init_NP(max_length);
		cout << "Init_NP finished. cost " << (clock()-t1)/(double)CLOCKS_PER_SEC << " s, NP_l size is " << NP_l.size() << endl;

		// 构建每个节点的超节点
		// cout << "正在构建VsMap" << endl;
		// Cre_VsMap();
		cout << "开始计算SimRank" << endl;
		int l = 1;


		robin_set<int> R;
		for (int i = 0; i < vert_num; i++)
			R.insert(i);

		while (l < max_length)
		{
			cout << "compute IncrementalSimRank(R, " << l << "), get_second_possibility() already cost " << sec_p_cost << " s" << endl;
			IncrementalSimRank(R, l);

			// cout << "K size is " << K.size() << endl;
			// Sim_pair sigma = K.top();  // 获取最小sim的节点对
			// robin_set<int> newR;
			// for (const auto &v : R)
			// {
			// 	double U = UpperBound(v, l);
				
			// 	if (U >= sigma.sim)
			// 	{
			// 		newR.insert(v);
			// 		// cout << v << " is insert newR" << endl;
			// 	}
			// }
			++l;
			// R = newR;
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
				// u必须要比v大
				if(v >= u) continue;
				double sim = v_sim.second;

				// 全输出，用来测error
				K.push(Sim_pair(u, v, Sim[u][v]));

				// // Top-k输出
				// if (K.size() < k)
				// {
				//   K.push(Sim_pair(u, v, Sim[u][v]));
				// }
				// else if (sim > K.top().sim)
				// {
				//   K.pop();
				//   K.push(Sim_pair(u, v, Sim[u][v]));
				// }
			}
		}

		return K;
	}
};

#endif
