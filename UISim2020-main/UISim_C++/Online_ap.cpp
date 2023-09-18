
#include "include/Config.h"
#include "include/Graph.h"
#include "include/QueryProcessor1.h"
#include "TextReader.h"
#include "TextWriter.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <ctime>
#include <sstream>

using namespace std;
bool com(const pair<int, double> &a, const pair<int, double> &b)
{
  return a.second > b.second;
}
int main(int argc, char *argv[])
{
  clock_t startTime, elapsed;
  Config::hubType = argv[1];
  Config::numHubs = atoi(argv[2]);
  Config::depth = atoi(argv[3]);
  Config::stopRea = atof(argv[4]);
  Config::eta = atof(argv[5]);
  Config::nodeFile = argv[6];
  Config::edgeFile = argv[7];
  Config::indexDir = argv[8];
  Config::outputDir = argv[9];


  int k = -1;
  cout << "Graph nodes is " << Config::nodeFile << endl;
  cout << "Graph edges is " << Config::edgeFile << endl;
  cout << "Graph indexDir is " << Config::indexDir << endl;
  cout << "Graph outputDir is " << Config::outputDir << endl;
  cout<< "HubType:" << Config::hubType << ", numhubs:" << Config::numHubs << ", depth:" << Config::depth << ", stopRea:" << Config::stopRea << ", eta:" << Config::eta << endl;


  Graph graph;
  if (Config::numHubs > 0)
    graph.loadFromFile(Config::nodeFile, Config::edgeFile, true);
  else
    graph.loadFromFile(Config::nodeFile, Config::edgeFile, false);

  cout << "Starting query processing..." << endl;
  QueryProcessor1 qp(graph);
  
  int count = 0;
  startTime = clock();
  priority_queue<nodePairVal> H;  

  qp.query(k, H);
  elapsed = clock() - startTime;
  cout << "query finished. cost " << (double)elapsed / CLOCKS_PER_SEC << "s" << endl;

  string m_filepath= Config::outputDir + "eta" + to_string(int(Config::eta)) + "_stopRea" + to_string(int(Config::stopRea * 10000)) + "_topk5000";

  TextWriter out(m_filepath);
  out.write(to_string((double)elapsed / CLOCKS_PER_SEC) + "s \n");
  cout << "Savad to file: " << m_filepath << endl;
  startTime = clock();
  ostringstream outf;  
  outf.precision(6);

  while (!H.empty())
  {
    outf.str("");
    nodePairVal npv = H.top();
    H.pop();
    outf << npv.val;
    out.writeln(to_string(graph.findNode(npv.n1)) + "\t" + to_string(graph.findNode(npv.n2)) + "\t" + outf.str());
  }
  out.flush();
  out.close();


  elapsed = clock() - startTime;
  cout << "Saved finished. cost " << (double)elapsed / CLOCKS_PER_SEC << "s\n" << endl;
  return 0;
}
