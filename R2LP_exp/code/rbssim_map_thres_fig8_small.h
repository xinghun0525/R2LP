#ifndef RBS_THRESHOLD_FIG8_SMALL_H
#define RBS_THRESHOLD_FIG8_SMALL_H
#include <bits/stdc++.h>

#include "Graph.h"
#include "Timer.h"
#include "Random.h"

#include "robin_map.h"
#include "robin_set.h"
#include "gperftools/tcmalloc.h"
using namespace std;
using namespace tsl;
typedef long long ll;
bool maxScoreCmp(const pair<int, double>& a, const pair<int, double>& b){
    return a.second > b.second;
}

struct hash_pair { 
    template <class T1, class T2> 
    size_t operator()(const pair<T1, T2>& p) const
    { 
        auto hash1 = hash<T1>{}(p.first); 
        auto hash2 = hash<T2>{}(p.second); 
        return hash1 ^ hash2; 
    } 
}; 

struct Sim_pair{
    int a, b;
    double sim;
};

// bool sim_pair_larger(Sim_pair s1, Sim_pair s2){
//     return s1.sim > s2.sim;
// }
bool max_comp_Sim_pair(const Sim_pair & sp1, const Sim_pair & sp2){
	if(sp1.sim == sp2.sim && sp1.a == sp2.a)
		return sp1.b < sp2.b;
	if(sp1.sim == sp2.sim)
		return sp1.a < sp2.a;
	return sp1.sim > sp2.sim;
}

class Rbs{
public:
    Graph g;
    int vert_num;
    int edge_num;
    double cvalue;// prob of walk
    double sqrtc;
    double eps;
    //robin_map<int, double> rmp;
    double rsum;
    double rmax;
    vector<robin_map<int, robin_map<int, double> > > vppr;
    //vector<robin_map<int, robin_map<int, double> > >appr;
    robin_map<int, robin_map<int, double> > ppr;

    double rou; // 
    int pprtimes;
    double theta;
    double eps_term; // terminal eps
    double time_per_turn_upper_bound = 3600;
    double full_time_upper_bound = 10000;
    ll gt_num;
    Random mR;
    robin_map<int, robin_map<int, double> > ground_truth;
    unordered_set<int> ground_idx;
    robin_map<int, robin_map<int, double> > ground_topk;
    robin_set<int> nodeseta;
    robin_set<int> nodesetb;
    int topk = 1000;
    string file_name;
    Rbs(string _file_name, string path_name, int _topk, double _rou, double _eps_init = 0.1, double _eps_term=0.002,
        double _c = 0.6, double _time_per_turn_upper_bound = 3600, double _full_time_upper_bound = 10000){
        file_name = _file_name;
        rou = _rou;
        g.inputGraph(path_name);
        g.sort_node();
        topk = _topk;
        // eps_term = _eps_term;
        vert_num = g.n;
        edge_num = g.m;
        cvalue = _c;
        sqrtc = sqrt(cvalue);
        eps = _eps_init;
        pprtimes = log(vert_num) + 1; //need to make sure
        srand(unsigned(time(0)));
        mR = Random(unsigned(rand()));
        cout << "n = " << vert_num << ", m = " << edge_num << endl;
        eps_term = theta/100.0;
        time_per_turn_upper_bound = _time_per_turn_upper_bound;
        full_time_upper_bound = _full_time_upper_bound;
    }

    double Mento_Carlo(int a, int b){
        int ta = a, tb = b;
        if(a == b){
            cout << "wrong!! given singleton nodes" << endl;
            return 1.0;
        }
            
        int meet_times = 0;
       
        while(mR.drand() < cvalue){
            int idxa = mR.drand() * g.getInSize(ta);
            ta = g.getInVert(ta, idxa);
            int idxb = mR.drand() * g.getInSize(tb);
            tb = g.getInVert(tb, idxb);
            if(ta == tb){
                return 1;
            }
        }
        
        return 0;
    }

    // void get_ground_truth(string filename){
    //     if(filename == "CG" || filename == "WV" || filename == "EN" || filename == "SD" || 
    //     filename == "DB" || filename == "WF" || filename == "ND") {
    //         string path = "/data/geQian/packing_gq/SimRankRelease-master_exp/Local-Push/ground_truth/" + filename;
    //         ifstream fin;
    //         fin.open(path.data());
    //         int a, b; double sim;
    //         gt_num = 0;
    //         while(fin >> a >> b >> sim){
    //             if(a == b) continue;
    //             if(sim >= theta - 1e-6){
    //                 ground_truth[a][b] = sim;
    //                 ++gt_num;
    //             }
    //         }
    //         cout << "load gt finished, gt_pairs num = " << gt_num << endl;
    //         return ;
    //     }
    //     else{
    //         cout << "filename = " << filename << ", eps = " << eps << endl;
    //         // unordered_set<int> ground_idx;
    //         ifstream fin;
    //         string idx_path = "../ground_idx/" + filename + "/idx1";
    //         cout << "idx_path = " << idx_path << endl;
    //         fin.open(idx_path.data());
    //         assert(fin.is_open());
    //         int x;
    //         while(fin >> x){
    //             ground_idx.insert(x);
    //         }
    //         cout << "ground_idx.size = " << ground_idx.size() << endl;

    //         fin.close();
    //         // unordered_map<int, unordered_map<int, double> > gt_sim;
            
    //         int gtnum = 0;
    //         string groundtruth_path = "../ExactSim-master/results/" + filename + "/1e-06/";
    //         for(auto a: ground_idx){
    //             string gtpath = groundtruth_path + to_string(a) + ".txt";
    //                 // cout << gtpath << endl;
    //             fin.open(gtpath.data());
    //             assert(fin.is_open());
    //             int b; double sim;
    //             while(fin >> b >> sim){
    //                 if(sim < threshold - 1e-7 || a == b)
    //                     continue;
    //                 if(a > b)
    //                     gt_sim[a][b] = sim;
    //                 else
    //                     gt_sim[b][a] = sim;
    //                 ++gtnum;
    //             }
    //             fin.close();
    //         }
    //     }
    //     return;
    // }

    void get_ground_truth(string filename, int topk){
        if(filename == "CG" || filename == "WV" || filename == "EN" || filename == "SD" || 
        filename == "DB" || filename == "WF" || filename == "ND") {
            string path = "/data/geQian/packing_gq/rbs-sim_exp_yuetian/gt/" + filename + "_topk" + to_string(topk) + ".gt";
            ifstream fin;
            fin.open(path.data());
            int a, b; double sim;
            gt_num = 0;
            while(fin >> a >> b >> sim){
                if(a == b) continue;
                if (a > b)
                    ground_topk[a][b] = sim;
                else 
                    ground_topk[b][a] = sim;
                ++gt_num;
            }
            fin.close();
            cout << "load gt topk finished, gt_pairs num = " << gt_num << endl;
            if(filename != "CG" && filename != "WV"){
                string idx_path = "/data/geQian/packing_gq/rbs-sim_exp_yuetian/gt/" + filename;
                fin.open(idx_path.data());
                int a;
                vector<int> ve;
                while(fin >> a){
                    ve.emplace_back(a);
                }
                int ven = ve.size();
                for(int i = 0; i < ven/2; ++i){
                    nodeseta.insert(ve[i]);
                }
                for(int i = ven/2; i < ven; ++i){
                    nodesetb.insert(ve[i]);
                }
            }
            return ;
        }
        else{
            // cout << "filename = " << filename << ", eps = " << eps << endl;
            // // unordered_set<int> ground_idx;
            // ifstream fin;
            // string idx_path = "../ground_idx/" + filename + "/idx1";
            // cout << "idx_path = " << idx_path << endl;
            // fin.open(idx_path.data());
            // assert(fin.is_open());
            // int x;
            // while(fin >> x){
            //     ground_idx.insert(x);
            // }
            // cout << "ground_idx.size = " << ground_idx.size() << endl;

            // fin.close();
            // // unordered_map<int, unordered_map<int, double> > gt_sim;
            
            // int gtnum = 0;
            // string groundtruth_path = "../ExactSim-master/results/" + filename + "/1e-06/";
            // for(auto a: ground_idx){
            //     string gtpath = groundtruth_path + to_string(a) + ".txt";
            //         // cout << gtpath << endl;
            //     fin.open(gtpath.data());
            //     assert(fin.is_open());
            //     int b; double sim;
            //     while(fin >> b >> sim){
            //         if(sim < threshold - 1e-7 || a == b)
            //             continue;
            //         if(a > b)
            //             gt_sim[a][b] = sim;
            //         else
            //             gt_sim[b][a] = sim;
            //         ++gtnum;
            //     }
            //     fin.close();
            // }
        }
        return;
    }

    void rbspush_idx(){
        Timer fulltimer;
        ppr.clear();
        fulltimer.start();
        cout << "rbspush start" << endl;

        robin_map<int, robin_map<int, double> > appr;
        robin_map<int, robin_map<int, double> > bppr;
        vppr.push_back(appr);
        vppr.push_back(bppr);
        
        while(eps >= eps_term){
            cout << endl << "**** eps = " << eps << endl;
            vppr[0].clear(); vppr[1].clear();
            Timer turn_timer;
            turn_timer.start();
            Timer mytimer;
            double tmp_theta = eps;
            int L = log(tmp_theta)/log(cvalue);
            double bound = cvalue/(1 - cvalue)/tmp_theta;   // /10 则为divide

            double bound1 = bound*bound;
            double rd;
            double bound2;
            // while(rd == 0)
            //     rd = mR.drand();                
            // double bound2 = bound1/rd/rd;

            robin_map<int, robin_map<int, double> > &nppr = vppr[0];

            int norandnum = 0;
            int all_norandum = 0;
            mytimer.start();
            for(int v = 0; v < vert_num; ++v){ // (v,v)->(u1,u2)
                rd = 0;
                while(rd == 0)
                    rd = mR.drand();                
                bound2 = bound1/rd/rd;
                int vd = g.getOutSize(v);
                for(int i = 1; i < vd; ++i){
                    int u1 = g.getOutVert(v, i);
                    int ud1 = g.getInSize(u1);
                    int idx1 = 0;
                    for(; idx1 < i; ++idx1){
                        int u2 = g.getOutVert(v, idx1);
                        int ud2 = g.getInSize(u2);
                        int pud = ud2*ud1;
                        if(pud <= bound1){
                            if(u1 > u2)
                                nppr[u1][u2] += cvalue/pud;
                            else
                                nppr[u2][u1] += cvalue/pud;
                        }
                        else 
                            break;
                    }
                    norandnum += idx1;
                    int idx2 = idx1;
                    
                    
                    for(; idx2 < i; ++idx2){
                        int u2 = g.getOutVert(v, idx2);
                        int ud2 = g.getInSize(u2);
                        int pud = ud2*ud1;
                        if(pud <= bound2){
                            if(u1 > u2)
                                nppr[u1][u2] += (1 - cvalue)*tmp_theta/sqrt(pud);
                            else
                                nppr[u2][u1] += (1 - cvalue)*tmp_theta/sqrt(pud);
                        }
                        else
                            break;
                    }
                    
                }
            }
            int pushsize = 0;
            for(const auto&p: nppr){
                pushsize += p.second.size();
            }
            cout << "pushsize = " << pushsize << endl;
            cout << "norandom num = " << norandnum << endl;
            all_norandum += norandnum;
            ppr = nppr;
            mytimer.end();
            cout << "inital finished " << ", use time = " << mytimer.timeCost << endl;
            
            
            cout << "L = " << L << endl;
            
            for(int l = 0; l < L; ++l){
                robin_map<int, robin_map<int, double> > &tmpppr = vppr[l%2];
                robin_map<int, robin_map<int, double> > &nxtppr = vppr[1 - l%2];
                mytimer.start();
                Timer atimer;
                atimer.start();
                nxtppr.clear();
                atimer.end();
                norandnum = 0;
                int pushnum = 0;
                int pushsize = 0;
                for(const auto&p: tmpppr){
                    pushsize += p.second.size();
                }
                cout << "pushsize = " << pushsize << endl;
                cout << "**** " << l << " ****" << " pushnum = " << tmpppr.size() << endl; 
                cout << "clear hash map use time = " << atimer.timeCost << endl;
                atimer.start();
                if(tmpppr.empty())
                    break;
                for(const auto &p1: tmpppr){
                    int v1 = p1.first;
                    for(const auto &p2: p1.second){
                        int v2 = p2.first;
                        double tpi = p2.second;
                        //// rmax < eps: not push
                        //********
                        if(tpi < eps)
                            continue;
                        int vd1 = g.getOutSize(v1);
                        bound1 = bound*bound*tpi*tpi;

                        rd = 0;
                        while(rd == 0)
                        rd = mR.drand();                
                        bound2 = bound1/rd/rd;

                        for(int i = 0; i < vd1; ++i){
                            int u1 = g.getOutVert(v1, i);
                            int ud1 = g.getInSize(u1);
                            int vd2 = g.getOutSize(v2);
                            int idx1 = 0;
                            for(; idx1 < vd2; ++idx1){
                                int u2 = g.getOutVert(v2, idx1);
                                if(u1 == u2) continue; //can't meet twice.
                                int ud2 = g.getInSize(u2);
                                int pud = ud2*ud1;

                                if(pud <= bound1){
                                    if(u1 > u2)
                                        nxtppr[u1][u2] += (cvalue)*tpi/pud;
                                    else
                                        nxtppr[u2][u1] += (cvalue)*tpi/pud;
                                }
                                else 
                                    break;
                            }
                            int idx2 = idx1;
                            norandnum += idx1;
                            
                            for(; idx2 < vd2; ++idx2){
                                int u2 = g.getOutVert(v2, idx2);
                                if(u1 == u2) continue;
                                int ud2 = g.getInSize(u2);
                                int pud = ud2*ud1;
                                if(pud <= bound2){
                                    if(u1 > u2)
                                        nxtppr[u1][u2] += (1 - cvalue)*tmp_theta/sqrt(pud);
                                    else
                                        nxtppr[u2][u1] += (1 - cvalue)*tmp_theta/sqrt(pud);
                                }
                                else
                                    break;
                            }   
                        } 

                    }
                }
                cout << "norandom num = " << norandnum << endl;
                all_norandum += norandnum;

                cout << "cycle finished \n";
                atimer.end();
                cout << "cycle use time = " << atimer.timeCost << ", " ;
                atimer.start();
                for(const auto& p1 : nxtppr){
                    int u1 = p1.first;
                    for(const auto& p2: p1.second){
                        ppr[u1][p2.first] += p2.second;
                    }
                }
                atimer.end();
                cout << "ppr inc use time = " << atimer.timeCost << ", ";
                mytimer.end();
                cout << "all use time = " << mytimer.timeCost << endl;
            }
            cout << "allnorandom num = " << all_norandum << endl;
            
            turn_timer.end();
            cout << endl << "eps = " << eps << ", this turn finished " << ", use time = " << turn_timer.timeCost << endl;
            // if (turn_timer.timeCost > time_per_turn_upper_bound){
            //     cout << "Stop !!! this turn time is larger than setting time " << time_per_turn_upper_bound << endl; 
            //     break;
            // }
            
            Timer tmp_timer;
            tmp_timer.t_start = fulltimer.t_start;
            tmp_timer.end();
            // if (tmp_timer.timeCost > full_time_upper_bound){
            //     cout << "Stop !!! full time is larger than full setting time !!" << full_time_upper_bound << endl; 
            //     break;
            // }

            if(check_finish_medieum(eps))
                break;
            eps /= 2;
            if(eps<eps_term)
                break;
            vppr[0].clear();
            vppr[1].clear();
            ppr.clear();
            //appr.push_back(ppr);
        }
        
    }
    
    bool check_finish_medieum(double tmp_eps){
        vector<Sim_pair> ve;
        ve.clear();
        cout << "file name = " << file_name << endl;
        if(file_name == "CG" || file_name == "WV"){
            for(auto i: ppr){
                for(auto j: i.second){
                    if(j.second > 0){
                        Sim_pair sp; sp.a = i.first; sp.b = j.first;
                        sp.sim = j.second;
                        ve.emplace_back(sp);
                    }
                }
            }
        }
        else{
            for (auto u1 : nodeseta) {
                if (ppr.find(u1) == ppr.end())
                    continue;
                for (auto p2 : ppr[u1]) {
                    int u2 = p2.first;
                    if (nodesetb.find(p2.first) != nodesetb.end()) {
                        Sim_pair sp;
                        sp.a = u1; sp.b = u2; sp.sim = p2.second;
                        ve.emplace_back(sp);
                    }
                }
            }
            for (auto u1 : nodesetb) {
                if (ppr.find(u1) == ppr.end())
                    continue;
                for (auto p2 : ppr[u1]) {
                    int u2 = p2.first;
                    if (nodeseta.find(p2.first) != nodeseta.end()) {
                        Sim_pair sp;
                        sp.a = u1; sp.b = u2; sp.sim = p2.second;
                        ve.emplace_back(sp);
                    }
                }
            }
        }
        sort(ve.begin(), ve.end(), max_comp_Sim_pair);
        int ven = ve.size();
        int min_topk = min(topk, ven);
        if(min_topk/(topk+0.0) < rou){
            cout << "few number of true num " << min_topk << endl;
            return false;
        }

        int true_num = 0;
        for(int i = 0; i < min_topk; ++i){
            Sim_pair sp = ve[i];
            if((ground_topk.find(sp.a) != ground_topk.end() && ground_topk[sp.a].find(sp.b) != ground_topk[sp.a].end())||
            (ground_topk.find(sp.b) != ground_topk.end() && ground_topk[sp.b].find(sp.a) != ground_topk[sp.b].end()))
                ++true_num;
        }
        cout << "true num = " << true_num << ", topk = " << topk << endl;
        if(true_num / (topk + 0.0) >= rou)
            return true;
        return false;
    }


    
};

#endif