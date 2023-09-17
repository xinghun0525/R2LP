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
#include "rbssim_map_wo_pruning.h"
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

int main(int argc, char **argv){
	string filename = argv[1];
	// double threshold = atof(argv[2]);
	double eps = atof(argv[2]);
	double save_threshold = 9e-4;
	
	time_t now = time(nullptr);   
	char* curr_time = ctime(&now); cout << curr_time <<endl;
	cout << filename << " , eps = " << eps << endl;
    string dataname = "../../dataset/" + filename + ".txt";
	cout << "dataname = " << dataname << endl;
	Timer mytime;
    Rbs myrbs = Rbs(dataname);


	cout << "rlp start" << endl;
	double all_time = 0;

	mytime.start();
	myrbs.rbspush_idx(eps);
	mytime.end();
	cout << "rlp finish, use time = " << mytime.timeCost << endl;
	cout << "use mem = " << getValue() << " KB" << endl;

	all_time += mytime.timeCost;
	mkdir_p("../result_eps/" + filename + "/");

	// std::stringstream ss;
	// ss << setprecision(15) << d;
	// str = ss.str();
	string outputfile = "../result_eps/" + filename + "/eps_" + to_string(eps);
	cout << "outputfile = " << outputfile << endl;
	ofstream fout;
	fout.open(outputfile.data());
	assert(fout.is_open());
	// robin_set<int> ground_idx;
	vector<int> ground_idx;
	unordered_set <int> ground_idx_set;
	string idx_path = "../../ground_idx/" + filename + "/idx1";
	ifstream fidx;
	fidx.open(idx_path.data());
	assert(fidx.is_open());
	cout << "start reading ground_idx" << endl;
	int x;
	while(fidx >> x){
		ground_idx.push_back(x);
		ground_idx_set.insert(x);
	}
	sort(ground_idx.begin(), ground_idx.end());
	
	cout << "finished reading idx, num = " << ground_idx.size() << endl;

	for(const auto &p1 : myrbs.ppr){
		int u1 = p1.first;
		if (ground_idx_set.find(u1) != ground_idx_set.end()){
			for(auto p2: p1.second){
				if(p2.second >= save_threshold)
					fout << u1 << " " << p2.first << " " << p2.second << endl;
			}
			continue;
		}

		auto p2 = p1.second;
		for(auto u2: ground_idx){
			if(u2 >= u1)
				break;
			if(p2.find(u2) != p2.end() && p2[u2] >= save_threshold)
				fout << u1 << " " << u2 << " " << p2[u2] << endl;
		}
	}
	fout.close();
	cout << "finish writing!!" << endl;
	
	
    return 0;
}
