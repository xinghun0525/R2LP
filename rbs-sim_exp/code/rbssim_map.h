#ifndef RBS_H
#define RBS_H

#include <bits/stdc++.h>

#include "Graph.h"
#include "Timer.h"
#include "Random.h"

#include "robin_map.h"
#include "robin_set.h"
#include "gperftools/tcmalloc.h"
using namespace std;
using namespace tsl;


// template <typename T>
// inline T const& Max (T const& a, T const& b) 
// { 
//     return a < b ? b:a; 
// } 

bool maxScoreCmp(const pair<int, double>& a, const pair<int, double>& b){
    return a.second > b.second;
}


// bool minScoreCmp(const pair<int, double>& a, const pair<int, double>& b){
//     return a.second < b.second;
// }

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
    vector<Sim_pair> final_topk;
    int pprtimes;
    Random mR;
    double rho = 0.8;
    int sample_times = 1000;

    Rbs(string file_name, int _sample_times=10000, double _rho=0.8, double _c=0.6){
        g.inputGraph(file_name);
        g.sort_node();
        vert_num = g.n;
        edge_num = g.m;
        cvalue = _c;
        sample_times = _sample_times;
        rho = _rho;
        sqrtc = sqrt(cvalue);
        // eps = _eps;
        pprtimes = log(vert_num) + 1; //need to make sure
        srand(unsigned(time(0)));
        mR = Random(unsigned(rand()));
        cout << "n = " << vert_num << ", m = " << edge_num << endl;
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

    void rbspush_idx(double eps){
        ppr.clear();
        cout << "rbspush start" << endl;
        
        //robin_map<int, robin_map<int, double> > ppr;
        vppr.clear();
        robin_map<int, robin_map<int, double> > appr;
        robin_map<int, robin_map<int, double> > bppr;
        vppr.push_back(appr);
        vppr.push_back(bppr);
        
        // double rate = 0.1;
        double eps_prune = eps; // forget to init rate at first, log in rbslog_random_rate;
        
        Timer mytime;
        double theta = eps;
        // double theta = eps * (1 - rate);
        int L = log(theta)/log(cvalue);
        // double bound = cvalue/(1 - cvalue)/theta;   // /10 则为divide
        double bound = cvalue/(1 - cvalue)/theta;   // check why thres=0.25 accuracy not get higher 

        double bound1 = bound*bound;
        double rd;
        double bound2;

        robin_map<int, robin_map<int, double> > &nppr = vppr[0];

        int norandnum = 0;
        int all_norandum = 0;
        mytime.start();
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
                            nppr[u1][u2] += (1 - cvalue)*theta/sqrt(pud);
                        else
                            nppr[u2][u1] += (1 - cvalue)*theta/sqrt(pud);
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
        mytime.end();
        cout << "inital finished " << ", use time = " << mytime.timeCost << endl;
        
        
        cout << "L = " << L << endl;
        
        for(int l = 0; l < L; ++l){
            robin_map<int, robin_map<int, double> > &tmpppr = vppr[l%2];
            robin_map<int, robin_map<int, double> > &nxtppr = vppr[1 - l%2];
            mytime.start();
            Timer atime;
            atime.start();
            nxtppr.clear();
            atime.end();
            norandnum = 0;
            int pushnum = 0;
            int pushsize = 0;
            for(const auto&p: tmpppr){
                pushsize += p.second.size();
            }
            cout << "pushsize = " << pushsize << endl;
            cout << "**** " << l << " ****" << " pushnum = " << tmpppr.size() << endl; 
            cout << "clear hash map use time = " << atime.timeCost << endl;
            atime.start();
            if(tmpppr.empty())
                break;
            for(const auto &p1: tmpppr){
                int v1 = p1.first;
                for(const auto &p2: p1.second){
                    int v2 = p2.first;
                    double tpi = p2.second;
                    //// rmax < eps: not push
                    //********
                    if(tpi < eps_prune)
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
                            // if(u1 == v2 && u2 == v1){
                            //     if(u1 > u2)
                            //         ppr[u1][u2] += cvalue*tpi/(pud-cvalue);
                            //     else
                            //         ppr[u2][u1] += cvalue*tpi/(pud-cvalue);
                            //     continue;
                            // }
                            if(pud <= bound2){
                                if(u1 > u2)
                                    nxtppr[u1][u2] += (1 - cvalue)*theta/sqrt(pud);
                                else
                                    nxtppr[u2][u1] += (1 - cvalue)*theta/sqrt(pud);
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
            atime.end();
            cout << "cycle use time = " << atime.timeCost << ", " ;
            atime.start();
            for(const auto& p1 : nxtppr){
                int u1 = p1.first;
                for(const auto& p2: p1.second){
                    ppr[u1][p2.first] += p2.second;
                }
            }
            atime.end();
            cout << "ppr inc use time = " << atime.timeCost << ", ";
            mytime.end();
            cout << "all use time = " << mytime.timeCost << endl;
        }
        cout << "allnorandom num = " << all_norandum << endl;
        //appr.push_back(ppr);
        
    }
    

    vector<Sim_pair> MAB(const vector<Sim_pair>& candidates, int topk, double eps_min, int sample_num=1000){
        cout << "MAB start !! topk = " << topk << ", eps_min = "<< eps_min << ", sample_num = " << sample_num << endl;
        cout << "candidates num = " << candidates.size() << endl;
        // candidates [a,b,sim] pair const  idx
        int cand_num = candidates.size();
        // vector<double> UB(cand_num, 0);
        robin_map<int, double> UB, LB;
        vector<pair<int, double> > vecUB, vecLB;
        vector<pair<int, double> > vecB;
        // vector<double> tmp_sim(cand_num, 0);
        robin_map<int,int> cntMABSample;
        robin_map<int, double> sim;
        // robin_set<int> Vk;
        // vector<double> B(cand_num, 0); 
        int turns = 0;
        double fail_prob = 1e-3;
        for(int i = 0; i < cand_num; ++i){
            UB[i] = 1.1;
            LB[i] = -1.1;
        }
        while(true){
            vecB.clear(); vecLB.clear(); vecUB.clear();
            cout << "****turns = " << turns << " ***** " << endl;
            ++turns;
            if(turns > 100)
                cout << "turns> 100 " << endl;
            for(int i = 0; i < cand_num; ++i)
                vecUB.push_back(make_pair(i, UB[i]));
            sort(vecUB.begin(), vecUB.end(), maxScoreCmp);
            for(int i = 0; i < topk; ++i){
                int v = vecUB[i].first;
                vecB.push_back(make_pair(v, vecUB[topk].second - LB[v]));
            }
            for(int i = topk; i < cand_num; ++i){
                int v = vecUB[i].first;
                vecB.push_back(make_pair(v, vecUB[topk-1].second - LB[v]));
            }
            sort(vecB.begin(), vecB.end(), minScoreCmp);
            robin_set<int> Vk;
            for(int i = 0; i < topk; ++i)
                Vk.insert(vecB[i].first);
            cout << "topk*rho = " << topk*rho << ", vecB[topk*rho] = " << vecB[int(topk*rho)].second << endl;
            if(vecB[int(topk*rho)].second <= eps_min){
                vector<Sim_pair> ans;
                for(auto i : Vk){
                    Sim_pair sp;
                    sp.a = candidates[i].a; sp.b = candidates[i].b;
                    sp.sim = sim[i];
                    ans.push_back(sp);
                }
                return ans;
            }
            for(auto i: LB)
                vecLB.push_back(make_pair(i.first, i.second));
            
            sort(vecLB.begin(), vecLB.end(), minScoreCmp);

            int sample_h = 0;
            // int sp_num = min(sample_num+topk, vecLB.size());
            for(int i = 0; i < cand_num; ++i){
                int idx = vecLB[i].first; 
                // if(Vk.find(idx) != Vk.end()){ 
                    ++sample_h;  
                    int a = candidates[idx].a; int b = candidates[idx].b;
                    double tmp_sum = 0;
                    for(int times = 0; times < sample_times; ++times){
                        tmp_sum += Mento_Carlo(a, b);
                    }
                    sim[idx] = (sim[idx]*cntMABSample[idx] + tmp_sum)/(cntMABSample[idx]+sample_times);
                    cntMABSample[idx] += sample_times;
                    double std_var = sqrt(sim[idx]*(1-sim[idx]));
                    double tmp_val = log(cand_num*pow(turns,3.0)/fail_prob);
                    double beta = std_var*sqrt(tmp_val/cntMABSample[idx])+7.0/6*tmp_val/(cntMABSample[idx]-1);
                    UB[idx] = sim[idx] + beta;
                    LB[idx] = sim[idx] - beta;
                // }
                if(sample_h >= sample_num)
                    break;
            }

            int sample_l = 0;
            for(int i = 0; i < cand_num; ++i){
                int idx = vecUB[i].first; 
                // if(Vk.find(idx) == Vk.end()){
                    ++sample_l;  
                    int a = candidates[idx].a; int b = candidates[idx].b;
                    double tmp_sum = 0;
                    for(int times = 0; times < sample_times; ++times){
                        tmp_sum += Mento_Carlo(a, b);
                    }
                    sim[idx] = (sim[idx]*cntMABSample[idx] + tmp_sum)/(cntMABSample[idx]+sample_times);
                    cntMABSample[idx] += sample_times;
                    double std_var = sqrt(sim[idx]*(1-sim[idx]));
                    double tmp_val = log(cand_num*pow(turns,3.0)/fail_prob);
                    double beta = std_var*sqrt(tmp_val/cntMABSample[idx])+7.0/6*tmp_val/(cntMABSample[idx]-1);
                    LB[idx] = sim[idx] - beta;
                    UB[idx] = sim[idx] + beta;
                // }
                if(sample_l >= sample_num)
                    break;
            }

        }

        
    
    }

    vector<Sim_pair> get_topk_basic(int topk, double eps_init){
        // rbspush_idx(eps_init);
        double eps = eps_init;
        // double theta = theta_init;
        robin_map<int, robin_set<int> > candidate;
        // robin_map<int, robin_set<int> > R;
        int Rnum = 0;
        while(true){
            rbspush_idx(eps);
            vector<Sim_pair> ve;  
            for(const auto &p1 : ppr){
                int u1 = p1.first;
                for(auto p2: p1.second){
                    if(p2.second - eps >= eps){
                        Sim_pair sp;
                        sp.a = u1; sp.b = p2.first; sp.sim = p2.second;
                        ve.push_back(sp);
                    }
                }
            }
            if(ve.size() > topk){
                sort(ve.begin(), ve.end(), max_comp_Sim_pair);
                double prune_bound = ve[topk - 1].sim - 2*eps;
                if(prune_bound > 0){
                    int stop_idx = topk;
                    for(; stop_idx < ve.size(); ++stop_idx){
                        if(ve[stop_idx].sim < prune_bound)
                            break;
                    }

                    sort(ve.begin(), ve.end(), max_comp_Sim_pair);
                    int idx_06 = 0;

                    vector<Sim_pair> Final_ans;
                    for(idx_06 = 0; idx_06 < stop_idx; ++idx_06){
                        if(ve[idx_06].sim >= 0.6 - 1e-8){
                            Final_ans.push_back(ve[idx_06]);
                        }
                    }
                    if(Final_ans.size() >= topk){
                        return Final_ans;
                    }
                    topk = topk - Final_ans.size();
                    vector<Sim_pair> ans(ve.begin() + Final_ans.size(), ve.begin() + stop_idx);
                    cout << "new topk = " << topk << ", ans.size() = " << ans.size() << endl;
                    vector<Sim_pair> Final_ans1 = MAB(ans, topk, eps);
                    Final_ans.insert(Final_ans.end(), Final_ans1.begin(), Final_ans1.end());
                    return Final_ans;
                }
            }  
            eps *= 0.9;
        }   
    }
    

    vector<Sim_pair> get_topk(int topk, double eps_init = 0.1, double topk_theta_init = 0.15){
        // rbspush_idx(eps_init);
        
        double eps = eps_init;
        double theta = topk_theta_init;
        // double theta = theta_init;
        robin_map<int, robin_set<int> > candidate;
        // robin_map<int, robin_set<int> > R;
        int Rnum = 0;
        vector<robin_map<int, robin_set<int> > > R, C;
        vector<robin_map<int, robin_set<int> > > R_, C_; // R_[i] = R[0]+...+R[i] 
        robin_map<int, robin_set<int> > pair_06; // store the pair of 0.6;
        int turn = 0;
        double decrease_ratio = 1.1;
        int all_topk = topk;
        while(true){
            cout << "***turn " << turn << " **** starts" << endl;
            cout << "eps = " << eps << ", theta = " << theta << endl;
            robin_map<int, robin_set<int> > tmpR, tmpC;
            rbspush_idx(eps);
            vector<Sim_pair> Final_ans;
            int num_06 = 0;
            for(const auto &p1 : ppr){
                int u1 = p1.first;
                
                for(auto p2: p1.second){
                    
                    if(p2.second >= 0.6 - 1e-7){
                        ++num_06;
                        pair_06[u1].insert(p2.first);
                        Sim_pair sp; sp.a = u1; sp.b = p2.first; sp.sim = p2.second;
                        Final_ans.push_back(sp);
                        continue;
                    }
                    if(p2.second > theta + eps){
                        tmpR[u1].insert(p2.first);
                    }
                    else if(p2.second >= theta - eps){
                        tmpC[u1].insert(p2.first);
                       
                    }
                    
                }
                
            }

            if(num_06 >= all_topk)
                return Final_ans;
            topk = all_topk - num_06;
            if(turn == 0){
                int R_num = 0;
                for(auto i: tmpR){
                    R_num += i.second.size();
                }
                if(R_num >= topk){
                    vector<Sim_pair> intoMAB;
                    for(auto i: tmpR){
                        for(auto j: i.second){
                            Sim_pair sp;
                            sp.a = i.first; sp.b = j; sp.sim = 0;
                            intoMAB.push_back(sp);
                        }
                    }
                    for(auto i: tmpC){
                        for(auto j: i.second){
                            Sim_pair sp;
                            sp.a = i.first; sp.b = j; sp.sim = 0;
                            intoMAB.push_back(sp);
                        }
                    }
                    vector<Sim_pair> Final_ans1 = MAB(intoMAB, topk, eps);
                    Final_ans.insert(Final_ans.end(), Final_ans1.begin(), Final_ans1.end());
                    return Final_ans;
                }
            }
            if(turn > 0){
                robin_map<int, robin_set<int> > R_C_ = C_[turn-1]; // line 5 in algo3
                for(auto i: R_[turn - 1]){
                    for(auto j: i.second){
                        R_C_[i.first].insert(j);
                    }
                }
                robin_map<int, robin_set<int> > R_C_R = R_C_;
                for(auto i: tmpR){
                    for(auto j: i.second){
                        R_C_R[i.first].insert(j);
                    }
                }
                int num_R_C_R = 0;
                for(auto i: R_C_R)
                    num_R_C_R += i.second.size();
                if(num_R_C_R >= topk){
                    int num_R_C = 0;
                    for(auto i: R_C_){
                        num_R_C += i.second.size();
                    }
                    if(num_R_C < topk){
                        robin_map<int, robin_set<int> > ground_R = R_[turn - 1];
                        if(turn > 1){
                            for(auto i: C_[turn-2]){
                                for(auto j: i.second){
                                    ground_R[i.first].insert(j);
                                }
                            }
                        }

                        for(auto i : ground_R){
                            for(auto j: i.second){
                                Sim_pair sp; sp.a = i.first; sp.b = j; sp.sim = ppr[i.first][j];
                                Final_ans.push_back(sp);
                            }
                        }
                        for(auto i: ground_R){
                            for(auto j: i.second){
                                Sim_pair sp; sp.a = i.first; sp.b = j; sp.sim = ppr[i.first][j];
                                Final_ans.push_back(sp);
                            }
                        }
                        robin_map<int, robin_set<int> > ground_C = C[turn-1];
                        for(auto i: tmpR){
                            for(auto j: i.second){
                                ground_C[i.first].insert(j);
                            }
                        }
                        for(auto i: tmpC){
                            for(auto j: i.second){
                                ground_C[i.first].insert(j);
                            }
                        }
                        vector<Sim_pair> intoMAB;
                        for(auto i: ground_C){
                            if(ground_R.find(i.first) == ground_R.end()){
                                for(auto j: i.second){
                                    Sim_pair sp; sp.a = i.first; sp.b = j; sp.sim = 0.0;
                                    intoMAB.push_back(sp);
                                    // intoMAB[i.first].insert(i.second.begin(), i.second.end());
                                }
                                continue;
                            }
                            for(auto j:i.second){
                                if(ground_R[i.first].find(j) == ground_R[i.first].end()){
                                    Sim_pair sp; sp.a = i.first; sp.b = j; sp.sim = 0.0;
                                    intoMAB.push_back(sp);
                                }
                            }
                        }
                        vector<Sim_pair> Final_ans1 = MAB(intoMAB, topk, eps);
                        Final_ans.insert(Final_ans.end(), Final_ans1.begin(), Final_ans1.end());
                    }
                    
                    else{
                        robin_map<int, robin_set<int> > ground_R;
                        if(turn > 1){
                            for(auto i: C_[turn-2]){
                                for(auto j: i.second){
                                    ground_R[i.first].insert(j);
                                }
                            }
                            for(auto i: R_[turn-2]){
                                for(auto j: i.second){
                                    ground_R[i.first].insert(j);
                                }
                            }
                        }
                        for(auto i: ground_R){
                            for(auto j: i.second){
                                Sim_pair sp; sp.a = i.first; sp.b = j; sp.sim = ppr[i.first][j];
                                Final_ans.push_back(sp);
                            }
                        }
                        
                        robin_map<int, robin_set<int> > ground_C = C[turn-1];
                        for(auto i: tmpR){
                            for(auto j: i.second){
                                ground_C[i.first].insert(j);
                            }
                        }
                        for(auto i: R[turn - 1]){
                            for(auto j: i.second){
                                ground_C[i.first].insert(j);
                            }
                        }
                        vector<Sim_pair> intoMAB;
                        for(auto i: ground_C){
                            if(ground_R.find(i.first) == ground_R.end()){
                                for(auto j: i.second){
                                    Sim_pair sp; sp.a = i.first; sp.b = j; sp.sim = 0.0;
                                    intoMAB.push_back(sp);
                                    // intoMAB[i.first].insert(i.second.begin(), i.second.end());
                                }
                                continue;
                            }
                            for(auto j:i.second){
                                if(ground_R[i.first].find(j) == ground_R[i.first].end()){
                                    Sim_pair sp; sp.a = i.first; sp.b = j; sp.sim = 0.0;
                                    intoMAB.push_back(sp);
                                }
                            }
                        }
                        vector<Sim_pair> Final_ans1 = MAB(intoMAB, topk, eps);
                        Final_ans.insert(Final_ans.end(), Final_ans1.begin(), Final_ans1.end());
                    }
                    return Final_ans;
                }
            }
            R.push_back(tmpR); C.push_back(tmpC);
            // robin_map<int, robin_set<int> > tmpR_ = R_[turn - 1];
            // robin_map<int, robin_set<int> > tmpC_ = C_[turn - 1];
            if (turn > 1){
                for(auto i: R_[turn-1]){
                    for(auto j: i.second){
                        tmpR[i.first].insert(j);
                    }
                }
                for(auto i: C_[turn-1]){
                    for(auto j: i.second){
                        tmpC[i.first].insert(j);
                    }
                }
            }
            
            R_.push_back(tmpR); C_.push_back(tmpC);
            // eps *= 0.9;
            ++turn; theta /= decrease_ratio; eps /= decrease_ratio;
        }   
    }
    

    double fora(int a, int b, double eps, int push_lens = 3){
        double rmax = eps/2;
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
    
        double rsum = 0;
        for(const auto& i: residue){
            for(const auto& j: i.second){
                rsum += j.second;
            }
        }
        int full_random_walks = rsum/eps/eps*(2*eps/3+2)*log(2e4); // delta = 1 ???


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
    
};

#endif