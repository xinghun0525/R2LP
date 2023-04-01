#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <time.h>
#include <unordered_set>
#include <unordered_map>
using namespace std;
bool minScoreCmp(const pair<int, int>& a, const pair<int, int>& b){
    return a.second < b.second;
}

void rewrite(string old_path, string new_path){
    ifstream infile1(old_path.c_str());
    unordered_map<int, int> us;

    int from;
    int to;
    int edge_num = 0;
    while(infile1>>from>>to){
        ++edge_num;
        int lens = us.size();
        if (us.find(from) == us.end())
            us[from] = lens;
        lens = us.size();
        if (us.find(to) == us.end())
            us[to] = lens;
    }
    infile1.close();
    ifstream infile2(old_path.c_str());

    ofstream outfile(new_path.c_str());
    outfile << us.size() << endl;
    while(infile2>>from>>to){
        outfile << us[from] << " " << us[to] << endl;
    }
    cout << "node num = " << us.size() << ", edge num = " << edge_num << endl;
    cout << "finished" << endl;
}

void check(string filename){
    ifstream infile(filename.c_str());
    unordered_set<int> us;
    int max_id = 0;
    
    //read graph and get degree info
    int from;
    int to;
    while(infile>>from>>to){
        us.insert(from);
        us.insert(to);
        max_id = max(max_id, from);
        max_id = max(max_id, to);
    }
    infile.close();
    cout << "max_id = " << max_id << endl;
    for(int i = 0; i <= max_id; ++i){
        if(us.find(i) == us.end()){
            cout << "wrong!! need rematch! " << i << endl;
            break;
        }
    }
    cout << "finish !" << endl;
}

int main(int argc, char** argv){
    string old_path = argv[1];
    string new_path = argv[2];
    rewrite(old_path, new_path);
    return 0;
}
