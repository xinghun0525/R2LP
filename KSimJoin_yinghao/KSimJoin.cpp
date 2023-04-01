#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>

#include "Graph.h"
#include "robin_map.h"
#include "robin_set.h"

using namespace std;
using namespace tsl;

class Ksimjoin {
public:
	Graph g;
	int vert_num;
	int edge_num;
	double c;
	double sqrtc;
	double eps;
	vector<robin_map<int, robin_map<int, double> > > NP_l;	//l-<v, x, pv> v->x
	vector<robin_map<int, robin_map<int, double> > > RNP_l;	//l-<x, u, pu> u->x
	vector<robin_map<int, robin_map<int, double> > > M_l;	//l-<u, x, px>

	Ksimjoin(string file_name, double _eps = 0.01, double _c = 0.6) {
		g.inputGraph(file_name);
		g.sort_node();
		vert_num = g.n;
		edge_num = g.m;
		c = _c;
		sqrtc = sqrt(c);
		eps = _eps;
	}
};

priority_queue<pair<double, pair<int, int> > > IncrementalSimRank(Ksimjoin &ksj, vector<int> R, robin_map<int, robin_map<int, double> > &S, int l) {
	priority_queue<pair<double, pair<int, int> > > K;
	robin_map<int, robin_map<int, double> > S_wave;

	robin_map<int, robin_map<int, double> > NP;		//<v, x, pv> v->x
	robin_map<int, robin_map<int, double> > RNP;	//<x, u, pu> u->x
	
	if(l<=ksj.NP_l.size()) NP=ksj.NP_l[l-1];
	else return K;
	if(l<=ksj.RNP_l.size()) RNP=ksj.RNP_l[l-1];
	else return K;

	for (auto vert : R) {
		robin_map<int, robin_map<int, double> > M;	//<u, x, px>
		vector<int> vpair;
		//find x
		auto ittrx = NP.find(vert);
		if (ittrx != NP.end()) {
			for (auto ittr = ittrx->second.begin(); ittr != ittrx->second.end(); ittr++) {
				int vertx = ittr->first;
				//find u
				auto ittru = RNP.find(vertx);
				if (ittru != RNP.end()) {
					for (auto it = ittru->second.begin(); it != ittru->second.end(); it++) {
						int vertu = it->first;
						double px = ittr->second*it->second;	//px=pv*pu

						if (find(vpair.begin(), vpair.end(), vertu) == vpair.end()) {
							S_wave[vert][vertu] = px;
							vpair.push_back(vertu);
						}
						else {
							S_wave[vert][vertu] += px;
						}

						M[vertu][vertx] = px;
					}
				}
			}
		}
		
		ksj.M_l.push_back(M);

		for (auto vertu : vpair) {
			int j = 1;
			while (j < l) {
				auto ittrx = M.find(vertu);
				if (ittrx != M.end()) {
					for (auto it = ittrx->second.begin(); it != ittrx->second.end(); it++) {
						//SecondMeetingProbability
						//S_wave[vert][vertu] = S_wave[vert][vertu] - it.second*Second(it.first, l - j);
					}
				}

				j++;
			}
			S[vert][vertu] += S_wave[vert][vertu];
			K.push(make_pair(S[vert][vertu], make_pair(vert, vertu)));
		}
	}

	return K;
}

double GetValue(robin_map<int, robin_map<int, double> > m,int x,int y){
	if(m.find(x)!=m.end()){
		if(m[x].find(y)!=m[x].end()){
			return m[x][y];
		}
		else{
			return 0.0;
		}
	}
	else{
		return 0.0;
	}
} 

void ComputeNP(Ksimjoin &ksj, vector<int> R, int l){
	if(l==0) return;
	
	Graph g=ksj.g;
	robin_map<int, robin_map<int, double> > NP;
	robin_map<int, robin_map<int, double> > RNP;
	//from->to
	if(l==1){
		for(auto from:R){
			double sum=0.0;
			for(auto to:R){
				if(from==to) continue;
				NP[from][to]=ksj.sqrtc/g.getInSize(to);
				RNP[to][from]=ksj.sqrtc/g.getInSize(to);
			}
		}
		ksj.NP_l.push_back(NP);
		ksj.RNP_l.push_back(RNP);
	}
	else{
		robin_map<int, robin_map<int, double> > NP_l=ksj.NP_l[l-1];
		robin_map<int, robin_map<int, double> > RNP_l=ksj.RNP_l[l-1];
		for(auto vert:R){
			double sumnp=0.0;
			double sumrnp=0.0;
			for(auto to:R){
				if(vert==to) continue;
				//vert~>from->to
				for(int i=0;i<g.getInSize(to);i++){
					int from=g.getInVert(to,i);
					sumnp+=GetValue(NP_l,vert,from);
				}
				NP[vert][to]=sumnp*ksj.sqrtc/g.getInSize(to);
				//to->from~>vert
				for(int i=0;i<g.getOutSize(to);i++){
					int from=g.getOutVert(to,i);
					sumrnp=sumrnp+GetValue(RNP_l,vert,from)*ksj.sqrtc/g.getInSize(from);
				}
				RNP[vert][to]=sumrnp;
			}
		}
		ksj.NP_l.push_back(NP);
		ksj.RNP_l.push_back(RNP);
	}
} 

vector<pair<int, int> > IterativePruneTopk(Ksimjoin &ksj, int k, int max_length) {
	priority_queue<pair<double, pair<int, int> > > K;

	Graph G=ksj.g;
	double c=ksj.c;
	
	int l = 1;
	robin_map<int, robin_map<int, double> > Sim;
	vector<int> R;
	for (int i = 0; i < G.n; i++) R.push_back(i);

	while (l <= max_length) {
		ComputeNP(ksj, R, l);
		K = IncrementalSimRank(ksj, R, Sim, l);
		while (K.size() > k) K.pop();

		//double sigma = K.top().first;

		//UpperBound

		l++;
	}

	
	vector<pair<int, int> > Topk;
	while (!K.empty()) {
		Topk.push_back(K.top().second);
		cout<<K.top().first<<" ("<<K.top().second.first<<" "<<K.top().second.second<<")"<<endl; 
		K.pop();
	}
	return Topk;
}

int main() {
	string file_name = "CG.txt";
	Ksimjoin ksj = Ksimjoin(file_name);

	int k = 100;
	int length = 1;

	vector<pair<int, int> > topk = IterativePruneTopk(ksj, k, length);
	cout<<"Compute finish"<<endl;
}
