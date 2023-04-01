//
// Created by yche on 1/22/18.
//

#include <boost/program_options.hpp>

#include "../yche_refactor/simrank.h"
#include "../yche_refactor/local_push_yche.h"
#include <ctime>

using namespace std;
using namespace boost::program_options;

int main(int argc, char *argv[]) {
    string g_name = argv[1];
    double eps = atof(argv[2]);
    double threshold = atof(argv[3]);
    string q_name = "../../../ground_idx/" + g_name + "/idx1";
    ifstream fin;
    fin.open(q_name.data());
    // assert(fin.is_open());
    vector<int> veidx;
    int nd;
    int adfx = 0;
    while(fin >> nd){
        veidx.push_back(nd);
    }
    
    // 1st: compute reduce local push
    
    string path = get_edge_list_path(g_name);
    cout << "path = " << path << endl;
    GraphYche g(path);
    double c = 0.6;
//    double eps = 0.0001;

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
    auto start_time = std::chrono::high_resolution_clock::now();
    lp->local_push(g);
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;
    cout << format("computation time: %s s") % elapsed.count() << endl; // record the pre-processing time
    cout << format("push num: %s") % lp->n_push << endl;
    cout << format("max queue size #: %s") % (lp->n_push / lp->max_q_size) << endl;
    cout << format("mem size: %s KB") % getValue() << endl;

    cout << format("P size: %s") % lp->P.size() << endl;
    cout << format("R size: %s") % lp->R.size() << endl;
    cout << format("avg push merge #: %s") % (lp->n_push / lp->R.size()) << endl;


    string output = "../result/" + g_name + ".ans" + to_string(eps);


    
    ofstream out;
    out.open(output.data());
    assert(out.is_open());
    
    // for(auto i :veidx){
    //     for(auto apair: lp->P.v[i]){
    //         int j = apair.first;
    //         double res = apair.second;
    //         if(i != j)
    //             res /= sqrt(2);
    //         if(res >= 0.001)
    //             out << i << " " << j << " " << res << endl;
    //     }
    // }
    for(const auto& i : veidx){
        for(int j = 0; j < n; ++j){
            double res = lp->query_P(i,j);
            if(res >= threshold*0.9){
                out << i << " " << j << " " << res << endl;
            }
        }
    }
    out.close();

    string gt = "../ground_truth/" + g_name;


    
    // cout << "going to compute groundtruth!! " << gt << endl;
    // ofstream fgt;
    // fgt.open(gt.data());
    // assert(fgt.is_open());
    // if(n < 1000000){
    //     TruthSim ts(g_name, g, c, eps);
    //     cout << "finished ts matrix " << endl;
    //     for(int i = 0; i < n; ++i){
    //         for(int j = 0; j <= i; ++j){
    //             float asim = ts.sim(i,j);
    //             if(asim >= 1e-5)
    //                 fgt << i << " " << j << " " << ts.sim(i,j) << endl;
    //         }
    //     }
    // }
    // fgt.close();
    

    // for(int i = 0; i < n; ++i){
    //     for(int j = 0; j < n; ++j){
    //         double res = lp->query_P(i,j);
    //         if(res != 0){
    //             out << i << " " << j << " " << res << endl;
    //         }
    //     }
    // }
    /*
    int psz = lp->P.v.size();
    for(int i = 0; i < psz; ++i){
        for(auto pi: lp->P.v[i]){
            int j = pi.first;
            double r = pi.second/sqrt(2);
            
                out << i << " " << j << " " << r << endl;
        }
    }
    */

    

}