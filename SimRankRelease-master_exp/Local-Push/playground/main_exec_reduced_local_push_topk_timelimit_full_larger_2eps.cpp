//
// Created by yche on 1/22/18.
//

#include <boost/program_options.hpp>

#include "../yche_refactor/simrank.h"
#include "../yche_refactor/local_push_yche.h"
#include <ctime>
#include <unordered_set>

using namespace std;
using namespace boost::program_options;
struct Sim_pair {
	int a, b;
	double sim;
};

bool max_comp_Sim_pair(const Sim_pair & sp1, const Sim_pair & sp2) {
	if (sp1.sim == sp2.sim && sp1.a == sp2.a)
		return sp1.b < sp2.b;
	if (sp1.sim == sp2.sim)
		return sp1.a < sp2.a;
	return sp1.sim > sp2.sim;
}



int main(int argc, char *argv[]) {
    string g_name = argv[1];
    string stopk = argv[2];
    int topk = stoi(stopk);
    double eps_min = atof(argv[3]);

    double rou = 0.9;
    double time_per_turn_upper_bound = 3600;
    double full_time_upper_bound = 10000;
    int nd;
    int adfx = 0;
    
    string path = get_edge_list_path(g_name);
    GraphYche g(path);
    double c = 0.6;
    double eps = 0.1;
    ifstream idxin;
    string idx_path = "/data/geQian/packing_gq/rbs_sim_yuetian/gt/" + g_name;
    idxin.open(idx_path.data());
    vector<int> idx_ve;
    int x;
    while(idxin >> x){
        idx_ve.emplace_back(x);
    }
    unordered_set<int> nodeseta, nodesetb;
    for(int i = 0; i < idx_ve.size()/2; ++i)
        nodeseta.insert(idx_ve[i]);
    for(int i = idx_ve.size()/2; i < idx_ve.size(); ++i)
        nodesetb.insert(idx_ve[i]);

    
    auto n = g.n;
    cout << "graph num = " << n << endl;
    cout << "nodeseta.size = " << nodeseta.size() << ", nodesetb.size = " << nodesetb.size() << endl;

    time_t now = time(nullptr);   
	char* curr_time = ctime(&now); cout << curr_time <<endl;
    auto start_time = std::chrono::high_resolution_clock::now();
#ifdef FLP
    auto lp = new Full_LocalPush(g, g_name, c, eps, n);
#else
    auto lp = new Reduced_LocalPush(g, g_name, c, eps, n);
#endif
    
    auto final_start_time = std::chrono::high_resolution_clock::now();
    double b = 2.0;
    int loop_times = 1;
    vector<Sim_pair> ve;
    while(true){
        std::cout << endl << "***tmp eps = " << eps << endl;
        auto start_time = std::chrono::high_resolution_clock::now();
        lp->local_push(g);
        ve.clear();
        int R_theta = 0; int R_candidate = 0;

        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end_time - start_time;
        std::chrono::duration<double> full_time_tmp = end_time - final_start_time;
        cout << format("computation time: %s s") % elapsed.count() << endl; // record the pre-processing time
        cout << format("push num: %s") % lp->n_push << endl;

        std::cout << "check_ topk start" << endl;
        for(auto i : nodeseta){
            auto tmp = lp->P.v[i];
            for(const auto j_pair:tmp){
                int j = j_pair.first;
                if(nodesetb.find(j) == nodesetb.end())
                    continue;
                if (j <= i)
                    continue;
                double r = j_pair.second/sqrt(2);
                if(r >= 2*eps){
                    Sim_pair sp;
                    sp.a = j; sp.b = i; sp.sim = r;
                    ve.emplace_back(sp);
                }
            }
        }
        for(auto i : nodesetb){
            auto tmp = lp->P.v[i];
            for(const auto j_pair:tmp){
                int j = j_pair.first;
                if(nodeseta.find(j) == nodeseta.end())
                    continue;
                if (j <= i)
                    continue;
                double r = j_pair.second/sqrt(2);
                if(r >= 2*eps){
                    Sim_pair sp;
                    sp.a = j; sp.b = i; sp.sim = r;
                    ve.emplace_back(sp);
                }
            }
        }
        cout << "this time larger than 2eps num = " << ve.size() << endl;
        if(ve.size() > topk){
            sort(ve.begin(), ve.end(), max_comp_Sim_pair);
            int idx_06 = 0, num_06 = 0;
            for (idx_06 = 0; idx_06 < topk; idx_06++) {
                if (ve[idx_06].sim >= 0.6 - 1e-8) {
                    num_06++;
                }
            }
            if (num_06 >= topk * rou) {
                break;
            }
            cout << "rou*k sim= " << ve[round(topk*rou) - 1].sim << ", topk sim=" << ve[topk].sim << ", eps=" << eps << endl;
            if (ve[round(topk*rou) - 1].sim - eps >= ve[topk].sim + eps) {
                break;
            }
        }

        eps /= b;
        if(ve.size() >= topk){
            if (loop_times >= (log(1 / eps_min)) / log(b)) {
                cout << "<" << rou << endl;
                cout << endl << "iter_num " << loop_times << " eps " << eps << endl;
                break;
            }
            if (elapsed.count() > time_per_turn_upper_bound){
                cout << "Stop !!! this turn time is larger than setting time per turn !!" << time_per_turn_upper_bound << endl;
                break;
            }

            if (full_time_tmp.count() > full_time_upper_bound){
                cout << "Stop !!! full time is larger than full setting time !!" << full_time_upper_bound << endl;
                cout << "full time = " << full_time_tmp.count() << endl; 
                break;
            }
        }

        ++loop_times;
        lp->r_max /= b;
        lp->epsilon = eps;
        auto reinit_start_time = std::chrono::high_resolution_clock::now();
        lp->reinit_PR(lp->r_max);
        auto reinit_end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> reinit_elapsed = reinit_end_time - reinit_start_time;
        cout << "this turn reinit time = " << reinit_elapsed.count() << endl;         
    }
    cout << endl << endl;
    auto final_end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = final_end_time - final_start_time;
    cout << format("computation time: %s s") % elapsed.count() << endl; // record the pre-processing time
    cout << format("push num: %s") % lp->n_push << endl;
    cout << format("max queue size #: %s") % (lp->n_push / lp->max_q_size) << endl;
    cout << format("mem size: %s KB") % getValue() << endl;

    cout << format("P size: %s") % lp->P.size() << endl;
    cout << format("R size: %s") % lp->R.size() << endl;
    cout << format("avg push merge #: %s") % (lp->n_push / lp->R.size()) << endl;




    string output = "../topk_result/" + g_name + ".ans" + stopk;


    
    ofstream out;
    out.open(output.data());
    assert(out.is_open());

    // sort(ve.begin(), ve.end(), )
    for(int i = 0; i < topk; ++i){
        Sim_pair sp = ve[i];
        out << sp.a << " " << sp.b << " " << sp.sim << endl;    
    }
    out.close();
    std::cout << "finish writing !" << endl;
    return 0;
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