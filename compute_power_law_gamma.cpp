#include <iostream>
#include <cmath>
#include <fstream>

using namespace std;

int main(){
    string path = "/home/geQian/packing_gq/dataset/wiki_zh.txt";
    ifstream infile(path.c_str());
    int from, to;
    int n = 0; infile >> n;
    int deg[2000000] = {0};
    while(infile>>from>>to){
        deg[from]++;
        deg[to]++;
    }
    int deg_min = 100;
    double tmp_d_ratio = 0.0;
    for(int i = 0; i < n; ++i){
        if(deg[i] < deg_min)
            deg_min = deg[i];
    }
    cout << log(10) << endl;
    for(int i = 0; i < n; ++i){
        tmp_d_ratio += log(deg[i]/deg_min);
    }

    double gamma = 1;
    gamma = 1 + n/tmp_d_ratio;
    cout << "gamma = " << gamma << endl;
    
    cout << "finished" << endl;
    return 0;
}