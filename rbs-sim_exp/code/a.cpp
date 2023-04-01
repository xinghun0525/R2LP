#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <cstring>
#include <unordered_set>
#include <unordered_map>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "robin_map.h"
#include "robin_set.h"
using namespace std;
using namespace tsl;

bool maxScoreCmp(const pair<int, double>& a, const pair<int, double>& b){
    return a.second > b.second;
}

int main(int argc, char **argv){
	string filename = argv[1];
	int num = 0;
	ifstream ifs("../dataset/" + filename + ".txt");
	ifs >> num;
	ifs.close();
	cout << num << endl;
	ofstream ofs("query/" + filename + ".idx");
	for(int i = 0; i < num; ++i)
		ofs << i << endl;
	
	cout << num << endl;
    return 0;
}
