#ifndef RBS_H
#define RBS_H
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
    int pprtimes;
    Random mR;

    Rbs(string file_name, double _eps = 0.01, double _c = 0.6){
        g.inputGraph(file_name);
        g.sort_node();
        vert_num = g.n;
        edge_num = g.m;
        cvalue = _c;
        sqrtc = sqrt(cvalue);
        eps = _eps;
        pprtimes = log(vert_num) + 1; //need to make sure
        srand(unsigned(time(0)));
        mR = Random(unsigned(rand()));
        cout << "n = " << vert_num << ", m = " << edge_num << endl;
    }

    void rbspush_idx(){
        cout << "rbspush start" << endl;
        
        //robin_map<int, robin_map<int, double> > ppr;

        robin_map<int, robin_map<int, double> > appr;
        robin_map<int, robin_map<int, double> > bppr;
        vppr.push_back(appr);
        vppr.push_back(bppr);
        
        Timer mytime;
        double theta = eps;
        int L = log(theta)/log(cvalue);
        double bound = cvalue/(1 - cvalue)/theta;   // /10 则为divide

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
    
};

#endif