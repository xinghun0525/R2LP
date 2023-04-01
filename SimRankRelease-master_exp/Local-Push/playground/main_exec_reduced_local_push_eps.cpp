//
// Created by yche on 1/22/18.
//

#include <boost/program_options.hpp>

#include "../yche_refactor/simrank.h"
#include "../yche_refactor/local_push_yche.h"
#include <unordered_set>
#include <ctime>

using namespace std;
using namespace boost::program_options;

int main(int argc, char *argv[]) {
    string g_name = argv[1];
    // double eps = atof(argv[2]);
    double threshold = 9e-4;
    double eps = atof(argv[2]);

    string q_name = "../../../ground_idx/" + g_name + "/idx1";
    ifstream fin;
    fin.open(q_name.data());
    // assert(fin.is_open());
    vector<int> veidx;
    unordered_set<int> veidx_set;
    int nd;
    int adfx = 0;
    while(fin >> nd){
        veidx.push_back(nd);
        veidx_set.insert(nd);
    }
    
    // 1st: compute reduce local push
    
    string path = get_edge_list_path(g_name);
    cout << "path = " << path << endl;
    GraphYche g(path);
    double c = 0.6;

    auto n = g.n;
    cout << "graph num = " << n << endl;
    cout << "query node num = " << veidx.size() << endl;

    time_t now = time(nullptr);   
	char* curr_time = ctime(&now); cout << curr_time <<endl;

#ifdef FLP
    auto lp = new Full_LocalPush(g, g_name, c, eps, n);
#else
    auto lp = new Reduced_LocalPush(g, g_name, c, eps, n);
#endif
    auto final_start_time = std::chrono::high_resolution_clock::now();

    cout << endl << "***tmp eps = " << eps << endl;
    // auto start_time = std::chrono::high_resolution_clock::now();
    lp->local_push(g);
    
    auto final_end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = final_end_time - final_start_time;
    cout << format("computation time: %s s") % elapsed.count() << endl; // record the pre-processing time
    cout << format("push num: %s") % lp->n_push << endl;
    cout << format("max queue size #: %s") % (lp->n_push / lp->max_q_size) << endl;
    cout << format("mem size: %s KB") % getValue() << endl;

    cout << format("P size: %s") % lp->P.size() << endl;
    cout << format("R size: %s") % lp->R.size() << endl;
    cout << format("avg push merge #: %s") % (lp->n_push / lp->R.size()) << endl;
    

    string output = "../eps_result/" + g_name + ".ans_eps" + to_string(eps);


    
    ofstream out;
    out.open(output.data());
    assert(out.is_open());
    
    
    sort(veidx.begin(), veidx.end(), greater<int>());
    for(int i = 0; i < lp->P.v.size(); ++i){
        if (veidx_set.find(i) != veidx_set.end()){
			for(auto p2: lp->P.v[i]){
				if(p2.second/sqrt(2) >= threshold && p2.first != i)
					out << i << " " << p2.first << " " << p2.second/sqrt(2) << endl;
			}
			continue;
		}
        auto tmp = lp->P.v[i];

		for(auto j: veidx){
			if(j <= i)
				break;
			if(tmp.find(j) != tmp.end() && tmp[j]/sqrt(2) >= threshold)
				out << i << " " << j << " " << tmp[j]/sqrt(2) << endl;
		}
    }


    out.close();
    now = time(nullptr);   
	curr_time = ctime(&now); cout << curr_time <<endl;
    
    

    
    

}