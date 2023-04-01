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

int main(int argc, char **argv){
	// double eps = atof(argv[2]);
	int num = 1;// atoi(argv[2]); //times of cycle
	string filename = argv[1];
	int topk = atoi(argv[2]);
	double eps = 0.08;
	bool output_full = true;
	// int output_full = atoi(argv[3]); // output all node-pair or index node-pair

	// std::time_t t = std::time(nullptr);
    // std::cout << std::put_time(std::localtime(&t), "%Y-%m-%d %H:%M:%S") << std::endl;
	time_t now = time(nullptr);   
	char* curr_time = ctime(&now); cout << curr_time <<endl;
	cout << filename << " , eps = " << eps << ", output_full = " << output_full << endl;
	cout << "output_full = " << output_full<< endl;
    string dataname = "../dataset/" + filename + ".txt";
	cout << "dataname = " << dataname << endl;
	Timer mytime;
	
    Rbs myrbs = Rbs(dataname, eps);


	cout << "forwardpush start" << endl;
	double all_time = 0;
	mytime.start();
	while(true){
		int true_num = 0;
		myrbs.rbspush_idx(eps);
		for(const auto &p1 : myrbs.ppr){
			int u1 = p1.first;
			for(const auto &p2: p1.second){
				if(p2.second >= 10*eps && u1 > p2.first){
					++true_num;
				}
			}
		}
		cout << "eps = " << eps << ", true_num = " << true_num << endl << endl << endl;
		if(true_num >= topk)
			break;
		eps /= 2;
		myrbs.vppr.clear();
		myrbs.ppr.clear();
		myrbs.eps = eps;
	}
		mytime.end();
		cout << "backwardpush finish, use time = " << mytime.timeCost << endl;
		all_time += mytime.timeCost;

	mkdir_p("./rbs_topk_result/" + filename + "/");
	string outputfile = "./rbs_topk_result/" + filename + "/" + to_string(topk);
	cout << "outputfile = " << outputfile << endl;
	ofstream fout;
	fout.open(outputfile.data());
	assert(fout.is_open());

	if(output_full==0){
		robin_set<int> ground_idx;
		string idx_path = "./query/" + filename + ".idx";
		ifstream fidx;
		fidx.open(idx_path.data());
		assert(fidx.is_open());
		cout << "start reading ground_idx" << endl;
		int x;
		while(fidx >> x){
			ground_idx.insert(x);
		}
		cout << "finished reading idx, num = " << ground_idx.size() << endl;

		
		for(const auto &x: ground_idx){
			if(myrbs.ppr.find(x) != myrbs.ppr.end()){
				for(const auto &p1: myrbs.ppr[x]){
					if(p1.second > 0 && x != p1.first){
						fout << x << " " << p1.first << " " << p1.second << endl;
					}
				}
			}
			for(auto p1 : myrbs.ppr){
				int u1 = p1.first;
				if(u1 == x) continue;
				if(p1.second.find(x) != p1.second.end() && p1.second[x] > 0){
					fout << x << " " << u1 << " " << p1.second[x] << endl;
				}
			}
		}
	}
	else{
		for(const auto &p1 : myrbs.ppr){
			int u1 = p1.first;
			for(const auto &p2: p1.second){
				if(p2.second > 0 && u1 != p2.first)
					fout << u1 << " " << p2.first << " " << p2.second << endl;
			}
		}
	}
	
	
	fout.close();
	
	cout << endl << "all use time = " << all_time << endl;
	cout << "average time = " << all_time/num << endl;
	cout << "use mem = " << getValue() << " KB" << endl;
    return 0;
}
