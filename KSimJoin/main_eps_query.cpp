#include <bits/stdc++.h>
#include <iostream>
#include "KSimJoin_eps.h"

using namespace std;
using namespace tsl;
int main(int argc, char** argv){
	string filename = argv[1];
	int times = stoi(argv[2]);
	int max_length = stoi(argv[3]);
	double eps = stof(argv[4]);
	
	time_t now = time(nullptr);   
	char* curr_time = ctime(&now); cout << curr_time <<endl;
	int topk = 500;
	
	string filepath = "../../dataset/" + filename + ".txt";
	

	cout << "filepath: " << filepath << ", max_length: " << max_length << ", times: " << times << ", eps: " << eps << endl;

    Ksimjoin ksj = Ksimjoin(filepath, max_length, topk, times, eps);
	priority_queue<Sim_pair> K = ksj.IterativePruneTopk(topk, max_length);
	

	string respath = "./result/"+filename+"/times"+to_string(times)+"_length" + to_string(max_length) + ".txt";
	cout << respath << endl;
	ofstream fout;
	fout.open(respath);
	while (!K.empty())
	{
		Sim_pair sp = K.top();
		fout << sp.a << " " << sp.b << " " << sp.sim << endl;
		K.pop();
	}
	fout.close();
	cout<<"Compute finish"<<endl;
    return 0;
}
