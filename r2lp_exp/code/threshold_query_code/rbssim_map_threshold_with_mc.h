#ifndef RBS_THRESHOLD_WITH_MC_H
#define RBS_THRESHOLD_WITH_MC_H
#include <iostream>
#include <cmath>
#include <unordered_map>
#include <unordered_set>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <omp.h>
#include <sys/time.h>
#include <vector>
#include <algorithm>
#include <string>
#include <limits.h>


#include "Graph.h"
#include "Timer.h"
#include "Random.h"

#include "robin_map.h"
#include "robin_set.h"
#include "gperftools/tcmalloc.h"
using namespace std;
using namespace tsl;


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
    robin_map<int, robin_map<int, double> > first_directed_ppr;
    double rou; // 
    int pprtimes;
    double theta;
    double eps_term; // terminal eps

    Random mR;

    Rbs(string file_name, double _theta, double _rou, double _eps_init = 0.1, double _eps_term=0.002,
        double _c = 0.6){
        rou = _rou;
        g.inputGraph(file_name);
        g.sort_node();
        theta = _theta;
        eps_term = _eps_term;
        vert_num = g.n;
        edge_num = g.m;
        cvalue = _c;
        sqrtc = sqrt(cvalue);
        eps = _eps_init;
        pprtimes = log(vert_num) + 1; //need to make sure
        srand(unsigned(time(0)));
        mR = Random(unsigned(rand()));
        cout << "n = " << vert_num << ", m = " << edge_num << endl;
    }

    void rbspush_idx(){
        cout << "rbspush start" << endl;

        robin_map<int, robin_map<int, double> > appr;
        robin_map<int, robin_map<int, double> > bppr;
        vppr.push_back(appr);
        vppr.push_back(bppr);
        
        while(eps >= eps_term){
            cout << endl << "**** eps = " << eps << endl;
            Timer mytime;
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
                                if(u1 == v2 && u2 == v1){
                                    if(u1 > u2)
                                        ppr[u1][u2] += cvalue*tpi/(pud-cvalue);
                                    else
                                        ppr[u2][u1] += cvalue*tpi/(pud-cvalue);
                                    continue;
                                }
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
                                if(u1 == v2 && u2 == v1){
                                    if(u1 > u2)
                                        ppr[u1][u2] += cvalue*tpi/(pud-cvalue);
                                    else
                                        ppr[u2][u1] += cvalue*tpi/(pud-cvalue);
                                    continue;
                                }
                                if(pud <= bound1){
                                    if(u1 > u2)
                                        nxtppr[u1][u2] += (cvalue)*tpi/pud;
                                    else
                                        nxtppr[u2][u1] += (cvalue)*tpi/pud;
                                }
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
            double next_eps = eps/4;
            vppr[0].clear();
            vppr[1].clear();
            bool choice = continue_rbs(eps, next_eps);
            if(!choice)
                break;
            eps = next_eps;
            if(eps<eps_term)
                break;
            ppr.clear();
            //appr.push_back(ppr);
        }
        
    }

    int continue_rbs(double tmp_eps, double next_eps){
        int R_theta = 0; int R_candidate = 0;
        double time_rbs = 0.0;
        double time_mc = 0.0;
        vector<int> v1;
        vector<int> v2;
        robin_set<int> rs;
        for(const auto& p1 : ppr){
            int u1 = p1.first;
            for(const auto& p2: p1.second){
                if(p2.second >= theta - 1e-8)
                    ++R_theta;
                
                else if(p2.second > theta - tmp_eps){
                    ++R_candidate;
                    v1.push_back(u1);
                    v2.push_back(p2.first);
                    rs.insert(u1);
                    rs.insert(p2.first);
                }
                time_rbs += sqrt(g.getInSize(u1) * g.getInSize(p2.first))*p2.second;
            }
        }
        cout << "num of larger than threshold = " << R_theta << ", " 
            << "num of candidate pair = " << R_candidate << endl;
        cout << "num of candidate node = " << rs.size() << endl;
        if(R_theta > 0 && R_theta/(R_theta + R_candidate+0.0) >= rou)
            return false;
        time_rbs = time_rbs/next_eps * log(g.n);
        double delta = 1e-4;
        time_mc = 1.0/next_eps/next_eps * log(cvalue/delta) * R_candidate;
        cout << "time_rbs = " << time_rbs << ", time_mc = " << time_mc << endl;
        int sample_num = 1.0/next_eps/next_eps * log(cvalue/delta);
        if(time_mc > time_rbs)
            return true;
        
        // use mento_carlo
        mento_carlo(v1, v2, next_eps, R_theta, sample_num);
        return false;
    }

    void mento_carlo(vector<int> & v1, vector<int> & v2, double tmp_eps, int R_theta, int sample_num){
        // cout << endl << "get into mento_carlo, tmp_eps = " << tmp_eps << ", R_theta = " << 
        //     R_theta << " , R_candidate = " << v1.size() << ", sample_num = " << sample_num << endl;
        Random mcr;

        vector<int> nv1, nv2;
        robin_set<int> rs;
        
        for(int i = 0; i < v1.size(); ++i){
            int x = v1[i]; int y = v2[i];
            int meet_times = 0;
            assert(x != y);
            for(int j = 0; j < sample_num; ++j){
                double prob = mcr.drand();
                int length;
                int r;
                while(mcr.drand() < cvalue){
                    length = g.getInSize(x);
                    if(length == 0)
                        break;
                    r = mcr.generateRandom() % length;
                    int nx = g.getInVert(x, r);
                    x = nx;
                    length = g.getInSize(y);
                    if(length == 0)
                        break;
                    r = mcr.generateRandom() % length;
                    int ny = g.getInVert(y, r);
                    y = ny;
                    if(x == y){
                        ++meet_times;
                        break;
                    }
                }
            }
            double tmp_sim = meet_times/(sample_num+0.0);
            
            ppr[v1[i]][v2[i]] = tmp_sim;
            if(tmp_sim > theta - 1e-8)
                ++R_theta;
            
            else if(tmp_sim > theta - tmp_eps){
                rs.insert(x); rs.insert(y);
                nv1.push_back(x); nv2.push_back(y);
            }
        }
        cout << endl << "finish this Mento carlo, tmp_eps = " << tmp_eps << ", R_theta = " << 
            R_theta << " , R_candidate pair num = " << nv1.size() << ", sample_num = " << sample_num << endl;
        
        cout << "R_candidate node num = " << rs.size() << endl;
        if (R_theta / (R_theta + nv1.size() + 0.0) > rou)
            return;
        
        tmp_eps /= 4;
        if (tmp_eps < eps_term)
            return;
        double delta = 1e-4;
        sample_num = int(1.0/tmp_eps/tmp_eps * log(cvalue/delta));
        mento_carlo(nv1, nv2, tmp_eps, R_theta, sample_num);

    }

};

#endif