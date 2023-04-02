/*
 * Config.cpp
 *
 *  Created on: 2019��3��13��
 *      Author: yichen_shen
 */

#include "Config.h"

int Config::numIterations = 30;
int Config::numHubs = 2000;
double Config::epsilon = 1E-8;
double Config::clip = 1E-3;
double Config::delta = 1E-4;
double Config::eta = 2;
double Config::alpha = 0.60;
double Config::sqrt_alpha = 0.77459666924;
int Config::numClusters = 3;
int Config::maxClusterFaults = 10;
int Config::numRepetitions = 1;
string Config::file_sep = "/";


string Config::nodeFile = "/UISim2020-main/data/CG/edges.txt";
string Config::edgeFile = "/UISim2020-main/data/CG/edges.txt";
string Config::indexDir = "/UISim2020-main/data/CG/";


string Config::clusterDir = "";
string Config::communityFile = "";

string Config::outputDir = "/UISim2020-main/result/CG/";

string Config::queryFile = "/UISim2020-main/data/WV/WV.query-SS";


string Config::hubType = "indeg";

int Config::resultTop = 50;
int Config::hubTop = 4000000;
int Config::iterations = 10;
int Config::progressiveTopK = 100;
int Config::depth = 5;
double Config::stopRea = 1E-4;
int Config::maxNode = 7000;

int Config::correctionLevel = 1;

Config::Config()
{
}

Config::~Config()
{
}

int Config::i_total = 0;
int Config::i_hubs_end = 0;
int Config::i_iter_stop = 0;
unordered_map<string, int> Config::exp_eta_nums;