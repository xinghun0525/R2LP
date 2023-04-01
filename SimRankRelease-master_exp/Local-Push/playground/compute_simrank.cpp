//
// Created by yche on 1/22/18.
//

#include <boost/program_options.hpp>

#include "../yche_refactor/simrank.h"
#include "../yche_refactor/local_push_yche.h"

using namespace std;
using namespace boost::program_options;
std::time_t getTimeStamp()
{
    std::chrono::time_point<std::chrono::system_clock,std::chrono::milliseconds> tp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
    auto tmp=std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch());
    std::time_t timestamp = tmp.count();
    //std::time_t timestamp = std::chrono::system_clock::to_time_t(tp);
    return timestamp;
}
std::tm* gettm(std::time_t timestamp)
{
    std::time_t milli = timestamp/*+ (std::time_t)8*60*60*1000*/;//此处转化为东八区北京时间，如果是其它时区需要按需求修改
    auto mTime = std::chrono::milliseconds(milli);
    auto tp=std::chrono::time_point<std::chrono::system_clock,std::chrono::milliseconds>(mTime);
    auto tt = std::chrono::system_clock::to_time_t(tp);
    std::tm* now = std::gmtime(&tt);
    printf("%4d年%02d月%02d日 %02d:%02d:%02d.%d\n",now->tm_year+1900,now->tm_mon+1,now->tm_mday,now->tm_hour,now->tm_min,now->tm_sec, milli%1000);
   return now;
}

int getValue1() { //Note: this value is in KB!
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

int main(int argc, char *argv[]) {
    cout << "start!!" << endl;
    gettm(getTimeStamp());
    string g_name = argv[1];
    double eps = atof(argv[2]);
    double save_eps = atof(argv[3]);
    cout << "gname = " << g_name << ", eps = " << eps << ", save_eps = " << save_eps << endl;
    
    // 1st: compute reduce local push
    
    string path = get_edge_list_path(g_name);
    GraphYche g(path);
    double c = 0.6;
//    double eps = 0.0001;
    cout << "graph num = " << g.n << endl;

    
    // 2nd: verify the correcness
    
    TruthSim ts(g_name, g, c, eps);
    auto max_err = 0.0;
    double err = 0;
    string gt = "../ground_truth/" + g_name;
    cout << "going to compute groundtruth!! " << gt << endl;
    ofstream fgt;
    fgt.open(gt.data());
    assert(fgt.is_open());
    if(g.n < 1000000){
        // TruthSim ts(g_name, g, c, eps);
        cout << "finished ts matrix " << endl;
        for(int i = 0; i < g.n; ++i){
            for(int j = 0; j <= i; ++j){
                float asim = ts.sim(i,j);
                if(asim >= save_eps - 1e-5)
                    fgt << i << " " << j << " " << ts.sim(i,j) << endl;
            }
        }
    }
    fgt.close();
    cout << "use mem = " << getValue1() << " KB" << endl;
    cout << "finished !!" << endl;
    gettm(getTimeStamp());
    return 0;
    
}