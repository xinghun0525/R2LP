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
    double threshold = atof(argv[2]);
    double rou = atof(argv[3]);
    // double eps_t = 1e-4;
    double eps_t = threshold/100.0;
    double time_per_turn_upper_bound = 3600;
    double full_time_upper_bound = 10000;
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
    double eps = threshold/2;
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
    auto final_start_time = std::chrono::high_resolution_clock::now();
    while(eps >= eps_t){
        cout << endl << "***tmp eps = " << eps << endl;
        auto start_time = std::chrono::high_resolution_clock::now();
        lp->local_push(g);
        int R_theta = 0; int R_candidate = 0;


        cout << "check_ candidate start" << endl;
        for(int i = 0; i < lp->P.v.size(); ++i){
            auto tmp = lp->P.v[i];
            // cout << "i = " << i << endl;
            for(const auto j_pair:tmp){
                int j = j_pair.first;
                if (j <= i)
                    continue;
                double r = j_pair.second/sqrt(2);
                if (r >= threshold + eps)
                    ++R_theta;
                else if(r >= threshold - eps)
                    ++R_candidate;
                
                // cout << "j = " << j << ", r = " << r << endl;
                // if (fabs(r) > r_max) {
                //     NodePair pab(i,j);
                //     auto &is_in_q_flag_ref = marker[pab];
                //     if (!is_in_q_flag_ref) {
                //         Q.push(pab);
                //         is_in_q_flag_ref = true;
                //     }
                // }
            }
        }


        // for(const auto& i : veidx){
        //     for(int j = 0; j < i; ++j){
        //         double res = lp->query_P(i,j);
        //         if (res >= threshold - 1e-8)
        //             ++R_theta;
        //         else if(res > threshold - eps)
        //             ++R_candidate;
        //     }
        // }
        cout << "num of larger than threshold = " << R_theta << ", " 
            << "num of candidate = " << R_candidate << endl;
        
        
        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end_time - start_time;
        std::chrono::duration<double> full_time_tmp = end_time - final_start_time;
        cout << format("computation time: %s s") % elapsed.count() << endl; // record the pre-processing time
        
        if (elapsed.count() > time_per_turn_upper_bound){
            cout << "Stop !!! this turn time is larger than setting time per turn !!" << time_per_turn_upper_bound << endl;
            break;
        }
        
        if (full_time_tmp.count() > full_time_upper_bound){
            cout << "Stop !!! full time is larger than full setting time !!" << full_time_upper_bound << endl;
            cout << "full time = " << full_time_tmp.count() << endl; 
            break;
        }
        

        cout << format("push num: %s") % lp->n_push << endl;
        cout << format("max queue size #: %s") % (lp->n_push / lp->max_q_size) << endl;
        cout << format("mem size: %s KB") % getValue() << endl;

        cout << format("P size: %s") % lp->P.size() << endl;
        cout << format("R size: %s") % lp->R.size() << endl;
        cout << format("avg push merge #: %s") % (lp->n_push / lp->R.size()) << endl;

        

        if(R_theta > 0 && R_theta/(R_theta + R_candidate+0.0) >= rou)
            break;
        eps /= 2;
        lp->r_max /= 2;
        if(eps < eps_t)
            break;
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
    

    string output = "../thres_result/" + g_name + ".ans_t" + to_string(threshold) + "_r" + to_string(rou);


    
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
    // for(const auto& i : veidx){
    //     for(int j = 0; j < i; ++j){
    //         // if(i == j)
    //         //     continue;
    //         double res = lp->query_P(i,j);
    //         if(res >= threshold*0.9){
    //             out << i << " " << j << " " << res << endl;
    //         }
    //     }
    // }
    sort(veidx.begin(), veidx.end(), greater<int>());
    for(int i = 0; i < lp->P.v.size(); ++i){
        if (veidx_set.find(i) != veidx_set.end()){
			for(auto p2: lp->P.v[i]){
				if(p2.second/sqrt(2) >= threshold*0.99 && p2.first != i)
					out << i << " " << p2.first << " " << p2.second/sqrt(2) << endl;
			}
			continue;
		}
        auto tmp = lp->P.v[i];

		for(auto j: veidx){
			if(j <= i)
				break;
			if(tmp.find(j) != tmp.end() && tmp[j]/sqrt(2) >= threshold*0.99)
				out << i << " " << j << " " << tmp[j]/sqrt(2) << endl;
		}
        // cout << "i = " << i << endl;
        // for(const auto j_pair:tmp){
        //     int j = j_pair.first;
        //     if (j <= i)
        //         continue;
        //     double r = j_pair.second/sqrt(2);
        //     if (r >= threshold*0.99)
        //         out << i << " " << j << " " << r << endl;
            
        // }
    }

    // for(const auto &p1 : myrbs.ppr){
	// 	int u1 = p1.first;
	// 	if (ground_idx_set.find(u1) != ground_idx_set.end()){
	// 		for(auto p2: p1.second){
	// 			if(p2.second >= threshold*0.99)
	// 				fout << u1 << " " << p2.first << " " << p2.second << endl;
	// 		}
	// 		continue;
	// 	}

	// 	auto p2 = p1.second;
	// 	for(auto u2: ground_idx){
	// 		if(u2 >= u1)
	// 			break;
	// 		if(p2.find(u2) != p2.end() && p2[u2] >= threshold*0.99)
	// 			fout << u1 << " " << u2 << " " << p2[u2] << endl;
	// 	}
	// }

    out.close();
    now = time(nullptr);   
	curr_time = ctime(&now); cout << curr_time <<endl;
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