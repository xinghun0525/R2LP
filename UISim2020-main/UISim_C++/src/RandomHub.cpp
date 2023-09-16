/*
 * RandomHub.cpp
 *
 *  Created on: 2019��3��16��
 *      Author: yichen_shen
 */

#include "RandomHub.h"
#include <random>
#include <time.h>
#include <iostream>
RandomHub::RandomHub() {
}

void RandomHub::fillNodes() {
		static default_random_engine rnd(time(NULL));
		static uniform_real_distribution<double> distribution(0.0, 1.0);

		for (auto &n : graph.getNodes())
			nodes.push_back(pair<int,double>(n->id, distribution(rnd)));
}

RandomHub::RandomHub(string nodeFile, string edgeFile) :HubSelection(nodeFile,edgeFile){
}

RandomHub::~RandomHub() {}

