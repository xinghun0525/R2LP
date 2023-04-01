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
    int topk = atof(argv[2]);
    // assert(fin.is_open());

    int nd;
    int adfx = 0;
    // while(fin >> nd){
    //     ++adfx;
    //     veidx.push_back(nd);
    //     // cout << "adfx = " << adfx << ", nd = " << nd << endl;
    // }
    
    // 1st: compute reduce local push
    
    string path = get_edge_list_path(g_name);
    GraphYche g(path);
    double c = 0.6;
    double eps = 0.06;
//    double eps = 0.0001;

    auto n = g.n;
    cout << "graph num = " << n << endl;
    

    time_t now = time(nullptr);   
	char* curr_time = ctime(&now); cout << curr_time <<endl;
    auto start_time = std::chrono::high_resolution_clock::now();
#ifdef FLP
    auto lp = new Full_LocalPush(g, g_name, c, eps, n);
#else
    auto lp = new Reduced_LocalPush(g, g_name, c, eps, n);
#endif
    

    while(true){
        int total_num = 0;
        lp->local_push(g);
        for(int i = 0; i < n; ++i){
            for(auto apair: lp->P.v[i]){
                int j = apair.first;
                double res = apair.second;
                if(i != j)
                    res /= sqrt(2);
                if(res >= eps*10 && i != j)
                    ++total_num;
            }
        }
        cout << "eps = " << eps << ", total_num = " << total_num << endl;
        if(total_num >= topk)
            break;
        eps /= 2;
#ifdef FLP
    lp = new Full_LocalPush(g, g_name, c, eps, n);
#else
    lp = new Reduced_LocalPush(g, g_name, c, eps, n);
#endif

    }
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;
    cout << format("computation time: %s s") % elapsed.count() << endl; // record the pre-processing time
    cout << format("push num: %s") % lp->n_push << endl;
    cout << format("max queue size #: %s") % (lp->n_push / lp->max_q_size) << endl;
    cout << format("mem size: %s KB") % getValue() << endl;

    cout << format("P size: %s") % lp->P.size() << endl;
    cout << format("R size: %s") % lp->R.size() << endl;
    cout << format("avg push merge #: %s") % (lp->n_push / lp->R.size()) << endl;


    string output = "../topk_result/" + g_name + ".ans" + to_string(topk);


    
    ofstream out;
    out.open(output.data());
    assert(out.is_open());
    // for(auto i : veidx){
    //     // cout << i << endl;
    //     for(int j = 0; j < n; ++j){
    //         double res = lp->query_P(i,j);
    //         if(res != 0){
    //             out << i << " " << j << " " << res << endl;
    //         }
    //     }
    // }
    for(int i = 0; i < n; ++i){
        for(auto apair: lp->P.v[i]){
            int j = apair.first;
            double res = apair.second;
            if(i != j)
                res /= sqrt(2);
            out << i << " " << j << " " << res << endl;
        }
    }
    out.close();

    // string gt = "../ground_truth/" + g_name;


    
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