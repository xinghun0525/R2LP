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
#include "rbssim_map_topk.h"
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
	// string filename = "CG";
	// double topk = 5000;
	// double eps_init = 0.1;
	string filename = argv[1];
	string stopk = argv[2];
	int topk = stoi(stopk);
	double eps_init = 0.1; // 
	// double rate2 = ato
	bool output_full = false;
	
	time_t now = time(nullptr);   
	char* curr_time = ctime(&now); cout << curr_time <<endl;
	cout << filename << " , eps_init = " << eps_init << ", topk = " << topk << ", output_full = " << output_full << endl;
	cout << "output_full = " << output_full<< endl;
    string dataname = "../../dataset/" + filename + ".txt";
	cout << "dataname = " << dataname << endl;
	Timer mytime;

    Rbs myrbs = Rbs(dataname);


	cout << "rlp start" << endl;
	double all_time = 0;

	mytime.start();
	// myrbs.rbspush_idx();
	vector<Sim_pair> topk_ans;
	topk_ans = myrbs.get_topk_basic(topk, eps_init);
	mytime.end();
	sort(topk_ans.begin(), topk_ans.end(), max_comp_Sim_pair);
	cout << "rlp finish, use time = " << mytime.timeCost << endl;
	cout << "use mem = " << getValue() << " KB" << endl;

	all_time += mytime.timeCost;
	mkdir_p("../result_topk/" + filename + "/");

	// std::stringstream ss;
	// ss << setprecision(15) << d;
	// str = ss.str();
	string outputfile = "../result_topk/" + filename + "/topk_" + stopk;
	cout << "outputfile = " << outputfile << endl;
	ofstream fout;
	fout.open(outputfile.data());
	assert(fout.is_open());

	for(const auto &i: topk_ans)
		fout << i.a << " " << i.b << " " << i.sim << endl;

	fout.close();
	cout << "finish writing!!" << endl;
	
	
    return 0;
}
