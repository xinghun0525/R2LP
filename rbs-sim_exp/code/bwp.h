#ifndef BWP_H
#define BWP_H
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

using namespace std;

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
    double alpha;// prob of stop
    double eps;
    //unordered_map<int, double> rmp;
    double rsum;
    double rmax;
    bool *vis;
    double **mppr;
    double *residue;
    int *push1;
    int *push2;
    int pushnum1;
    int pushnum2;
    int pprtimes;
    Rbs(string file_name, double _eps = 0.01, double _alpha = 0.2){
        g.inputGraph(file_name);
        g.sort_node();
        vert_num = g.n;
        edge_num = g.m;
        alpha = _alpha;
        eps = _eps;
        pprtimes = log(vert_num) + 1;
        vis = new bool[vert_num];
        mppr = new double*[pprtimes];
        for(int i = 0; i < pprtimes; ++i){
            mppr[i] = new double[vert_num];
        }
        residue = new double[vert_num];
        push1 = new int[vert_num];
        push2 = new int[vert_num];
        pushnum1 = 0;
        pushnum2 = 0;
    }
    ~Rbs(){
        for(int i = 0; i < pprtimes; ++i)
            delete []mppr[i];
        delete[]mppr;
        delete[]vis;
        delete[]residue;
        delete[]push1;
        delete[]push2; 
    }
    

    void backwardpush(int tnode, double* ppr){
        cout << "backwardpush start" << endl;
        memset(residue, 0, sizeof(double) * vert_num);
        memset(ppr, 0, sizeof(double) * vert_num);
        memset(vis, 0, sizeof(bool) * vert_num);
        pushnum1 = 0;
        residue[tnode] = 1;
        vis[tnode] = 1;
        push1[pushnum1++] = tnode;
        while(pushnum1 > 0){
            cout << "pushnum1 = " << pushnum1 << endl;
            pushnum2 = 0;
            for(int i = 0; i < pushnum1; ++i){
                int u = push1[i];
                double r = residue[u];
                cout << "u = " << u << ", r = " << r << endl;
                ppr[u] += r*alpha;
                double delta = r*(1-alpha);
                int sz = g.getInSize(u);
                residue[u] = 0;
                vis[u] = false;
                for(int j = 0; j < sz; ++j){
                    int v = g.getInVert(u, j);
                    int outd = g.getOutSize(v);
                    residue[v] += delta/outd;
                    if(!vis[v] && residue[v] > eps){
                        vis[v] = true;
                        push2[pushnum2++] = v;
                    }
                }
            }
            int *c;
            c = push1;
            push1 = push2;
            push2 = c;
            pushnum1 = pushnum2;
        }
    }

    

};

#endif