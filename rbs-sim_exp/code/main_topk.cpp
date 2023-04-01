#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <cstring>
#include <unordered_set>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <iomanip>
#include <ctime>

#include "Timer.h"
#include "rbssim_map.h"
using namespace std; 

bool mkdir_p(string folderPath){
    string command = "mkdir -p " + folderPath;  
    return system(command.c_str());
}
int parseLine(char *line) {
    // This assumes that a digit will be found and the line ends in " Kb".
    int i = strlen(line);
    const char *p = line;
    while (*p < '0' || *p > '9') p++;
    line[i - 3] = '\0';
    i = atoi(p);
    return i;
}

int getValue() { //Note: this value is in KB!
    FILE *file = fopen("/proc/self/status", "r");
    int result = -1;
    char line[128];

    while (fgets(line, 128, file) != NULL) {
        if (strncmp(line, "VmRSS:", 6) == 0) {
            result = parseLine(line);
            break;
        }
    }
    fclose(file);
    return result;
}

bool check_topk(){
	
}

int main(int argc, char **argv){
	string filename = argv[1];
	double eps = atof(argv[2]);
	int topk = atoi(argv[3]);
	bool output_full = false;

	robin_set<int> ground_idx1;
	string idx_path = "../../ground_idx/" + filename + "/idx1";
	ifstream fidx;
	fidx.open(idx_path.data());
	assert(fidx.is_open());
	cout << "start reading ground_idx1" << endl;
	int x;
	while(fidx >> x){
		ground_idx1.insert(x);
	}
	cout << "finished reading idx, num = " << ground_idx1.size() << endl;

	robin_set<int> ground_idx2;
	idx_path = "../../ground_idx/" + filename + "/idx2";
	fidx.open(idx_path.data());
	assert(fidx.is_open());
	cout << "start reading ground_idx2" << endl;
	int x;
	while(fidx >> x){
		ground_idx2.insert(x);
	}
	cout << "finished reading idx, num = " << ground_idx2.size() << endl;
	
	time_t now = time(nullptr);   
	char* curr_time = ctime(&now); cout << curr_time <<endl;
	cout << filename << " , eps = " << eps << ", output_full = " << output_full << endl;
	cout << "output_full = " << output_full<< endl;
    string dataname = "../../dataset/" + filename + ".txt";
	cout << "dataname = " << dataname << endl;
	Timer mytime;
    Rbs myrbs = Rbs(dataname, eps);


	cout << "forwardpush start" << endl;

	mytime.start();
	myrbs.rbspush_idx();
	mytime.end();
	cout << "backwardpush finish, use time = " << mytime.timeCost << endl;
	cout << "use mem = " << getValue() << " KB" << endl;

	all_time += mytime.timeCost;
	mkdir_p("../result/threshold/" + filename + "/");
	string outputfile = "../result/threshold/" + filename + "/" + to_string(eps);
	cout << "outputfile = " << outputfile << endl;
	ofstream fout;
	fout.open(outputfile.data());
	assert(fout.is_open());
	

	
	
	for(const auto &x: ground_idx){
		fout << x << " " << x << " " << "1.0" << endl;
		if(myrbs.ppr.find(x) != myrbs.ppr.end()){
			for(const auto &p1: myrbs.ppr[x]){
				if(p1.second >= threshold && x != p1.first){
					fout << x << " " << p1.first << " " << p1.second << endl;
				}
			}
		}
		for(auto p1 : myrbs.ppr){
			int u1 = p1.first;
			if(u1 == x) continue;
			if(p1.second.find(x) != p1.second.end() && p1.second[x] >= threshold){
				fout << x << " " << u1 << " " << p1.second[x] << endl;
			}
		}
	}

	
	fout.close();
	cout << "finish writing!!" << endl;
	
	
    return 0;
}
