#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <cstring>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <unordered_map>
using namespace std;

int main()
{
    ifstream inf("dataset/WV0.txt");
    ofstream outf("dataset/WV.txt");
    unordered_map<int, int>  mp;
    int nodenum = 0;
    inf >> nodenum;
    int x,y;
    int tmpnum = 0;
    while(inf >> x >> y){
        if(mp.find(x) == mp.end()){
            mp[x] = tmpnum++;
        }
        if(mp.find(y) == mp.end()){
            mp[y] = tmpnum++;
        }
    }
    ifstream inf1("dataset/WV0.txt");
    inf1 >> nodenum;
    cout << nodenum << endl;
    outf << nodenum << endl;
    while(inf1 >> x >> y){
        outf << mp[x] << " " << mp[y] << endl;
    }
    outf.close();
    return 0;
}