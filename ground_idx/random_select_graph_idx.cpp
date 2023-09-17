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
    int n;
    infile1 >> n;

    ofstream outfile(new_path.c_str());
    for(int i = 0; i < n; ++i)
        outfile << i << endl;

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

int main(int argc, char **argv){
    // string old_path = "/home/geQian/indochina-2004.tsv";
    string filename = argv[1];
    string old_path = "dataset/" + filename + ".txt";
    string new_path = "ground_idx/"+ filename + "/idx1";
    rewrite(old_path, new_path);
    return 0;
}
