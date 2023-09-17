#ifndef RBS_THRESHOLD_FIG7_H
#define RBS_THRESHOLD_FIG7_H
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

    Rbs(string file_name, double _theta, double _rou, double _eps_init = 0.1, double _eps_term=0.002,
        double _c = 0.6, double _time_per_turn_upper_bound = 3600, double _full_time_upper_bound = 10000){
        rou = _rou;
        g.inputGraph(file_name);
        g.sort_node();
        theta = _theta;
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

    void get_ground_truth(string filename){
        if(filename == "CG" || filename == "WV" || filename == "EN" || filename == "SD" || 
        filename == "DB" || filename == "WF" || filename == "ND") {
            string path = "/data/geQian/packing_gq/SimRankRelease-master_exp/Local-Push/ground_truth/" + filename;
            ifstream fin;
            fin.open(path.data());
            int a, b; double sim;
            gt_num = 0;
            while(fin >> a >> b >> sim){
                if(a == b) continue;
                if(sim >= theta - 1e-6){
                    ground_truth[a][b] = sim;
                    ++gt_num;
                }
            }
            cout << "load gt finished, gt_pairs num = " << gt_num << endl;
            return ;
        }
        else{
            cout << "filename = " << filename << ", eps = " << eps << endl;
            // unordered_set<int> ground_idx;
            ifstream fin;
            string idx_path = "/data/geQian/packing_gq/ground_idx/" + filename + "/idx1";
            cout << "idx_path = " << idx_path << endl;
            fin.open(idx_path.data());
            assert(fin.is_open());
            int x;
            while(fin >> x){
                ground_idx.insert(x);
            }
            cout << "ground_idx.size = " << ground_idx.size() << endl;

            fin.close();
            // unordered_map<int, unordered_map<int, double> > ground_truth;
            
            // int gtnum = 0;
            gt_num = 0;
            string groundtruth_path = "/data/geQian/packing_gq/ExactSim-master/results/" + filename + "/1e-06/";
            for(auto a: ground_idx){
                string gtpath = groundtruth_path + to_string(a) + ".txt";
                cout << gtpath << endl;
                fin.open(gtpath.data());
                assert(fin.is_open());
                int b; double sim;
                while(fin >> b >> sim){
                    if(sim < theta - 1e-7 || a == b)
                        continue;
                    if(a > b)
                        ground_truth[a][b] = sim;
                    else
                        ground_truth[b][a] = sim;
                    ++gt_num;
                }
                fin.close();
            }
        }
        cout << "load gt finished, gt_pairs num = " << gt_num << endl;
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

            if(check_finish(eps))
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
    
    

    bool check_finish(double tmp_eps){
        
        ll R_theta = 0; int R_candidate = 0;
        for(const auto &p1 : ppr){
            int u1 = p1.first;
            if (ground_idx.find(u1) != ground_idx.end()){
                for(auto p2: p1.second){
                    if(p2.second >= theta - 1e-7){
                        if(ground_truth.find(u1) != ground_truth.end() && ground_truth[u1].find(p2.first) != ground_truth[u1].end())
                            ++R_theta;
                    }
                        
                }
                continue;
            }

            auto p2 = p1.second;
            for(auto u2: ground_idx){
                if(p2.find(u2) != p2.end() && p2[u2] >= theta - 1e-7){
                    if(ground_truth.find(u1) != ground_truth.end() && ground_truth[u1].find(u2) != ground_truth[u1].end())
                        ++R_theta;
                }
            }
        }

        // for(const auto& p1 : ppr){
        //     int u1 = p1.first;
        //     for(const auto& p2: p1.second){
        //         if(p2.first == u1)
        //             continue;
        //         if(p2.second >= theta){
        //             if(ground_truth.find(u1) != ground_truth.end() && ground_truth[u1].find(p2.first) != ground_truth[u1].end())
        //                 ++R_theta;
        //         }
        //     }
        // }
        
        cout << "num of larger than threshold = " << R_theta << ", " << "accuracy ratio = " << (R_theta+0.0)/gt_num << endl;

        if(R_theta > 0 && (R_theta+0.0)/gt_num >= rou)
            return true;
        return false;
    }

    double fora(int a, int b, double eps){
        cout << "fora start a=" << a << ", b = " << b << ", eps = " << eps << endl;
        double rmax = eps;
        Timer mytimer;
        mytimer.start();
        // vector<robin_map<int, robin_set<int>> > residue_idx_vec;
        robin_map<int, robin_set<int>> tmp_residue_idx;
        robin_map<int, robin_map<int, double> > residue;
        // robin_map<int, int> next_residue_idx;
        // robin_map<int, int> next_residue_idx;
        // residue_idx_vec.emplace_back(residue);
        // residue_idx_vec.emplace_back(residue);
        if(a == b) return 1.0;
        if(a < b){
            int c = a; a = b; b = c;
        }
        residue[a][b] = 1;
        tmp_residue_idx[a].insert(b);
        double ans = 0.0;
        // robin_map<int, robin_set<int> > &tmp_residue_idx = residue_idx_vec[0];
        // robin_map<int, robin_set<int> > &nxt_residue_idx = residue_idx_vec[1];
        bool finished = false;
        while(true){
            // tmp_residue_idx = residue_idx_vec[i%2];
            // nxt_residue_idx = residue_idx_vec[1-i%2];
            tmp_residue_idx.clear();
            for(auto i: residue){
                int u = i.first;
                int du = g.getInSize(u);
                if(du == 0) continue;
                for(auto j: i.second){
                    int v = j.first;
                    double res = j.second;
                    int dv = g.getInSize(v);
                    if(dv == 0) continue;
                    if(res/du/dv > rmax)
                        tmp_residue_idx[u].insert(v);

                }
            }
            cout << "residue_idx.size = " << tmp_residue_idx.size() << endl;
            if(tmp_residue_idx.size() == 0)
                break;
            for(const auto& x: tmp_residue_idx){
                int u = x.first;
                int du = g.getInSize(u);
                for(int j = 0; j < du; ++j){
                    int nextu = g.getInVert(u, j);
                    for(const auto&v: x.second){
                        double tmpres = residue[u][v];
                        int dv = g.getInSize(v);   
                        residue[u][v] = 0;
                        finished = false;
                        for(int k = 0; k < dv; ++k){
                            int nextv = g.getInVert(v, k);
                            if(nextu == nextv){
                                ans += tmpres*cvalue/du/dv;
                                continue;
                            }
                            if(nextu < nextv){
                                int tmpu = nextu; nextu = nextv; nextv = tmpu;
                            }
                            residue[nextu][nextv] += tmpres*cvalue/du/dv;
                            
                        }
                    }
                }
            }
            
            
        }
        mytimer.end();
        cout << "push use time = " << mytimer.timeCost << endl;
        double rsum = 0;
        for(const auto& i: residue){
            for(const auto& j: i.second){
                rsum += j.second;
            }
        }
        int full_random_walks = rsum/eps/eps*(2*eps/3+2)*log(2e4); // delta = 1 ???
        cout << "rsum = " << rsum << endl;
        cout << "full_random_walks = " << full_random_walks << endl;

        for(const auto& i: residue){
            int u = i.first;
            // int v = 
            for(const auto& j: i.second){
                int v = j.first;
                double res = j.second;
                int sample_times = res/rsum*full_random_walks;
                double meet_times = 0;
                for(int k = 0; k < sample_times; ++k){
                    meet_times += Mento_Carlo(u, v);
                }
                ans += res*meet_times/sample_times;
            }
        }
        return ans;

    }
    

    double fora_pure_push(int a, int b, double eps_min){
        cout << "fora start a=" << a << ", b = " << b << ", eps = " << eps << endl;
        double rmax = eps_min;
        Timer mytimer;
        mytimer.start();
        // vector<robin_map<int, robin_set<int>> > residue_idx_vec;
        robin_map<int, robin_set<int>> tmp_residue_idx;
        robin_map<int, robin_map<int, double> > residue;
        // robin_map<int, int> next_residue_idx;
        // robin_map<int, int> next_residue_idx;
        // residue_idx_vec.emplace_back(residue);
        // residue_idx_vec.emplace_back(residue);
        if(a == b) return 1.0;
        if(a < b){
            int c = a; a = b; b = c;
        }
        residue[a][b] = 1;
        tmp_residue_idx[a].insert(b);
        double ans = 0.0;
        // robin_map<int, robin_set<int> > &tmp_residue_idx = residue_idx_vec[0];
        // robin_map<int, robin_set<int> > &nxt_residue_idx = residue_idx_vec[1];
        bool finished = false;
        double r_sum = 1;
        while(r_sum > eps_min){
            // tmp_residue_idx = residue_idx_vec[i%2];
            // nxt_residue_idx = residue_idx_vec[1-i%2];
            tmp_residue_idx.clear();
            for(auto i: residue){
                int u = i.first;
                int du = g.getInSize(u);
                if(du == 0) continue;
                for(auto j: i.second){
                    int v = j.first;
                    double res = j.second;
                    int dv = g.getInSize(v);
                    if(dv == 0) continue;
                    if(res > rmax)
                        tmp_residue_idx[u].insert(v);
                }
            }
            cout << "residue_idx.size = " << tmp_residue_idx.size() << endl;
            if(tmp_residue_idx.size() == 0)
                break;
            for(const auto& x: tmp_residue_idx){
                int u = x.first;
                int du = g.getInSize(u);
                for(int j = 0; j < du; ++j){
                    int nextu = g.getInVert(u, j);
                    for(const auto&v: x.second){
                        double tmpres = residue[u][v];
                        int dv = g.getInSize(v);   
                        residue[u][v] = 0;
                        finished = false;
                        for(int k = 0; k < dv; ++k){
                            int nextv = g.getInVert(v, k);
                            if(nextu == nextv){
                                ans += tmpres*cvalue/du/dv;
                                continue;
                            }
                            if(nextu < nextv){
                                int tmpu = nextu; nextu = nextv; nextv = tmpu;
                            }
                            residue[nextu][nextv] += tmpres*cvalue/du/dv;
                        }
                    }
                }
            }
            
            
        }
        mytimer.end();
        cout << "push use time = " << mytimer.timeCost << endl;
        double rsum = 0;
        for(const auto& i: residue){
            for(const auto& j: i.second){
                rsum += j.second;
            }
        }
        int full_random_walks = rsum/eps/eps*(2*eps/3+2)*log(2e4); // delta = 1 ???
        cout << "rsum = " << rsum << endl;
        cout << "full_random_walks = " << full_random_walks << endl;

        // for(const auto& i: residue){
        //     int u = i.first;
        //     // int v = 
        //     for(const auto& j: i.second){
        //         int v = j.first;
        //         double res = j.second;
        //         int sample_times = res/rsum*full_random_walks;
        //         double meet_times = 0;
        //         for(int k = 0; k < sample_times; ++k){
        //             meet_times += Mento_Carlo(u, v);
        //         }
        //         ans += res*meet_times/sample_times;
        //     }
        // }
        return ans;

    }
    
};

#endif